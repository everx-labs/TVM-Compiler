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

constexpr int PUSH_C0_FUNCTION_UNIQUE_ID = -2;

//===----------------------------------------------------------------------===//
// Command line options for TVM
//===----------------------------------------------------------------------===//
// Standard semantic for trunc operation is to extract a subregister and do not
// care about bits that doesn't belong to it. TVM doesn't have subregisters and
// all arithmetic operations are 257 bits wide. However, in some cases the
// numbers that guaranteed to fit in less than 257 bits may be used.
// The option UseTruncMasks emulates trunc using AND operation. It's a hack and
// might not work well with LLVM optimizations that assume another semantic
// for trunc.
static cl::opt<bool> UseTruncMasks(
    "tvm-trunc-masks", cl::Hidden,
    cl::desc("Simulate trunc operation for integer values using masks"),
    cl::init(false));

TVMTargetLowering::TVMTargetLowering(const TargetMachine &TM,
                                     const TVMSubtarget &STI)
    : TargetLowering(TM) {

  // Set up the register classes.
  addRegisterClass(MVT::i257, &TVM::I257RegClass);
  addRegisterClass(MVT::TVMSlice, &TVM::SliceRegClass);
  addRegisterClass(MVT::TVMBuilder, &TVM::BuilderRegClass);
  addRegisterClass(MVT::TVMCell, &TVM::CellRegClass);
  addRegisterClass(MVT::TVMTuple, &TVM::TupleRegClass);

  // Compute derived properties from the register classes
  computeRegisterProperties(STI.getRegisterInfo());

  setOperationAction(ISD::MULHS, MVT::i257, Expand);
  setOperationAction(ISD::MULHU, MVT::i257, Expand);
  setOperationAction(ISD::SMUL_LOHI, MVT::i257, Expand);
  setOperationAction(ISD::UMUL_LOHI, MVT::i257, Expand);
  setOperationAction(ISD::MULHS, MVT::i256, Expand);
  setOperationAction(ISD::MULHU, MVT::i256, Expand);
  setOperationAction(ISD::SMUL_LOHI, MVT::i256, Expand);
  setOperationAction(ISD::UMUL_LOHI, MVT::i256, Expand);

  setOperationAction(ISD::GlobalAddress, MVT::i256, Custom);
  setOperationAction(ISD::GlobalAddress, MVT::i257, Custom);
  setOperationAction(ISD::LOAD, MVT::i256, Custom);
  setOperationAction(ISD::STORE, MVT::i256, Custom);
  setOperationAction(ISD::TargetFrameIndex, MVT::i256, Custom);
  setOperationAction(ISD::FrameIndex, MVT::i256, Custom);
  setOperationAction(ISD::FrameIndex, MVT::i257, Custom);
  setOperationAction(ISD::ExternalSymbol, MVT::i256, Custom);
  setOperationAction(ISD::ExternalSymbol, MVT::i257, Custom);
  setOperationAction(ISD::BR, MVT::Other, Custom);
  setOperationAction(ISD::BRCOND, MVT::Other, Custom);
  setOperationAction(ISD::CopyToReg, MVT::Other, Custom);

  setOperationAction(ISD::ADD, MVT::i256, Promote);

  // Custom lowering for intrinsics that unrolls into more than one
  // MIR instructions.
  setOperationAction(ISD::INTRINSIC_W_CHAIN, MVT::Other, Custom);
  setOperationAction(ISD::INTRINSIC_WO_CHAIN, MVT::Other, Custom);
  setOperationAction(ISD::ABS, MVT::i257, Legal);
  setOperationAction(ISD::SMAX, MVT::i257, Legal);
  setOperationAction(ISD::SMIN, MVT::i257, Legal);

  setMinFunctionAlignment(1);
  setPrefFunctionAlignment(1);

  // Support of truncate, sext, zext
  setOperationAction(ISD::SIGN_EXTEND_INREG, MVT::i1, Expand);
  setOperationAction(ISD::SIGN_EXTEND_INREG, MVT::i8, Expand);
  setOperationAction(ISD::SIGN_EXTEND_INREG, MVT::i16, Expand);
  setOperationAction(ISD::SIGN_EXTEND_INREG, MVT::i32, Expand);

  // Support of truncate, sext, zext
  setTargetDAGCombine(ISD::TRUNCATE);

  setTargetDAGCombine(ISD::ANY_EXTEND);

  // Expand these forms; we pattern-match the forms that we can handle in isel.
  for (auto Op : {ISD::BR_CC, ISD::SELECT_CC})
    setOperationAction(Op, MVT::i257, Expand);
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

  SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
  SmallVectorImpl<SDValue> &OutVals = CLI.OutVals;
  for (unsigned i = 0; i < Outs.size(); ++i) {
    const ISD::OutputArg &Out = Outs[i];
    assert((Out.VT.SimpleTy == MVT::SimpleValueType::i257 ||
            Out.VT.SimpleTy == MVT::SimpleValueType::TVMSlice ||
            Out.VT.SimpleTy == MVT::SimpleValueType::TVMBuilder ||
            Out.VT.SimpleTy == MVT::SimpleValueType::TVMCell ||
            Out.VT.SimpleTy == MVT::SimpleValueType::TVMTuple) &&
           "Unsupported type in call");
  }

  // Compute the operands for the CALLn node.
  SmallVector<SDValue, 16> Ops;
  Ops.push_back(Chain);
  Ops.append(OutVals.begin(), OutVals.end());
  Ops.push_back(Callee);

  SmallVector<EVT, 8> InTys;
  for (const auto &In : Ins) {
    // TODO: add checks for In.Flags (copy from
    // WebAssemblyTargetLowering::LowerCall())
    assert((In.VT.SimpleTy == MVT::SimpleValueType::i257 ||
            In.VT.SimpleTy == MVT::SimpleValueType::TVMSlice ||
            In.VT.SimpleTy == MVT::SimpleValueType::TVMBuilder ||
            In.VT.SimpleTy == MVT::SimpleValueType::TVMCell ||
            In.VT.SimpleTy == MVT::SimpleValueType::TVMTuple) &&
           "Unsupported type in call");
    InTys.push_back(In.VT);
  }
  InTys.push_back(MVT::Other);

  SDVTList InTyList = DAG.getVTList(InTys);
  unsigned CallCmd;
  switch (Ins.size()) {
  case 0:  CallCmd = TVMISD::CALL0; break;
  case 1:  CallCmd = TVMISD::CALL1; break;
  default: CallCmd = TVMISD::CALLN; break;
  }
  if (CallCmd == TVMISD::CALLN)
    Ops.push_back(DAG.getTargetConstant(Ins.size(), DL, MVT::i257));
  SDValue Res = DAG.getNode(CallCmd, DL, InTyList, Ops);
  if (Ins.empty()) {
    Chain = Res;
  } else {
    for (unsigned i = 0; i < Ins.size(); ++i)
      InVals.push_back(Res.getValue(i));
    Chain = Res.getValue(Ins.size());
  }
  return Chain;
}

