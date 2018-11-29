//==-- TVM.h - Top-level interface for TVM representation --------*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM TVM backend.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVM_H
#define LLVM_LIB_TARGET_TVM_TVM_H

#include "MCTargetDesc/TVMMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class TVMTargetMachine;
class FunctionPass;
class formatted_raw_ostream;

FunctionPass *createTVMISelDag(TVMTargetMachine &TM,
                               CodeGenOpt::Level OptLevel);
} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVM_H
