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
#include "TVMExtras.h"
#include "TVMInstrInfo.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMSubtarget.h"
#include "TVMUtilities.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

//===----------------------------------------------------------------------===//
// Command line options for TVM
//===----------------------------------------------------------------------===//
// Enable debug trace for function calls
static cl::opt<bool> TraceCalls("tvm-trace-calls", cl::Hidden,
                                cl::desc("Trace function calls"),
                                cl::init(false));

bool TVMFrameLowering::hasFP(const MachineFunction &MF) const { return false; }

bool TVMFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
  // TODO: Consider var arg.
  assert(!MF.getFrameInfo().hasVarSizedObjects() &&
         "Var arg is not supported yet");
  return false;
}

void TVMFrameLowering::emitPrologue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
  const auto *TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();
  auto &MFI = MF.getFrameInfo();
  auto &MRI = MF.getRegInfo();
  uint64_t StackSize = MFI.getStackSize();
  if (StackSize == 0 && !TraceCalls)
    return;

  auto InsertPt =
      llvm::find_if(MBB, [&](auto &pt) { return !TVM::isArgument(pt); });

  DebugLoc DL;
  if (TraceCalls) {
    const Function &Fn = MF.getFunction();
    if (MBB.getBasicBlock() == &Fn.getEntryBlock())
      BuildMI(MBB, InsertPt, DL, TII->get(TVM::LOGSTR)).addGlobalAddress(&Fn);
  }
  unsigned StSizeReg = MRI.createVirtualRegister(&TVM::I64RegClass);
  BuildMI(MBB, InsertPt, DL, TII->get(TVM::CONST_I64), StSizeReg)
      .addImm(StackSize);
  BuildMI(MBB, InsertPt, DL, TII->get(TVM::CALL_ENTER)).addReg(StSizeReg);
}

void TVMFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
  const auto *TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();
  auto &MFI = MF.getFrameInfo();
  auto &MRI = MF.getRegInfo();
  uint64_t StackSize = MFI.getStackSize();
  if (StackSize == 0 && !TraceCalls)
    return;

  auto InsertPt = MBB.getFirstTerminator();
  DebugLoc DL;

  if (InsertPt != MBB.end())
    DL = InsertPt->getDebugLoc();

  unsigned StSizeReg = MRI.createVirtualRegister(&TVM::I64RegClass);
  BuildMI(MBB, InsertPt, DL, TII->get(TVM::CONST_I64), StSizeReg)
      .addImm(StackSize);
  BuildMI(MBB, InsertPt, DL, TII->get(TVM::CALL_LEAVE)).addReg(StSizeReg);
}