bool TVMTargetLowering::CanLowerReturn(
    CallingConv::ID /*CallConv*/, MachineFunction & /*MF*/, bool /*IsVarArg*/,
    const SmallVectorImpl<ISD::OutputArg> &/*Outs*/,
    LLVMContext & /*Context*/) const {
  return true;
}

SDValue
TVMTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                               bool /*IsVarArg*/,
                               const SmallVectorImpl<ISD::OutputArg> &Outs,
                               const SmallVectorImpl<SDValue> &OutVals,
                               const SDLoc &DL, SelectionDAG &DAG) const {
  if (!CallingConvSupported(CallConv))
    fail(DL, DAG, "TVM doesn't support non-C calling conventions");

  // Restore C0 for returning to correct continuation
  SDValue SetC0 = RestoreC0(Chain, DL, DAG);

  // Lower outputs
  SmallVector<SDValue, 4> RetOps(1, SetC0);
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
  auto *FI = MF.getInfo<TVMFunctionInfo>();

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
                                                                 DL, MVT::i257))
                             : DAG.getUNDEF(In.VT));
    FI->addParam(In.VT);
  }

  // Save C0 for returning to correct continuation
  return SaveC0(Chain, DL, DAG);
}

SDValue TVMTargetLowering::SaveC0(SDValue Chain, const SDLoc &DL,
                                  SelectionDAG &DAG) const {
  MachineFunction &MF = DAG.getMachineFunction();
  auto *FI = MF.getInfo<TVMFunctionInfo>();

  // Save c0 for further return lowering
  SDValue GetC0Ops[] = {
      DAG.getTargetConstant(Intrinsic::tvm_getreg, DL, MVT::i257),
      DAG.getConstant(0, DL, MVT::i257)};
  SDValue GetC0 = DAG.getNode(ISD::INTRINSIC_WO_CHAIN, DL, MVT::i257, GetC0Ops);

  MachineRegisterInfo &MRI = MF.getRegInfo();
  unsigned C0VirtReg = MRI.createVirtualRegister(&TVM::I257RegClass);
  SDValue C0VirtRegNode = DAG.getCopyToReg(Chain, DL, C0VirtReg, GetC0);

  FI->setC0VirtReg(C0VirtReg);
  C0VirtRegNode.getNode()->setNodeId(PUSH_C0_FUNCTION_UNIQUE_ID);

  return C0VirtRegNode;
}

