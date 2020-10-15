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
  return MI.getOpcode() == TVM::ARGUMENT ||
      MI.getOpcode() == TVM::ARGUMENT_SLICE ||
      MI.getOpcode() == TVM::ARGUMENT_BUILDER ||
      MI.getOpcode() == TVM::ARGUMENT_CELL ||
      MI.getOpcode() == TVM::ARGUMENT_TUPLE;
}

bool TVM::isArgumentNum(const MachineInstr &MI) {
  return MI.getOpcode() == TVM::ARGUMENT_NUM;
}

bool TVM::isConstInt(const MachineInstr &MI) {
  return MI.getOpcode() == TVM::CONST_I257
         || MI.getOpcode() == TVM::CONST_U257;
}

// A shortcut overload for BuildMI() function
MachineInstrBuilder llvm::BuildMI(MachineInstr *InsertPoint,
                                  const MCInstrDesc &InstrDesc) {
  return BuildMI(*InsertPoint->getParent(), InsertPoint,
                 InsertPoint->getDebugLoc(), InstrDesc);
}
