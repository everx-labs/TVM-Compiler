//===-- TVMISelLowering.cpp - TVM DAG Lowering Implementation  ------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements the TVMTargetLowering class.
///
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
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/IR/DiagnosticPrinter.h"
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
  addRegisterClass(MVT::i64, &TVM::I64RegClass);

  // Compute derived properties from the register classes
  computeRegisterProperties(STI.getRegisterInfo());

  setOperationAction(ISD::GlobalAddress, MVT::i64, Custom);

  // Custom lowering for intrinsics that unrolls into more than one
  // MIR instructions.
  setOperationAction(ISD::INTRINSIC_W_CHAIN, MVT::Other, Custom);
  setOperationAction(ISD::ABS, MVT::i64, Legal);
  setOperationAction(ISD::SMAX, MVT::i64, Legal);
  setOperationAction(ISD::SMIN, MVT::i64, Legal);

  setMinFunctionAlignment(1);
  setPrefFunctionAlignment(1);

  // Expand these forms; we pattern-match the forms that we can handle in isel.
  for (auto Op : {ISD::BR_CC, ISD::SELECT_CC})
    setOperationAction(Op, MVT::i64, Expand);
}

//===----------------------------------------------------------------------===//
//                      Calling Convention Implementation
//===----------------------------------------------------------------------===//

static void fail(const SDLoc &DL, SelectionDAG &DAG, const char *msg) {
  MachineFunction &MF = DAG.getMachineFunction();
  DAG.getContext()->diagnose(
      DiagnosticInfoUnsupported(MF.getFunction(), msg, DL.getDebugLoc()));
}

/// Test whether the given calling convention is supported.
static bool CallingConvSupported(CallingConv::ID CallConv) {
  // We currently support the language-independent target-independent
  // conventions. We don't yet have a way to annotate calls with properties like
  // "cold", and we don't have any call-clobbered registers, so these are mostly
  // all handled the same.
  return CallConv == CallingConv::C || CallConv == CallingConv::Fast ||
         CallConv == CallingConv::Cold ||
         CallConv == CallingConv::PreserveMost ||
         CallConv == CallingConv::PreserveAll ||
         CallConv == CallingConv::CXX_FAST_TLS;
}

SDValue TVMTargetLowering::LowerCall(CallLoweringInfo &CLI,
                                     SmallVectorImpl<SDValue> &InVals) const {
  SelectionDAG &DAG = CLI.DAG;
  SDLoc DL = CLI.DL;
  SDValue Chain = CLI.Chain;
  SDValue Callee = CLI.Callee;
  MachineFunction &MF = DAG.getMachineFunction();

  CallingConv::ID CallConv = CLI.CallConv;
  if (!CallingConvSupported(CallConv))
    fail(DL, DAG,
         "TVM doesn't support language-specific or target-specific "
         "calling conventions yet");
  if (CLI.IsPatchPoint)
    fail(DL, DAG, "TVM doesn't support patch point yet");

  // TVM doesn't currently support explicit tail calls. If they are
  // required, fail. Otherwise, just disable them.
  if ((CallConv == CallingConv::Fast && CLI.IsTailCall &&
       MF.getTarget().Options.GuaranteedTailCallOpt) ||
      (CLI.CS && CLI.CS.isMustTailCall()))
    fail(DL, DAG, "TVM doesn't support tail call yet");
  CLI.IsTailCall = false;

  SmallVectorImpl<ISD::InputArg> &Ins = CLI.Ins;
  if (Ins.size() > 1)
    fail(DL, DAG, "TVM doesn't support more than 1 returned value yet");

  SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
  SmallVectorImpl<SDValue> &OutVals = CLI.OutVals;
  for (unsigned i = 0; i < Outs.size(); ++i) {
    const ISD::OutputArg &Out = Outs[i];
    assert(Out.VT.SimpleTy == MVT::SimpleValueType::i64 &&
           "only i64 is supported");
  }

  // Compute the operands for the CALLn node.
  SmallVector<SDValue, 16> Ops;
  Ops.push_back(Chain);
  Ops.push_back(Callee);
  Ops.append(OutVals.begin(), OutVals.end());

  SmallVector<EVT, 8> InTys;
  for (const auto &In : Ins) {
    // TODO: add checks for In.Flags (copy from
    // WebAssemblyTargetLowering::LowerCall())
    assert(In.VT.SimpleTy == MVT::SimpleValueType::i64 &&
           "only i64 is supported");
    InTys.push_back(In.VT);
  }
  InTys.push_back(MVT::Other);

  SDVTList InTyList = DAG.getVTList(InTys);
  SDValue Res = DAG.getNode(Ins.empty() ? TVMISD::CALL0 : TVMISD::CALL1, DL,
                            InTyList, Ops);
  // TODO: investigate chaining logic (copied from WebAssembly)
  if (Ins.empty()) {
    Chain = Res;
  } else {
    InVals.push_back(Res);
    Chain = Res.getValue(1);
  }
  return Chain;
}

