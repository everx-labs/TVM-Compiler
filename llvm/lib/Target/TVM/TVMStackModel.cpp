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
  bool processInstruction(MachineInstr &MI, Stack &TheStack);

  static char ID; // Pass identification, replacement for typeid
  TVMStackModel() : MachineFunctionPass(ID) {}

private:
  bool runOnBasicBlocks(MachineFunction &MF, const Stack &StartStack);
  Stack prepareMultiEndStack(MachineBasicBlock *MBB);
  void fillBackEdgesLiveouts(MachineBasicBlock &MBB,
                             SmallVector<unsigned, 16> &Regs);

  void prepareRoads(MachineFunction &MF);
  Stack prepareRoadPattern(MachineFunction &MF,
                           std::vector<MachineBasicBlock *> &BBs,
                           unsigned RoadIdx);
  void fillBlocksBeginEndPatterns(MachineFunction &MF);

  void gatherBlockLiveIns(MachineBasicBlock &MBB, std::set<unsigned> &vregs);
  void gatherBlockLiveOuts(MachineBasicBlock &MBB, std::set<unsigned> &vregs);

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
  LiveIntervals *LIS;

  DenseMap<MachineBasicBlock *, TVMStackBlockInfo> BBInfo;
  unsigned MaxRoads = 0;
  unsigned MaxRoadWidth = 0;
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

bool TVMStackModel::processInstruction(MachineInstr &MI, Stack &TheStack) {
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

  auto prepareArgsFx = TheStack.reqArgs(Stack::MIArgs(MI, *LIS)) - TheStack;
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
    // Global operands and external symbols are represented using GlobalAddress
    // and ExternalSymbol DAG nodes. Because of convention of instruction
    // operands ordering global addresses and external symbols must be placed
    // first before instruction. To avoid custom logic of stack reordering for
    // global/external operands, we transfrom all GlobalAddress and
    // ExternalSymbol nodes to the chain of PUSH_GLOBAL_ADDRESS(_S) instruction
    // and TargetGlobalAddress / TargetExternalSymbol nodes. So by default only
    // PUSH_GLOBAL_ADDRESS instruction may have global/external operand. This
    // instruction has definition with address which can be normally processed
    // using stack model for all further uses of the address result.
    // There are may be exceptions when the transformation to
    // PUSH_GLOBAL_ADDRESS is not needed (for example, for instructions with
    // immediate string operands like LOGSTR). For such cases operands will be
    // passed up to lowering to MCInst where they can be customly processed.

    // add global addresses before the command
    // TODO: continuation must be modelled in the stack then.
    for (unsigned I = 0; I < NumGlobals; I++) {
      const auto &Op = MI.getOperand(NumDefs + I);
      assert((Op.isGlobal() || Op.isSymbol()) &&
             "Expected GlobalAddress/ExternalSymbol");
      if (NewOpcode == TVM::PUSH_GLOBAL_ADDRESS_S) {
        if (Op.isGlobal()) {
          BuildMI(&MI, TII->get(TVM::PUSHCONT_LABEL))
              .addGlobalAddress(Op.getGlobal(), Op.getOffset());
        } else {
          BuildMI(&MI, TII->get(TVM::PUSHCONT_LABEL))
              .addExternalSymbol(Op.getSymbolName(), Op.getOffset());
        }
      }
    }

    MachineInstrBuilder MIB = BuildMI(&MI, TII->get(NewOpcode));

    if (NewOpcode != TVM::PUSH_GLOBAL_ADDRESS_S) {
      for (unsigned I = 0; I < NumGlobals; I++) {
        const auto &Op = MI.getOperand(NumDefs + I);
        assert(Op.isGlobal() && "Expected GlobalAddress");
        MIB->addOperand(Op);
      }
    }

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

// Traverse all basic blocks in machine function and rewrite all instructions
// from R-form to S-form. All function arguments are already in stack.
bool TVMStackModel::runOnBasicBlocks(MachineFunction &MF,
                                     const Stack &StartStack) {
  bool Changed = false;

  auto &FirstBB = MF.front();
  BBInfo[&FirstBB].setFixedBegin(StartStack);

  for (auto &MBB : MF) {
    auto &bbInfo = BBInfo[&MBB];
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
        bbInfo.setTerminatorArgs(Stack::MIArgs(MI, *LIS));
      }

      Changed |= processInstruction(MI, CurrentStack);
    }
    bbInfo.setCalculatedEnd(CurrentStack);

    if (MBB.succ_size()) {
      // Insert stack fixup from calculated current stack into exit pattern
      bbInfo.doEndFixup(TII, MFI);
    }
  }
  return Changed;
}

Stack TVMStackModel::
prepareRoadPattern(MachineFunction &MF,
                   std::vector<MachineBasicBlock *> &BBs, unsigned RoadIdx) {
  std::set<unsigned> vregs;
  for (auto MBB : BBs) {
    auto &CurInfo = BBInfo[MBB];
    if (CurInfo.roadBegin() == RoadIdx)
      gatherBlockLiveIns(*MBB, vregs);
    if (CurInfo.roadEnd() == RoadIdx)
      gatherBlockLiveOuts(*MBB, vregs);
  }

  Stack roadPattern(MF, 0);
  for (auto Reg : vregs)
    roadPattern.addDef(Reg, nullptr);
  return roadPattern;
}

