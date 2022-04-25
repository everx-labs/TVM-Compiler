//===-- TVMSubtarget.cpp - TVM Subtarget Information ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the TVM specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "TVMSubtarget.h"
#include "TVM.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "tvm-subtarget"

void TVMSubtarget::anchor() {}

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "TVMGenSubtargetInfo.inc"

TVMSubtarget &TVMSubtarget::initializeSubtargetDependencies(StringRef CPU,
                                                            StringRef FS) {
  (void) FS;
  std::string CPUName = CPU.str();
  if (CPUName.empty())
    CPUName = "tvm";

  ParseSubtargetFeatures(CPUName, CPUName, FS);

  return *this;
}

TVMSubtarget::TVMSubtarget(const Triple &TT, const std::string &CPU,
                           const std::string &FS, const TargetMachine &TM)
    : TVMGenSubtargetInfo(TT, CPU, CPU, FS), FrameLowering(),
      InstrInfo(initializeSubtargetDependencies(CPU, FS)), TLInfo(TM, *this) {}