bool TVMTargetLowering::CanLowerReturn(
    CallingConv::ID /*CallConv*/, MachineFunction & /*MF*/, bool /*IsVarArg*/,
    const SmallVectorImpl<ISD::OutputArg> &Outs,
    LLVMContext & /*Context*/) const {
  // TVM can't currently handle returning tuples.
  return Outs.size() <= 1;
}

SDValue
TVMTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                               bool /*IsVarArg*/,
                               const SmallVectorImpl<ISD::OutputArg> &Outs,
                               const SmallVectorImpl<SDValue> &OutVals,
                               const SDLoc &DL, SelectionDAG &DAG) const {
  assert(Outs.size() <= 1 && "TVM can only return up to one value");
  if (!CallingConvSupported(CallConv))
    fail(DL, DAG, "TVM doesn't support non-C calling conventions");

  SmallVector<SDValue, 4> RetOps(1, Chain);
  RetOps.append(OutVals.begin(), OutVals.end());
  Chain = DAG.getNode(TVMISD::RETURN, DL, MVT::Other, RetOps);

  // Record the number and types of the return values.
  for (const ISD::OutputArg &Out : Outs) {
    assert(!Out.Flags.isByVal() && "byval is not valid for return values");
    assert(!Out.Flags.isNest() && "nest is not valid for return values");
    assert(Out.IsFixed && "non-fixed return value is not valid");
    // TODO: Just copied this logic from WASM backend.
    // Flags need to be investigated.
    if (Out.Flags.isInAlloca())
      fail(DL, DAG, "TVM hasn't implemented inalloca results");
    if (Out.Flags.isInConsecutiveRegs())
      fail(DL, DAG, "TVM hasn't implemented cons regs results");
    if (Out.Flags.isInConsecutiveRegsLast())
      fail(DL, DAG, "TVM hasn't implemented cons regs last results");
  }

  return Chain;
}

SDValue TVMTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  if (!CallingConvSupported(CallConv))
    fail(DL, DAG, "TVM doesn't support non-C calling conventions");
  if (IsVarArg)
    fail(DL, DAG, "VarArg is not supported yet");

  MachineFunction &MF = DAG.getMachineFunction();

  // Set up the incoming ARGUMENTS value, which serves to represent the liveness
  // of the incoming values before they're represented by virtual registers.
  MF.getRegInfo().addLiveIn(TVM::ARGUMENTS);
  auto *MFI = MF.getInfo<TVMFunctionInfo>();

  for (const ISD::InputArg &In : Ins) {
    // TODO: Copied from WASM. Chack.
    if (In.Flags.isInAlloca())
      fail(DL, DAG, "TVM hasn't implemented inalloca arguments");
    if (In.Flags.isNest())
      fail(DL, DAG, "TVM hasn't implemented nest arguments");
    if (In.Flags.isInConsecutiveRegs())
      fail(DL, DAG, "TVM hasn't implemented cons regs arguments");
    if (In.Flags.isInConsecutiveRegsLast())
      fail(DL, DAG, "TVM hasn't implemented cons regs last arguments");
    // Ignore In.getOrigAlign() because all our arguments are passed in
    // registers.
    InVals.push_back(In.Used ? DAG.getNode(TVMISD::ARGUMENT, DL, In.VT,
                                           DAG.getTargetConstant(InVals.size(),
                                                                 DL, MVT::i64))
                             : DAG.getUNDEF(In.VT));
    MFI->addParam(In.VT);
  }

  return Chain;
}

//===----------------------------------------------------------------------===//
//                      Other Lowerings
//===----------------------------------------------------------------------===//

SDValue TVMTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  case ISD::GlobalAddress:
    return LowerGlobalAddress(Op, DAG);
  case ISD::INTRINSIC_W_CHAIN:
    return LowerINTRINSIC_W_CHAIN(Op, DAG);
  default:
    llvm_unreachable("unimplemented operand");
  }
}

SDValue TVMTargetLowering::LowerGlobalAddress(SDValue Op,
                                              SelectionDAG &DAG) const {
  SDLoc DL(Op);
  const auto *GA = cast<GlobalAddressSDNode>(Op);
  EVT VT = Op.getValueType();
  assert(GA->getTargetFlags() == 0 &&
         "Unexpected target flags on generic GlobalAddressSDNode");
  if (GA->getAddressSpace() != 0)
    fail(DL, DAG, "TVM only expects the 0 address space");
  return DAG.getTargetGlobalAddress(GA->getGlobal(), DL, VT, GA->getOffset());
}

