//===------------ TVMPeephole.cpp - TVM Peephole Optimiztions -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Late peephole optimizations for TVM.
///
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMMachineInstrMatcher.h"
#include "TVMSubtarget.h"
#include "TVMUtilities.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "tvm-peephole"

static cl::opt<bool>
    DisableTVMReturnOpt("disable-tvm-return-opt", cl::Hidden,
                        cl::desc("TVM: Disable return optimizations."),
                        cl::init(false));
static cl::opt<bool>
    DisableTVMIfElseOpt("disable-tvm-ifelse-peephole-opt", cl::Hidden,
                        cl::desc("TVM: Disable IFELSE peephole optimizations."),
                        cl::init(false));
static cl::opt<bool>
    DisableTVMDropOpt("disable-tvm-drop-opt", cl::Hidden,
                      cl::desc("TVM: Disable DROP peephole optimizations."),
                      cl::init(false));
static cl::opt<bool> DisableTVMBlkPushOpt(
    "disable-tvm-push-opt", cl::Hidden,
    cl::desc("TVM: Disable BLKPUSH peephole optimizations."), cl::init(false));
static cl::opt<bool> DisableTVMShorterFormOpt(
    "disable-tvm-shorter-form-opt", cl::Hidden,
    cl::desc("TVM: Disable shorter form replacement peephole optimizations."),
    cl::init(false));
