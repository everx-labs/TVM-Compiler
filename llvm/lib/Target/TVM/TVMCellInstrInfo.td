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

/// TODO: If to remove the following line Pat doesn't work. Investigate why.
let hasSideEffects = 1 in
defm NEWC : I<(outs I64 : $bldr), (ins), (outs), (ins),
              [(set I64 : $bldr, (TVMnewc))],
              "NEWC\t$bldr", "NEWC", 0xc8>;
def : Pat<(int_tvm_newc), (NEWC)>;

let hasSideEffects = 1 in
defm ENDC : I<(outs I64 : $cell), (ins I64 : $bldr), (outs), (ins),
              [(set I64 : $cell, (TVMendc I64 : $bldr))],
              "ENDC\t$cell, $bldr", "ENDC", 0xc9>;
def : Pat<(int_tvm_endc I64 : $bldr), (ENDC I64 : $bldr)>;

defm CTOS : I<(outs I64 : $slice), (ins I64 : $cell), (outs), (ins),
              [(set I64 : $slice, (TVMctos I64 : $cell))],
              "CTOS\t$slice, $cell", "CTOS", 0xd0>;

/// TODO: Support other integer types.
defm STI : I<(outs I64 : $obldr),
             (ins I64 : $val, I64 : $bldr, uimm1_256 : $precision),
             (outs), (ins uimm8 : $precision),
             [(set I64 : $obldr, (TVMsti I64 : $val, I64 : $bldr, uimm1_256 : $precision))],
             "STI\t $precision, $obldr, $val, $bldr", "STI\t$precision", 0xcaff>;

let hasSideEffects = 1 in
defm STU : I<(outs I64 : $obldr),
             (ins I64 : $val, I64 : $bldr, uimm1_256 : $precision),
             (outs), (ins uimm8 : $precision),
             [(set I64 : $obldr, (TVMstu I64 : $val, I64 : $bldr, uimm1_256 : $precision))],
             "STU\t $precision, $obldr, $val, $bldr", "STU\t$precision", 0xcbff>;
def : Pat<(int_tvm_stu I64 : $val, I64 : $bldr, uimm1_256 : $precision), 
          (STU I64 : $val, I64 : $bldr, uimm1_256 : $precision)>;

defm STREF : I<(outs I64 : $obldr), (ins I64 : $icell, I64 : $ibldr),
               (outs), (ins),
               [(set I64 : $obldr, (int_tvm_stref I64 : $icell, I64 : $ibldr))],
               "STREF\t$obldr, $icell, $ibldr", "STREF", 0xcc>;

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
defm LDREF : I<(outs I64 : $ocell, I64 : $oslice), (ins I64 : $icell),
               (outs), (ins),
               [(set I64 : $ocell, I64 : $oslice, (int_tvm_ldref I64 : $icell))],
               "LDREF\t$ocell, $oslice, $icell", "LDREF", 0xd4>;
defm LDSLICEX : I<(outs I64 : $result, I64 : $remainder), (ins I64 : $slice, I64 : $size),
                  (outs), (ins),
                  [(set I64 : $result, I64 : $remainder,
                      (int_tvm_ldslicex I64 : $slice, I64 : $size))],
                  "LDSLICEX\t$result, $remainder, $slice, $size", "LDSLICEX", 0xd718>;

/// TODO: If to remove the following line Pat doesn't work. Investigate why.
let hasSideEffects = 1 in
defm STSLICE : I<(outs I64 : $sliceo), (ins I64 : $slicei, I64 : $bldr),
                 (outs), (ins),
                 [(set I64 : $sliceo, (TVMstslice I64 : $slicei, I64 : $bldr))],
                 "STSLICE\t$slicei, $sliceo, $bldr", "STSLICE", 0xce>;

def : Pat<(int_tvm_stslice I64 : $slicei, I64 : $bldr),
          (STSLICE I64 : $slicei, I64 : $bldr)>;

defm SENDRAWMSG : I<(outs), (ins I64 : $slice, I64 : $flags),
                    (outs), (ins),
                    [(int_tvm_sendrawmsg I64 : $slice, I64 : $flags)],
                    "SENDRAWMSG\t$slice, $flags", "SENDRAWMSG", 0xfb00>;
