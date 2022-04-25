//===-- TVMReplacePhysRegs.cpp - Replace phys regs with virt regs ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements a pass that replaces physical registers with
/// virtual registers.
///
/// LLVM expects certain physical registers, such as a stack pointer. However,
/// TVM doesn't actually have such physical registers. This pass is run
/// once LLVM no longer needs these registers, and replaces them with virtual
/// registers, so they can participate in register stackifying and coloring in
/// the normal way.
///
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/TVMMCTargetDesc.h"
#include "TVM.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMSubtarget.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "tvm-replace-phys-regs"

namespace {
class TVMReplacePhysRegs final : public MachineFunctionPass {
public:
  static char ID; // Pass identification, replacement for typeid
  TVMReplacePhysRegs() : MachineFunctionPass(ID) {}

private:
  StringRef getPassName() const override {
    return "TVM Replace Physical Registers";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;
};
} // end anonymous namespace

char TVMReplacePhysRegs::ID = 0;
INITIALIZE_PASS(TVMReplacePhysRegs, DEBUG_TYPE,
                "Replace physical registers with virtual registers", false,
                false)

FunctionPass *llvm::createTVMReplacePhysRegs() {
  return new TVMReplacePhysRegs();
}

bool TVMReplacePhysRegs::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG({
    dbgs() << "********** Replace Physical Registers **********\n"
           << "********** Function: " << MF.getName() << '\n';
  });

  MachineRegisterInfo &MRI = MF.getRegInfo();
  const auto &TRI = *MF.getSubtarget<TVMSubtarget>().getRegisterInfo();
  bool Changed = false;

  assert(!mustPreserveAnalysisID(LiveIntervalsID) &&
         "LiveIntervals shouldn't be active yet!");
  // We don't preserve SSA or liveness.
  MRI.leaveSSA();
  MRI.invalidateLiveness();

  for (unsigned PReg = TVM::NoRegister + 1; PReg < TVM::NUM_TARGET_REGS;
       ++PReg) {
    // Skip fake registers that are never used explicitly.
    if (PReg == TVM::VALUE_STACK || PReg == TVM::ARGUMENTS)
      continue;

    // Replace explicit uses of the physical register with a virtual register.
    const TargetRegisterClass *RC = TRI.getMinimalPhysRegClass(PReg);
    unsigned VReg = TVM::NoRegister;
    for (auto I = MRI.reg_begin(PReg), E = MRI.reg_end(); I != E;) {
      MachineOperand &MO = *I++;
      if (!MO.isImplicit()) {
        if (VReg == TVM::NoRegister)
          VReg = MRI.createVirtualRegister(RC);
        MO.setReg(VReg);
        if (MO.getParent()->isDebugValue())
          MO.setIsDebug();
        Changed = true;
      }
    }
  }

  return Changed;
}
