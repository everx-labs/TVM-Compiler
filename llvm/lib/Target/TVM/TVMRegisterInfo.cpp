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
  Reserved.set(TVM::DUMMYRESERVED);
  return Reserved;
}

unsigned TVMRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return TVM::DUMMY;
}

const MCPhysReg *
TVMRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  static const MCPhysReg CalleeSavedRegs[] = {0};
  return CalleeSavedRegs;
}

void TVMRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                          int SPAdj, unsigned FIOperandNum,
                                          RegScavenger *RS) const {
  llvm_unreachable("Unimplemented");
}
