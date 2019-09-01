//===----- TVMComparisonInstrInfo.td - A.3 instructions ----*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes A.3 null, tuple and list instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Immediate operands
def ExplImm : ImmAsmOperand<1, 15> { let Name = "CPImm"; }
def explimm : Operand<i257>, ImmLeaf<i257, [{
  return Imm >= 1 && Imm <= 15;
}]> {
  let ParserMatchClass = ExplImm;
}

//===----------------------------------------------------------------------===//
// Instruction list..
defm FIRST     : I0<(outs I257 : $result), (ins I257 : $tuple),
                    [(set I257 : $result, (int_tvm_first I257 : $tuple))],
                    "FIRST", 0x6f10>;
defm SECOND    : I0<(outs I257 : $result), (ins I257 : $tuple),
                    [(set I257 : $result, (int_tvm_second I257 : $tuple))],
                    "SECOND", 0x6f11>;
defm SETFIRST  : I0<(outs I257 : $result), (ins I257 : $tuple, I257 : $value),
                    [(set I257 : $result, (int_tvm_setfirst I257 : $tuple, I257 : $value))],
                    "SETFIRST", 0x6f50>;
defm SETSECOND : I0<(outs I257 : $result), (ins I257 : $tuple, I257 : $value),
                    [(set I257 : $result, (int_tvm_setsecond I257 : $tuple, I257 : $value))],
                    "SETSECOND", 0x6f51>;

// Note: EXPLODE unpacks $tuple and produces variable number of outputs.
// The instruction is not supposed to be selected and we ignore the fact that it
// produces multiple elements on stack, stack model has to handle it
// in a special way.
defm EXPLODE : I<(outs), (ins I257 : $tuple, explimm : $size),
                 (outs), (ins explimm : $size), [],
                 "EXPLODE\t$size", "EXPLODE\t$size", 0x6f4>;
defm TPUSH   : I0<(outs I257 : $tuple), (ins I257 : $tuplei, I257 : $value),
                  [], "TPUSH", 0x6f8c>;
defm TPOP    : I0<(outs I257 : $tuple, I257 : $value), (ins I257 : $tuplei),
                  [], "TPOP", 0x6f8d>;
