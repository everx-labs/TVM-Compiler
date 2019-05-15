//===--------- TVMStackModel.cpp - Rewrite Reg-forms with S-forms ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Introduce explicit stack manipulation. Rewrite all register forms of
/// instructions with their stack counterparts.
///
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/TVMMCTargetDesc.h"
#include "TVM.h"
#include "TVMStack.h"
#include "TVMStackBlockInfo.h"
#include "TVMSubtarget.h"
#include "TVMUtilities.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/LiveIntervals.h"
#include "llvm/CodeGen/MachineBlockFrequencyInfo.h"
#include "llvm/CodeGen/MachineDominators.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/Support/raw_ostream.h"

#include "TVMInstMappingInfo.inc"

using namespace llvm;

#define DEBUG_TYPE "tvm-stack-model"

namespace {

class TVMStackModel final : public MachineFunctionPass {
public:
  StringRef getPassName() const override { return "TVM Stack Model"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<LiveIntervals>();
    AU.addPreservedID(LiveVariablesID);
    AU.addRequired<MachineLoopInfo>();
    AU.addPreserved<MachineLoopInfo>();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;
  /// Inserts necessary stack manipulation instructions to supply \par MI with
  /// the correct data.
  bool processInstruction(MachineInstr &MI, LiveIntervals &LIS,
                          Stack &TheStack);

  static char ID; // Pass identification, replacement for typeid
  TVMStackModel() : MachineFunctionPass(ID) {}

private:
  bool runOnBasicBlocks(MachineFunction &MF, const Stack &StartStack);
  Stack prepareWantedStack(MachineBasicBlock *MBB, LiveIntervals &LIS);
  Stack prepareMultiEndStack(MachineBasicBlock *MBB, LiveIntervals &LIS);

  const DILocalVariable *findDebugValue(const MachineInstr &MI,
                                        unsigned Vreg) const;
  std::string prepareInstructionComment(const MachineInstr &MI) const;

  /// Returns true if From -> To branch is backedge
  bool isBackEdge(const MachineBasicBlock *From,
                  const MachineBasicBlock *To) const;

  TVMFunctionInfo *MFI;
  MachineRegisterInfo* MRI;
  const TargetInstrInfo *TII;
  MachineLoopInfo *Loops;
};

} // end anonymous namespace

char TVMStackModel::ID = 0;
INITIALIZE_PASS(TVMStackModel, DEBUG_TYPE, "Stackify register instructions",
                false, false)

FunctionPass *llvm::createTVMStackModel() { return new TVMStackModel(); }

const DILocalVariable *TVMStackModel::findDebugValue(const MachineInstr &MI,
                                                     unsigned Vreg) const {
  auto it = llvm::find_if(*MI.getParent(), [Vreg](const MachineInstr &DI) {
    return DI.isDebugValue() &&
           (DI.getOperand(0).isReg() && DI.getOperand(0).getReg() == Vreg);
  });
  return (it != MI.getParent()->end()) ? it->getDebugVariable() : nullptr;
}

std::string
TVMStackModel::prepareInstructionComment(const MachineInstr &MI) const {
  if (!MI.getNumExplicitDefs())
    return std::string();

  std::string rv;
  raw_string_ostream OS(rv);

  auto OpPrinter = [&OS, &MI, this](const MachineOperand &Operand) {
    if (Operand.isReg()) {
      OS << printReg(Operand.getReg());
      if (auto DbgVar = findDebugValue(MI, Operand.getReg()))
        OS << "(" << DbgVar->getName() << ")";
    } else {
      OS << Operand;
    }
  };
  auto CommaOpPrinter = [&OS, &OpPrinter](const MachineOperand &Operand) {
    OS << ", ";
    OpPrinter(Operand);
  };

  OS << ">";
  OpPrinter(*MI.defs().begin());
  llvm::for_each(drop_begin(MI.defs(), 1), CommaOpPrinter);

  OS << " = " << TII->getName(MI.getOpcode()) << " ";

  auto Uses = MI.uses();
  if (Uses.begin() != Uses.end()) {
    OpPrinter(*MI.uses().begin());
    llvm::for_each(drop_begin(MI.uses(), 1), CommaOpPrinter);
  }
  OS.flush();
  return OS.str();
}

// Returns true if From -> To branch is backedge
bool TVMStackModel::isBackEdge(const MachineBasicBlock *From,
                               const MachineBasicBlock *To) const {
  for (const auto *Lp = Loops->getLoopFor(From); Lp; Lp = Lp->getParentLoop()) {
    if (Lp->getHeader() == To)
      return true;
  }
  return false;
}

bool TVMStackModel::processInstruction(MachineInstr &MI, LiveIntervals &LIS,
                                       Stack &TheStack) {
  if (MI.isImplicitDef())
    return false;

  StackFixup::InstructionGenerator gen(TII, MFI, MI.getParent(), MI);

  auto removeUnusedDefsFx = StackFixup::RemoveUnusedDefinitions(MI, TheStack);
  gen(removeUnusedDefsFx);

  size_t NumDefs = MI.getNumDefs();
  size_t NumOperands = MI.getNumOperands();

  // FIXME: wrong assumption. There is no function scope in TVM so RET
  // terminates the current continuation - not necessary a function.
  if (MI.isReturn()) {
    assert(NumOperands <= 2 && "Multiple returns are not implemented yet");
    if (NumOperands == 0)
      gen(StackFixup::DiffForReturn(TheStack));
    else
      gen(StackFixup::DiffForReturn(TheStack, MI.getOperand(0).getReg()));
    return true;
  }

  auto prepareArgsFx = TheStack.reqArgs(&MI, LIS) - TheStack;
  gen(prepareArgsFx);
  prepareArgsFx.apply(TheStack);

  size_t NumStackOperands = 0, NumGlobals = 0, NumImms = 0, NumMBBs = 0;
  for (const MachineOperand &Op : MI.uses()) {
    if (Op.isGlobal() || Op.isSymbol())
      NumGlobals++;
    else if (Op.isImm())
      NumImms++;
    else if (Op.isMBB())
      NumMBBs++;
    else if (Op.isReg())
      NumStackOperands++;
  }
  unsigned NewOpcode = TVM::RegForm2SForm[MI.getOpcode()];
  unsigned NumToConsume = NumStackOperands;
#ifndef NDEBUG
  // Let's ensure that consumed registers are used in instruction
  // TODO: Doesn't cover numerous corner cases. Covering them would require to
  // reimplement consumption under NDEBUG or extending consumption interface.
  for (unsigned I = 0; I < NumToConsume; I++)
    if (TheStack.isReg(I))
      assert(llvm::count_if(MI.operands(),
                            [&](const MachineOperand &Op) {
                              return Op.isReg() &&
                                     Op.getReg() == TheStack.reg(I);
                            }) &&
             "Consuming register not used in instruction");
#endif
  TheStack.consumeArguments(NumToConsume);

  for (size_t ROpNo = 0; ROpNo < NumDefs; ++ROpNo) {
    const auto &Operand = MI.getOperand(NumDefs - ROpNo - 1);
    assert(Operand.isReg() && "Def must be a register");
    TheStack.addDef(Operand.getReg(), findDebugValue(MI, Operand.getReg()));
  }
  if (NewOpcode >= 0) {
    // add global addresses before the command
    // TODO: continuation must be modelled in the stack then.
    for (unsigned I = 0; I < NumGlobals; I++) {
      const auto &Op = MI.getOperand(NumDefs + I);
      assert((Op.isGlobal() || Op.isSymbol()) &&
             "Expected GlobalAddress/ExternalSymbol");
      if (Op.isGlobal()) {
        BuildMI(&MI, TII->get(TVM::PUSHCONT_LABEL))
            .addGlobalAddress(Op.getGlobal(), Op.getOffset());
      } else {
        BuildMI(&MI, TII->get(TVM::PUSHCONT_LABEL))
            .addExternalSymbol(Op.getSymbolName(), Op.getOffset());
      }
      if (NewOpcode == TVM::CALL_STORE_S) {
        BuildMI(&MI, TII->get(TVM::XCHG)).addImm(0).addImm(1);
      }
    }

    MachineInstrBuilder MIB = BuildMI(&MI, TII->get(NewOpcode));

    // Additional immediate is fake op for TVM::PUSHCONT_MBB operation
    if (MI.getOpcode() == TVM::PUSHCONT_MBB) {
      MIB->addOperand(MI.getOperand(1));
    } else {
      for (unsigned I = 0; I < NumImms; I++) {
        // Imms are expected to be in continuous sequence
        //  in register version of MI
        const auto &Op = MI.getOperand(NumOperands - NumImms + I);
        assert(Op.isImm() && "Expected Imm");
        MIB.addImm(Op.getImm());
      }
    }
    if (NumDefs)
      MFI->addStackModelComment(MIB.getInstr(), prepareInstructionComment(MI));
    MFI->addStackModelComment(MIB.getInstr(), TheStack.toString());

    auto pruneDeadDefsFx = StackFixup::PruneDeadDefinitions(MI, TheStack);
    gen(pruneDeadDefsFx);

    MI.removeFromParent();
  }
  return true;
}

Stack TVMStackModel::prepareWantedStack(MachineBasicBlock *MBB,
                                        LiveIntervals &LIS) {
  Stack rv(*MBB->getParent(), 0);

  for (unsigned i = 0, e = MRI->getNumVirtRegs(); i != e; ++i) {
    unsigned Reg = TargetRegisterInfo::index2VirtReg(i);
    if (LIS.hasInterval(Reg)) {
      if (LIS.isLiveInToMBB(LIS.getInterval(Reg), MBB)) {
        rv.addDef(Reg, findDebugValue(*MBB->begin(), Reg));
      }
    }
  }
  return rv;
}

Stack TVMStackModel::prepareMultiEndStack(MachineBasicBlock *MBB,
                                          LiveIntervals &LIS) {
  Stack rv(*MBB->getParent(), 0);

  for (unsigned i = 0, e = MRI->getNumVirtRegs(); i != e; ++i) {
    unsigned Reg = TargetRegisterInfo::index2VirtReg(i);
    if (LIS.hasInterval(Reg)) {
      if (LIS.isLiveOutOfMBB(LIS.getInterval(Reg), MBB)) {
        rv.addDef(Reg, findDebugValue(*MBB->begin(), Reg));
      }
    }
  }
  return rv;
}

// Traverse all basic blocks in machine function and rewrite all instructions
// from R-form to S-form. All function arguments are already in stack.
// Topological order visitation, back edges of UNTIL terminators are ignored.
bool TVMStackModel::runOnBasicBlocks(MachineFunction &MF,
                                     const Stack &StartStack) {
  bool Changed = false;

  LiveIntervals &LIS = getAnalysis<LiveIntervals>();
  DenseMap<MachineBasicBlock *, TVMStackBlockInfo> BBStack;

  while (BBStack.size() < MF.size()) {
    size_t Size = BBStack.size();
    for (MachineBasicBlock &MBB : MF) {
      if (BBStack.count(&MBB) != 0u)
        continue;
      SmallVector<MachineBasicBlock *, 4> fwdPreds;
      llvm::copy_if(MBB.predecessors(), std::back_inserter(fwdPreds),
          [this, &MBB] (MachineBasicBlock *Pred) {
            return !isBackEdge(Pred, &MBB);
      });
      bool allFwdPredsVisited = llvm::all_of(fwdPreds,
          [&BBStack] (MachineBasicBlock *Pred) {
            return BBStack.count(Pred);
      });
      if (!allFwdPredsVisited)
        continue;

      auto &bbInfo = BBStack[&MBB];
      bbInfo.setMBB(&MBB);

      if (fwdPreds.empty()) {
        bbInfo.setWantedBegin(StartStack);
        bbInfo.setFixedBegin(StartStack);
      } else {
        bbInfo.setWantedBegin(prepareWantedStack(&MBB, LIS));

        // splitting preds into blocks with single successor (this block)
        // and multi successors
        auto multiPred = llvm::partition(fwdPreds, [](MachineBasicBlock *Pred) {
          return Pred->succ_size() == 1;
        });
        auto singlePreds = llvm::make_range(fwdPreds.begin(), multiPred);
        auto multiPreds = llvm::make_range(multiPred, fwdPreds.end());
        if (llvm::size(multiPreds) == 0) {
          // If we don't have multi preds, we can just request wanted begin
          bbInfo.setFixedBegin(bbInfo.wantedBegin());
        } else {
          auto fxdE = llvm::partition(multiPreds, [&](MachineBasicBlock *Pred) {
            return BBStack[Pred].isFixedEnd();
          });
          auto fxdPreds = llvm::make_range(multiPreds.begin(), fxdE);
          auto unfxdPreds = llvm::make_range(fxdE, multiPreds.end());
          if (llvm::size(fxdPreds) == 0) {
            // If we don't have fxd preds, we can just request wanted begin
            bbInfo.setFixedBegin(bbInfo.wantedBegin());
          } else if (llvm::size(fxdPreds) == 1) {
            bbInfo.setFixedBegin(BBStack[*fxdPreds.begin()].fixedEnd()
                .filteredByLiveIns(MBB, LIS));
          } else {
            SmallVector<MachineBasicBlock *, 4> uniq(fxdPreds);
            auto uniqIt = std::unique(uniq.begin(), uniq.end(),
              [&](MachineBasicBlock *L, MachineBasicBlock *R) {
                return BBStack[L].fixedEnd() == BBStack[R].fixedEnd();
            });
            if (std::distance(fxdPreds.begin(), uniqIt) == 1) {
              // good, all fixed predecessors ends already are the same
              bbInfo.setFixedBegin(BBStack[*uniq.begin()].fixedEnd()
                  .filteredByLiveIns(MBB, LIS));
            } else {
              // requires insertion of fixup mbb
              llvm_unreachable("Unimplemented - fixup mbb"); // TODO
            }
          }
          llvm::for_each(unfxdPreds, [&](MachineBasicBlock *pred) {
            BBStack[pred].setFixedEndWithFixup(bbInfo.fixedBegin(), TII, MFI);
          });
        }

        // all single-target preds will have exact fixed end = cur.begin
        // We dont need to merge them, just insert instructions for diff
        llvm::for_each(singlePreds, [&](MachineBasicBlock *pred) {
          BBStack[pred].setFixedEndWithFixup(bbInfo.fixedBegin(), TII, MFI);
        });
      }
      auto CurrentStack = bbInfo.fixedBegin(); // start bb stack here

      for (auto I = MBB.begin(), E = MBB.end(); I != E;) {
        MachineInstr &MI = *I++;
        assert(!TVM::isArgument(MI));

        if (MI.isDebugInstr() || MI.isLabel())
          continue;
        if (MI.isTerminator()) {
          SmallVector<StackVreg, 4> Args;
          for (auto Op : MI.uses())
            if (Op.isReg())
              Args.push_back(StackVreg(Op.getReg()));
          bbInfo.setTerminatorArgs(Args);
        }

        Changed |= processInstruction(MI, LIS, CurrentStack);
      }
      SmallVector<MachineBasicBlock *, 4> bkSuccessors;
      llvm::copy_if(MBB.successors(), std::back_inserter(bkSuccessors),
          [this, &MBB] (MachineBasicBlock *Succ) {
            return isBackEdge(&MBB, Succ);
      });
      bbInfo.setCalculatedEnd(CurrentStack);
      if (size_t backEdges = llvm::size(bkSuccessors)) {
        if (backEdges == 1) {
          bbInfo.setFixedEndWithFixup(
                BBStack[*bkSuccessors.begin()].fixedBegin(), TII, MFI);
        } else {
          // requires insertion of fixup mbb
          llvm_unreachable("Unimplemented - fixup mbb"); // TODO
        }
      } else if (MBB.succ_size() > 1) {
        bbInfo.setFixedEndWithFixup(prepareMultiEndStack(&MBB, LIS), TII, MFI);
      }
    }
    assert(BBStack.size() > Size && "No progress in TVMStackModel loop.");
  }
  return Changed;
}

// TODO: For now it only stackifies function arguments. Extend.
bool TVMStackModel::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(
      dbgs()
      << "********** Rewrite Instructions in Reg-form to S-form **********\n"
         "********** Function: "
      << MF.getName() << '\n');