namespace {

bool hasPushC0Predecessor(SDNode *Node) {
  if (Node->getNodeId() == PUSH_C0_FUNCTION_UNIQUE_ID)
    return true;

  for (const SDUse &Use : Node->ops()) {
    if (hasPushC0Predecessor(Use.getNode()))
      return true;
  }

  return false;
}

} // namespace

SDValue TVMTargetLowering::RestoreC0(SDValue Chain, const SDLoc &DL,
                                     SelectionDAG &DAG) const {
  MachineFunction &MF = DAG.getMachineFunction();
  auto *FI = MF.getInfo<TVMFunctionInfo>();

  assert(FI->hasC0VirtReg() && "C0 virtual register has not been saved");

  if (hasPushC0Predecessor(Chain.getNode()))
    return Chain;

  unsigned C0VirtReg = FI->getC0VirtReg();
  SDValue GetC0VirtReg = DAG.getCopyFromReg(Chain, DL, C0VirtReg, MVT::i257);
  SDValue SetC0Ops[] = {
      Chain, DAG.getTargetConstant(Intrinsic::tvm_setreg, DL, MVT::i257),
      DAG.getConstant(0, DL, MVT::i257), GetC0VirtReg};
  SDValue SetC0 = DAG.getNode(ISD::INTRINSIC_VOID, DL, MVT::Other, SetC0Ops);

  return SetC0;
}

EVT TVMTargetLowering::getOptimalMemOpType(uint64_t /*Sz*/, unsigned /*DstAl*/,
                                           unsigned /*SrcAl*/, bool /*Memset*/,
                                           bool /*ZeroMemset*/,
                                           bool /*MemcpyStrSrc*/,
                                           MachineFunction &/*MF*/) const {
  return MVT::i257;
}

bool TVMTargetLowering::allowsMisalignedMemoryAccesses(EVT /*VT*/,
                                                       unsigned /*AddrSpace*/,
                                                       unsigned /*Align*/,
                                                       bool */*Fast*/) const {
  return true;
}

//===----------------------------------------------------------------------===//
//                      Other Lowerings
//===----------------------------------------------------------------------===//

