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
// newc is declared as mayLoad from inaccessible memory to avoid being moved
//  through `ACCEPT` instruction (with other cell build instructions)
// We need to find right way to represent gas-related primitives
//  (ACCEPT/SETGASLIMIT/BUYGAS) as a super-barriers
//  (no instructions reorder across them).

let hasSideEffects = 1, mayLoad=1 in
defm NEWC : I<(outs Builder:$bldr), (ins), (outs), (ins),
              [(set Builder:$bldr, (int_tvm_newc))],
              "NEWC\t$bldr", "NEWC", 0xc8>;

let hasSideEffects = 1 in
defm ENDC : I<(outs Cell:$cell), (ins Builder:$bldr), (outs), (ins),
              [(set Cell:$cell, (int_tvm_endc Builder:$bldr))],
              "ENDC\t$cell, $bldr", "ENDC", 0xc9>;

let hasSideEffects = 1 in
defm CTOS : I<(outs Slice:$slice), (ins Cell:$cell), (outs), (ins),
              [(set Slice:$slice, (int_tvm_ctos Cell:$cell))],
              "CTOS\t$slice, $cell", "CTOS", 0xd0>;

defm ENDS : I0<(outs), (ins Slice : $slice),
               [(int_tvm_ends Slice : $slice)],
               "ENDS", 0xd1>;

let hasSideEffects = 1 in {
defm STI : I<(outs Builder : $builder),
             (ins I257 : $val, Builder : $builderi, uimm1_256 : $precision),
             (outs), (ins uimm1_256 : $precision),
             [(set Builder : $builder, (TVMsti I257 : $val, Builder : $builderi,
               uimm1_256 : $precision))],
             "STI\t$precision", "STI\t$precision", 0xca>;

defm STU : I<(outs Builder : $obldr),
             (ins I257 : $val, Builder : $bldr, uimm1_256 : $precision),
             (outs), (ins uimm1_256 : $precision),
             [(set Builder : $obldr, (TVMstu I257 : $val, Builder : $bldr,
               uimm1_256 : $precision))],
             "STU\t$precision, $obldr, $val, $bldr", "STU\t$precision", 0xcb>;

defm STIR : I<(outs Builder : $builder),
              (ins Builder : $builderi, I257 : $val, uimm1_256 : $precision),
              (outs), (ins uimm1_256 : $precision), [],
              "STIR\t$precision", "STIR\t$precision", 0xcf0a>;

defm STUR : I<(outs Builder : $obldr),
              (ins Builder : $bldr, I257 : $val, uimm1_256 : $precision),
              (outs), (ins uimm1_256 : $precision), [],
              "STUR\t$precision", "STUR\t$precision", 0xcf0b>;
}

def : Pat<(int_tvm_stu I257 : $val, Builder : $bldr, uimm1_256 : $precision),
          (STU I257 : $val, Builder : $bldr, uimm1_256 : $precision)>;

def : Pat<(int_tvm_sti I257 : $val, Builder : $bldr, uimm1_256 : $precision),
          (STI I257 : $val, Builder : $bldr, uimm1_256 : $precision)>;

defm STREF : I<(outs Builder:$obldr), (ins Cell:$icell, Builder:$ibldr),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_stref Cell:$icell, Builder:$ibldr))],

               "STREF\t$obldr, $icell, $ibldr", "STREF", 0xcc>;

