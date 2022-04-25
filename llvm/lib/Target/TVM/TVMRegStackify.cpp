//===-- TVMRegStackify.cpp - Register Stackification ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements a register stacking pass.
///
/// This pass reorders instructions to put register uses and defs in an order
/// such that they form single-use expression trees. Registers fitting this form
/// are then marked as "stackified", meaning references to them are replaced by
/// "push" and "pop" from the value stack.
///
/// This is primarily a code size optimization, since temporary values on the
/// value stack don't need to be named.
///
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "TVMSubtarget.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/CodeGen/LiveIntervals.h"
#include "llvm/CodeGen/MachineBlockFrequencyInfo.h"
#include "llvm/CodeGen/MachineDominators.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfoImpls.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "tvm-reg-stackify"

namespace {
class TVMRegStackify final : public MachineFunctionPass {
  StringRef getPassName() const override { return "TVM Register Stackify"; }

  bool runOnMachineFunction(MachineFunction &MF) override;

public:
  static char ID; // Pass identification, replacement for typeid
  TVMRegStackify() : MachineFunctionPass(ID) {}
};
} // end anonymous namespace

char TVMRegStackify::ID = 0;
INITIALIZE_PASS(TVMRegStackify, DEBUG_TYPE,
                "Reorder instructions to use the TVM value stack", false, false)

FunctionPass *llvm::createTVMRegStackify() { return new TVMRegStackify(); }

bool TVMRegStackify::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(dbgs() << "********** Register Stackifying **********\n"
                       "********** Function: "
                    << MF.getName() << '\n');

  bool Changed = false;
  return Changed;
}
