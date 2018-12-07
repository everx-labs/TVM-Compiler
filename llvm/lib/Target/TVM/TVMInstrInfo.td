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
// Instruction list..

defm ADJCALLSTACKDOWN : NRI<(outs), (ins i64imm:$amt, i64imm:$amt2),
                            [(TVMcallseq_start timm:$amt, timm:$amt2)]>;
defm ADJCALLSTACKUP : NRI<(outs), (ins i64imm:$amt, i64imm:$amt2),
                          [(TVMcallseq_end timm:$amt, timm:$amt2)]>;

let hasSideEffects = 1, Uses = [ARGUMENTS], isCodeGenOnly = 1 in
defm ARGUMENT : I<(outs I64:$res), (ins i64imm:$argno),
                  (outs), (ins i64imm:$argno),
                  [(set I64:$res, (TVMargument timm:$argno))]>;


defm NOP : NRI<(outs), (ins), [], "nop", 0x00>;

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
// Arithmetic instructions
//===----------------------------------------------------------------------===//
let isCommutable = 1 in
defm ADD : BinaryInt<add, "ADD ", 0xa0>;
defm SUB : BinaryInt<sub, "SUB ", 0xa1>;