static unsigned BlkPushLimit = 15;
namespace {
class TVMPeephole final : public MachineFunctionPass {
  StringRef getPassName() const override { return "TVM peephole optimizer"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;
  bool runOnMachineBasicBlock(MachineBasicBlock &MBB,
                              const TargetInstrInfo &TII);
  bool runImplicitReturnOptimization(MachineBasicBlock &MBB,
                                     const TargetInstrInfo &TII);
  bool runBlkDropCombine(MachineBasicBlock &MBB, const TargetInstrInfo &TII);
  bool runBlkPushCombine(MachineBasicBlock &MBB, const TargetInstrInfo &TII);
  bool runReplaceWithShorterForm(MachineBasicBlock &MBB,
                                 const TargetInstrInfo &TII);
  bool runMbbInlineOptimization(MachineBasicBlock &MBB,
                                const TargetInstrInfo &TII);
  bool runIfElseOptimization(MachineBasicBlock &MBB,
                             const TargetInstrInfo &TII);
  bool runIfElseInlining(MachineBasicBlock::instr_iterator &Iter,
                         MachineBasicBlock &MBB, const TargetInstrInfo &TII);
  bool runIfElseEmptyBranchRemoving(MachineBasicBlock::instr_iterator &Iter,
                                    MachineBasicBlock &MBB,
                                    const TargetInstrInfo &TII);

public:
  static char ID;
  TVMPeephole() : MachineFunctionPass(ID) {}
};
} // end anonymous namespace

char TVMPeephole::ID = 0;
INITIALIZE_PASS(TVMPeephole, DEBUG_TYPE, "TVM peephole optimizations", false,
                false)

FunctionPass *llvm::createTVMPeephole() { return new TVMPeephole(); }

static bool MaybeOptimizeReturn(MachineInstr &MI, const TargetInstrInfo &TII) {
  MachineBasicBlock *MBB = MI.getParent();
  MachineFunction *MF = MBB->getParent();
  if (DisableTVMReturnOpt)
    return false;

  assert(!MBB->empty() && "Empty BBs must already have been removed");

  if (MBB != &MF->back())
    return false;

  // If we don't need to clean up stack on return, we could omit this
  // instruction in the exit BB.
  BuildMI(&MI, TII.get(TVM::FALLTHROUGH_RETURN));
  MI.eraseFromParent();

  return true;
}

bool TVMPeephole::runBlkDropCombine(MachineBasicBlock &MBB,
                                    const TargetInstrInfo &TII) {
  bool Changed = false;
  std::vector<MachineInstr *> MIRemove;
  for (auto It = std::begin(MBB), E = std::end(MBB); It != E; ++It) {
    unsigned count = 0;
    auto DropIt = It;
    for (; DropIt != E; ++DropIt) {
      if (DropIt->getOpcode() != TVM::POP ||
          DropIt->getOperand(0).getImm() != 0)
        break;
      else
        ++count;
    }
    if (count > 1u) {
      for (auto RmIt = It; RmIt != DropIt; ++RmIt)
        MIRemove.push_back(&*RmIt);
      It = BuildMI(&*DropIt, TII.get(TVM::BLKDROP)).addImm(count);
      LLVM_DEBUG(dbgs() << "Replaced " << count << "DROPs with BLKDROP");
      Changed = true;
    }
    count = 0;
  }
  for (auto *I : MIRemove)
    I->eraseFromParent();
  return Changed;
}

bool TVMPeephole::runBlkPushCombine(MachineBasicBlock &MBB,
                                    const TargetInstrInfo &TII) {
  bool Changed = false;
  std::vector<MachineInstr *> MIRemove;
  for (auto It = std::begin(MBB), E = std::end(MBB); It != E; ++It) {
    std::vector<MachineInstr *> MaybeRemove;
    unsigned Reg = TVMFunctionInfo::UnusedReg;
    const ConstantInt *ConstVal;
    bool IsConstant = false;
    if (It->getOpcode() == TVM::PUSH) {
      Reg = It->getOperand(0).getImm();
      if (Reg > BlkPushLimit)
        continue;
      MaybeRemove.push_back(&*It);
    } else if (It->getOpcode() == TVM::CONST_I257_S ||
               It->getOpcode() == TVM::CONST_I257_S) {
      IsConstant = true;
      ConstVal = It->getOperand(0).getCImm();
    } else {
      continue;
    }
    auto PushIt = std::next(It);
    for (; PushIt != E; ++PushIt) {
      if (MaybeRemove.size() == BlkPushLimit)
        break;
      if (PushIt->getOpcode() == TVM::PUSH) {
        unsigned R = PushIt->getOperand(0).getImm();
        if (R > BlkPushLimit)
          break;
        // If we perform constant sequence and new PUSH register points to one
        //  of just pushed constants, then we may continue the sequence
        if (IsConstant && R >= MaybeRemove.size())
          break;
        // It we perform push sequence, next register must be exactly
        //  (Reg + size) to continue blkpush sequence
        if (!IsConstant && R != Reg + MaybeRemove.size())
          break;
        MaybeRemove.push_back(&*PushIt);
      } else if (PushIt->getOpcode() == TVM::CONST_I257_S ||
                 PushIt->getOpcode() == TVM::CONST_U257_S) {
        if (!IsConstant || ConstVal != PushIt->getOperand(0).getCImm())
          break;
        MaybeRemove.push_back(&*PushIt);
      } else {
        break;
      }
    }
    if (MaybeRemove.size() > 1u) {
      std::copy(std::begin(MaybeRemove), std::end(MaybeRemove),
                std::back_inserter(MIRemove));
      It = BuildMI(&*PushIt, TII.get(TVM::BLKPUSH))
               .addImm(MaybeRemove.size())
               .addImm((Reg == TVMFunctionInfo::UnusedReg) ? 0 : Reg);
      LLVM_DEBUG(dbgs() << "Replaced " << MaybeRemove.size()
                        << " PUSH/PUSHINT with BLKPUSH");
      Changed = true;
    }
  }
  for (auto *I : MIRemove)
    I->eraseFromParent();
  return Changed;
}

bool TVMPeephole::runReplaceWithShorterForm(MachineBasicBlock &MBB,
                                            const TargetInstrInfo &TII) {
  DenseMap<MachineInstr *, unsigned> MIReplace;
  for (auto It = std::begin(MBB), E = std::end(MBB); It != E; ++It) {
    if (It->getOpcode() == TVM::BLKDROP && It->getOperand(0).getImm() == 2)
      MIReplace.insert({&*It, TVM::DROP2});
    if (It->getOpcode() == TVM::PUSH2 && It->getOperand(0).getImm() == 1 &&
        It->getOperand(1).getImm() == 0)
      MIReplace.insert({&*It, TVM::DUP2});
    if (It->getOpcode() == TVM::PUSH2 && It->getOperand(0).getImm() == 3 &&
        It->getOperand(1).getImm() == 2)
      MIReplace.insert({&*It, TVM::OVER2});
    if (It->getOpcode() == TVM::BLKSWAP && It->getOperand(0).getImm() == 1 &&
        It->getOperand(1).getImm() == 2)
      MIReplace.insert({&*It, TVM::ROT});
    if (It->getOpcode() == TVM::BLKSWAP && It->getOperand(0).getImm() == 2 &&
        It->getOperand(1).getImm() == 1)
      MIReplace.insert({&*It, TVM::ROTREV});
    if (It->getOpcode() == TVM::ROLLREV && It->getOperand(0).getImm() == 2)
      MIReplace.insert({&*It, TVM::ROTREV});
    if (It->getOpcode() == TVM::XC2PU && It->getOperand(0).getImm() == 0 &&
        It->getOperand(1).getImm() == 0 && It->getOperand(2).getImm() == 1)
      MIReplace.insert({&*It, TVM::TUCK});
  }
  for (auto I : MIReplace) {
    BuildMI(I.first, TII.get(I.second));
    I.first->eraseFromParent();
  }
  return !MIReplace.empty();
}

bool TVMPeephole::runImplicitReturnOptimization(MachineBasicBlock &MBB,
                                                const TargetInstrInfo &TII) {
  if (MBB.empty())
    return false;

  auto &MI = MBB.back();
  return MI.isReturn() && MaybeOptimizeReturn(MI, TII);
}

bool TVMPeephole::runMbbInlineOptimization(MachineBasicBlock &MBB,
                                           const TargetInstrInfo &TII) {
  if (MBB.empty())
    return false;

  auto InstrIter = MBB.instr_rbegin();
  auto &JmpX = *InstrIter;

  if (JmpX.getOpcode() != TVM::JMPX_S)
    return false;

  ++InstrIter;

  auto FirstStackInstrIter = InstrIter;
  bool PushContMBBFound = false;

  while (InstrIter != MBB.instr_rend() && !PushContMBBFound) {
    switch (InstrIter->getOpcode()) {
    case TVM::PUSHCONT_MBB_S:
      PushContMBBFound = true;
      break;
    // Auxilary stack operations should be skipped here and then converted
    // below to transform the MI sequence to (PUSHCONT, JMPX) form
    case TVM::POP:
      if (InstrIter->getOperand(0).getImm() == 1)
        ++InstrIter;
      else
        return false;
      break;
    default:
      return false;
    }
  }

  auto LastStackInstrIter = InstrIter;

  if (!PushContMBBFound)
    return false;

  auto &PushContMBB = *InstrIter;

  if (PushContMBB.getOpcode() != TVM::PUSHCONT_MBB_S)
    return false;

  assert(PushContMBB.getNumOperands() >= 1 &&
         PushContMBB.getOperand(0).isMBB() &&
         "MachineBasicBlock should be an operand for PUSHCONT_MBB_S");

  LLVM_DEBUG(
      { dbgs() << "  inline JMPX  %bb." + Twine(MBB.getNumber()) << "\n"; });

  const MachineBasicBlock &SourceMBB = *PushContMBB.getOperand(0).getMBB();
  auto InsertionIter = InstrIter.getReverse();
  MachineFunction *MF = MBB.getParent();
  TVMFunctionInfo *MFI = MF->getInfo<TVMFunctionInfo>();

  // Convert MI sequence to expected form (PUSHCONT, JMPX) and move all
  // stack operations between PUSHCONT and JMPX before the PUSHCONT
  // (the list of possible transformations may be extended in future)
  for (InstrIter = FirstStackInstrIter; InstrIter != LastStackInstrIter;) {
    if (InstrIter->getOpcode() == TVM::POP &&
        InstrIter->getOperand(0).getImm() == 1) {
      MFI->clearIntermediateData(
          BuildMI(&*InsertionIter, TII.get(TVM::POP)).addImm(0));
    }

    auto Next = InstrIter;
    ++Next;
    InstrIter->eraseFromParent();
    InstrIter = Next;
  }

  for (auto &MI : SourceMBB) {
    MachineInstr &NewMI = MF->CloneMachineInstrBundle(MBB, InsertionIter, MI);
    MFI->cloneMachineInstrIntermediateData(&MI, &NewMI);
  }

  PushContMBB.eraseFromParent();
  JmpX.eraseFromParent();

  return true;
}

bool TVMPeephole::runIfElseInlining(
    MachineBasicBlock::instr_iterator &InstrIter, MachineBasicBlock &MBB,
    const TargetInstrInfo &TII) {
  MachineInstr *PushContA = nullptr;
  MachineInstr *PushContB = nullptr;
  MachineInstr *PushContC = nullptr;
  MachineInstr *Ifelse = nullptr;

  static constexpr int MaxInstructionsToRemove = 10;
  std::array<MachineInstr *, MaxInstructionsToRemove> InstrToRemove;
  int NumInstrToRemove = 0;

  MachineInstrMatcher Matcher;
  bool MatchFound = false;
  bool NeedInsertSwapBeforePostDominator = false;

  if (!MatchFound) {
    Matcher = MachineInstrMatcher(InstrIter, MBB.instr_end());

    MachineInstr *Roll = nullptr;
    MachineInstr *Blkswap = nullptr;
    MachineInstr *Swap = nullptr;

    if (Matcher.match(PushContA, TVM::PUSHCONT_MBB_S) &&
        Matcher.match(PushContB, TVM::PUSHCONT_MBB_S) &&
        Matcher.match(PushContC, TVM::PUSHCONT_MBB_S) &&
        Matcher.match(Roll, TVM::ROLL, 3) &&
        Matcher.match(Blkswap, TVM::BLKSWAP, 2, 2) &&
        Matcher.match(Swap, TVM::XCHG_TOP, 1) &&
        Matcher.match(Ifelse, TVM::IFELSE_S)) {
      MatchFound = true;
      InstrToRemove[NumInstrToRemove++] = Roll;
      InstrToRemove[NumInstrToRemove++] = Blkswap;
      InstrToRemove[NumInstrToRemove++] = Swap;
    }
  }

  if (!MatchFound) {
    Matcher = MachineInstrMatcher(InstrIter, MBB.instr_end());

    MachineInstr *Blkswap1 = nullptr;
    MachineInstr *Swap1 = nullptr;
    MachineInstr *Roll1 = nullptr;
    MachineInstr *Roll2 = nullptr;
    MachineInstr *Blkswap2 = nullptr;
    MachineInstr *Swap2 = nullptr;

    if (Matcher.match(PushContA, TVM::PUSHCONT_MBB_S) &&
        Matcher.match(PushContB, TVM::PUSHCONT_MBB_S) &&
        Matcher.match(PushContC, TVM::PUSHCONT_MBB_S) &&
        Matcher.match(Blkswap1, TVM::BLKSWAP, 2, 4) &&
        Matcher.match(Swap1, TVM::XCHG_TOP, 1) &&
        Matcher.match(Roll1, TVM::ROLL, 2) &&
        Matcher.match(Roll2, TVM::ROLL, 5) &&
        Matcher.match(Blkswap2, TVM::BLKSWAP, 2, 4) &&
        Matcher.match(Swap2, TVM::XCHG_TOP, 1) &&
        Matcher.match(Ifelse, TVM::IFELSE_S)) {
      MatchFound = true;
      NeedInsertSwapBeforePostDominator = true;
      InstrToRemove[NumInstrToRemove++] = Blkswap1;
      InstrToRemove[NumInstrToRemove++] = Swap1;
      InstrToRemove[NumInstrToRemove++] = Roll1;
      InstrToRemove[NumInstrToRemove++] = Roll2;
      InstrToRemove[NumInstrToRemove++] = Blkswap2;
      InstrToRemove[NumInstrToRemove++] = Swap2;
    }
  }

  if (!MatchFound)
    return false;

  MachineBasicBlock *ContA = PushContA->getOperand(0).getMBB();
  MachineBasicBlock *ContB = PushContB->getOperand(0).getMBB();
  MachineBasicBlock *ContC = PushContC->getOperand(0).getMBB();

  assert(ContA && ContB && ContC &&
         "PUSHCONT_MBB_S should have MachineBasicBlock as an operand #0");

  if (ContA->empty() || ContB->empty()) {
    return false;
  }

  MachineInstr &ContAJmpX = ContA->instr_back();
  MachineInstr &ContBJmpX = ContB->instr_back();

  if (ContAJmpX.getOpcode() != TVM::JMPX_S ||
      ContBJmpX.getOpcode() != TVM::JMPX_S) {
    return false;
  }

  MachineFunction *MF = MBB.getParent();
  TVMFunctionInfo *MFI = MF->getInfo<TVMFunctionInfo>();

  // Check if the C branch is the same for THEN & ELSE
  const std::vector<unsigned> *ContAJmpXRegs =
      MFI->getStackModelSourceRegs(&ContAJmpX);
  const std::vector<unsigned> *ContBJmpXRegs =
      MFI->getStackModelSourceRegs(&ContBJmpX);

  if (!ContAJmpXRegs || !ContBJmpXRegs || ContAJmpXRegs->empty() ||
      ContBJmpXRegs->empty()) {
    return false;
  }

  unsigned AJmpXReg = (*ContAJmpXRegs)[0];
  unsigned BJmpXReg = (*ContBJmpXRegs)[0];

  if (AJmpXReg != BJmpXReg) {
    return false;
  }

  LLVM_DEBUG({
    dbgs() << "  inline IFELSE post-dominator  %bb." + Twine(ContC->getNumber())
           << "\n";
  });

  auto InsertionIter = Matcher.iter();

  // NULL instead of post-dominator continuation
  DebugLoc DL;
  MFI->clearIntermediateData(BuildMI(PushContA, TII.get(TVM::PUSHNULL_S)));
  MFI->clearIntermediateData(
      BuildMI(PushContA, TII.get(TVM::XCHG_TOP)).addImm(1));
  MFI->clearIntermediateData(
      BuildMI(MBB, InsertionIter, DL, TII.get(TVM::POP)).addImm(0));

  // Insert swap for case #2
  if (NeedInsertSwapBeforePostDominator) {
    MFI->clearIntermediateData(
        BuildMI(MBB, InsertionIter, DL, TII.get(TVM::XCHG_TOP)).addImm(1));
  }

  // Copy post-dominator commands after IFELSE
  for (auto &MI : *ContC) {
    MachineInstr &NewMI = MF->CloneMachineInstrBundle(MBB, InsertionIter, MI);
    MFI->cloneMachineInstrIntermediateData(&MI, &NewMI);
  }

  MFI->clearIntermediateData(
      BuildMI(&ContAJmpX, TII.get(TVM::FALLTHROUGH_RETURN)));
  MFI->clearIntermediateData(
      BuildMI(&ContBJmpX, TII.get(TVM::FALLTHROUGH_RETURN)));

  // Swap then / else branches
  MBB.insert(InstrIter, PushContB->removeFromParent());
  --InstrIter;

  PushContC->eraseFromParent();

  for (int i = 0; i < NumInstrToRemove; i++)
    InstrToRemove[i]->eraseFromParent();

  ContAJmpX.eraseFromParent();
  ContBJmpX.eraseFromParent();

  return true;
}

namespace {

bool isContinuationEmpty(const MachineBasicBlock &MBB) {
  if (MBB.empty())
    return true;

  if (MBB.size() > 1)
    return false;

  const MachineInstr &LastInstr = MBB.instr_back();

  if (LastInstr.getOpcode() == TVM::FALLTHROUGH_RETURN)
    return true;

  return false;
}

} // namespace

bool TVMPeephole::runIfElseEmptyBranchRemoving(
    MachineBasicBlock::instr_iterator &InstrIter, MachineBasicBlock &MBB,
    const TargetInstrInfo &TII) {
  MachineInstrMatcher Matcher(InstrIter, MBB.instr_end());

  MachineInstr *PushContA = nullptr;
  MachineInstr *PushContB = nullptr;
  MachineInstr *Ifelse = nullptr;

  if (!Matcher.match(PushContA, TVM::PUSHCONT_MBB_S) ||
      !Matcher.match(PushContB, TVM::PUSHCONT_MBB_S) ||
      !Matcher.match(Ifelse, TVM::IFELSE_S)) {
    return false;
  }

  MachineFunction *MF = MBB.getParent();
  TVMFunctionInfo *MFI = MF->getInfo<TVMFunctionInfo>();

  MachineBasicBlock *ContA = PushContA->getOperand(0).getMBB();
  MachineBasicBlock *ContB = PushContB->getOperand(0).getMBB();

  assert(ContA && ContB &&
         "PUSHCONT_MBB_S should have MachineBasicBlock as an operand #0");

  bool ContAEmpty = isContinuationEmpty(*ContA);
  bool ContBEmpty = isContinuationEmpty(*ContB);

  if (ContAEmpty && ContBEmpty) {
    LLVM_DEBUG({
      dbgs() << "  remove IFELSE (%bb." + Twine(ContA->getNumber())
             << ", %bb." + Twine(ContB->getNumber()) << ")\n";
    });

    InstrIter = Matcher.iter(); // move to the next instruction after IFELSE

    MFI->clearIntermediateData(
        BuildMI(&*PushContA, TII.get(TVM::POP)).addImm(0));

    PushContA->eraseFromParent();
    PushContB->eraseFromParent();
    Ifelse->eraseFromParent();

    return true;
  }

  if (ContAEmpty) {
    LLVM_DEBUG({
      dbgs() << "  remove IFELSE 'then' branch %bb." + Twine(ContA->getNumber())
             << "\n";
    });

    InstrIter++; // move to 'else' branch

    MFI->clearIntermediateData(BuildMI(&*Ifelse, TII.get(TVM::IFNOT_S)));

    PushContA->eraseFromParent();
    Ifelse->eraseFromParent();

    return true;
  }

  if (ContBEmpty) {
    LLVM_DEBUG({
      dbgs() << "  remove IFELSE 'else' branch %bb." + Twine(ContB->getNumber())
             << "\n";
    });

    MFI->clearIntermediateData(BuildMI(&*Ifelse, TII.get(TVM::IF_S)));

    PushContB->eraseFromParent();
    Ifelse->eraseFromParent();

    return true;
  }

  return false;
}

bool TVMPeephole::runIfElseOptimization(MachineBasicBlock &MBB,
                                        const TargetInstrInfo &TII) {
  if (DisableTVMIfElseOpt)
    return false;

  auto InstrIter = MBB.instr_begin();
  bool Changed = false;

  for (; InstrIter != MBB.instr_end();) {
    if (!runIfElseInlining(InstrIter, MBB, TII) &&
        !runIfElseEmptyBranchRemoving(InstrIter, MBB, TII)) {
      ++InstrIter;
      continue;
    }

    Changed = true;
  }

  return Changed;
}

bool TVMPeephole::runOnMachineBasicBlock(MachineBasicBlock &MBB,
                                         const TargetInstrInfo &TII) {
  bool Changed = false;

  while (runMbbInlineOptimization(MBB, TII))
    Changed |= true;

  Changed |= runImplicitReturnOptimization(MBB, TII);
  if (!DisableTVMDropOpt)
    Changed |= runBlkDropCombine(MBB, TII);
  if (!DisableTVMBlkPushOpt)
    Changed |= runBlkPushCombine(MBB, TII);
  if (!DisableTVMShorterFormOpt)
    Changed |= runReplaceWithShorterForm(MBB, TII);

  return Changed;
}

bool TVMPeephole::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG({
    dbgs() << "********** Peephole **********\n"
           << "********** Function: " << MF.getName() << '\n';
  });

  const auto *TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();

  assert(TII && "TargetInstrInfo must be a valid object");

  bool Changed = false;

  for (auto &MBB : MF) {
    Changed |= runOnMachineBasicBlock(MBB, *TII);
  }

  for (auto &MBB : MF) {
    Changed |= runIfElseOptimization(MBB, *TII);
  }

  return Changed;
}
