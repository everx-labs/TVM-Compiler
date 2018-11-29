//===-- TVMTargetInfo.cpp - TVM Target Implementation ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target &llvm::getTheTVMTarget() {
  static Target TheTVMTarget;
  return TheTVMTarget;
}

extern "C" void LLVMInitializeTVMTargetInfo() {
  RegisterTarget<Triple::tvm> X(getTheTVMTarget(), "tvm", "TVM [experimental]",
                                "TVM");
}
