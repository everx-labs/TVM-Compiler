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

let mayLoad = 1 in
defm PUSHSLICE_EMPTY : I0<(outs Slice : $slice), (ins),
                          [(set Slice : $slice, (int_tvm_pushslice_empty))],
                          "PUSHSLICE x8_", 0x8b08>;

def : Pat<(int_tvm_ctos(int_tvm_endc (int_tvm_newc))), (PUSHSLICE_EMPTY)>;

defm PUSHSLICE_0 : I0<(outs Slice : $slice), (ins),
                      [(set Slice : $slice, (int_tvm_ctos (int_tvm_endc (int_tvm_stu (i257 0), (int_tvm_newc), (i257 1)))))],
                      "PUSHSLICE x4_", 0x8b04>;

def : Pat<(int_tvm_ctos(int_tvm_endc (int_tvm_sti (i257 0), (int_tvm_newc), (i257 1)))), (PUSHSLICE_0)>;

defm PUSHSLICE_1 : I0<(outs Slice : $slice), (ins),
                      [(set Slice : $slice, (int_tvm_ctos (int_tvm_endc (int_tvm_stu (i257 1), (int_tvm_newc), (i257 1)))))],
                      "PUSHSLICE xc_", 0x8b04>;

def : Pat<(int_tvm_ctos(int_tvm_endc (int_tvm_sti (i257 1), (int_tvm_newc), (i257 1)))), (PUSHSLICE_1)>;
