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

//===----------------------------------------------------------------------===//
// TVM-specific Operands.
//===----------------------------------------------------------------------===//

let OperandNamespace = "TVM" in {

// This operand represents stack slots (s0, s1, s2 etc.)
let OperandType = "OPERAND_STACK" in
def stack_op : Operand<i8>;

let OperandType = "OPERAND_I64IMM" in
def i64imm_op : Operand<i64>;

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


let isTerminator = 1, hasCtrlDep = 1, isBarrier = 1, isReturn = 1 in {
  defm RETURN_I64 : NRI<(outs), (ins i64imm:$val), [(TVMreturn i64:$val)],
                        "NOP", 0x00>;
  defm RETURN_VOID : NRI<(outs), (ins), [(TVMreturn)], "NOP", 0x00>;
  let isCodeGenOnly = 1 in
  defm FALLTHROUGH_RETURN_VOID : NRI<(outs), (ins), []>;
}

let isMoveImm = 1, isAsCheapAsAMove = 1, isReMaterializable = 1 in {
defm CONST_I64 : I<(outs I64:$res), (ins i64imm:$imm),
                   (outs), (ins i64imm:$imm),
                   [(set I64:$res, imm:$imm)],
                   "PUSHINT\t$res, $imm", "PUSHINT\t$imm", 0x82>;
}
//===----------------------------------------------------------------------===//
// Selection patterns
//===----------------------------------------------------------------------===//
def inc : PatFrag<(ops node:$in), (add node:$in, 1)>;
def dec : PatFrag<(ops node:$in), (add node:$in, -1)>;
//===----------------------------------------------------------------------===//
// Basic stack manipulation primitives
//===----------------------------------------------------------------------===//
// Stack manipulation instructions are not supposed to be selected, instead
// ExplicitLocal pass inserts them to manipulate with local variables.
// They has mayLoad because it reads from a local, which is a side effect
// not otherwise modeled in LLVM.

// Peudo XCHG s0, s0.
let mayLoad = 1, isAsCheapAsAMove = 1, isCodeGenOnly = 1 in
defm CG_XCHG : I<(outs I64:$res), (ins stack_op:$local),
                 (outs), (ins stack_op:$local), []>;
let mayLoad = 1, isAsCheapAsAMove = 1 in
defm PUSH : I<(outs I64:$res), (ins stack_op:$local),
              (outs), (ins stack_op:$local), [],
              "PUSH\t$res, s$local", "PUSH\ts$local", 0x20>;

let mayLoad = 1, isAsCheapAsAMove = 1 in
defm XCHG : I<(outs I64:$res), (ins stack_op:$local),
              (outs), (ins stack_op:$local), [],
              "XCHG\t$res, $local", "XCHG\t$local", 0x10>;
defm POP : NRI<(outs), (ins stack_op:$dst), [], "POP\ts$dst", 0x30>;
defm NOP : NRI<(outs), (ins), [], "NOP", 0x00>;
//===----------------------------------------------------------------------===//
// Arithmetic instructions
//===----------------------------------------------------------------------===//
let isCommutable = 1 in {
defm ADD      : BinaryRR<add, "ADD ", 0xa0>;
defm MUL      : BinaryRR<mul, "MUL ", 0xa8>;
defm AND      : BinaryRR<and, "AND ", 0xb0>;
defm OR       : BinaryRR<or, "OR  ", 0xb1>;
defm XOR      : BinaryRR<xor, "XOR ", 0xb2>;
}
defm SUB      : BinaryRR<sub, "SUB ", 0xa1>;
defm NEGATE   : UnaryR<ineg, "NEGATE ", 0xa3>;
defm INC      : UnaryR<inc, "INC ", 0xa4>;
defm DEC      : UnaryR<dec, "DEC ", 0xa5>;
defm DIV      : BinaryRR<sdiv, "DIV ", 0xa904>;
defm NOT      : UnaryR<not, "NOT ", 0xb3>;
defm SHL      : BinaryRR<shl, "SHL ", 0xaa>;
defm SHR      : BinaryRR<sra, "SHR ", 0xab>;
defm MOD      : BinaryRR<srem, "MOD ", 0x81>;
defm ADDCONST : BinaryRI<add, "ADDCONST ", simm8, 0xa6>;
defm MULCONST : BinaryRI<mul, "MULCONST ", simm8, 0xa7>;