let hasSideEffects = 1 in {
defm LDI : I<(outs I257 : $x, Slice : $sliceo),
             (ins Slice : $slicei, uimm1_256 : $precision),
             (outs), (ins uimm1_256 : $precision),
             [(set I257 : $x, Slice : $sliceo,
               (int_tvm_ldi Slice : $slicei, uimm1_256 : $precision))],
             "LDI\t $precision, $slicei, $x, $sliceo",
             "LDI\t $precision", 0xd2>;

defm LDU : I<(outs I257 : $x, Slice : $sliceo),
             (ins Slice : $slicei, uimm1_256 : $precision),
             (outs), (ins uimm1_256 : $precision),
             [(set I257 : $x, Slice : $sliceo,
               (int_tvm_ldu Slice : $slicei, uimm1_256 : $precision))],
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

defm LDUQ : I<(outs I257:$x, Slice:$sliceo, I257:$succ),
              (ins Slice:$slicei, uimm1_256:$precision),
              (outs), (ins uimm1_256:$precision),
              [(set I257:$x, Slice:$sliceo, I257:$succ,
                (int_tvm_lduq Slice:$slicei, uimm1_256:$precision))],
              "LDUQ\t $precision, $slicei, $x, $sliceo, $succ",
              "LDUQ\t $precision NULLROTRIFNOT", 0xd70d>;

defm LDUXQ : I0<(outs I257:$x, Slice:$sliceo, I257:$succ),
                (ins Slice:$slicei, I257:$precision),
                [(set I257:$x, Slice:$sliceo, I257:$succ,
                  (int_tvm_lduq Slice:$slicei, I257:$precision))],
                "LDUXQ NULLROTRIFNOT", 0xd705>;

defm LDVARUINT16 : I0<(outs I257:$x, Slice:$sliceo), (ins Slice:$slicei),
                      [(set I257:$x, Slice:$sliceo,
                        (int_tvm_ldvaruint16 Slice:$slicei))],
                      "LDVARUINT16", 0xfa00>;

defm LDVARINT16 : I0<(outs I257:$x, Slice:$sliceo), (ins Slice:$slicei),
                     [(set I257:$x, Slice:$sliceo,
                       (int_tvm_ldvarint16 Slice:$slicei))],
                     "LDVARINT16", 0xfa01>;

defm STVARUINT16 : I0<(outs Builder:$new_b), (ins Builder:$b, I257:$x),
                      [(set Builder:$new_b,
                        (int_tvm_stvaruint16 Builder:$b, I257:$x))],
                      "STVARUINT16", 0xfa02>;

defm STVARINT16 : I0<(outs Builder:$new_b), (ins Builder:$b, I257:$x),
                     [(set Builder:$new_b,
                       (int_tvm_stvarint16 Builder:$b, I257:$x))],
                     "STVARINT16", 0xfa03>;


defm LDVARUINT32 : I0<(outs I257:$x, Slice:$sliceo), (ins Slice:$slicei),
                      [(set I257:$x, Slice:$sliceo,
                        (int_tvm_ldvaruint32 Slice:$slicei))],
                      "LDVARUINT32", 0xfa04>;

defm LDVARINT32 : I0<(outs I257:$x, Slice:$sliceo), (ins Slice:$slicei),
                     [(set I257:$x, Slice:$sliceo,
                       (int_tvm_ldvarint32 Slice:$slicei))],
                     "LDVARINT32", 0xfa05>;

defm STVARUINT32 : I0<(outs Builder:$new_b), (ins Builder:$b, I257:$x),
                      [(set Builder:$new_b,
                        (int_tvm_stvaruint32 Builder:$b, I257:$x))],
                      "STVARUINT32", 0xfa06>;

defm STVARINT32 : I0<(outs Builder:$new_b), (ins Builder:$b, I257:$x),
                     [(set Builder:$new_b,
                       (int_tvm_stvarint32 Builder:$b, I257:$x))],
                     "STVARINT32", 0xfa07>;

defm PLDI : I<(outs I257:$x),
              (ins Slice:$slicei, uimm1_256:$precision),
              (outs), (ins uimm1_256:$precision),
              [(set I257:$x,
                (int_tvm_pldi Slice:$slicei, uimm1_256:$precision))],
              "PLDI\t $precision, $slicei, $x",
              "PLDI\t $precision", 0xd70a00>;

defm PLDU : I<(outs I257:$x),
              (ins Slice:$slicei, uimm1_256:$precision),
              (outs), (ins uimm1_256:$precision),
              [(set I257:$x,
                (int_tvm_pldu Slice:$slicei, uimm1_256:$precision))],
              "PLDU\t $precision, $slicei, $x",
              "PLDU\t $precision", 0xd70b00>;

defm PLDIX : I0<(outs I257:$x), (ins Slice:$slicei, I257:$precision),
                [(set I257:$x, (int_tvm_pldu Slice:$slicei, I257:$precision))],
                "PLDIX", 0xd702>;

defm PLDUX : I0<(outs I257:$x), (ins Slice:$slicei, I257:$precision),
                [(set I257:$x, (int_tvm_pldu Slice:$slicei, I257:$precision))],
                "PLDUX", 0xd703>;

defm STIXR : I0<(outs Builder : $builder),
                (ins Builder : $builderi, I257 : $val, I257 : $precision), [],
                "STIXR", 0xcf02>;

defm STUXR : I0<(outs Builder : $builder),
                (ins Builder : $builderi, I257 : $val, I257 : $precision), [],
                "STUXR", 0xcf03>;
}

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
defm LDSLICE : I<(outs Slice:$result, Slice:$remainder),
                 (ins Slice:$slice, uimm1_256:$size),
                 (outs), (ins uimm1_256:$size),
                 [(set Slice:$result, Slice:$remainder,
                   (int_tvm_ldslice Slice:$slice, uimm1_256:$size))],
                 "LDSLICE\t$result, $remainder, $slice, $size",
                 "LDSLICE\t$size", 0xd600>;

