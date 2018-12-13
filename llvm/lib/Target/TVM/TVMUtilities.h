//===---------- TVMUtilities - TVM Utility Functions -----------*- C++ -*-====//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the declaration of the TVM-specific utility functions.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMUTILITIES_H
#define LLVM_LIB_TARGET_TVM_TVMUTILITIES_H

#include "llvm/CodeGen/MachineBasicBlock.h"

namespace llvm {

class TVMFunctionInfo;

namespace TVM {

bool isArgument(const MachineInstr &MI);
} // end namespace TVM

} // end namespace llvm

#endif
