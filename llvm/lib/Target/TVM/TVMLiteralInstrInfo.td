//===------ TVMLiteralInstrInfo.td - A11 instructions ------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes A4 constant and literal instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instruction list..
let hasSideEffects = 1 in {
defm PUSHREF      : I0<(outs Cell : $cell), (ins),
                       [(set Cell : $cell, (int_tvm_pushref))],
                       "PUSHREF", 0x88>;
defm PUSHREFSLICE : I0<(outs Slice : $slice), (ins),
                       [(set Slice : $slice, (int_tvm_ctos(int_tvm_pushref)))],
                       "PUSHREFSLICE", 0x89>;
}