const char *TVMTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (static_cast<TVMISD::NodeType>(Opcode)) {
  case TVMISD::FIRST_NUMBER:
    break;
#define HANDLE_NODETYPE(NODE)                                                  \
  case TVMISD::NODE:                                                           \
    return "TVMISD::" #NODE;
#include "TVMISD.def"
#undef HANDLE_NODETYPE
  }
  return nullptr;
}

SDValue TVMTargetLowering::LowerINTRINSIC_W_CHAIN(SDValue Op,
                                                  SelectionDAG &DAG) const {
  unsigned IntNo = cast<ConstantSDNode>(Op.getOperand(1))->getZExtValue();
  SDLoc DL(Op);
  SDValue Chain = Op->getOperand(0);
  switch (IntNo) {
  default:
    break;
  /// Instrinsic operands are {chain, ID, parameters...} tuple.
  case Intrinsic::tvm_get_persistent_data: {
    SDValue Result = DAG.getNode(TVMISD::PUSHROOT, DL, MVT::i64, Chain);
    Result = DAG.getNode(TVMISD::CTOS, DL, MVT::i64, Result);
    return DAG.getMergeValues({Result.getValue(0), Chain}, DL);
  }
  case Intrinsic::tvm_inttoslice: {
    SDValue Precision = DAG.getConstant(256, DL, MVT::i64);
    SDValue Result = DAG.getNode(TVMISD::NEWC, DL, MVT::i64);
    Result = DAG.getNode(TVMISD::STU, DL, MVT::i64, Op->getOperand(2),
                         Result.getValue(0), Precision);
    Result = DAG.getNode(TVMISD::ENDC, DL, MVT::i64, Result);
    Result = DAG.getNode(TVMISD::CTOS, DL, MVT::i64, Result);
    return DAG.getMergeValues({Result.getValue(0), Chain}, DL);
  }
  case Intrinsic::tvm_stoc: {
    SDValue Result = DAG.getNode(TVMISD::NEWC, DL, MVT::i64);
    Result =
        DAG.getNode(TVMISD::STSLICE, DL, MVT::i64, Op->getOperand(2), Result);
    Result = DAG.getNode(TVMISD::ENDC, DL, MVT::i64, Result);
    return DAG.getMergeValues({Result.getValue(0), Chain}, DL);
  }
  case Intrinsic::tvm_ldu: {
    SDValue Precision = DAG.getConstant(256, DL, MVT::i64);
    SDValue Result =
        DAG.getNode(TVMISD::LDU, DL, DAG.getVTList(MVT::i64, MVT::i64),
                    Op->getOperand(2), Precision);
    return DAG.getMergeValues({Result.getValue(0), Result.getValue(1), Chain},
                              DL);
  }
  case Intrinsic::tvm_retrieve_signed: {
    // arguments: slice, from, len
    SDValue Skip1 =
        DAG.getNode(TVMISD::LDSLICEX, DL, DAG.getVTList(MVT::i64, MVT::i64),
                    Op->getOperand(2), Op->getOperand(3));
    SDValue Result =
        DAG.getNode(TVMISD::LDIX, DL, DAG.getVTList(MVT::i64, MVT::i64),
                    Skip1.getValue(0), Op->getOperand(4));
    return DAG.getMergeValues({Result.getValue(0), Chain}, DL);
  }
  case Intrinsic::tvm_retrieve_unsigned: {
    // arguments: slice, from, len
    SDValue Skip1 =
        DAG.getNode(TVMISD::LDSLICEX, DL, DAG.getVTList(MVT::i64, MVT::i64),
                    Op->getOperand(2), Op->getOperand(3));
    SDValue Result =
        DAG.getNode(TVMISD::LDUX, DL, DAG.getVTList(MVT::i64, MVT::i64),
                    Skip1.getValue(0), Op->getOperand(4));
    return DAG.getMergeValues({Result.getValue(0), Chain}, DL);
  }
  case Intrinsic::tvm_retrieve_ref: {
    // arguments: slice
    SDValue Result =
        DAG.getNode(TVMISD::LDREF, DL, DAG.getVTList(MVT::i64, MVT::i64),
                    Op->getOperand(2));
    return DAG.getMergeValues({Result.getValue(1), Chain}, DL);
  }
  }
  return SDValue();
}

//===----------------------------------------------------------------------===//
//                       TVM Inline Assembly Support
//===----------------------------------------------------------------------===//
// TODO: implement