void TVMStackModel::prepareRoads(MachineFunction &MF) {
  MachineBasicBlock &FirstBB = MF.front();

  BBInfo.clear();
  BBInfo.reserve(MF.size());
  for (auto &MBB : MF) {
    BBInfo[&MBB].setMBB(&MBB);
  }

  unsigned curRoad = 1;
  std::deque<MachineBasicBlock *> queueThisRoad;
  queueThisRoad.push_back(&FirstBB);
  BBInfo[&FirstBB].setRoadEnd(curRoad);
  std::deque<MachineBasicBlock *> queueNextRoads;

  auto processSuccessors = [&](MachineBasicBlock *CurBB) {
    auto endRoad = BBInfo[CurBB].roadEnd();
    assert(endRoad && "null CurBB road in processing successors");
    for (auto Succ : CurBB->successors()) {
      auto &CurInfo = BBInfo[Succ];
      auto beginRoad = CurInfo.roadBegin();
      assert((beginRoad == endRoad || !beginRoad) &&
             "Already defined different road");
      if (!beginRoad) {
        CurInfo.setRoadBegin(endRoad);
        queueThisRoad.push_back(Succ);

        // other side of the block
        if (!CurInfo.roadEnd())
          queueNextRoads.push_back(Succ);
      }
    }
  };
  auto processPredecessors = [&](MachineBasicBlock *CurBB) {
    auto beginRoad = BBInfo[CurBB].roadBegin();
    assert(beginRoad && "null CurBB road in processing predecessors");
    for (auto Pred : CurBB->predecessors()) {
      auto &CurInfo = BBInfo[Pred];
      auto endRoad = CurInfo.roadEnd();
      assert((beginRoad == endRoad || !endRoad) &&
             "Already defined different road");
      if (!endRoad) {
        CurInfo.setRoadEnd(beginRoad);
        queueThisRoad.push_back(Pred);

        // other side of the block
        if (!CurInfo.roadBegin())
          queueNextRoads.push_back(Pred);
      }
    }
  };
  do {
    while (!queueThisRoad.empty()) {
      auto CurBB = queueThisRoad.front();
      queueThisRoad.pop_front();
      auto &CurInfo = BBInfo[CurBB];
      if (CurInfo.roadBegin() == curRoad)
        processPredecessors(CurBB);
      if (CurInfo.roadEnd() == curRoad)
        processSuccessors(CurBB);
    }

    ++curRoad;

    while (!queueNextRoads.empty()) {
      auto CurBB = queueNextRoads.front();
      queueNextRoads.pop_front();
      auto &CurInfo = BBInfo[CurBB];
      if (CurInfo.roadBegin() && CurInfo.roadEnd())
        continue;
      assert(!(!CurInfo.roadBegin() && !CurInfo.roadEnd()) &&
             "Both side roads can't be null for block in queueNextRoads");
      if (!CurInfo.roadBegin())
        CurInfo.setRoadBegin(curRoad);
      if (!CurInfo.roadEnd())
        CurInfo.setRoadEnd(curRoad);
      queueThisRoad.push_back(CurBB);
      break;
    }
  } while (!queueThisRoad.empty());

  MaxRoads = curRoad;
}

void TVMStackModel::fillBlocksBeginEndPatterns(MachineFunction &MF) {
  typedef std::vector<MachineBasicBlock *> BBVecT;
  std::vector<BBVecT> roadsBBs(MaxRoads);

  for (auto &MBB : MF) {
    auto &CurInfo = BBInfo[&MBB];
    roadsBBs[CurInfo.roadBegin()].push_back(&MBB);
    if (CurInfo.roadBegin() != CurInfo.roadEnd())
      roadsBBs[CurInfo.roadEnd()].push_back(&MBB);
  }

  for (unsigned i = 1; i < MaxRoads; ++i) {
    auto &BBs = roadsBBs[i];

    auto roadPattern = prepareRoadPattern(MF, BBs, i);
    MaxRoadWidth = std::max<unsigned>(MaxRoadWidth, roadPattern.size());

    for (auto MBB : BBs) {
      auto &CurInfo = BBInfo[MBB];
      if (CurInfo.roadBegin() == i)
        CurInfo.setFixedBegin(roadPattern.filteredByLiveIns(*MBB, *LIS));
      if (CurInfo.roadEnd() == i)
        CurInfo.setFixedEnd(roadPattern.filteredByLiveOuts(*MBB, *LIS));
    }
  }
}

void TVMStackModel::gatherBlockLiveIns(MachineBasicBlock &MBB,
                                       std::set<unsigned> &vregs) {
  for (unsigned i = 0, e = MRI->getNumVirtRegs(); i != e; ++i) {
    unsigned Reg = TargetRegisterInfo::index2VirtReg(i);
    if (LIS->hasInterval(Reg)) {
      if (LIS->isLiveInToMBB(LIS->getInterval(Reg), &MBB)) {
        vregs.insert(Reg);
      }
    }
  }
}

void TVMStackModel::gatherBlockLiveOuts(MachineBasicBlock &MBB,
                                        std::set<unsigned> &vregs) {
  for (unsigned i = 0, e = MRI->getNumVirtRegs(); i != e; ++i) {
    unsigned Reg = TargetRegisterInfo::index2VirtReg(i);
    if (LIS->hasInterval(Reg)) {
      if (LIS->isLiveOutOfMBB(LIS->getInterval(Reg), &MBB)) {
        vregs.insert(Reg);
      }
    }
  }
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
  LIS = &getAnalysis<LiveIntervals>();

  BBInfo.clear();
  MaxRoads = 0;
  MaxRoadWidth = 0;

  prepareRoads(MF);
  fillBlocksBeginEndPatterns(MF);

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
