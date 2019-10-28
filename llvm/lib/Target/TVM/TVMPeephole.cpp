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
  bool runMbbInlineOptimization(MachineBasicBlock &MBB);

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
  if (MBB.empty())
    return false;

  auto &MI = MBB.back();
  return MI.isReturn() && MaybeOptimizeReturn(MI, TII);
}

bool TVMPeephole::runMbbInlineOptimization(MachineBasicBlock &MBB) {
  if (MBB.empty())
    return false;

  auto InstrIter = MBB.instr_rbegin();
  auto &JmpX = *InstrIter;

  if (JmpX.getOpcode() != TVM::JMPX_S)
    return false;

  ++InstrIter;

  if (InstrIter == MBB.instr_rend())
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

  for (auto &MI : SourceMBB) {
    MF->CloneMachineInstrBundle(MBB, InsertionIter, MI);
  }

  PushContMBB.eraseFromParent();
  JmpX.eraseFromParent();

  return true;
}

bool TVMPeephole::runOnMachineBasicBlock(MachineBasicBlock &MBB,
                                         const TargetInstrInfo &TII) {
  bool Changed = false;

  Changed |= runImplicitReturnOptimization(MBB, TII);

  while (runMbbInlineOptimization(MBB))
    Changed |= true;

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

  return Changed;
}
