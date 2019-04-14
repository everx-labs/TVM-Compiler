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

bool TVMPeephole::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG({
    dbgs() << "********** Peephole **********\n"
           << "********** Function: " << MF.getName() << '\n';
  });

  const auto &TII = *MF.getSubtarget<TVMSubtarget>().getInstrInfo();

  bool Changed = false;

  for (auto &MBB : MF) {
    auto& MI = MBB.back();
    if (MI.isReturn())
      Changed |= MaybeOptimizeReturn(MI, TII);
  }


  return Changed;
}
