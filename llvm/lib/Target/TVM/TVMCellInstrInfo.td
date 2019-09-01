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
defm NEWC : I<(outs Builder:$bldr), (ins), (outs), (ins),
              [(set Builder:$bldr, (TVMnewc))],
              "NEWC\t$bldr", "NEWC", 0xc8>;
def : Pat<(int_tvm_newc), (NEWC)>;

let hasSideEffects = 1 in
defm ENDC : I<(outs Cell:$cell), (ins Builder:$bldr), (outs), (ins),
              [(set Cell:$cell, (TVMendc Builder:$bldr))],
              "ENDC\t$cell, $bldr", "ENDC", 0xc9>;
def : Pat<(int_tvm_endc Builder:$bldr), (ENDC Builder:$bldr)>;

let hasSideEffects = 1 in
defm CTOS : I<(outs Slice:$slice), (ins Cell:$cell), (outs), (ins),
              [(set Slice:$slice, (TVMctos Cell:$cell))],
              "CTOS\t$slice, $cell", "CTOS", 0xd0>;
def : Pat<(int_tvm_ctos Cell:$cell), (CTOS Cell:$cell)>;

defm ENDS : I0<(outs), (ins Slice : $slice),
               [(int_tvm_ends Slice : $slice)],
               "ENDS", 0xd1>;

let hasSideEffects = 1 in {
defm STI : I<(outs Builder : $builder),
             (ins I257 : $val, Builder : $builderi, uimm0_257 : $precision),
             (outs), (ins uimm0_257 : $precision),
             [(set Builder : $builder, (TVMsti I257 : $val, Builder : $builderi,
               uimm0_257 : $precision))],
             "STI\t$precision", "STI\t$precision", 0xca>;

defm STU : I<(outs Builder : $obldr),
             (ins I257 : $val, Builder : $bldr, uimm0_256 : $precision),
             (outs), (ins uimm0_256 : $precision),
             [(set Builder : $obldr, (TVMstu I257 : $val, Builder : $bldr,
               uimm0_256 : $precision))],
             "STU\t$precision, $obldr, $val, $bldr", "STU\t$precision", 0xcb>;
}

def : Pat<(int_tvm_stu I257 : $val, Builder : $bldr, uimm0_256 : $precision),
          (STU I257 : $val, Builder : $bldr, uimm0_256 : $precision)>;

def : Pat<(int_tvm_sti I257 : $val, Builder : $bldr, uimm0_257 : $precision),
          (STI I257 : $val, Builder : $bldr, uimm0_257 : $precision)>;

defm STREF : I<(outs Builder:$obldr), (ins Cell:$icell, Builder:$ibldr),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_stref Cell:$icell, Builder:$ibldr))],

               "STREF\t$obldr, $icell, $ibldr", "STREF", 0xcc>;

defm LDI : I<(outs I257 : $x, Slice : $sliceo),
             (ins Slice : $slicei, uimm0_257 : $precision),
             (outs), (ins uimm0_257 : $precision),
             [(set I257 : $x, Slice : $sliceo,
               (int_tvm_ldi Slice : $slicei, uimm0_257 : $precision))],
             "LDI\t $precision, $slicei, $x, $sliceo",
             "LDI\t $precision", 0xd2>;

defm LDU : I<(outs I257 : $x, Slice : $sliceo),
             (ins Slice : $slicei, uimm0_256 : $precision),
             (outs), (ins uimm0_256 : $precision),
             [(set I257 : $x, Slice : $sliceo,
               (int_tvm_ldu Slice : $slicei, uimm0_256 : $precision))],
             "LDU\t $precision, $slicei, $x, $sliceo",
             "LDU\t $precision", 0xd3>;

defm LDIX : I0<(outs I257 : $x, Slice : $sliceo),
               (ins Slice : $slicei, I257 : $precision),
               [(set I257 : $x, Slice : $sliceo,
                 (int_tvm_ldi Slice : $slicei, I257 : $precision))],
               "LDIX", 0xd700>;

defm LDUX : I0<(outs I257 : $x, Slice : $sliceo),
               (ins Slice : $slicei, I257 : $precision),
               [(set I257 : $x, Slice : $sliceo,
                 (int_tvm_ldu Slice : $slicei, I257 : $precision))],
               "LDUX", 0xd701>;

/// TODO: It should be a pipehole optimization for LDREF; SWAP; CTOS
defm LDREFRTOS : I<(outs Slice:$cellslice, Slice:$sliceo), (ins Slice:$slicei),
                   (outs), (ins),
                   [(set Slice:$cellslice, Slice:$sliceo, (int_tvm_ldrefrtos Slice:$slicei))],
                   "LDREFRTOS\t $cellslice, $sliceo, $slicei", "LDREFRTOS", 0xd5>;

let hasSideEffects = 1 in
defm LDREF : I<(outs Cell:$ocell, Slice:$oslice), (ins Slice:$islice),
               (outs), (ins),
               [(set Cell:$ocell, Slice:$oslice, (TVMldref Slice:$islice))],
               "LDREF\t$ocell, $oslice, $islice", "LDREF", 0xd4>;
def : Pat<(int_tvm_ldref Slice:$islice), (LDREF Slice:$islice)>;

let hasSideEffects = 1 in
defm LDSLICEX : I<(outs Slice:$result, Slice:$remainder), (ins Slice:$slice, I257:$size),
                  (outs), (ins),
                  [(set Slice:$result, Slice:$remainder,
                      (TVMldslicex Slice:$slice, I257:$size))],
                  "LDSLICEX\t$result, $remainder, $slice, $size", "LDSLICEX", 0xd718>;
def : Pat<(int_tvm_ldslicex Slice:$slice, I257:$size),
          (LDSLICEX Slice:$slice, I257:$size)>;

/// TODO: If to remove the following line Pat doesn't work. Investigate why.
let hasSideEffects = 1 in
defm STSLICE : I<(outs Builder:$out_bldr), (ins Slice:$slicei, Builder:$bldr),
                 (outs), (ins),
                 [(set Builder:$out_bldr, (TVMstslice Slice:$slicei, Builder:$bldr))],
                 "STSLICE\t$out_bldr, $slicei, $bldr", "STSLICE", 0xce>;

def : Pat<(int_tvm_stslice Slice:$slicei, Builder:$bldr),
          (STSLICE Slice:$slicei, Builder:$bldr)>;

let hasSideEffects = 1 in
defm SENDRAWMSG : I<(outs), (ins Cell:$cell, I257:$flags),
                    (outs), (ins),
                    [(TVMsendrawmsg Cell:$cell, I257:$flags)],
                    "SENDRAWMSG\t$cell, $flags", "SENDRAWMSG", 0xfb00>;
def : Pat<(int_tvm_sendrawmsg Cell:$cell, I257:$flags),
          (SENDRAWMSG Cell:$cell, I257:$flags)>;

defm STIX : I0<(outs Builder : $builder),
               (ins I257 : $value, Builder : $builderi, I257 : $size),
               [(set Builder : $builder, (int_tvm_sti I257 : $value,
                                          Builder : $builderi, I257 : $size))],
               "STIX", 0xcf00>;

defm STUX : I0<(outs Builder : $builder),
               (ins I257 : $value, Builder : $builderi, I257 : $size),
               [(set Builder : $builder, (int_tvm_stu I257 : $value,
                                          Builder : $builderi, I257 : $size))],
               "STUX", 0xcf01>;
