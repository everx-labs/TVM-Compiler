//===-- TVMInstrInfo.td - TVM Instruction defs -------------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the TVM instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

include "TVMInstrFormats.td"

//===----------------------------------------------------------------------===//
// Type Profiles.
//===----------------------------------------------------------------------===//
def SDT_TVMCall         : SDTypeProfile<0, -1, [SDTCisVT<0, iPTR>]>;
def SDT_TVMCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i257>,
                                          SDTCisVT<1, i257>]>;
def SDT_TVMCallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i257>, SDTCisVT<1, i257>]>;
def SDT_TVMReturn       : SDTypeProfile<0, -1, []>;
def SDT_TVMArgument     : SDTypeProfile<1, 1, [SDTCisVT<1, i257>]>;
def SDT_TVMCall0        : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;
def SDT_TVMCall1        : SDTypeProfile<1, -1, []>;

def SDT_TVMCtos         : SDTypeProfile<1, 1, [SDTCisVT<1, TVMCell>]>;
def SDT_TVMStSlice      : SDTypeProfile<1, 2, []>;
def SDT_TVMNewc         : SDTypeProfile<1, 0, []>;
def SDT_TVMEndc         : SDTypeProfile<1, 1, [SDTCisVT<1, TVMBuilder>]>;
def SDT_TVMDictGet      : SDTypeProfile<2, 3, []>;
def SDT_TVMDictSt       : SDTypeProfile<1, 3, []>;
def SDT_TVMDictLd       : SDTypeProfile<2, 2, []>;
def SDT_TVMLdRef        : SDTypeProfile<2, 1, []>;
def SDT_TVMSendRawMsg   : SDTypeProfile<0, 2, []>;
def SDT_TVMIfElse       : SDTypeProfile<0, 3, [SDTCisVT<0, i257>,
                                               SDTCisVT<1, i257>,
                                               SDTCisVT<2, i257>]>;
def SDT_TVMJumpX        : SDTypeProfile<0, 1, [SDTCisVT<0, i257>]>;
def SDT_TVMIfJmp        : SDTypeProfile<0, 2, [SDTCisVT<0, i257>,
                                               SDTCisVT<1, i257>]>;

// We need additional i257 operand after OtherVT, because otherwise
//  OtherVT (BasicBlock) operand will be considered as a chain
def SDT_TVMBBWrapper    : SDTypeProfile<1, 2, [SDTCisVT<0, i257>,
                                               SDTCisVT<1, OtherVT>,
                                               SDTCisVT<2, i257>]>;

def SDT_TVMAnd          : SDTypeProfile<1, 2, [SDTCisVT<0, i257>,
                                               SDTCisVT<1, i257>,
                                               SDTCisVT<2, i257>]>;
def SDT_TVMMul          : SDTypeProfile<1, 2, [SDTCisVT<0, i257>,
                                               SDTCisVT<1, i257>,
                                               SDTCisVT<2, i257>]>;
def SDT_TVMRShift       : SDTypeProfile<1, 2, [SDTCisVT<0, i257>,
                                               SDTCisVT<1, i257>,
                                               SDTCisVT<2, i257>]>;
def SDT_TVMConstU257     : SDTypeProfile<1, 1,
                           [SDTCisVT<0, i257>, SDTCisVT<1, i257>]>;
def SDT_TVMGlobalAddressWrapper : SDTypeProfile<1, 1, [SDTCisPtrTy<0>]>;

//===----------------------------------------------------------------------===//
// TVM specific node definitions
//===----------------------------------------------------------------------===//
def TVMcallseq_start :
                 SDNode<"ISD::CALLSEQ_START", SDT_TVMCallSeqStart,
                        [SDNPHasChain, SDNPOutGlue]>;
