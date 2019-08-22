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

/// The pass makes stack explicit by rewriting Reg-form instructions with S-form
/// and inserting stack manipulation instructions. The rewriting is done in the
/// following stages:
/// 1. Define roads. Roads are equivalence classes for initial and final stack
/// configurations.
/// 2. Define stack configurations for start and end of each basic block. For
/// now we do it arbitrary having the information about live-ins and live-outs.
/// TODO: We should consider to be more flexibe with stack patterns to reduce
/// number of stack manipulations required.
/// 3. Process basic blocks one by one, knowing the initial and final stack
/// configurations and rewriting all Reg-form instructions to S-form.
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

  /// Calculate required stack manipulations to ship arguments to \par MI.
  StackFixup prepareStackFor(MachineInstr &MI, const Stack &StackBefore,
                             std::string *DebugMessage = nullptr);

  /// Calculate stack after \par MI execution.
  /// This fixup is not supposed to be code generated.
  void modelInstructionExecution(MachineInstr &MI, Stack &StackBefore);

  static char ID; // Pass identification, replacement for typeid
  TVMStackModel() : MachineFunctionPass(ID) {}

private:
  bool runOnBasicBlocks(MachineFunction &MF, const Stack &StartStack);
  void fillBackEdgesLiveouts(MachineBasicBlock &MBB,
                             SmallVector<unsigned, 16> &Regs);

  /// Calculate equvalent classes on BB's entry / exit stack configuration
  /// and store them as road numbers in BBInfo.
  void prepareRoads(MachineFunction &MF);

  /// Helper function to calculate initial and final stack configurations
  Stack prepareRoadPattern(MachineFunction &MF,
                           std::vector<MachineBasicBlock *> &BBs,
                           unsigned RoadIdx);

  /// Calculate initial and final stack state for a basic block.
  /// When processing a basic blocks, it's initial state is pre calculate and
  /// in the end the pass must ensure that the control flow leaves stack in
  /// final stack configuration.
  void fillBlocksBeginEndPatterns(MachineFunction &MF);

  /// Rewrite an instruction in Reg-form to S-form.
  /// \see TVMInstructionInfo.td to learn more.
  void rewriteToSForm(MachineInstr &MI, std::string& PreTermStackString,
                      Stack &TheStack);


  /// Append \par MMB live-ins to \par vregs
  void gatherBlockLiveIns(MachineBasicBlock &MBB, std::set<unsigned> &vregs);
  /// Append \par MMB live-outs to \par vregs
  void gatherBlockLiveOuts(MachineBasicBlock &MBB, std::set<unsigned> &vregs);

  const DILocalVariable *findDebugValue(const MachineInstr &MI,
                                        unsigned Vreg) const;
  std::string prepareInstructionComment(const MachineInstr &MI) const;

  /// Returns true if From -> To branch is a backedge
  bool isBackEdge(const MachineBasicBlock *From,
                  const MachineBasicBlock *To) const;

  TVMFunctionInfo *MFI;
  MachineRegisterInfo *MRI;
  const TargetInstrInfo *TII;
  MachineLoopInfo *Loops;
  LiveIntervals *LIS;

  /// Store requirements on for BB stack configurations
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
      if (Operand.isUndef())
        OS << "x";
      else
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
  auto *MBB = MI.getParent();
  StackFixup::InstructionGenerator InsertMIs(TII, MFI, MBB, MI);
  std::string PreTermStackStr;
  StackFixup Fix = prepareStackFor(MI, TheStack, &PreTermStackStr);
  InsertMIs(Fix);
  Fix.apply(TheStack);

  modelInstructionExecution(MI, TheStack);
  rewriteToSForm(MI, PreTermStackStr, TheStack);
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
      Changed |= processInstruction(MI, CurrentStack);
    }
    bbInfo.setCalculatedEnd(CurrentStack);
  }
  return Changed;
}

Stack TVMStackModel::prepareRoadPattern(MachineFunction &MF,
                                        std::vector<MachineBasicBlock *> &BBs,
                                        unsigned RoadIdx) {
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

  /// TODO: It's better to encode keep infor about roads with a more expressive
  /// class than unsigned. The suggestion is to use shared_ptr<Stack> instead of
  /// unsigned.
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
  using BBVecT = std::vector<MachineBasicBlock *>;
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
      if (CurInfo.roadBegin() == i) {
        CurInfo.setFixedBegin(roadPattern.filteredByLiveIns(*MBB, *LIS));
        MFI->setStackModelBBComment(MBB, CurInfo.fixedBegin().toString());
      }
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

  MFI->setStackModelBBComment(&MF.front(), StartStack.toString());

  if (runOnBasicBlocks(MF, StartStack))
    Changed = true;

  return Changed;
}

