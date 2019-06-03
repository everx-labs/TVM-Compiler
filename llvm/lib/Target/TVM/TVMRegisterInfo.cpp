//===-- TVMRegisterInfo.cpp - TVM Register Information --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the TVM implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "TVMRegisterInfo.h"
#include "TVM.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMTargetMachine.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

#define DEBUG_TYPE "tvm-reg-info"

#define GET_REGINFO_TARGET_DESC
#include "TVMGenRegisterInfo.inc"

TVMRegisterInfo::TVMRegisterInfo() : TVMGenRegisterInfo(TVM::DUMMY) {}

BitVector TVMRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  Reserved.set(TVM::SP);
  return Reserved;
}

unsigned TVMRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return TVM::SP;
}

const MCPhysReg *
TVMRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  static const MCPhysReg CalleeSavedRegs[] = {0};
  return CalleeSavedRegs;
}

void TVMRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                          int SPAdj, unsigned FIOperandNum,
                                          RegScavenger *RS) const {
  assert(SPAdj == 0);
  MachineInstr &MI = *II;

  MachineBasicBlock &MBB = *MI.getParent();
  MachineFunction &MF = *MBB.getParent();
  MachineRegisterInfo &MRI = MF.getRegInfo();
  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  const auto *TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();

  int64_t FrameOffset = MFI.getStackSize() + MFI.getObjectOffset(FrameIndex);

  assert(MFI.getObjectSize(FrameIndex) != 0 &&
         "We assume that variable-sized objects have already been lowered, "
         "and don't use FrameIndex operands.");

  unsigned OffsetOperand = MRI.createVirtualRegister(&TVM::I64RegClass);
  BuildMI(MBB, *II, II->getDebugLoc(), TII->get(TVM::CONST_I64), OffsetOperand)
      .addImm(FrameOffset);

  unsigned FIRegOperand = MRI.createVirtualRegister(&TVM::I64RegClass);
  BuildMI(MBB, *II, II->getDebugLoc(), TII->get(TVM::CALL_FRAMEIDX),
          FIRegOperand)
      .addReg(OffsetOperand);

  MI.getOperand(FIOperandNum).ChangeToRegister(FIRegOperand, false);
}
