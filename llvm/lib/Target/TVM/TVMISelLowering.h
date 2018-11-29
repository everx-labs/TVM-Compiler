//===-- TVMISelLowering.h - TVM DAG Lowering Interface ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that TVM uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMISELLOWERING_H
#define LLVM_LIB_TARGET_TVM_TVMISELLOWERING_H

#include "TVM.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

class TVMSubtarget;
class TVMTargetLowering : public TargetLowering {
public:
  explicit TVMTargetLowering(const TargetMachine &TM, const TVMSubtarget &STI);

  //TODO: Copied from MSP430. Check.
  MVT getScalarShiftAmountTy(const DataLayout &, EVT) const override {
    return MVT::i8;
  }

  /// LowerOperation - Provide custom lowering hooks for some operations.
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

  /// getTargetNodeName - This method returns the name of a target specific
  /// DAG node.
  const char *getTargetNodeName(unsigned Opcode) const override;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMISELLOWERING_H