/// Model stack for a single instruction.
StackFixup TVMStackModel::prepareStackFor(MachineInstr &MI,
                                          const Stack &StackBefore,
                                          std::string *DebugMessage) {
  if (MI.isImplicitDef())
    return {};

  auto *MBB = MI.getParent();

#ifndef NDEBUG
  {
    SmallVector<StackVreg, 32> Vregs(
        llvm::make_filter_range(StackBefore, [](StackVreg vreg) {
          return vreg.VirtReg != TVMFunctionInfo::UnusedReg;
        }));

    llvm::sort(Vregs.begin(), Vregs.end());
    auto it = std::unique(Vregs.begin(), Vregs.end());
    assert(it == Vregs.end() && "Vreg duplicates found in stack");
  }
#endif

  // TODO: We can do it inplace.
  Stack TheStack = StackBefore.filteredByMIdefs(MI);

  size_t NumOperands = MI.getNumOperands();

  // FIXME: wrong assumption. There is no function scope in TVM so RET
  // terminates the current continuation - not necessary a function.
  if (MI.isReturn()) {
    assert(NumOperands <= 2 && "Multiple returns are not implemented yet");
    if (NumOperands == 0)
      return StackFixup::DiffForReturn(TheStack);
    else
      return StackFixup::DiffForReturn(TheStack, MI.getOperand(0).getReg());
  }

  if (MI.isTerminator() && MBB->succ_size()) {
    // For terminator instruction we need to prepare stack as
    //  dst road pattern plus required arguments
    auto AfterTermStack = BBInfo[MBB].fixedEnd();
    auto NeedStack = AfterTermStack.withArgs(MIArgs(MI, *LIS));
    auto Fix = NeedStack - TheStack;
    if (DebugMessage)
      *DebugMessage = NeedStack.toString();
    return Fix;
  } else {
    return StackFixup::DiffForArgs(TheStack, MIArgs(MI, *LIS),
                                   MI.isCommutable());
  }
}

void TVMStackModel::modelInstructionExecution(MachineInstr &MI,
                                              Stack &StackBefore) {
  size_t NumDefs = MI.getNumDefs();
  size_t NumStackOperands = llvm::count_if(MI.uses(), [](const MachineOperand& MO) { return MO.isReg(); });
  unsigned NumToConsume = NumStackOperands;
#ifndef NDEBUG
  // Let's ensure that consumed registers are used in instruction
  // TODO: Doesn't cover numerous corner cases. Covering them would require to
  // reimplement consumption under NDEBUG or extending consumption interface.
  auto revUses = reverse(MI.uses());
  for (unsigned I = 0; I < NumToConsume; I++) {
    if (MI.isCommutable()) {
      auto regUse = llvm::find_if(revUses, [&](const MachineOperand &Op) {
        return Op.isReg() && StackBefore.reg(I) == Op.getReg();
      });
      assert(regUse != revUses.end() &&
             "Consuming register not used in instruction");
    } else {
      auto regUse = llvm::find_if(
          revUses, [&](const MachineOperand &Op) { return Op.isReg(); });
      assert(regUse != revUses.end() &&
             "Consuming register not used in instruction");

      assert(regUse->isUndef() ||
             regUse->getReg() == StackBefore.reg(I) &&
                 "Wrong register for consuming in instruction");

      revUses = llvm::make_range(++regUse, revUses.end());
    }
  }
#endif
  StackBefore.consumeArguments(NumToConsume);
  for (size_t ROpNo = 0; ROpNo < NumDefs; ++ROpNo) {
    const auto &Operand = MI.getOperand(NumDefs - ROpNo - 1);
    assert(Operand.isReg() && "Def must be a register");
    StackBefore.addDef(Operand.getReg(), findDebugValue(MI, Operand.getReg()));
  }
}

void TVMStackModel::rewriteToSForm(MachineInstr &MI, std::string &PreTermStackString,
                                   Stack &TheStack) {
  size_t NumDefs = MI.getNumDefs();
  size_t NumOperands = MI.getNumOperands();
  unsigned NewOpcode = TVM::RegForm2SForm[MI.getOpcode()];

  size_t NumGlobals = llvm::count_if(MI.uses(), [](const MachineOperand& MO) { return MO.isGlobal(); });
  size_t NumImms = llvm::count_if(MI.uses(), [](const MachineOperand& MO) { return MO.isImm(); });

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

    TheStack.filterByDeadDefs(MI);

    if (NumDefs)
      MFI->addStackModelComment(MIB.getInstr(), prepareInstructionComment(MI));

    if (MI.isTerminator())
      MFI->addStackModelComment(MIB.getInstr(),
                                PreTermStackString + " => " + TheStack.toString());
    else
      MFI->addStackModelComment(MIB.getInstr(), TheStack.toString());

    MI.removeFromParent();
  }
}
