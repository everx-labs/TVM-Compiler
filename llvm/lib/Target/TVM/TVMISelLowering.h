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
#include <set>

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
    return MVT::i257;
  }
  /// Return the ValueType of the result of SETCC operations.
  EVT getSetCCResultType(const DataLayout &DL, LLVMContext &Context,
                         EVT VT) const override {
    return MVT::i257;
  }

  /// LowerOperation - Provide custom lowering hooks for some operations.
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

  /// Replace the results of node with an illegal result
  /// type with new values built out of custom code.
  void ReplaceNodeResults(SDNode *N, SmallVectorImpl<SDValue> &Results,
                          SelectionDAG &DAG) const override;

  /// Places new result values for the node in Results (their number
  /// and types must exactly match those of the original return values of
  /// the node), or leaves Results empty, which indicates that the node is not
  /// to be custom lowered after all.
  void LowerOperationWrapper(SDNode *N,
                             SmallVectorImpl<SDValue> &Results,
                             SelectionDAG &DAG) const override;

  /// Provide custom lowering hooks for intrinsics that unroll to more than one
  /// MIR instructions.
  SDValue LowerINTRINSIC_W_CHAIN(SDValue Op, SelectionDAG &DAG) const;

  SDValue LowerINTRINSIC_WO_CHAIN(SDValue Op, SelectionDAG &DAG) const;

  // Provide custom lowering for leading zeros and leading ones calculation
  SDValue LowerLeadingBits(SDValue Op, SelectionDAG &DAG) const;

  /// getTargetNodeName - This method returns the name of a target specific
  /// DAG node.
  const char *getTargetNodeName(unsigned Opcode) const override;

  SDValue PerformDAGCombine(SDNode *N, DAGCombinerInfo &DCI) const override;

  EVT getOptimalMemOpType(uint64_t Size, unsigned DstAlign, unsigned SrcAlign,
                          bool IsMemset, bool ZeroMemset, bool MemcpyStrSrc,
                          MachineFunction &MF) const override;

  /// Returns true if the target allows unaligned memory accesses of the
  /// specified type.
  bool allowsMisalignedMemoryAccesses(EVT VT, unsigned AddrSpace = 0,
                                      unsigned Align = 1,
                                      bool *Fast = nullptr) const override;
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
  SDValue LowerLoad(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerStore(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerExternalSymbol(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerBR(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerBRCOND(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFrameIndex(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerCopyToReg(SDValue Op, SelectionDAG &DAG) const;

  SDValue LowerSMUL_LOHI(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerUMUL_LOHI(SDValue Op, SelectionDAG &DAG) const;

  // Save/restore C0 for return lowering
  SDValue SaveC0(SDValue Chain, const SDLoc &DL, SelectionDAG& DAG) const;
  SDValue RestoreC0(SDValue Chain, const SDLoc &DL, SelectionDAG& DAG) const;

  bool hasPushC0Predecessor(SDNode *Node) const;
  mutable std::set<int> HasNoPushC0PredecessorCache;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMISELLOWERING_H