SDValue TVMTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  case ISD::GlobalAddress:
    return LowerGlobalAddress(Op, DAG);
  case ISD::LOAD:
    return LowerLoad(Op, DAG);
  case ISD::STORE:
    return LowerStore(Op, DAG);
  case ISD::ExternalSymbol:
    return LowerExternalSymbol(Op, DAG);
  case ISD::BR:
    return LowerBR(Op, DAG);
  case ISD::BRCOND:
    return LowerBRCOND(Op, DAG);
  case ISD::TargetFrameIndex:
  case ISD::FrameIndex:
    return LowerFrameIndex(Op, DAG);
  case ISD::CopyToReg:
    return LowerCopyToReg(Op, DAG);
  case ISD::INTRINSIC_W_CHAIN:
    return LowerINTRINSIC_W_CHAIN(Op, DAG);
  case ISD::INTRINSIC_WO_CHAIN:
    return LowerINTRINSIC_WO_CHAIN(Op, DAG);
  default:
    llvm_unreachable("unimplemented operand");
  }
}

/// Replace a node with an illegal result type with a new node built out of
/// custom code.
void TVMTargetLowering::ReplaceNodeResults(SDNode *N,
                                           SmallVectorImpl<SDValue> &Results,
                                           SelectionDAG &DAG) const {
  switch (N->getOpcode()) {
  default:
    llvm_unreachable("Do not know how to custom type legalize this operation!");
  case ISD::GlobalAddress:
    Results.push_back(LowerGlobalAddress(SDValue(N, 0), DAG));
    return;
  case ISD::ExternalSymbol:
    Results.push_back(LowerExternalSymbol(SDValue(N, 0), DAG));
    return;
  case ISD::TargetFrameIndex:
  case ISD::FrameIndex:
    Results.push_back(LowerFrameIndex(SDValue(N, 0), DAG));
    return;
  case ISD::LOAD:
    LowerOperationWrapper(N, Results, DAG);
    return;
  }
}

/// Places new result values for the node in Results (their number
/// and types must exactly match those of the original return values of
/// the node), or leaves Results empty, which indicates that the node is not
/// to be custom lowered after all.
void TVMTargetLowering::LowerOperationWrapper(SDNode *N,
                                              SmallVectorImpl<SDValue> &Results,
                                              SelectionDAG &DAG) const {
  // Code taken from X86 backend
  SDValue Res = LowerOperation(SDValue(N, 0), DAG);

  if (!Res.getNode())
    return;

  assert((N->getNumValues() <= Res->getNumValues()) &&
      "Lowering returned the wrong number of results!");

  // Places new result values base on N result number.
  // In some cases Res has more result values
  // than original node, chain should be dropped(last value).
  for (unsigned I = 0, E = N->getNumValues(); I != E; ++I)
    Results.push_back(Res.getValue(I));
}

SDValue TVMTargetLowering::PerformDAGCombine(SDNode *N,
                                             DAGCombinerInfo &DCI) const {
  SelectionDAG &DAG = DCI.DAG;
  SDLoc DL(N);
  switch (N->getOpcode()) {
  case ISD::ANY_EXTEND:
    if (N->getOperand(0)->getValueType(0) == N->getValueType(0))
      return N->getOperand(0);
    break;
  case ISD::TRUNCATE: {
    // TVM uses only one value type for all operations. This leads to folding
    // of all truncate nodes during legalization because there is no truncation
    // if the types are same. To prevent this TVM replaces 'truncate' nodes
    // with chain of two nodes: 'and' with corresponding truncation mask and
    // new 'truncate' node. This method also uses non usual call to
    // DAGCombinerInfo::CombineTo method instead of returning of new SDValue.
    // This is done to prevent further folding of replaced 'truncate' node
    // during the current stage.
    if (!UseTruncMasks)
      break;

    if (!DCI.isBeforeLegalizeOps())
      break;

    MVT ResultType = N->getSimpleValueType(0);
    MVT OperandType = N->getOperand(0).getSimpleValueType();

    if (!ResultType.isInteger() || !OperandType.isInteger())
      break;

    if (OperandType.getSizeInBits() == ResultType.getSizeInBits())
      break;

    APInt TruncateMask(OperandType.getSizeInBits(), 0);
    TruncateMask.setLowBits(ResultType.getSizeInBits());

    SDValue AndNode =
        DAG.getNode(TVMISD::AND, DL, OperandType, N->getOperand(0),
                    DAG.getConstant(TruncateMask, DL, OperandType));
    SDValue Replacement = DAG.getNode(ISD::TRUNCATE, DL, ResultType, AndNode);

    DCI.CombineTo(N, Replacement, false);

    break;
  }
  }

  return SDValue();
}

