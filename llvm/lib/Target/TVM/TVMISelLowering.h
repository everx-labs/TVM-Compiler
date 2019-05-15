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

namespace TVMISD {

enum NodeType : unsigned {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,
#define HANDLE_NODETYPE(NODE) NODE,
#include "TVMISD.def"
#undef HANDLE_NODETYPE
};

} // namespace TVMISD

class TVMSubtarget;
class TVMTargetLowering : public TargetLowering {
public:
  explicit TVMTargetLowering(const TargetMachine &TM, const TVMSubtarget &STI);

  MVT getScalarShiftAmountTy(const DataLayout &, EVT) const override {
    return MVT::i64;
  }

  /// LowerOperation - Provide custom lowering hooks for some operations.
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
  /// Provide custom lowering hooks for intrinsics that unroll to more than one
  /// MIR instructions.
  SDValue LowerINTRINSIC_W_CHAIN(SDValue Op, SelectionDAG &DAG) const;

  /// getTargetNodeName - This method returns the name of a target specific
  /// DAG node.
  const char *getTargetNodeName(unsigned Opcode) const override;

  SDValue PerformDAGCombine(SDNode *N, DAGCombinerInfo &DCI) const override;

private:
  SDValue LowerCall(CallLoweringInfo &CLI,
                    SmallVectorImpl<SDValue> &InVals) const override;
  bool CanLowerReturn(CallingConv::ID CallConv, MachineFunction &MF,
                      bool isVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      LLVMContext &Context) const override;
  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &dl,
                      SelectionDAG &DAG) const override;
  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &DL, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  // Custom lowering hooks.
  SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerExternalSymbol(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFrameIndex(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerCopyToReg(SDValue Op, SelectionDAG &DAG) const;

  SDValue LowerSMUL_LOHI(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerUMUL_LOHI(SDValue Op, SelectionDAG &DAG) const;

  // Save/restore C0 for return lowering
  SDValue SaveC0(SDValue Chain, const SDLoc &DL, SelectionDAG& DAG) const;
  SDValue RestoreC0(SDValue Chain, const SDLoc &DL, SelectionDAG& DAG) const;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMISELLOWERING_H
