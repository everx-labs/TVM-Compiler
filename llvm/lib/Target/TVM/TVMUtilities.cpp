//===------------- TVMUtilities.cpp - TVM Utility Functions ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements several utility functions for TVM.
///
//===----------------------------------------------------------------------===//

#include "TVMUtilities.h"
#include "TVMMachineFunctionInfo.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineLoopInfo.h"

using namespace llvm;

bool TVM::isArgument(const MachineInstr &MI) {
  switch (MI.getOpcode()) {
  case TVM::ARGUMENT:
    return true;
  default:
    return false;
  }
}

// A shortcut overload for BuildMI() function
MachineInstrBuilder llvm::BuildMI(MachineInstr *InsertPoint,
                                  const MCInstrDesc &InstrDesc) {
  return BuildMI(*InsertPoint->getParent(), InsertPoint,
                 InsertPoint->getDebugLoc(), InstrDesc);
}
