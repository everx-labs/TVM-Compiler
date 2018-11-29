//===-- TVMInstrInfo.h - TVM Instruction Information ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the TVM implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMINSTRINFO_H
#define LLVM_LIB_TARGET_TVM_TVMINSTRINFO_H

#include "TVMRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "TVMGenInstrInfo.inc"

namespace llvm {

class TVMSubtarget;

class TVMInstrInfo : public TVMGenInstrInfo {
public:
  explicit TVMInstrInfo(TVMSubtarget &STI);

  /// getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  /// such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  ///
  const TargetRegisterInfo &getRegisterInfo() const { return RI; }

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
                   const DebugLoc &DL, unsigned DestReg, unsigned SrcReg,
                   bool KillSrc) const override;

  unsigned getInstSizeInBytes(const MachineInstr &MI) const override;

  int64_t getFramePoppedByCallee(const MachineInstr &I) const {
    assert(isFrameInstr(I) && "Not a frame instruction");
    assert(I.getOperand(1).getImm() >= 0 && "Size must not be negative");
    return I.getOperand(1).getImm();
  }

private:
  const TVMRegisterInfo RI;
  virtual void anchor();
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMINSTRINFO_H
