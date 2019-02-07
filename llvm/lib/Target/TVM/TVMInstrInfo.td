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

//===----------------------------------------------------------------------===//
// TVM-specific Operands.
//===----------------------------------------------------------------------===//

let OperandNamespace = "TVM" in {

// This operand represents stack slots (s0, s1, s2 etc.)
let OperandType = "OPERAND_STACK" in
def stack_op : Operand<i8>;

let OperandType = "OPERAND_I64IMM" in
def i64imm_op : Operand<i64>;

let OperandType = "OPERAND_FUNCTION" in
def function_op : Operand<i32>;

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

let isTerminator = 1, hasCtrlDep = 1 in {
  let isReturn = 1, isBarrier = 1 in {
    defm RETURN_I64 : NRI<(outs), (ins i64imm:$val), [(TVMreturn i64:$val)],
                          "NOP", 0x00>;
    defm RETURN_VOID : NRI<(outs), (ins), [(TVMreturn)], "NOP", 0x00>;
    let isCodeGenOnly = 1 in
    defm FALLTHROUGH_RETURN_VOID : NRI<(outs), (ins), []>;
  } // isReturn = 1, isBarrier = 1

  defm ISZERO : I<(outs I64:$dst), (ins I64:$src), (outs), (ins),
                   [(set I64:$dst, (setcc I64:$src, 0, SETEQ))],
                   "ISZERO\t$dst, $src", "ISZERO", 0xc000>;

  let isBranch = 1 in {
    // The condition operand is a boolean value which TVM represents as i64.
    defm IFJMP : I<(outs), (ins I64:$cond, bb_op:$dst),
                   (outs), (ins),
                   [(brcond I64:$cond, bb:$dst)],
                    "IFJMP\t$dst, $cond", "IFJMP", 0xe0>;

    let isBarrier = 1 in {
      defm JMPX  : I<(outs), (ins bb_op:$dst),
                     (outs), (ins),
                     [(br bb:$dst)],
                     "JMPX\t$dst", "JMPX", 0xd9>;
    } // isBarrier = 1
  } // isBranch = 1
} // isTerminator = 1, hasCtrlDep = 1


// TODO: The approach with two CALLs was copied from WebAssembly backend.
// We need to investigate how it works and why 2 CALLs are needed
let isCall = 1, hasCtrlDep = 1 in {
  defm CALL_VOID : NRI<(outs), (ins i64imm:$callee, variable_ops),
                       [(TVMcall0 i64:$callee)], "CALLX", 0xD9>;
  defm CALL_1 : NRI<(outs I64:$dst), (ins i64imm:$callee, variable_ops),
                    [(set I64:$dst, (TVMcall1 i64:$callee))],
                    "CALLX", 0xD9>;
}

let isMoveImm = 1, isAsCheapAsAMove = 1, isReMaterializable = 1 in {
defm CONST_I64 : I<(outs I64:$res), (ins i64imm:$imm),
                   (outs), (ins i64imm:$imm),
                   [(set I64:$res, imm:$imm)],
                   "PUSHINT\t$res, $imm", "PUSHINT\t$imm", 0x82>;
}

defm PUSHCONT_MBB : NRI<(outs), (ins bb_op:$bb), [], "PUSHCONT\t$bb", 0x8f>;
defm PUSHCONT_FUNC : NRI<(outs), (ins function_op:$callee), [],
                         "PUSHCONT\t$callee", 0x8f>;

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
// ExplicitLocal pass inserts them to manipulate with local variables.
// They has mayLoad because it reads from a local, which is a side effect
// not otherwise modeled in LLVM.

let mayLoad = 1, isAsCheapAsAMove = 1 in
defm PUSH : SI<(ins stack_op:$local), "PUSH\t$local", 0x20>;

let mayLoad = 1, isAsCheapAsAMove = 1 in
defm XCHG : SI<(ins stack_op:$src, stack_op:$dst), "XCHG\t$src, $dst", 0x10>;
defm POP  : SI<(ins stack_op:$dst), "POP\t$dst", 0x30>;
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
defm SUBR     : BinaryRR<sub, "SUBR ", 0xa2>;
defm NEGATE   : UnaryR<ineg, "NEGATE ", 0xa3>;
defm INC      : UnaryR<inc, "INC ", 0xa4>;
defm DEC      : UnaryR<dec, "DEC ", 0xa5>;
defm DIV      : BinaryRR<int_tvm_div, "DIV ", 0xa904>;
defm NOT      : UnaryR<not, "NOT ", 0xb3>;
defm SHL      : BinaryRR<shl, "SHL ", 0xaa>;
defm SHR      : BinaryRR<sra, "SHR ", 0xab>;
defm MOD      : BinaryRR<srem, "MOD ", 0x81>;
defm ADDCONST : BinaryRI<add, "ADDCONST ", simm8, 0xa6>;
defm MULCONST : BinaryRI<mul, "MULCONST ", simm8, 0xa7>;
