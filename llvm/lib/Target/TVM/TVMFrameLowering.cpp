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
  LLVMContext &C = MF.getFunction().getContext();
  const auto *TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();
  auto &MFI = MF.getFrameInfo();
  auto &MRI = MF.getRegInfo();
  uint64_t StackSize = MFI.getStackSize();
  if (StackSize == 0 && !TraceCalls)
    return;
  if (MF.getFunction().hasFnAttribute("tvm_raw_func") && StackSize) {
    report_fatal_error("Raw function requires stack");
    return;
  }

  auto InsertPt =
      llvm::find_if(MBB, [&](auto &pt) { return !TVM::isArgument(pt); });

  DebugLoc DL;
  if (TraceCalls) {
    const Function &Fn = MF.getFunction();
    if (MBB.getBasicBlock() == &Fn.getEntryBlock())
      BuildMI(MBB, InsertPt, DL, TII->get(TVM::LOGSTR)).addGlobalAddress(&Fn);
  }

  // %RegFrameBase:i257 = GETGLOB i257 5
  unsigned RegFrameBase = MRI.createVirtualRegister(&TVM::I257RegClass);
  BuildMI(MBB, InsertPt, DL, TII->get(TVM::GETGLOB), RegFrameBase)
      .addImm(5);

  unsigned RegFrameBaseNew = MRI.createVirtualRegister(&TVM::I257RegClass);
  if (StackSize <= 128) {
    // %RegFrameBaseNew:i257 = SUB %RegFrameBase:i257, %RegStSize:i257
    BuildMI(MBB, InsertPt, DL, TII->get(TVM::ADDCONST), RegFrameBaseNew)
        .addReg(RegFrameBase)
        .addImm(-StackSize);
  } else {
    // %RegStSize:i257 = PUSHINT i257 StackSize
    unsigned RegStSize = MRI.createVirtualRegister(&TVM::I257RegClass);
    BuildMI(MBB, InsertPt, DL, TII->get(TVM::CONST_I257), RegStSize)
        .addCImm(cimm(C, StackSize));
    // %RegFrameBaseNew:i257 = SUB %RegFrameBase:i257, %RegStSize:i257
    BuildMI(MBB, InsertPt, DL, TII->get(TVM::SUB), RegFrameBaseNew)
        .addReg(RegFrameBase)
        .addReg(RegStSize);
  }

  // SETGLOB i257 5, %RegFrameBaseNew:i257
  BuildMI(MBB, InsertPt, DL, TII->get(TVM::SETGLOB))
      .addReg(RegFrameBaseNew)
      .addImm(5);
}

void TVMFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
  LLVMContext &C = MF.getFunction().getContext();
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

  // %RegFrameBase:i257 = GETGLOB i257 5
  unsigned RegFrameBase = MRI.createVirtualRegister(&TVM::I257RegClass);
  BuildMI(MBB, InsertPt, DL, TII->get(TVM::GETGLOB), RegFrameBase)
      .addImm(5);

  unsigned RegFrameBaseNew = MRI.createVirtualRegister(&TVM::I257RegClass);
  if (StackSize <= 127) {
    // %RegFrameBaseNew:i257 = SUB %RegFrameBase:i257, %RegStSize:i257
    BuildMI(MBB, InsertPt, DL, TII->get(TVM::ADDCONST), RegFrameBaseNew)
        .addReg(RegFrameBase)
        .addImm(StackSize);
  } else {
    // %RegStSize:i257 = PUSHINT i257 StackSize
    unsigned RegStSize = MRI.createVirtualRegister(&TVM::I257RegClass);
    BuildMI(MBB, InsertPt, DL, TII->get(TVM::CONST_I257), RegStSize)
        .addCImm(cimm(C, StackSize));
    // %RegFrameBaseNew:i257 = SUB %RegFrameBase:i257, %RegStSize:i257
    BuildMI(MBB, InsertPt, DL, TII->get(TVM::ADD), RegFrameBaseNew)
        .addReg(RegFrameBase)
        .addReg(RegStSize);
  }

  // SETGLOB i257 5, %RegFrameBaseNew:i257
  BuildMI(MBB, InsertPt, DL, TII->get(TVM::SETGLOB))
      .addReg(RegFrameBaseNew)
      .addImm(5);
}
