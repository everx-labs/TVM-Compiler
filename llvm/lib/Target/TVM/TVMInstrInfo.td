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
def SDT_TVMCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i64>,
                                          SDTCisVT<1, i64>]>;
def SDT_TVMCallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i64>, SDTCisVT<1, i64>]>;
def SDT_TVMReturn       : SDTypeProfile<0, -1, []>;
def SDT_TVMArgument     : SDTypeProfile<1, 1, [SDTCisVT<1, i64>]>;
def SDT_TVMCall0        : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;
def SDT_TVMCall1        : SDTypeProfile<1, -1, [SDTCisPtrTy<1>]>;

def SDT_TVMCtos         : SDTypeProfile<1, 1, [SDTCisVT<1, i64>]>;
def SDT_TVMStSlice      : SDTypeProfile<1, 2, []>;
def SDT_TVMNewc         : SDTypeProfile<1, 0, []>;
def SDT_TVMEndc         : SDTypeProfile<1, 1, [SDTCisVT<1, i64>]>;
def SDT_TVMPushRoot     : SDTypeProfile<1, 0, []>;
def SDT_TVMDictGet      : SDTypeProfile<2, 3, []>;
def SDT_TVMDictSt       : SDTypeProfile<1, 3, []>;
def SDT_TVMDictLd       : SDTypeProfile<2, 2, []>;
def SDT_TVMLdRef        : SDTypeProfile<2, 1, []>;
def SDT_TVMSendRawMsg   : SDTypeProfile<0, 2, []>;
def SDT_TVMIfElse       : SDTypeProfile<0, 3, [SDTCisVT<0, i64>, SDTCisPtrTy<1>, SDTCisPtrTy<2>]>;
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
                       SDT_TVMReturn, [SDNPHasChain]>;
def TVMargument : SDNode<"TVMISD::ARGUMENT", SDT_TVMArgument>;
def TVMcall0 : SDNode<"TVMISD::CALL0", SDT_TVMCall0,
                      [SDNPHasChain, SDNPVariadic]>;
def TVMcall1 : SDNode<"TVMISD::CALL1", SDT_TVMCall1,
                      [SDNPHasChain, SDNPVariadic]>;

def TVMctos     : SDNode<"TVMISD::CTOS", SDT_TVMCtos>;
def TVMstslice  : SDNode<"TVMISD::STSLICE", SDT_TVMStSlice>;
def TVMendc     : SDNode<"TVMISD::ENDC", SDT_TVMEndc>;
def TVMpushroot : SDNode<"TVMISD::PUSHROOT", SDT_TVMPushRoot, [SDNPHasChain]>;
def TVMdictuget : SDNode<"TVMISD::DICTUGET", SDT_TVMDictGet, [SDNPHasChain]>;
def TVMsti      : SDNode<"TVMISD::STI", SDT_TVMDictSt, []>;
def TVMstu      : SDNode<"TVMISD::STU", SDT_TVMDictSt, []>;
def TVMldu      : SDNode<"TVMISD::LDU", SDT_TVMDictLd, []>;
def TVMldref    : SDNode<"TVMISD::LDREF", SDT_TVMLdRef, []>;
def TVMldix     : SDNode<"TVMISD::LDIX", SDT_TVMDictLd, []>;
def TVMldux     : SDNode<"TVMISD::LDUX", SDT_TVMDictLd, []>;
def TVMnewc     : SDNode<"TVMISD::NEWC", SDT_TVMNewc, []>;
def TVMldslicex : SDNode<"TVMISD::LDSLICEX", SDT_TVMDictLd, []>;
def TVMsendrawmsg : SDNode<"TVMISD::SENDRAWMSG", SDT_TVMSendRawMsg, []>;
def TVMifelse : SDNode<"TVMISD::IFELSE", SDT_TVMIfElse, [SDNPHasChain]>;

//===----------------------------------------------------------------------===//
// TVM-specific Operands.
//===----------------------------------------------------------------------===//

let OperandNamespace = "TVM" in {

// This operand represents stack slots (s0, s1, s2 etc.)
let OperandType = "OPERAND_STACK" in
def stack_op : Operand<i8>;

// This operand represents builder argument (see A.6 Cell primitives)
let OperandType = "OPERAND_BUILDER" in
def builder_op : Operand<i64>;

// This operand represents cell argument (see A.6 Cell primitives)
let OperandType = "OPERAND_CELL" in
def cell_op : Operand<i64>;

// This operand represents slice argument (see A.6 Cell primitives)
let OperandType = "OPERAND_SLICE" in
def slice_op : Operand<i64>;

let OperandType = "OPERAND_I64IMM" in
def i64imm_op : Operand<i64>;

let OperandType = "OPERAND_FUNCTION" in
def function_op : Operand<i64>;

let OperandType = "OPERAND_BASIC_BLOCK" in
def bb_op : Operand<OtherVT>;
}