SDValue TVMTargetLowering::LowerGlobalAddress(SDValue Op,
                                              SelectionDAG &DAG) const {
  SDLoc DL(Op);
  const auto *GA = cast<GlobalAddressSDNode>(Op);
  EVT VT = MVT::i257;
  assert(GA->getTargetFlags() == 0 &&
         "Unexpected target flags on generic GlobalAddressSDNode");
  if (GA->getAddressSpace() != 0)
    fail(DL, DAG, "TVM only expects the 0 address space");
  return DAG.getNode(
      TVMISD::GLOBAL_ADDRESS_WRAPPER, DL, VT,
      DAG.getTargetGlobalAddress(GA->getGlobal(), DL, VT, GA->getOffset()));
}

SDValue TVMTargetLowering::LowerLoad(SDValue Op, SelectionDAG &DAG) const {
  assert(Op.getOpcode() == ISD::LOAD && "Wrong op");
  const auto *Ld = cast<LoadSDNode>(Op);
  SDLoc DL(Op);
  auto VT = Op.getValueType();
  auto Chain = Ld->getChain();
  auto Ptr = Ld->getBasePtr();

  if (Ptr.getOpcode() == ISD::TRUNCATE)
    Ptr = Ptr.getOperand(0);
  if (Ptr.getValueType() != MVT::i257)
    Ptr = DAG.getNode(ISD::ANY_EXTEND, DL, MVT::i257, Ptr);

  if (Ld->getMemoryVT() == MVT::i256) {
    return DAG.getExtLoad(ISD::ZEXTLOAD, DL, MVT::i257, Chain, Ptr, MVT::i256,
                          Ld->getMemOperand());
  }
  assert(Ld->getMemoryVT() == MVT::i257 && "Wrong Mem VT");
  assert(Ld->getExtensionType() == ISD::NON_EXTLOAD && "Non-expected ext load");
  return DAG.getLoad(VT, DL, Chain, Ptr, Ld->getMemOperand());
}

SDValue TVMTargetLowering::LowerStore(SDValue Op, SelectionDAG &DAG) const {
  assert(Op.getOpcode() == ISD::STORE && "Wrong op");
  const auto *St = cast<StoreSDNode>(Op);
  SDLoc DL(Op);
  auto Chain = St->getChain();
  auto Ptr = St->getBasePtr();
  auto Val = St->getValue();
  auto PtrVT = Ptr.getValueType();
  assert((PtrVT == MVT::i256 || PtrVT == MVT::i257) && "Wrong Ptr type");
  if (Ptr.getOpcode() == ISD::TRUNCATE)
    Ptr = Ptr.getOperand(0);
  if (Ptr.getValueType() != MVT::i257)
    Ptr = DAG.getNode(ISD::ANY_EXTEND, DL, MVT::i257, Ptr);
  if (Val.getValueType() != MVT::i257)
    Val = DAG.getNode(ISD::ANY_EXTEND, DL, MVT::i257, Val);
  if (St->getMemoryVT() == MVT::i256) {
    return DAG.getTruncStore(Chain, DL, Val, Ptr, MVT::i256,
                             St->getMemOperand());
  }
  assert(St->getMemoryVT() == MVT::i257 && "Wrong Mem VT");
  assert(!St->isTruncatingStore() && "Non-expected trunc store");

  return DAG.getStore(Chain, DL, Val, Ptr, St->getMemOperand());
}

SDValue TVMTargetLowering::LowerExternalSymbol(SDValue Op,
                                               SelectionDAG &DAG) const {
  SDLoc DL(Op);
  const auto *ES = cast<ExternalSymbolSDNode>(Op);
  EVT VT = MVT::i257;
  assert(ES->getTargetFlags() == 0 &&
         "Unexpected target flags on generic ExternalSymbolSDNode");
  // Code excerpt: copied from WebAssembly
  // Set the TargetFlags to 0x1 which indicates that this is a "function"
  // symbol rather than a data symbol. We do this unconditionally even though
  // we don't know anything about the symbol other than its name, because all
  // external symbols used in target-independent SelectionDAG code are for
  // functions.
  return DAG.getNode(TVMISD::GLOBAL_ADDRESS_WRAPPER, DL, VT,
                     DAG.getTargetExternalSymbol(ES->getSymbol(), VT,
                                                 /*TargetFlags=*/0x1));
}