defm SBITS : I<(outs I257:$result), (ins Slice:$slice), (outs), (ins),
               [(set I257:$result, (int_tvm_sbits Slice:$slice))],
               "SBITS\t$result, $slice", "SBITS", 0xd749>;

defm SREFS : I<(outs I257:$result), (ins Slice:$slice), (outs), (ins),
               [(set I257:$result, (int_tvm_srefs Slice:$slice))],
               "SREFS\t$result, $slice", "SREFS", 0xd74a>;

defm SBITREFS : I<(outs I257:$bits, I257:$refs), (ins Slice:$slice),
                  (outs), (ins),
                  [(set I257:$bits, I257:$refs,
                    (int_tvm_sbitrefs Slice:$slice))],
                  "SBITREFS\t$bits, $refs, $slice", "SBITREFS", 0xd74b>;

defm BBITS : I<(outs I257:$result), (ins Builder:$b), (outs), (ins),
               [(set I257:$result, (int_tvm_bbits Builder:$b))],
               "BBITS\t$result, $b", "BBITS", 0xcf31>;

defm BREFS : I<(outs I257:$result), (ins Builder:$b), (outs), (ins),
               [(set I257:$result, (int_tvm_brefs Builder:$b))],
               "BREFS\t$result, $b", "BREFS", 0xcf32>;

defm BREMBITS : I<(outs I257:$result), (ins Builder:$b), (outs), (ins),
                  [(set I257:$result, (int_tvm_brembits Builder:$b))],
                  "BREMBITS\t$result, $b", "BREMBITS", 0xcf35>;

defm BREMREFS : I<(outs I257:$result), (ins Builder:$b), (outs), (ins),
                  [(set I257:$result, (int_tvm_bremrefs Builder:$b))],
                  "BREMREFS\t$result, $b", "BREMREFS", 0xcf36>;

let hasSideEffects = 1 in
defm LDSLICEX : I<(outs Slice:$result, Slice:$remainder),
                  (ins Slice:$slice, I257:$size),
                  (outs), (ins),
                  [(set Slice:$result, Slice:$remainder,
                    (TVMldslicex Slice:$slice, I257:$size))],
                  "LDSLICEX\t$result, $remainder, $slice, $size",
                  "LDSLICEX", 0xd718>;
def : Pat<(int_tvm_ldslice Slice:$slice, I257:$size),
          (LDSLICEX Slice:$slice, I257:$size)>;

/// TODO: If to remove the following line Pat doesn't work. Investigate why.
let hasSideEffects = 1 in
defm STSLICE : I<(outs Builder:$out_bldr), (ins Slice:$slicei, Builder:$bldr),
                 (outs), (ins),
                 [(set Builder:$out_bldr, (TVMstslice Slice:$slicei, Builder:$bldr))],
                 "STSLICE\t$out_bldr, $slicei, $bldr", "STSLICE", 0xce>;

def : Pat<(int_tvm_stslice Slice:$slicei, Builder:$bldr),
          (STSLICE Slice:$slicei, Builder:$bldr)>;

let mayStore = 1, hasSideEffects = 1 in
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
