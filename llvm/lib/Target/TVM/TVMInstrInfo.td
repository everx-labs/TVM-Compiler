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
def SDT_TVMPushRoot     : SDTypeProfile<1, 0, []>;
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

def TVMctos     : SDNode<"TVMISD::CTOS", SDT_TVMCtos>;
def TVMstslice  : SDNode<"TVMISD::STSLICE", SDT_TVMStSlice>;
def TVMendc     : SDNode<"TVMISD::ENDC", SDT_TVMEndc>;
def TVMpushroot : SDNode<"TVMISD::PUSHROOT", SDT_TVMPushRoot, []>;
def TVMdictuget : SDNode<"TVMISD::DICTUGET", SDT_TVMDictGet, [SDNPHasChain]>;
def TVMsti      : SDNode<"TVMISD::STI", SDT_TVMDictSt, []>;
def TVMstu      : SDNode<"TVMISD::STU", SDT_TVMDictSt, []>;
def TVMldref    : SDNode<"TVMISD::LDREF", SDT_TVMLdRef, []>;
def TVMnewc     : SDNode<"TVMISD::NEWC", SDT_TVMNewc, []>;
def TVMldslicex : SDNode<"TVMISD::LDSLICEX", SDT_TVMDictLd, []>;
def TVMsendrawmsg : SDNode<"TVMISD::SENDRAWMSG", SDT_TVMSendRawMsg, [SDNPHasChain]>;
def TVMifelse   : SDNode<"TVMISD::IFELSE", SDT_TVMIfElse, [SDNPHasChain]>;
def TVMjumpx    : SDNode<"TVMISD::JUMPX", SDT_TVMJumpX, [SDNPHasChain]>;
def TVMifjmp    : SDNode<"TVMISD::IFJMP", SDT_TVMIfJmp, [SDNPHasChain]>;
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
defm SWAP : SI<(ins), "SWAP", 0x01>;
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

// XCHG, XCHG, XCHG
defm XCHG3 : SI<(ins stack_op:$i, stack_op:$j, stack_op:$k),
                "XCHG3\t$i, $j, $k", 0x04>;
// XCHG, XCHG, PUSH
defm XC2PU : SI<(ins stack_op:$i, stack_op:$j, stack_op:$k),
                "XC2PU\t$i, $j, $k", 0x541>;
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

defm POP  : SI<(ins stack_op:$dst), "POP\t$dst", 0x30>;
defm DROP : SI<(ins), "DROP", 0x30>;
defm BLKDROP : SI<(ins uimm8:$sz), "BLKDROP\t$sz", 0x5F0>;
defm DROPX: SI<(ins), "DROPX", 0x65>;
defm BLKSWAP : SI<(ins uimm8:$deep, uimm8:$top), "BLKSWAP\t$deep, $top", 0x55>;
defm BLKSWX : SI<(ins), "BLKSWX", 0x63>;
defm ROLL : SI<(ins uimm8:$sz), "ROLL\t$sz", 0x55>;
defm ROLLREV : SI<(ins uimm8:$sz), "ROLLREV\t$sz", 0x55>;
defm ROLLX : SI<(ins), "ROLLX", 0x61>;
defm ROLLREVX : SI<(ins), "ROLLREVX", 0x62>;
defm REVERSE : SI<(ins uimm8:$sz, uimm8:$topIdx),
                   "REVERSE\t$sz, $topIdx", 0x5E>;
defm REVX: SI<(ins), "REVX", 0x64>;
defm NIP  : SI<(ins), "NIP", 0x31>;
defm DUP  : SI<(ins), "DUP", 0x20>;
defm ZERO : SI<(ins), "ZERO", 0x70>;
}

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

defm CONDSEL : I<(outs I257:$dst), (ins I257:$lhs, I257:$rhs, I257:$cond),
                 (outs), (ins),
                 [(set I257:$dst, (select I257:$cond, I257:$lhs, I257:$rhs))],
                 "COND\t$dst, $lhs, $rhs, $cond", "CONDSEL", 0xe304>;

// ISD::SELECT requires its operand to conform to getBooleanContents, but
// TVM's CONDSEL interprets any non-zero value as true, so we can fold
// a setne and seteq with 0 into a select.
def : Pat<(select (i257 (seteq I257:$cond, 0)), I257:$lhs, I257:$rhs),
          (CONDSEL I257:$lhs, I257:$rhs, I257:$cond)>;
def : Pat<(select (i257 (setne I257:$cond, 0)), I257:$lhs, I257:$rhs),
          (CONDSEL I257:$rhs, I257:$lhs, I257:$cond)>;

include "TVMTupleInstrInfo.td"
include "TVMLiteralInstrInfo.td"
include "TVMComparisonInstrInfo.td"
include "TVMCellInstrInfo.td"
include "TVMDictionaryInstrInfo.td"
include "TVMApplicationInstrInfo.td"
include "TVMCodepageInstrInfo.td"
