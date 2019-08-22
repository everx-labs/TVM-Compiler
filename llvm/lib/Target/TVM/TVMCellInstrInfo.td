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
defm NEWC : I<(outs I257 : $bldr), (ins), (outs), (ins),
              [(set I257 : $bldr, (TVMnewc))],
              "NEWC\t$bldr", "NEWC", 0xc8>;
def : Pat<(int_tvm_newc), (NEWC)>;

let hasSideEffects = 1 in
defm ENDC : I<(outs I257 : $cell), (ins I257 : $bldr), (outs), (ins),
              [(set I257 : $cell, (TVMendc I257 : $bldr))],
              "ENDC\t$cell, $bldr", "ENDC", 0xc9>;
def : Pat<(int_tvm_endc I257 : $bldr), (ENDC I257 : $bldr)>;

let hasSideEffects = 1 in
defm CTOS : I<(outs I257 : $slice), (ins I257 : $cell), (outs), (ins),
              [(set I257 : $slice, (TVMctos I257 : $cell))],
              "CTOS\t$slice, $cell", "CTOS", 0xd0>;
def : Pat<(int_tvm_ctos I257 : $cell), (CTOS I257 : $cell)>;

/// TODO: Support other integer types.
defm STI : I<(outs I257 : $obldr),
             (ins I257 : $val, I257 : $bldr, uimm1_256 : $precision),
             (outs), (ins uimm8 : $precision),
             [(set I257 : $obldr, (TVMsti I257 : $val, I257 : $bldr, uimm1_256 : $precision))],
             "STI\t $precision, $obldr, $val, $bldr", "STI\t$precision", 0xcaff>;

let hasSideEffects = 1 in
defm STU : I<(outs I257 : $obldr),
             (ins I257 : $val, I257 : $bldr, uimm1_256 : $precision),
             (outs), (ins uimm8 : $precision),
             [(set I257 : $obldr, (TVMstu I257 : $val, I257 : $bldr, uimm1_256 : $precision))],
             "STU\t $precision, $obldr, $val, $bldr", "STU\t$precision", 0xcbff>;
def : Pat<(int_tvm_stu I257 : $val, I257 : $bldr, uimm1_256 : $precision),
          (STU I257 : $val, I257 : $bldr, uimm1_256 : $precision)>;

defm STREF : I<(outs I257 : $obldr), (ins I257 : $icell, I257 : $ibldr),
               (outs), (ins),
               [(set I257 : $obldr, (int_tvm_stref I257 : $icell, I257 : $ibldr))],
               "STREF\t$obldr, $icell, $ibldr", "STREF", 0xcc>;

defm LDU : I<(outs I257 : $x, I257  : $sliceo),
             (ins I257 : $slicei, uimm8 : $precision),
             (outs), (ins uimm8 : $precision),
             [(set I257 : $x, I257 : $sliceo, (TVMldu I257 : $slicei, uimm1_256 : $precision))],
             "LDU\t $precision, $slicei, $x, $sliceo",
             "LDU\t $precision", 0xd3ff>;

defm LDIX : I<(outs I257 : $x, I257 : $sliceo),
              (ins I257 : $slicei, I257 : $precision),
              (outs), (ins),
              [(set I257 : $x, I257 : $sliceo, (TVMldix I257 : $slicei, I257 : $precision))],
              "LDIX\t $precision, $slicei, $x, $sliceo",
              "LDIX", 0xd700>;

defm LDUX : I<(outs I257 : $x, I257  : $sliceo),
              (ins I257 : $slicei, I257 : $precision),
              (outs), (ins),
              [(set I257 : $x, I257 : $sliceo, (TVMldux I257 : $slicei, I257 : $precision))],
              "LDUX\t$precision, $slicei, $x, $sliceo",
              "LDUX", 0xd701>;

/// TODO: It should be a pipehole optimization for LDREF; SWAP; CTOS
defm LDREFRTOS : I<(outs I257 : $cellslice, I257 : $sliceo), (ins I257 : $slicei),
                   (outs), (ins),
                   [(set I257 : $cellslice, I257 : $sliceo, (int_tvm_ldrefrtos I257 : $slicei))],
                   "LDREFRTOS\t $cellslice, $sliceo, $slicei", "LDREFRTOS", 0xd5>;

let hasSideEffects = 1 in
defm LDREF : I<(outs I257 : $ocell, I257 : $oslice), (ins I257 : $icell),
               (outs), (ins),
               [(set I257 : $ocell, I257 : $oslice, (TVMldref I257 : $icell))],
               "LDREF\t$ocell, $oslice, $icell", "LDREF", 0xd4>;
def : Pat<(int_tvm_ldref I257 : $icell), (LDREF I257 : $icell)>;

let hasSideEffects = 1 in
defm LDSLICEX : I<(outs I257 : $result, I257 : $remainder), (ins I257 : $slice, I257 : $size),
                  (outs), (ins),
                  [(set I257 : $result, I257 : $remainder,
                      (TVMldslicex I257 : $slice, I257 : $size))],
                  "LDSLICEX\t$result, $remainder, $slice, $size", "LDSLICEX", 0xd718>;
def : Pat<(int_tvm_ldslicex I257 : $slice, I257 : $size),
          (LDSLICEX I257 : $slice, I257 : $size)>;

/// TODO: If to remove the following line Pat doesn't work. Investigate why.
let hasSideEffects = 1 in
defm STSLICE : I<(outs I257 : $sliceo), (ins I257 : $slicei, I257 : $bldr),
                 (outs), (ins),
                 [(set I257 : $sliceo, (TVMstslice I257 : $slicei, I257 : $bldr))],
                 "STSLICE\t$slicei, $sliceo, $bldr", "STSLICE", 0xce>;

def : Pat<(int_tvm_stslice I257 : $slicei, I257 : $bldr),
          (STSLICE I257 : $slicei, I257 : $bldr)>;

let hasSideEffects = 1 in
defm SENDRAWMSG : I<(outs), (ins I257 : $slice, I257 : $flags),
                    (outs), (ins),
                    [(TVMsendrawmsg I257 : $slice, I257 : $flags)],
                    "SENDRAWMSG\t$slice, $flags", "SENDRAWMSG", 0xfb00>;
def : Pat<(int_tvm_sendrawmsg I257 : $slice, I257 : $flags),
          (SENDRAWMSG I257 : $slice, I257 : $flags)>;