  bool Changed = false;
  MFI = MF.getInfo<TVMFunctionInfo>();
  MRI = &MF.getRegInfo();
  TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();
  Loops = &getAnalysis<MachineLoopInfo>();

  MachineBasicBlock &FirstBB = MF.front();
  assert(!FirstBB.empty());

  auto &ANI = FirstBB.front();

  // Process ARGUMENT_NUM instruction to adjust arguments number on stack.
  if (TVM::isArgumentNum(ANI)) {
    int args = ANI.getOperand(0).getImm();
    for (int i = 0; i < args; i++)
      MFI->addParam(MVT::i64);
    ANI.eraseFromParent();
    Changed = true;
  }

  size_t NumArgs = MFI->numParams();
  Stack StartStack(MF, NumArgs);

  // Handle ARGUMENTS first to ensure that they get the designated numbers.
  for (MachineBasicBlock::iterator I = FirstBB.begin(), E = FirstBB.end();
       I != E;) {
    MachineInstr &MI = *I++;
    if (!TVM::isArgument(MI))
      break;
    unsigned Reg = MI.getOperand(0).getReg();
    assert(!MFI->isVRegStackified(Reg));
    unsigned ArgNo = NumArgs - MI.getOperand(1).getImm() - 1;
    StartStack.set(ArgNo, StackVreg(Reg, findDebugValue(MI, Reg)));
    MI.eraseFromParent();
    Changed = true;
  }

  MFI->setStartStackModelComment(StartStack.toString());

  if (runOnBasicBlocks(MF, StartStack))
    Changed = true;

  return Changed;
}
