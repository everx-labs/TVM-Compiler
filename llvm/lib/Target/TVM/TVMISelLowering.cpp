//===-- TVMISelLowering.cpp - TVM DAG Lowering Implementation  ------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the TVMTargetLowering class.
//
//===----------------------------------------------------------------------===//

#include "TVMISelLowering.h"
#include "TVM.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMSubtarget.h"
#include "TVMTargetMachine.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalAlias.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "tvm-lower"

TVMTargetLowering::TVMTargetLowering(const TargetMachine &TM,
                                     const TVMSubtarget &STI)
    : TargetLowering(TM) {

  // Set up the register classes.
  // addRegisterClass(MVT::i8,  &TVM::GR8RegClass);
  // addRegisterClass(MVT::i16, &TVM::GR16RegClass);

  // Compute derived properties from the register classes
  computeRegisterProperties(STI.getRegisterInfo());

  setMinFunctionAlignment(1);
  setPrefFunctionAlignment(1);
}

SDValue TVMTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  default:
    llvm_unreachable("unimplemented operand");
  }
}

const char *TVMTargetLowering::getTargetNodeName(unsigned Opcode) const {
  return nullptr;
}

//===----------------------------------------------------------------------===//
//                       TVM Inline Assembly Support
//===----------------------------------------------------------------------===//
// TODO: implement