SDValue TVMTargetLowering::LowerBR(SDValue Op,
                                   SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Chain = Op.getOperand(0);
  SDValue Dest  = Op.getOperand(1);
  SDValue Zero = DAG.getTargetConstant(0, DL, MVT::i257);
  if (Chain.getOpcode() == ISD::BRCOND) {
    SDValue PrevChain = Chain->getOperand(0), Cond = Chain->getOperand(1),
            ThenBr = Chain->getOperand(2), ElseBr = Dest;

    ThenBr = DAG.getNode(TVMISD::BBWrapper, DL, MVT::i257, ThenBr, Zero);
    ElseBr = DAG.getNode(TVMISD::BBWrapper, DL, MVT::i257, ElseBr, Zero);

    return DAG.getNode(TVMISD::IFELSE, DL, MVT::Other, PrevChain, Cond,
                       ThenBr, ElseBr);
  }
  Dest = DAG.getNode(TVMISD::BBWrapper, DL, MVT::i257, Dest, Zero);
  return DAG.getNode(TVMISD::JUMPX, DL, MVT::Other, Chain, Dest);
}

SDValue TVMTargetLowering::LowerBRCOND(SDValue Op,
                                       SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Chain = Op.getOperand(0);
  SDValue Cond  = Op.getOperand(1);
  SDValue Dest  = Op.getOperand(2);
  SDValue Zero = DAG.getTargetConstant(0, DL, MVT::i257);
  Dest = DAG.getNode(TVMISD::BBWrapper, DL, MVT::i257, Dest, Zero);
  return DAG.getNode(TVMISD::IFJMP, DL, MVT::Other, Chain, Dest, Cond);
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

SDValue TVMTargetLowering::LowerCopyToReg(SDValue Op, SelectionDAG &DAG) const {
  SDValue Src = Op.getOperand(2);
  if (isa<FrameIndexSDNode>(Src.getNode())) {
    // CopyToReg nodes don't support FrameIndex operands.
    //   Implemented like WebAssemblyTargetLowering::LowerCopyToReg
    SDValue Chain = Op.getOperand(0);
    SDLoc DL(Op);
    unsigned Reg = cast<RegisterSDNode>(Op.getOperand(1))->getReg();
    EVT VT = Src.getValueType();

    SDValue Copy(DAG.getMachineNode(TVM::REG_TO_REG_COPY, DL, VT, Src), 0);
    return Op.getNode()->getNumValues() == 1
               ? DAG.getCopyToReg(Chain, DL, Reg, Copy)
               : DAG.getCopyToReg(Chain, DL, Reg, Copy, Op.getNumOperands() == 4
                                                            ? Op.getOperand(3)
                                                            : SDValue());
  }
  return SDValue();
}

SDValue TVMTargetLowering::LowerFrameIndex(SDValue Op,
                                           SelectionDAG &DAG) const {
  int FI = cast<FrameIndexSDNode>(Op)->getIndex();
  return DAG.getTargetFrameIndex(FI, MVT::i257);
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
    SDValue Result = DAG.getNode(TVMISD::PUSHROOT, DL, MVT::TVMCell, Chain);
    return DAG.getMergeValues({Result.getValue(0), Chain}, DL);
  }
  case Intrinsic::tvm_inttoslice: {
    SDValue Precision = DAG.getConstant(256, DL, MVT::i257);
    SDValue Result = DAG.getNode(TVMISD::NEWC, DL, MVT::TVMBuilder);
    Result = DAG.getNode(TVMISD::STU, DL, MVT::TVMBuilder, Op->getOperand(2),
                         Result.getValue(0), Precision);
    Result = DAG.getNode(TVMISD::ENDC, DL, MVT::TVMCell, Result);
    Result = DAG.getNode(TVMISD::CTOS, DL, MVT::TVMSlice, Result);
    return DAG.getMergeValues({Result.getValue(0), Chain}, DL);
  }
  case Intrinsic::tvm_stoc: {
    SDValue Result = DAG.getNode(TVMISD::NEWC, DL, MVT::TVMBuilder);
    Result = DAG.getNode(TVMISD::STSLICE, DL, MVT::TVMBuilder,
                         Op->getOperand(2), Result);
    Result = DAG.getNode(TVMISD::ENDC, DL, MVT::TVMCell, Result);
    return DAG.getMergeValues({Result.getValue(0), Chain}, DL);
  }
  case Intrinsic::tvm_ldu: {
    SDValue Precision = DAG.getConstant(256, DL, MVT::i257);
    SDValue Result =
        DAG.getNode(TVMISD::LDU, DL, DAG.getVTList(MVT::i257, MVT::TVMSlice),
                    Op->getOperand(2), Precision);
    return DAG.getMergeValues({Result.getValue(0), Result.getValue(1), Chain},
                              DL);
  }
  case Intrinsic::tvm_retrieve_signed: {
    // arguments: slice, from, len
    SDValue Skip1 =
        DAG.getNode(TVMISD::LDSLICEX, DL,
                    DAG.getVTList(MVT::TVMSlice, MVT::TVMSlice),
                    Op->getOperand(2), Op->getOperand(3));
    SDValue Result =
        DAG.getNode(TVMISD::LDIX, DL, DAG.getVTList(MVT::i257, MVT::TVMSlice),
                    Skip1.getValue(0), Op->getOperand(4));
    return DAG.getMergeValues({Result.getValue(0), Chain}, DL);
  }
  case Intrinsic::tvm_retrieve_unsigned: {
    // arguments: slice, from, len
    SDValue Skip1 =
        DAG.getNode(TVMISD::LDSLICEX, DL,
                    DAG.getVTList(MVT::TVMSlice, MVT::TVMSlice),
                    Op->getOperand(2), Op->getOperand(3));
    SDValue Result =
        DAG.getNode(TVMISD::LDUX, DL, DAG.getVTList(MVT::i257, MVT::TVMSlice),
                    Skip1.getValue(0), Op->getOperand(4));
    return DAG.getMergeValues({Result.getValue(0), Chain}, DL);
  }
  case Intrinsic::tvm_retrieve_ref: {
    // arguments: slice
    SDValue Result =
        DAG.getNode(TVMISD::LDREF, DL,
                    DAG.getVTList(MVT::TVMCell, MVT::TVMSlice),
                    Op->getOperand(2));
    return DAG.getMergeValues({Result.getValue(1), Chain}, DL);
  }

  }
  return SDValue();
}

SDValue TVMTargetLowering::LowerINTRINSIC_WO_CHAIN(SDValue Op,
                                                   SelectionDAG &DAG) const {
  unsigned IntNo = cast<ConstantSDNode>(Op.getOperand(0))->getZExtValue();
  SDLoc DL(Op);
  switch (IntNo) {
  default:
    break;
  case Intrinsic::tvm_tuple: {
    // arguments: vararg
    SmallVector<SDValue, 16> Ops(std::next(Op->op_begin()), Op->op_end());
    unsigned Size = Ops.size();
    bool SmallTuple = Size <= TVMTargetMachine::SmallTupleLimit;
    Ops.push_back(DAG.getConstant(Size, DL, MVT::i257, SmallTuple));
    unsigned Cmd = SmallTuple ? TVMISD::TUPLE : TVMISD::TUPLEVAR;
    return DAG.getNode(Cmd, DL, MVT::TVMTuple, Ops);
  }
  }
  return SDValue();
}

//===----------------------------------------------------------------------===//
//                       TVM Inline Assembly Support
//===----------------------------------------------------------------------===//
// TODO: implement
