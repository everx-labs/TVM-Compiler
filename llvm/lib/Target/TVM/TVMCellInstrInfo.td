//===-- TVMCellInstrInfo.td - TVM Cell Instruction defs ----*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
/// \file
/// This file describes Cell TVM instructions (A.6) in TableGen format.
//
//===----------------------------------------------------------------------===//

defm NEWC : I<(outs I64 : $bldr), (ins), (outs), (ins),
              [(set I64 : $bldr, (TVMnewc))],
              "NEWC\t$bldr", "NEWC", 0xc8>;
defm ENDC : I<(outs I64 : $cell), (ins I64 : $bldr), (outs), (ins),
              [(set I64 : $cell, (TVMendc I64 : $bldr))],
              "ENDC\t $cell, $bldr", "ENDC", 0xc9>;

defm CTOS : I<(outs I64 : $slice), (ins I64 : $cell), (outs), (ins),
              [(set I64 : $slice, (TVMctos I64 : $cell))],
              "CTOS\t $slice, $cell", "CTOS", 0xd0>;

/// TODO: Support other integer types.
defm STI : I<(outs I64 : $obldr),
             (ins I64 : $val, I64 : $bldr, uimm1_256 : $precision),
             (outs), (ins uimm8 : $precision),
             [(set I64 : $obldr, (TVMsti I64 : $val, I64 : $bldr, uimm1_256 : $precision))],
             "STI\t $precision, $obldr, $val, $bldr", "STI\t $precision", 0xcaff>;
defm STU : I<(outs I64 : $obldr),
             (ins I64 : $val, I64 : $bldr, uimm1_256 : $precision),
             (outs), (ins uimm8 : $precision),
             [(set I64 : $obldr, (TVMstu I64 : $val, I64 : $bldr, uimm1_256 : $precision))],
             "STU\t $precision, $obldr, $val, $bldr", "STU\t $precision", 0xcbff>;

/// TODO: Support other integer types.
defm LDI : I<(outs I64 : $x, I64 : $sliceo),
             (ins I64 : $slicei, uimm1_256 : $precision),
             (outs), (ins uimm1_256 : $precision),
             [(set I64 : $x, I64 : $sliceo, (TVMldi I64 : $slicei, uimm1_256 : $precision))],
             "LDI\t $precision, $slicei, $x, $sliceo",
             "LDI\t $precision", 0xd2ff>;
defm LDU : I<(outs I64 : $x, I64  : $sliceo),
             (ins I64 : $slicei, uimm8 : $precision),
             (outs), (ins uimm8 : $precision),
             [(set I64 : $x, I64 : $sliceo, (TVMldu I64 : $slicei, uimm1_256 : $precision))],
             "LDU\t $precision, $slicei, $x, $sliceo",
             "LDU\t $precision", 0xd3ff>;
/// TODO: It should be a pipehole optimization for LDREF; SWAP; CTOS
defm LDREFRTOS : I<(outs I64 : $cellslice, I64 : $sliceo), (ins I64 : $slicei),
                   (outs), (ins),
                   [(set I64 : $cellslice, I64 : $sliceo, (int_tvm_ldrefrtos I64 : $slicei))],
                   "LDREFRTOS\t $cellslice, $sliceo, $slicei", "LDREFRTOS", 0xd5>;
defm STSLICE : I<(outs I64 : $sliceo), (ins I64 : $slicei, I64 : $bldr),
                 (outs), (ins),
                 [(set I64 : $sliceo, (int_tvm_stslice I64 : $slicei, I64 : $bldr))],
                 "STSLICE\t $slicei, $sliceo, $bldr", "STSLICE", 0xce>;
