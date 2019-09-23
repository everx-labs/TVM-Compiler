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
#include "TVMMachineInstrMatcher.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMSubtarget.h"
#include "TVMUtilities.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "tvm-peephole"

static cl::opt<bool> DisableTVMReturnOpt(
    "disable-tvm-return-opt", cl::Hidden,
    cl::desc("TVM: Disable return optimizations."),
    cl::init(false));

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
  bool runMbbInlineOptimization(MachineBasicBlock &MBB,
                                const TargetInstrInfo &TII);
  bool runIfElseInlineOptimization(MachineBasicBlock &MBB,
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

static bool
MaybeOptimizeReturn(MachineInstr &MI, const TargetInstrInfo& TII) {
  MachineBasicBlock* MBB = MI.getParent();
  MachineFunction* MF = MBB->getParent();
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

bool TVMPeephole::runImplicitReturnOptimization(MachineBasicBlock &MBB,
                                                const TargetInstrInfo &TII) {
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

  for (; InstrIter != MBB.instr_rend() && !PushContMBBFound;) {
    switch (InstrIter->getOpcode()) {
    case TVM::PUSHCONT_MBB_S:
      PushContMBBFound = true;
      break;
    // Auxilary stack operations should be skipped here and then converted
    // below to transform the MI sequence to (PUSHCONT, JMPX) form
    case TVM::NIP:
      ++InstrIter;
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

  // Convert MI sequence to expected form (PUSHCONT, JMPX) and move all
  // stack operations between PUSHCONT and JMPX before the PUSHCONT
  // (the list of possible transformations may be extended in future)
  for (InstrIter = FirstStackInstrIter; InstrIter != LastStackInstrIter;) {
    switch (InstrIter->getOpcode()) {
    case TVM::NIP:
      BuildMI(&*InsertionIter, TII.get(TVM::DROP));
      break;
    default:
      break;
    }

    auto Next = InstrIter;
    ++Next;
    InstrIter->eraseFromParent();
    InstrIter = Next;
  }

  for (auto &MI : SourceMBB) {
    MF->CloneMachineInstrBundle(MBB, InsertionIter, MI);
  }

  PushContMBB.eraseFromParent();
  JmpX.eraseFromParent();

  return true;
}

bool TVMPeephole::runIfElseInlineOptimization(MachineBasicBlock &MBB,
                                              const TargetInstrInfo &TII) {
  auto InstrIter = MBB.instr_begin();
  bool Changed = false;

  for (; InstrIter != MBB.instr_end(); ++InstrIter) {
    MachineInstrMatcher Matcher(InstrIter, MBB.instr_end());

    MachineInstr *PushContA = nullptr;
    MachineInstr *PushContB = nullptr;
    MachineInstr *PushContC = nullptr;
    MachineInstr *Roll = nullptr;
    MachineInstr *Blkswap = nullptr;
    MachineInstr *Swap = nullptr;
    MachineInstr *Ifelse = nullptr;

    if (!Matcher.match(PushContA, TVM::PUSHCONT_MBB_S) ||
        !Matcher.match(PushContB, TVM::PUSHCONT_MBB_S) ||
        !Matcher.match(PushContC, TVM::PUSHCONT_MBB_S) ||
        !Matcher.match(Roll, TVM::ROLL, 3) ||
        !Matcher.match(Blkswap, TVM::BLKSWAP, 2, 2) ||
        !Matcher.match(Swap, TVM::SWAP) ||
        !Matcher.match(Ifelse, TVM::IFELSE_S)) {
      continue;
    }

    MachineBasicBlock *ContA = PushContA->getOperand(0).getMBB();
    MachineBasicBlock *ContB = PushContB->getOperand(0).getMBB();
    MachineBasicBlock *ContC = PushContC->getOperand(0).getMBB();

    assert(ContA && ContB && ContC &&
           "PUSHCONT_MBB_S should have MachineBasicBlock as an operand #0");

    if (ContA->empty() || ContB->empty()) {
      continue;
    }

    MachineInstr &ContAJmpX = ContA->instr_back();
    MachineInstr &ContBJmpX = ContB->instr_back();

    if (ContAJmpX.getOpcode() != TVM::JMPX_S ||
        ContBJmpX.getOpcode() != TVM::JMPX_S) {
      continue;
    }

    LLVM_DEBUG({
      dbgs() << "  inline IFELSE post-dominator  %bb." +
                    Twine(ContC->getNumber())
             << "\n";
    });

    InstrIter = --Matcher.iter();

    MachineFunction *MF = MBB.getParent();
    auto InsertionIter = InstrIter;
    InsertionIter++;

    for (auto &MI : *ContC) {
      MF->CloneMachineInstrBundle(MBB, InsertionIter, MI);
    }

    BuildMI(&ContAJmpX, TII.get(TVM::FALLTHROUGH_RETURN));
    BuildMI(&ContBJmpX, TII.get(TVM::FALLTHROUGH_RETURN));

    PushContC->eraseFromParent();
    Roll->eraseFromParent();
    Blkswap->eraseFromParent();
    Swap->eraseFromParent();

    ContAJmpX.eraseFromParent();
    ContBJmpX.eraseFromParent();

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
    Changed |= runIfElseInlineOptimization(MBB, *TII);
  }

  return Changed;
}
