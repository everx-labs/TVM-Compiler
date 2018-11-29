//===-- TVMFrameLowering.cpp - TVM Frame Information ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the TVM implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "TVMFrameLowering.h"
#include "TVMInstrInfo.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

bool TVMFrameLowering::hasFP(const MachineFunction &MF) const { return false; }

bool TVMFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
  // TODO: Consider var arg.
  assert(!MF.getFrameInfo().hasVarSizedObjects() &&
         "Var arg is not supported yet");
  return false;
}

void TVMFrameLowering::emitPrologue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
  // TODO: implement.
}

void TVMFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
  // TODO: implement.
}
