//==- TVMFrameLowering.h - Define frame lowering for TVM --------*- C++ -*--==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares TVM implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMFRAMELOWERING_H
#define LLVM_LIB_TARGET_TVM_TVMFRAMELOWERING_H

#include "TVM.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class TVMFrameLowering : public TargetFrameLowering {
protected:

public:
  //TODO: Copied from MSP430. Check.
  explicit TVMFrameLowering()
      : TargetFrameLowering(TargetFrameLowering::StackGrowsUp, 8, 0) {}

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  bool hasFP(const MachineFunction &MF) const override;
  bool hasReservedCallFrame(const MachineFunction &MF) const override;
};

} // End llvm namespace

#endif // LLVM_LIB_TARGET_TVM_TVMFRAMELOWERING_H