def TVMcallseq_end :
                 SDNode<"ISD::CALLSEQ_END",   SDT_TVMCallSeqEnd,
                        [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;
def TVMreturn : SDNode<"TVMISD::RETURN",
                       SDT_TVMReturn, [SDNPHasChain, SDNPVariadic]>;
def TVMargument : SDNode<"TVMISD::ARGUMENT", SDT_TVMArgument>;
def TVMcall0 : SDNode<"TVMISD::CALL0", SDT_TVMCall0,
                      [SDNPHasChain, SDNPVariadic]>;
def TVMcall1 : SDNode<"TVMISD::CALL1", SDT_TVMCall1,
                      [SDNPHasChain, SDNPVariadic]>;
def TVMcalldict0 : SDNode<"TVMISD::CALLDICT0", SDT_TVMCall0,
                      [SDNPHasChain, SDNPVariadic]>;
def TVMcalldict1 : SDNode<"TVMISD::CALLDICT1", SDT_TVMCall1,
                      [SDNPHasChain, SDNPVariadic]>;

def TVMctos     : SDNode<"TVMISD::CTOS", SDT_TVMCtos>;
def TVMstslice  : SDNode<"TVMISD::STSLICE", SDT_TVMStSlice>;
def TVMdictuget : SDNode<"TVMISD::DICTUGET", SDT_TVMDictGet, [SDNPHasChain]>;
def TVMsti      : SDNode<"TVMISD::STI", SDT_TVMDictSt, []>;
def TVMstu      : SDNode<"TVMISD::STU", SDT_TVMDictSt, []>;
def TVMldref    : SDNode<"TVMISD::LDREF", SDT_TVMLdRef, []>;
def TVMldslicex : SDNode<"TVMISD::LDSLICEX", SDT_TVMDictLd, []>;
def TVMsendrawmsg : SDNode<"TVMISD::SENDRAWMSG", SDT_TVMSendRawMsg, [SDNPHasChain, SDNPMayStore]>;
def TVMifelse   : SDNode<"TVMISD::IFELSE", SDT_TVMIfElse, [SDNPHasChain]>;
def TVMjumpx    : SDNode<"TVMISD::JUMPX", SDT_TVMJumpX, [SDNPHasChain]>;
def TVMifjmp    : SDNode<"TVMISD::IFJMP", SDT_TVMIfJmp, [SDNPHasChain]>;
def TVMif       : SDNode<"TVMISD::IF", SDT_TVMIfJmp, [SDNPHasChain]>;
def BBWrapper   : SDNode<"TVMISD::BBWrapper", SDT_TVMBBWrapper>;

def TVMmul       : SDNode<"TVMISD::MUL", SDT_TVMMul, []>;
def TVMand       : SDNode<"TVMISD::AND", SDT_TVMAnd, []>;
def TVMrshift    : SDNode<"TVMISD::RSHIFT", SDT_TVMRShift, []>;
def TVMconst_u257 : SDNode<"TVMISD::CONST_U257", SDT_TVMConstU257, []>;

def TVMtuple
    : SDNode<"TVMISD::TUPLE", SDTypeProfile<0, -1, []>, [SDNPVariadic]>;
def TVMtuplevar
    : SDNode<"TVMISD::TUPLEVAR", SDTypeProfile<0, -1, []>, [SDNPVariadic]>;

def TVMGlobalAddressWrapper
    : SDNode<"TVMISD::GLOBAL_ADDRESS_WRAPPER", SDT_TVMGlobalAddressWrapper>;

//===----------------------------------------------------------------------===//
// TVM-specific Operands.
//===----------------------------------------------------------------------===//

let OperandNamespace = "TVM" in {

// This operand represents stack slots (s0, s1, s2 etc.)
let OperandType = "OPERAND_STACK" in
def stack_op : Operand<i8>;

// This operand represents builder argument (see A.6 Cell primitives)
let OperandType = "OPERAND_BUILDER" in
def builder_op : Operand<i257>;

// This operand represents cell argument (see A.6 Cell primitives)
let OperandType = "OPERAND_CELL" in
def cell_op : Operand<i257>;

// This operand represents slice argument (see A.6 Cell primitives)
let OperandType = "OPERAND_SLICE" in
def slice_op : Operand<i257>;

let OperandType = "OPERAND_I64IMM" in
def i257imm_op : Operand<i257>;

let OperandType = "OPERAND_FUNCTION" in
def function_op : Operand<i257>;

let OperandType = "OPERAND_BASIC_BLOCK" in
def bb_op : Operand<OtherVT>;
}

//===----------------------------------------------------------------------===//
// Instruction list..

defm ADJCALLSTACKDOWN : NRI<(outs), (ins i257imm:$amt, i257imm:$amt2),
                            [(TVMcallseq_start timm:$amt, timm:$amt2)]>;
defm ADJCALLSTACKUP : NRI<(outs), (ins i257imm:$amt, i257imm:$amt2),
                          [(TVMcallseq_end timm:$amt, timm:$amt2)]>;

let hasSideEffects = 1, Uses = [ARGUMENTS], isCodeGenOnly = 1 in
defm ARGUMENT : I<(outs I257:$res), (ins i257imm:$argno),
                  (outs), (ins i257imm:$argno),
                  [(set I257:$res, (TVMargument timm:$argno))]>;

let hasSideEffects = 1, Uses = [ARGUMENTS], isCodeGenOnly = 1 in
defm ARGUMENT_SLICE : I<(outs Slice:$res), (ins i257imm:$argno),
                        (outs), (ins i257imm:$argno),
                        [(set Slice:$res, (TVMargument timm:$argno))]>;

let hasSideEffects = 1, Uses = [ARGUMENTS], isCodeGenOnly = 1 in
defm ARGUMENT_BUILDER : I<(outs Builder:$res), (ins i257imm:$argno),
                          (outs), (ins i257imm:$argno),
                          [(set Builder:$res, (TVMargument timm:$argno))]>;

let hasSideEffects = 1, Uses = [ARGUMENTS], isCodeGenOnly = 1 in
defm ARGUMENT_CELL : I<(outs Cell:$res), (ins i257imm:$argno),
                       (outs), (ins i257imm:$argno),
                       [(set Cell:$res, (TVMargument timm:$argno))]>;

let hasSideEffects = 1, Uses = [ARGUMENTS], isCodeGenOnly = 1 in
defm ARGUMENT_TUPLE : I<(outs Tuple:$res), (ins i257imm:$argno),
                        (outs), (ins i257imm:$argno),
                        [(set Tuple:$res, (TVMargument timm:$argno))]>;

// Auxiliary instruction for debugging purposes only: 
// specifies number of arguments on stack for the function;
// normally this number is taken from MachineInfo Param vector.
let hasSideEffects = 1, isCodeGenOnly = 1 in
defm ARGUMENT_NUM : I<(outs), (ins i257imm:$argnum), (outs),
                              (ins i257imm:$argnum), []>;

include "TVMControlFlowInstrInfo.td"

//===----------------------------------------------------------------------===//
// Selection patterns
//===----------------------------------------------------------------------===//
def inc : PatFrag<(ops node:$in), (add node:$in, 1)>;
def dec : PatFrag<(ops node:$in), (add node:$in, -1)>;
//===----------------------------------------------------------------------===//
// Basic stack manipulation primitives
//===----------------------------------------------------------------------===//

defm NOP : I<(outs), (ins), (outs), (ins), [(int_tvm_nop)], "NOP", "NOP", 0x00>;

// Stack manipulation instructions are not supposed to be selected, instead
// Stack Model pass inserts them to manipulate with local and global variables.
// They has mayLoad because it reads from a local, which is a side effect
// not otherwise modeled in LLVM.

let mayLoad = 1, isAsCheapAsAMove = 1 in {
defm PUSH : SI<(ins stack_op:$local), "PUSH\t$local", 0x20>;
defm XCHG_TOP : SI<(ins stack_op:$src), "XCHG\ts0, $src", 0x01>;
defm XCHG_TOP_DEEP : SI<(ins stack_op:$src), "XCHG\ts0, $src", 0x11>;
defm XCHG : SI<(ins stack_op:$src, stack_op:$dst), "XCHG\t$src, $dst", 0x10>;

// XCHG, XCHG
defm XCHG2 : SI<(ins stack_op:$i, stack_op:$j), "XCHG2\t$i, $j", 0x50>;
// XCHG, PUSH
defm XCPU : SI<(ins stack_op:$i, stack_op:$j), "XCPU\t$i, $j", 0x51>;
// PUSH, XCHG
defm PUXC : SI<(ins stack_op:$i, stack_op:$j), "PUXC\t$i, $j", 0x52>;
// PUSH, PUSH
defm PUSH2 : SI<(ins stack_op:$i, stack_op:$j), "PUSH2\t$i, $j", 0x53>;
defm DUP2 : SI<(ins), "DUP2", 0x5c>;
defm OVER2 : SI<(ins), "OVER2", 0x5d>;

// XCHG, XCHG, XCHG
defm XCHG3 : SI<(ins stack_op:$i, stack_op:$j, stack_op:$k),
                "XCHG3\t$i, $j, $k", 0x04>;
// XCHG, XCHG, PUSH
defm XC2PU : SI<(ins stack_op:$i, stack_op:$j, stack_op:$k),
                "XC2PU\t$i, $j, $k", 0x541>;
defm TUCK : SI<(ins), "TUCK", 0x66>;
// XCHG, PUSH, XCHG
defm XCPUXC : SI<(ins stack_op:$i, stack_op:$j, stack_op:$k),
                 "XCPUXC\t$i, $j, $k", 0x542>;
// XCHG, PUSH, PUSH
defm XCPU2 : SI<(ins stack_op:$i, stack_op:$j, stack_op:$k),
                "XCPU2\t$i, $j, $k", 0x543>;
// PUSH, XCHG, XCHG
defm PUXC2 : SI<(ins stack_op:$i, stack_op:$j, stack_op:$k),
                "PUXC2\t$i, $j, $k", 0x544>;
// PUSH, XCHG, PUSH
defm PUXCPU : SI<(ins stack_op:$i, stack_op:$j, stack_op:$k),
                 "PUXCPU\t$i, $j, $k", 0x545>;
// PUSH, PUSH, XCHG
defm PU2XC : SI<(ins stack_op:$i, stack_op:$j, stack_op:$k),
                "PU2XC\t$i, $j, $k", 0x546>;
// PUSH, PUSH, PUSH
defm PUSH3 : SI<(ins stack_op:$i, stack_op:$j, stack_op:$k),
                "PUSH3\t$i, $j, $k", 0x547>;

let isPseudo=1 in
def HIDDENSTACK : NI<(outs I257:$dst), (ins uimm8:$src), [(set I257:$dst, (int_tvm_hiddenstack uimm8:$src))], 1>;

defm BLKPUSH : SI<(ins uimm8: $sz, uimm8:$slot), "BLKPUSH\t$sz, $slot", 0x5F>;
defm POP  : SI<(ins stack_op:$dst), "POP\t$dst", 0x30>;
defm BLKDROP : SI<(ins uimm8:$sz), "BLKDROP\t$sz", 0x5F0>;
defm DROP2 : SI<(ins), "DROP2", 0x5B>;
defm DROPX: SI<(ins), "DROPX", 0x65>;
defm BLKSWAP : SI<(ins uimm8:$deep, uimm8:$top), "BLKSWAP\t$deep, $top", 0x55>;
defm ROT : SI<(ins), "ROT", 0x58>;
defm ROTREV : SI<(ins), "ROTREV", 0x59>;
defm BLKSWX : SI<(ins), "BLKSWX", 0x63>;
defm ROLL : SI<(ins uimm8:$sz), "ROLL\t$sz", 0x55>;
defm ROLLREV : SI<(ins uimm8:$sz), "ROLLREV\t$sz", 0x55>;
defm ROLLX : SI<(ins), "ROLLX", 0x61>;
defm ROLLREVX : SI<(ins), "ROLLREVX", 0x62>;
defm REVERSE : SI<(ins uimm8:$sz, uimm8:$topIdx),
                   "REVERSE\t$sz, $topIdx", 0x5E>;
defm REVX: SI<(ins), "REVX", 0x64>;
}

let isMoveImm = 1, isAsCheapAsAMove = 1, isReMaterializable = 1 in {
defm CONST_I257 : I<(outs I257:$res), (ins i257imm:$imm),
                    (outs), (ins i257imm:$imm),
                    [(set I257:$res, imm:$imm)],
                    "PUSHINT\t$res, $imm", "PUSHINT\t$imm", 0x82>;

defm CONST_U257 : I<(outs I257:$res), (ins UImm257:$uimm),
                    (outs), (ins UImm257:$uimm),
                    [(set I257:$res, (TVMconst_u257 timm:$uimm))],
                    "PUSHINT\t$res, $uimm", "PUSHINT\t$uimm", 0x82>;
}


def DUP  : InstAlias<"DUP", (PUSH 0)>;
def DROP  : InstAlias<"DROP", (POP 0)>;
def NIP  : InstAlias<"NIP", (POP 1)>;
def ZERO : InstAlias<"ZERO", (CONST_I257_S 0)>;
def FALSE : InstAlias<"FALSE", (CONST_I257_S 0), 0>;
def ONE : InstAlias<"ONE", (CONST_I257_S 1)>;
def TEN : InstAlias<"TEN", (CONST_I257_S 10)>;
def TRUE : InstAlias<"TRUE", (CONST_I257_S -1), 0>;
def SWAP : InstAlias<"SWAP", (XCHG_TOP 1)>;

let isCodeGenOnly = 1 in {
defm REG_TO_REG_COPY : I<(outs I257:$dst), (ins I257:$src), (outs), (ins), []>;
defm TO_TUPLE_COPY   : I<(outs Tuple:$dst), (ins I257:$src), (outs), (ins),
  [(set Tuple:$dst, (int_tvm_cast_to_tuple I257:$src))]>;
defm TO_SLICE_COPY   : I<(outs Slice:$dst), (ins I257:$src), (outs), (ins),
  [(set Slice:$dst, (int_tvm_cast_to_slice I257:$src))]>;
defm TO_BUILDER_COPY : I<(outs Builder:$dst), (ins I257:$src), (outs), (ins),
  [(set Builder:$dst, (int_tvm_cast_to_builder I257:$src))]>;
defm TO_CELL_COPY    : I<(outs Cell:$dst), (ins I257:$src), (outs), (ins),
  [(set Cell:$dst, (int_tvm_cast_to_cell I257:$src))]>;

defm FROM_TUPLE_COPY   : I<(outs I257:$dst), (ins Tuple:$src), (outs), (ins),
  [(set I257:$dst, (int_tvm_cast_from_tuple Tuple:$src))]>;
defm FROM_SLICE_COPY   : I<(outs I257:$dst), (ins Slice:$src), (outs), (ins),
  [(set I257:$dst, (int_tvm_cast_from_slice Slice:$src))]>;
defm FROM_BUILDER_COPY : I<(outs I257:$dst), (ins Builder:$src), (outs), (ins),
  [(set I257:$dst, (int_tvm_cast_from_builder Builder:$src))]>;
defm FROM_CELL_COPY    : I<(outs I257:$dst), (ins Cell:$src), (outs), (ins),
  [(set I257:$dst, (int_tvm_cast_from_cell Cell:$src))]>;
}

def : Pat<(TVMTuple (bitconvert I257:$src)), (TO_TUPLE_COPY $src)>;
def : Pat<(TVMSlice (bitconvert I257:$src)), (TO_SLICE_COPY $src)>;
def : Pat<(TVMBuilder (bitconvert I257:$src)), (TO_BUILDER_COPY $src)>;
def : Pat<(TVMCell (bitconvert I257:$src)), (TO_CELL_COPY $src)>;

def : Pat<(i257 (bitconvert Tuple:$src)), (FROM_TUPLE_COPY $src)>;
def : Pat<(i257 (bitconvert Slice:$src)), (FROM_SLICE_COPY $src)>;
def : Pat<(i257 (bitconvert Builder:$src)), (FROM_BUILDER_COPY $src)>;
def : Pat<(i257 (bitconvert Cell:$src)), (FROM_CELL_COPY $src)>;

//===----------------------------------------------------------------------===//
// Arithmetic instructions
//===----------------------------------------------------------------------===//
let isCommutable = 1 in {
defm ADD      : BinaryRR<add, "ADD", 0xa0>;
defm AND      : BinaryRR<and, "AND", 0xb0>;
defm OR       : BinaryRR<or, "OR", 0xb1>;
defm XOR      : BinaryRR<xor, "XOR", 0xb2>;
defm MIN      : BinaryRR<smin, "MIN", 0xb608>;
defm MAX      : BinaryRR<smax, "MAX", 0xb609>;
}
let isCommutable = 1 in {
defm MUL      : I<(outs I257:$dst), (ins I257:$lhs, I257:$rhs), (outs), (ins), [],
                  "MUL\t$dst, $lhs, $rhs", "MUL", 0xa8>;
}

defm SUB      : BinaryRR<sub, "SUB", 0xa1>;
defm SUBR     : BinaryRR<sub, "SUBR", 0xa2>;
defm NEGATE   : UnaryR<ineg, "NEGATE", 0xa3>;
defm INC      : UnaryR<inc, "INC", 0xa4>;
defm DEC      : UnaryR<dec, "DEC", 0xa5>;
defm ABS      : UnaryR<abs, "ABS", 0xb60b>;
defm DIV      : BinaryRR<int_tvm_div, "DIV", 0xa904>;
defm NOT      : UnaryR<not, "NOT", 0xb3>;
defm SHL      : BinaryRR<shl, "LSHIFT", 0xac>;
defm SHLCONST : BinaryRI<shl, "LSHIFT", simm8, 0xaa00>;
defm SHR      : BinaryRR<sra, "RSHIFT", 0xad>;
defm SHRCONST : BinaryRI<sra, "RSHIFT", simm8, 0xab00>;
defm MOD      : BinaryRR<int_tvm_mod, "MOD", 0x81>;
defm ADDCONST : BinaryRI<add, "ADDCONST", simm8, 0xa6>;
defm MULCONST : BinaryRI<mul, "MULCONST", simm8, 0xa7>;
defm BITSIZE  : UnaryR<int_tvm_bitsize, "BITSIZE", 0xb602>;
defm UBITSIZE : UnaryR<int_tvm_ubitsize, "UBITSIZE", 0xb603>;

defm MULDIVR  : I<(outs I257:$res), (ins I257:$x, I257:$y, I257:$z),
                  (outs), (ins I257:$x, I257:$y, I257:$z),
                  [(set I257:$res, (int_tvm_muldivr I257:$x, I257:$y, I257:$z))],
                  "MULDIVR\t$res, $x, $y, $z", "MULDIVR", 0xa985>;

def : Pat<(srl I257:$val, imm:$off), (SHRCONST I257:$val, imm:$off)>;

// TODO: maybe support mulhs/mulhu using double-length multiply-shift operation
def : Pat<(mul I257:$a, I257:$b), (MUL I257:$a, I257:$b)>;
def : Pat<(TVMmul I257:$a, I257:$b), (MUL I257:$a, I257:$b)>;
def : Pat<(TVMand I257:$a, I257:$b), (AND I257:$a, I257:$b)>;
def : Pat<(TVMrshift I257:$a, timm:$b), (SHRCONST I257:$a, imm:$b)>;
def : Pat<(TVMrshift I257:$a, I257:$b), (SHR I257:$a, I257:$b)>;

// unsigned a / b = a DIV b
// unsigned a % b = a MOD b
class UintBinopPat<SDPatternOperator instr_op, Instruction instr> :
  Pat<(instr_op I257:$a, I257:$b), (instr I257:$a, I257:$b)>;
def : UintBinopPat<udiv, DIV>;
def : UintBinopPat<urem, MOD>;

// TODO: restore DIV/MOD rounding correction
//   OR do early conversion of '/', '%' into our intrinsics in clang
//   to have described "round to minus infinity" behaviour
class IntDivPat<SDPatternOperator instr_op, Instruction instr> :
  Pat<(instr_op I257:$a, I257:$b), (instr I257:$a, I257:$b)>;
def : IntDivPat<sdiv, DIV>;
def : IntDivPat<srem, MOD>;

//===----------------------------------------------------------------------===//
// Fits
//===----------------------------------------------------------------------===//
def chkbool : PatFrag<(ops node:$in), (int_tvm_fitsx node:$in, 1)>;
def chkbit  : PatFrag<(ops node:$in), (int_tvm_ufitsx node:$in, 1)>;
let hasSideEffects = 1 in {
  defm FITSX   : BinaryRR<int_tvm_fitsx, "FITSX", 0xb600>;
  defm UFITSX  : BinaryRR<int_tvm_ufitsx, "UFITSX", 0xb601>;
  defm FITS    : BinaryRI<int_tvm_fitsx, "FITS", uimm8, 0xb4>;
  defm UFITS   : BinaryRI<int_tvm_ufitsx, "UFITS", uimm8, 0xb5>;
  defm CHKBOOL : UnaryR<chkbool, "CHKBOOL", 0xb400>;
  defm CHKBIT  : UnaryR<chkbit, "CHKBIT", 0xb500>;
}

//===----------------------------------------------------------------------===//
// Debug
//===----------------------------------------------------------------------===//

defm DUMPSTK : I<(outs), (ins), (outs), (ins), [(int_tvm_dumpstk)],
                 "DUMPSTK", "DUMPSTK", 0xfe00>;

defm DUMPSTKTOP : I<(outs), (ins uimm8:$src), (outs), (ins uimm8:$src),
                    [(int_tvm_dumpstktop uimm8:$src)],
                    "DUMPSTKTOP\t$src",
                    "DUMPSTKTOP\t$src", 0xfe00>;
defm DUMP : I<(outs), (ins uimm8:$src), (outs), (ins uimm8:$src),
              [(int_tvm_dump uimm8:$src)],
              "DUMP\t$src",
              "DUMP\t$src", 0xfe20>;
defm PRINT : I<(outs), (ins uimm8:$src), (outs), (ins uimm8:$src),
               [(int_tvm_print uimm8:$src)],
               "PRINT\t$src",
               "PRINT\t$src", 0xfe30>;
defm DUMP_VALUE : I<(outs I257:$dst), (ins I257:$src), (outs), (ins),
                    [(set I257:$dst, (int_tvm_dump_value I257:$src))],
                    "DUMP\t$dst, $src",
                    "DUMP\t0", 0xfe20>;
defm PRINT_VALUE : I<(outs I257:$dst), (ins I257:$src), (outs), (ins),
                     [(set I257:$dst, (int_tvm_print_value I257:$src))],
                     "PRINT\t$dst, $src",
                     "PRINT\t0", 0xfe30>;

defm LOGSTR : I<(outs), (ins I257:$addr),
                (outs), (ins I257:$addr),
                [(int_tvm_logstr (i257 (TVMGlobalAddressWrapper tglobaladdr : $addr)))],
                "LOGSTR\t$addr",
                "LOGSTR\t$addr", 0xfef>;
defm PRINTSTR : I<(outs), (ins I257:$addr),
                  (outs), (ins I257:$addr),
                  [(int_tvm_printstr (i257 (TVMGlobalAddressWrapper tglobaladdr : $addr)))],
                  "PRINTSTR\t$addr",
                  "PRINTSTR\t$addr", 0xfef>;

defm LOGFLUSH : I<(outs), (ins), (outs), (ins), [(int_tvm_logflush)],
                  "LOGFLUSH", "LOGFLUSH", 0xfef000>;


//===----------------------------------------------------------------------===//
// Integer comparison
//===----------------------------------------------------------------------===//

let isCommutable = 1 in {
defm EQ : ComparisonInt<SETEQ, "EQUAL", 0xba>;
defm NE : ComparisonInt<SETNE, "NEQ", 0xbe>;
} // isCommutable = 1
defm SLT : ComparisonInt<SETLT,  "LESS", 0xb9>;
defm SGT : ComparisonInt<SETGT,  "GREATER", 0xbc>;
defm SLE : ComparisonInt<SETLE,  "LEQ", 0xbb>;
defm SGE : ComparisonInt<SETGE,  "GEQ", 0xbe>;

defm EQIMM : ComparisonImm<SETEQ, "EQINT", simm8, 0xc0>;
defm STLIMM : ComparisonImm<SETLT, "LESSINT", simm8, 0xc1>;
defm SGTIMM : ComparisonImm<SETGT, "GTINT", simm8, 0xc2>;
defm ULTIMM : ComparisonImm<SETULT, "LESSINT", simm8, 0xc1>;
defm UGTIMM : ComparisonImm<SETUGT, "GTINT", simm8, 0xc2>;
defm NEIMM : ComparisonImm<SETNE, "NEQINT", simm8, 0xc3>;

// Unsigned comparison a ult b = (a slt b)
class ComparisonUint<CondCode cond, Instruction instr> :
  Pat<(setcc I257:$a, I257:$b, cond), (instr I257:$a, I257:$b)>;
def : ComparisonUint<SETULT, SLT>;
def : ComparisonUint<SETUGT, SGT>;
def : ComparisonUint<SETULE, SLE>;
def : ComparisonUint<SETUGE, SGE>;

defm ISZERO : I<(outs I257:$dst), (ins I257:$src), (outs), (ins),
                 [(set I257:$dst, (setcc I257:$src, 0, SETEQ))],
                 "ISZERO\t$dst, $src", "ISZERO", 0xc000>;

multiclass CONDSEL_TEMPLATE<TVMRegClass RC> :
  I<(outs RC:$dst), (ins I257:$cond, RC:$lhs, RC:$rhs), (outs), (ins),
    [(set RC:$dst, (select I257:$cond, RC:$lhs, RC:$rhs))],
    "COND\t$dst, $cond, $lhs, $rhs", "CONDSEL", 0xe304>;

defm CONDSEL_I : CONDSEL_TEMPLATE<I257>;
defm CONDSEL_T : CONDSEL_TEMPLATE<Tuple>;
defm CONDSEL_S : CONDSEL_TEMPLATE<Slice>;
defm CONDSEL_B : CONDSEL_TEMPLATE<Builder>;
defm CONDSEL_C : CONDSEL_TEMPLATE<Cell>;

// ISD::SELECT requires its operand to conform to getBooleanContents, but
// TVM's CONDSEL interprets any non-zero value as true, so we can fold
// a setne and seteq with 0 into a select.
multiclass CONDSEL_PATTERNS<ValueType VT, TVMRegClass RC, NI CS> {
def : Pat<(select (i257 (seteq I257:$cond, (i257 0))), RC:$lhs, RC:$rhs),
          (CS I257:$cond, RC:$rhs, RC:$lhs)>;
def : Pat<(select (i257 (setne I257:$cond, (i257 0))), RC:$lhs, RC:$rhs),
          (CS I257:$cond, RC:$lhs, RC:$rhs)>;
}
defm : CONDSEL_PATTERNS<i257, I257, CONDSEL_I>;
defm : CONDSEL_PATTERNS<TVMTuple, Tuple, CONDSEL_T>;
defm : CONDSEL_PATTERNS<TVMSlice, Slice, CONDSEL_S>;
defm : CONDSEL_PATTERNS<TVMBuilder, Builder, CONDSEL_B>;
defm : CONDSEL_PATTERNS<TVMCell, Cell, CONDSEL_C>;

/***********************************************************************/

// ******************************************* 
//
// A.5.2. Division
//
// *******************************************/

// A905 - DIVR (x y - q0:= bx/y + 1/2c)
defm DIVR : BinaryRR<int_tvm_divr, "DIVR", 0xa905>;

// A906 - DIVC (x y - q00:= dx/ye)
defm DIVRC : BinaryRR<int_tvm_divc, "DIVC", 0xa906>;

// A90C — DIVMOD (x y - q r), where q := bx/yc, r := x - yq
defm DIVMOD : I<(outs I257:$q, I257:$r), (ins I257:$x, I257:$y),
                  (outs), (ins),
                  [(set I257:$q, I257:$r,
                    (int_tvm_divmod I257:$x, I257:$y))],
                  "DIVMOD\t$x, $y", "DIVMOD", 0xa90c>;

// A90D — DIVMODR (x y - q0 r0), where q0:= bx/y + 1/2c, r0:= x - yq0
defm DIVMODR : I<(outs I257:$q, I257:$r), (ins I257:$x, I257:$y),
                  (outs), (ins),
                  [(set I257:$q, I257:$r,
                    (int_tvm_divmodr I257:$x, I257:$y))],
                  "DIVMODR\t$x, $y", "DIVMODR", 0xa90c>;

// A90E — DIVMODC	(x y - q00 r00), where q00:= dx/ye, r00:= x - yq00
defm DIVMODC : I<(outs I257:$q, I257:$r), (ins I257:$x, I257:$y),
                  (outs), (ins),
                  [(set I257:$q, I257:$r,
                    (int_tvm_divmodc I257:$x, I257:$y))],
                  "DIVMODC\t$x, $y", "DIVMODC", 0xa90e>;

// A938tt — MODPOW2 tt + 1: (x - x mod 2tt+1)
defm MODPOW2 : I<(outs I257:$q),
             (ins I257:$x, uimm1_256:$tt),
             (outs), (ins uimm1_256:$tt),
             [(set I257:$q, (int_tvm_modpow2 I257:$x, uimm1_256:$tt))],
             "MODPOW2\t$tt, $x", "MODPOW2\t$tt", 0xa938>;

// A98C — MULDIVMOD (x y z - q r), where q := bx · y/zc, r := x · y mod z (same as */MOD in Forth)
defm MULDIVMOD : I<(outs I257:$q, I257:$r), (ins I257:$x, I257:$y, I257:$z),
                  (outs), (ins),
                  [(set I257:$q, I257:$r,
                    (int_tvm_muldivmod I257:$x, I257:$y, I257:$z))],
                  "MULDIVMOD\t$x, $y, $z", "MULDIVMOD", 0xa98c>;

// A9A4 — MULRSHIFT (x y z - bxy * 2-zc) for 0 <= z <= 256
defm MULRSHIFT : I<(outs I257:$q), (ins I257:$x, I257:$y, I257:$z),
                  (outs), (ins),
                  [(set I257:$q, 
                    (int_tvm_mulrshift I257:$x, I257:$y, I257:$z))],
                  "MULRSHIFT\t$x, $y, $z", "MULRSHIFT", 0xa9a4>;

// A9A5 — MULRSHIFTR (x y z - bxy * 2-z+ 1/2c) for 0 <= z <= 256
defm MULRSHIFTR : I<(outs I257:$q), (ins I257:$x, I257:$y, I257:$z),
                  (outs), (ins),
                  [(set I257:$q, 
                    (int_tvm_mulrshiftr I257:$x, I257:$y, I257:$z))],
                  "MULRSHIFTR\t$x, $y, $z", "MULRSHIFTR", 0xa9a5>;

// A9B4tt — MULRSHIFT tt + 1 (x y - bxy · 2-tt-1c)
defm MULRSHIFTTT : I<(outs I257 : $q),
             (ins I257:$x, I257 : $y, uimm1_256:$tt),
             (outs), (ins uimm1_256:$tt),
             [(set I257 : $q, (int_tvm_mulrshift_tt I257:$x, I257:$y, uimm1_256:$tt))],
             "MULRSHIFT\t$tt, $x, $y", "MULRSHIFT\t$tt", 0xa9b4>;

// A9B5tt — MULRSHIFTR	tt + 1 (x y - bxy · 2-tt-1+ 1/2c)
defm MULRSHIFTRTT : I<(outs I257 : $q),
            (ins I257:$x, I257 : $y, uimm1_256:$tt),
             (outs), (ins uimm1_256:$tt),
             [(set I257 : $q, (int_tvm_mulrshiftr_tt I257:$x, I257:$y, uimm1_256:$tt))],
             "MULRSHIFTR\t$tt", "MULRSHIFTR\t$tt", 0xa9b5>;

// A9C4 — LSHIFTDIV (x y z - b2zx/yc) for 0 <= z <= 256
defm LSHIFTDIV : I<(outs I257:$q), (ins I257:$x, I257:$y, I257:$z),
                  (outs), (ins),
                  [(set I257:$q, 
                    (int_tvm_lshiftdiv I257:$x, I257:$y, I257:$z))],
                  "LSHIFTDIV\t$x, $y, $z", "LSHIFTDIV", 0xa9c4>;

// A9C5 — LSHIFTDIVR (x y z - b2zx/y + 1/2c) for 0 <= z <= 256
defm LSHIFTDIVR : I<(outs I257:$q), (ins I257:$x, I257:$y, I257:$z),
                  (outs), (ins),
                  [(set I257:$q, 
                    (int_tvm_lshiftdivr I257:$x, I257:$y, I257:$z))],
                  "LSHIFTDIVR\t$x, $y, $z", "LSHIFTDIVR", 0xa9c4>;

// A9D4tt — LSHIFTDIV tt + 1 (x y - b2tt+1x/yc)
defm LSHIFTDIVTT : I<(outs I257 : $q),
            (ins I257:$x, I257 : $y, uimm1_256:$tt),
             (outs), (ins uimm1_256:$tt),
             [(set I257 : $q, (int_tvm_lshiftdiv_tt I257:$x, I257:$y, uimm1_256:$tt))],
             "LSHIFTDIV\t$tt, $x, $y", "LSHIFTDIV\t$tt", 0xa9d4>;

// A9D5tt — LSHIFTDIVR tt + 1 (x y - b2tt+1x/y+ 1/2c)
defm LSHIFTDIVRTT : I<(outs I257 : $q),
            (ins I257:$x, I257 : $y, uimm1_256:$tt),
             (outs), (ins uimm1_256:$tt),
             [(set I257 : $q, (int_tvm_lshiftdivr_tt I257:$x, I257:$y, uimm1_256:$tt))],
             "LSHIFTDIVR\t$tt, $x, $y", "LSHIFTDIVR\t$tt", 0xa9d5>;

/******************************************* 

A.5.3. Shifts, logical operations

*******************************************/

// AE — POW2 (y - 2**y), 0 <= y <= 1023, equivalent to ONE; SWAP; LSHIFT
defm POW2 : UnaryR<int_tvm_pow2, "POW2", 0xae>;

// B60A — MINMAX or INTSORT2 (x y - x y or y x), sorts two integers
let hasSideEffects = 1 in
defm MINMAX : I<(outs I257:$ox, I257:$oy), (ins I257:$x, I257:$y),
                  (outs), (ins),
                  [(set I257:$ox, I257:$oy,
                    (int_tvm_minmax I257:$x, I257:$y))],
                  "MINMAX\t$x, $y", "MINMAX", 0xb60a>;

// ******************************************* 
//
// A.5.4. Quiet arithmetic primitives
//
// *******************************************/ 

// B7A0 - QADD (x y - x + y), always works if x and y are	Integer s, but returns a NaN if the addition cannot be performed
defm QADD : BinaryRR<int_tvm_qadd, "QADD", 0xb7a0>;

// B7A904 — QDIV (x y - bx/yc), returns a NaN if y = 0, or if y = -1 and
//   x = -2256, or if either of x or y is a NaN
defm QDIV : BinaryRR<int_tvm_qdiv, "QDIV", 0xb7a904>;

// B7B0 — QAND (x y - x&y), bitwise “and” (similar to AND), but returns a NaN if either x or y is a NaN
defm QAND : BinaryRR<int_tvm_qand, "QAND", 0xb7b0>;

// B7B1 —  QOR (x y - x|y), bitwise “or”. If  x = -1 or y = -1, the result is always -1,
//  even if the other argument is a NaN
defm QOR : BinaryRR<int_tvm_qor, "QOR", 0xb7b1>;

// ******************************************* 
//
// A.6	Comparison primitives
//
// *******************************************/ 

// B8 - SGN (x - sgn(x)), computes the sign of an integer x: -1 if x < 0, 0 if x = 0, 1 if x > 0
defm SGN : UnaryR<int_tvm_sgn, "SGN", 0xB8>;

// C4 - ISNAN (x - x = NaN), checks whether x is a NaN
defm ISNAN : UnaryR<int_tvm_isnan, "ISNAN", 0xC4>;

// C5 - CHKNAN (x - x), throws an arithmetic overflow exception if x is a NaN
let hasSideEffects = 1 in
defm CHKNAN : UnaryR<int_tvm_chknan, "CHKNAN", 0xC5>;

/*********************************************************************/

include "TVMTupleInstrInfo.td"
include "TVMLiteralInstrInfo.td"
include "TVMComparisonInstrInfo.td"
include "TVMCellInstrInfo.td"
include "TVMDictionaryInstrInfo.td"
include "TVMApplicationInstrInfo.td"
include "TVMCodepageInstrInfo.td"
include "TVMMsgAddressInstrInfo.td"
