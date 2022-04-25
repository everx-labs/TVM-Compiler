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
#include "TVMExtras.h"
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

Register TVMRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
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
  LLVMContext &C = MF.getFunction().getContext();
  MachineRegisterInfo &MRI = MF.getRegInfo();
  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  const auto *TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();

  int64_t FrameOffset = MFI.getObjectOffset(FrameIndex);

  assert(FrameOffset >= 0 && "FrameOffset < 0");
  assert(FrameOffset < static_cast<int64_t>(MFI.getStackSize()) &&
         "FrameOffset overflows stack size");
  assert(MFI.getObjectSize(FrameIndex) != 0 &&
         "We assume that variable-sized objects have already been lowered, "
         "and don't use FrameIndex operands.");

  // %RegFrameBase:i257 = GETGLOB i257 5
  unsigned RegFrameBase = MRI.createVirtualRegister(&TVM::I257RegClass);
  BuildMI(MBB, *II, II->getDebugLoc(), TII->get(TVM::GETGLOB), RegFrameBase)
      .addImm(5);

  unsigned RegFrameOffset;
  if (FrameOffset == 0)
    RegFrameOffset = RegFrameBase;
  else if (isInt<8>(FrameOffset)) {
    // %RegFrameOffset:i257 = ADDCONST %RegFrameBase:i257, i257 FrameOffset
    RegFrameOffset = MRI.createVirtualRegister(&TVM::I257RegClass);
    BuildMI(MBB, *II, II->getDebugLoc(), TII->get(TVM::ADDCONST), RegFrameOffset)
        .addReg(RegFrameBase)
        .addImm(FrameOffset);
  } else {
    // %RegConst:i257 = PUSHINT i257 FrameOffset
    unsigned RegConst = MRI.createVirtualRegister(&TVM::I257RegClass);
    BuildMI(MBB, *II, II->getDebugLoc(), TII->get(TVM::CONST_I257), RegConst)
        .addCImm(cimm(C, FrameOffset));
    // %RegFrameOffset:i257 = ADD %RegFrameBase:i257, %RegConst:i257
    RegFrameOffset = MRI.createVirtualRegister(&TVM::I257RegClass);
    BuildMI(MBB, *II, II->getDebugLoc(), TII->get(TVM::ADD), RegFrameOffset)
        .addReg(RegFrameBase)
        .addReg(RegConst);
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(RegFrameOffset, false);
}
