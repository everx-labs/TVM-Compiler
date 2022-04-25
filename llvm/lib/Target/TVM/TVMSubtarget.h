//===-- TVMSubtarget.h - Define TVM subtarget -------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the TVM specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMSUBTARGET_H
#define LLVM_LIB_TARGET_TVM_TVMSUBTARGET_H

#include "TVMFrameLowering.h"
#include "TVMISelLowering.h"
#include "TVMInstrInfo.h"
#include "TVMRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/DataLayout.h"
#include <string>

#define GET_SUBTARGETINFO_HEADER
#include "TVMGenSubtargetInfo.inc"

namespace llvm {
class StringRef;

class TVMSubtarget : public TVMGenSubtargetInfo {
public:
  TVMSubtarget(const Triple &TT, const std::string &CPU, const std::string &FS,
               const TargetMachine &TM);

  TVMSubtarget &initializeSubtargetDependencies(StringRef CPU, StringRef FS);

  void ParseSubtargetFeatures(StringRef CPU, StringRef TuneCPU, StringRef FS);

  const TargetFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }
  const TVMInstrInfo *getInstrInfo() const override { return &InstrInfo; }
  const TargetRegisterInfo *getRegisterInfo() const override {
    return &InstrInfo.getRegisterInfo();
  }
  const TVMTargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }
  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
    return &TSInfo;
  }

private:
  virtual void anchor();
  TVMFrameLowering FrameLowering;
  TVMInstrInfo InstrInfo;
  TVMTargetLowering TLInfo;
  SelectionDAGTargetInfo TSInfo;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMSUBTARGET_H