//===----------------------------------------------------------------------===//
// Instruction list..

defm ADJCALLSTACKDOWN : NRI<(outs), (ins i64imm:$amt, i64imm:$amt2),
                            [(TVMcallseq_start timm:$amt, timm:$amt2)]>;
defm ADJCALLSTACKUP : NRI<(outs), (ins i64imm:$amt, i64imm:$amt2),
                          [(TVMcallseq_end timm:$amt, timm:$amt2)]>;

let hasSideEffects = 1, Uses = [ARGUMENTS], isCodeGenOnly = 1 in
defm ARGUMENT : I<(outs I64:$res), (ins i64imm:$argno),
                  (outs), (ins i64imm:$argno),
                  [(set I64:$res, (TVMargument timm:$argno))]>;

// Auxiliary instruction for debugging purposes only: 
// specifies number of arguments on stack for the function;
// normally this number is taken from MachineInfo Param vector.
let hasSideEffects = 1, isCodeGenOnly = 1 in
defm ARGUMENT_NUM : I<(outs), (ins i64imm:$argnum), (outs), (ins i64imm:$argnum), []>;

include "TVMControlFlowInstrInfo.td"

//===----------------------------------------------------------------------===//
// Selection patterns
//===----------------------------------------------------------------------===//
def inc : PatFrag<(ops node:$in), (add node:$in, 1)>;
def dec : PatFrag<(ops node:$in), (add node:$in, -1)>;
//===----------------------------------------------------------------------===//
// Basic stack manipulation primitives
//===----------------------------------------------------------------------===//

defm NOP : SI<(ins), "NOP", 0x00>;

// Stack manipulation instructions are not supposed to be selected, instead
// Stack Model pass inserts them to manipulate with local and global variables.
// They has mayLoad because it reads from a local, which is a side effect
// not otherwise modeled in LLVM.

let mayLoad = 1, isAsCheapAsAMove = 1 in
defm PUSH : SI<(ins stack_op:$local), "PUSH\t$local", 0x20>;

let mayLoad = 1, isAsCheapAsAMove = 1 in
defm XCHG : SI<(ins stack_op:$src, stack_op:$dst), "XCHG\t$src, $dst", 0x10>;
defm POP  : SI<(ins stack_op:$dst), "POP\t$dst", 0x30>;

let isCodeGenOnly = 1 in
defm REG_TO_REG_COPY : I<(outs I64:$dst), (ins I64:$src), (outs), (ins), []>;

//===----------------------------------------------------------------------===//
// Arithmetic instructions
//===----------------------------------------------------------------------===//
let isCommutable = 1 in {
defm ADD      : BinaryRR<add, "ADD", 0xa0>;
defm MUL      : BinaryRR<mul, "MUL", 0xa8>;
defm AND      : BinaryRR<and, "AND", 0xb0>;
defm OR       : BinaryRR<or, "OR", 0xb1>;
defm XOR      : BinaryRR<xor, "XOR", 0xb2>;
defm MIN      : BinaryRR<smin, "MIN", 0xb608>;
defm MAX      : BinaryRR<smax, "MAX", 0xb609>;
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

def : Pat<(srl I64:$val, imm:$off),
          (SHRCONST (AND I64:$val, (CONST_U64 0xFFFFFFFFFFFFFFFF)), imm:$off)>;

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

defm CONDSEL : I<(outs I64:$dst), (ins I64:$lhs, I64:$rhs, I64:$cond),
                 (outs), (ins),
                 [(set I64:$dst, (select I64:$cond, I64:$lhs, I64:$rhs))],
                 "COND\t$dst, $lhs, $rhs, $cond", "CONDSEL", 0xe304>;

// ISD::SELECT requires its operand to conform to getBooleanContents, but
// TVM's CONDSEL interprets any non-zero value as true, so we can fold
// a setne and seteq with 0 into a select.
def : Pat<(select (i64 (seteq I64:$cond, 0)), I64:$lhs, I64:$rhs),
          (CONDSEL I64:$lhs, I64:$rhs, I64:$cond)>;
def : Pat<(select (i64 (setne I64:$cond, 0)), I64:$lhs, I64:$rhs),
          (CONDSEL I64:$rhs, I64:$lhs, I64:$cond)>;

include "TVMCellInstrInfo.td"
include "TVMDictionaryInstrInfo.td"
