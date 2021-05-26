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



// *******************************************        
//
//       A.7        Cell primitives        
//
// *******************************************/ 

// CD — STBREFR or ENDCST (b b00 – b), equivalent to ENDC; SWAP; STREF
let hasSideEffects = 1 in
defm ENDCST : I<(outs Builder:$obldr), (ins Builder:$ibldr0, Builder:$ibldr),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_stb Builder:$ibldr0, Builder:$ibldr))],
               "ENDCST\t$obldr, $ibldr0, $ibldr", "ENDCST", 0xcd>;

// CF11 — STBREF (b0 b – b00), equivalent to SWAP; STBREFREV
defm STBREF : I<(outs Builder:$obldr), (ins Builder:$ibldr0, Builder:$ibldr),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_stbref Builder:$ibldr0, Builder:$ibldr))],
               "STBREFt$obldr, $ibldr0, $ibldr", "STBREF", 0xcf11>;


// CF13 — STB (b0 b – b00), appends all data from Builder b0 to Builder b
defm STB : I<(outs Builder:$obldr), (ins Builder:$ibldr0, Builder:$ibldr),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_stb Builder:$ibldr0, Builder:$ibldr))],
               "STB\t$obldr, $ibldr0, $ibldr", "STB", 0xcf13>;

// CF14 — STREFR (b c – b0)
defm STREFR : I<(outs Builder:$obldr), (ins Builder:$ibldr, Cell:$icell),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_strefr Builder:$ibldr, Cell:$icell))],

               "STREFR\t$obldr, $ibldr, $icell", "STREFR", 0xcf14>;


// CF16 — STSLICER (b s – b0)
let hasSideEffects = 1 in
defm STSLICER : I<(outs Builder:$obldr), (ins Builder:$ibldr, Slice:$slice),
                 (outs), (ins),
                 [(set Builder:$obldr, (int_tvm_stslicer Builder:$ibldr, Slice:$slice))],
                 "STSLICER\t$obldr, $ibldr, $slice", "STSLICER", 0xcf16>;

// CF17 — STBR (b b0 – b00), concatenates two Builder s, equivalent to SWAP; STB
let hasSideEffects = 1 in
defm STBR : I<(outs Builder:$obldr), (ins Builder:$ibldr0, Builder:$ibldr),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_stbr Builder:$ibldr0, Builder:$ibldr))],
               "STBR\t$obldr, $ibldr0, $ibldr", "STBR", 0xcf17>;

// CF18 — STREFQ	(c b – c b -1 or b0 0)
defm STREFQ : I<(outs Cell:$ocell, Builder:$obldr, I257:$result), (ins  Cell:$icell, Builder:$ibldr),
               (outs), (ins),
               [(set Cell:$ocell, Builder:$obldr, I257:$result, (int_tvm_strefq Cell:$icell, Builder:$ibldr))],
               "STREFQ\t$ocell, $obldr, $result, $icell, $ibldr", "STREFQ", 0xcf18>;

// CF19 — STBREFQ (b0 b – b0 b -1 or b00 0)
defm STBREFQ : I<(outs Builder:$obldr0, Builder:$obldr, I257:$result), (ins Builder:$ibldr0, Builder:$ibldr),
               (outs), (ins),
               [(set Builder:$obldr0, Builder:$obldr, I257:$result, (int_tvm_stbrefq Builder:$ibldr0, Builder:$ibldr))],
               "STBREFQ\t$obldr0, $obldr, $result, $ibldr0, $ibldr", "STBREFQ", 0xcf19>;

// CF1A - STSLICEQ (s b - s b -1 or b0 0)
let hasSideEffects = 1 in
defm STSLICEQ : I<(outs Slice:$oslice, Builder:$obldr, I257:$result), (ins Slice:$islice, Builder:$ibldr),
                 (outs), (ins),
                 [(set Slice:$oslice, Builder:$obldr, I257:$result, (int_tvm_stsliceq Slice:$islice, Builder:$ibldr))],
                 "STSLICEQ\t$oslice, $obldr, $result, $islice, $ibldr", "STSLICEQ", 0xcf1a>;

// CF1B — STBQ (b0 b – b0 b -1 or b00 0)
let hasSideEffects = 1 in
defm STBQ : I<(outs  Builder:$obldr0, Builder:$obldr, I257:$result), (ins Builder:$ibldr0, Builder:$ibldr),
               (outs), (ins),
               [(set Builder:$obldr0,  Builder:$obldr, I257:$result, (int_tvm_stbq Builder:$ibldr0, Builder:$ibldr))],
               "STBQ\t$obldr0, $obldr, $result, $ibldr0, $ibldr", "STBQ", 0xcf1b>;

// CF1D — STBREFRQ (b b0 – b b0 -1 or b00 0)
defm STBREFRQ : I<(outs Builder:$obldr, Builder:$obldr0, I257:$result), (ins Builder:$ibldr, Builder:$ibldr0),
               (outs), (ins),
               [(set Builder:$obldr, Builder:$obldr0, I257:$result, (int_tvm_stbrefrq Builder:$ibldr, Builder:$ibldr0))],
               "STBREFRQ\t$obldr, $obldr0, $result, $ibldr, $ibldr0", "STBREFRQ", 0xcf1d>;

// CF1E — STSLICERQ (b s – b s -1 or b00 0)
let hasSideEffects = 1 in
defm STSLICERQ : I<(outs Builder:$obldr, Slice:$oslice, I257:$result), (ins Builder:$ibldr, Slice:$islice),
                 (outs), (ins),
                 [(set Builder:$obldr, Slice:$oslice, I257:$result, (int_tvm_stslicerq Builder:$ibldr, Slice:$islice))],
                 "STSLICEQ\t$obldr, $oslice, $result, $islice, $ibldr", "STSLICEQ", 0xcf1e>;

// CF1F — STBRQ (b b0 – b b0 -1 or b00 0)
let hasSideEffects = 1 in
defm STBRQ : I<(outs Builder:$obldr, Builder:$obldr0, I257:$result), (ins Builder:$ibldr, Builder:$ibldr0),
               (outs), (ins),
               [(set Builder:$obldr, Builder:$obldr0, I257:$result, (int_tvm_stbrq Builder:$ibldr0, Builder:$ibldr))],
               "STBR\t$obldr, $result, $ibldr0, $ibldr", "STBR", 0xcf1f>;

// CF20 — STREFCONST, equivalent to PUSHREF; STREFR
// TODO

// CF21 — STREF2CONST, equivalent to STREFCONST; STREFCONST
// TODO

// CF23  — ENDXC  (b x – c), if x6 = 0, creates a special or exotic cell from Builder b
let hasSideEffects = 1 in
defm ENDXC : I<(outs Cell:$cell), (ins Builder:$bldr, I257:$value), (outs), (ins),
              [(set Cell:$cell, (int_tvm_endxc Builder:$bldr, I257:$value))],
              "ENDXC\t$cell, $bldr, $value", "ENDXC", 0xcf23>;


// CF28 — STILE4 (x b – b0), stores a little-endian signed 32-bit integer
defm STILE4 : I<(outs Builder:$obldr), (ins I257:$value, Builder:$ibldr),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_stile4 I257:$value, Builder:$ibldr))],
               "STILE4\t$obldr, $value, $ibldr", "STILE4", 0xcf28>;

// CF29 — STULE4 (x b – b0), stores a little-endian unsigned 32-bit integer
defm STULE4 : I<(outs Builder:$obldr), (ins I257:$value, Builder:$ibldr),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_stule4 I257:$value, Builder:$ibldr))],
               "STULE4\t$obldr, $value, $ibldr", "STULE4", 0xcf29>;

// CF2A — STILE8 (x b – b0), stores a little-endian signed 64-bit integer
defm STILE8 : I<(outs Builder:$obldr), (ins I257:$value, Builder:$ibldr),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_stile8 I257:$value, Builder:$ibldr))],
               "STILE8\t$obldr, $value, $ibldr", "STILE8", 0xcf2a>;

// CF2B — STULE8 (x b – b0), stores a little-endian unsigned 64-bit integer
defm STULE8 : I<(outs Builder:$obldr), (ins I257:$value, Builder:$ibldr),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_stule8 I257:$value, Builder:$ibldr))],
               "STULE8\t$obldr, $value, $ibldr", "STULE8", 0xcf2b>;

// CF30 — BDEPTH  (b	– x), returns the depth of Builder b. If no cell references are stored in b, then x =  0; 
//   otherwise x is one plus the maximum of depths of cells referred to from b
defm BDEPTH : I<(outs I257:$result), (ins Builder:$b), (outs), (ins),
               [(set I257:$result, (int_tvm_bdepth Builder:$b))],
               "BDEPTH\t$result, $b", "BDEPTH", 0xcf30>;

// CF33 — BBITREFS (b – x y), returns the numbers of both data bits and cell references in b
defm BBITREFS : I<(outs I257:$x, I257:$y),
             (ins Builder : $bldr),
             (outs), (ins),
             [(set I257:$x, I257:$y,
               (int_tvm_bbitrefs Builder:$bldr))],
             "BBITREFS\t$x, $y, $bldr", "BBITREFS", 0xcf33>;

// CF37 — BREMBITREFS  (b – x0y0)
defm BREMBITREFS : I<(outs I257:$x, I257:$y),
             (ins Builder : $bldr),
             (outs), (ins),
             [(set I257:$x, I257:$y,
               (int_tvm_brembitrefs Builder:$bldr))],
             "BREMBITREFS\t$x, $y, $bldr", "BREMBITREFS", 0xcf37>;

// CF38cc — BCHKBITS cc + 1 (b –), checks whether cc + 1 bits can be stored into b, where 0 <= cc <= 255
// TODO

// CF39 — BCHKBITS (b x – ), checks whether x bits can be stored into b, 0 <= x <= 1023. If there is no space for x more bits in b ... throws an exception
let hasSideEffects = 1 in
defm BCHKBITS : I<(outs),
             (ins Builder:$bldr, I257:$x),
             (outs), (ins),
             [(int_tvm_bchkbits Builder:$bldr, I257:$x)],
             "BCHKBITS\t$bldr, $x", "BCHKBITS", 0xcf39>;

// CF3A — BCHKREFS (b y – ), checks whether y references can be stored into b, 0 <= y <= 7
let hasSideEffects = 1 in
defm BCHKREFS : I<(outs),
             (ins Builder:$bldr, I257:$y),
             (outs), (ins),
             [(int_tvm_bchkrefs Builder:$bldr, I257:$y)],
             "BCHKREFS\t$bldr, $y", "BCHKREFS", 0xcf3a>;

// CF3B — BCHKBITREFS (b x y – ), checks whether x bits and y references can be stored into b, 0 <= x <= 1023, 0 <= y <= 7
let hasSideEffects = 1 in
defm BCHKBITREFS : I<(outs),
             (ins Builder:$bldr, I257:$x, I257:$y),
             (outs), (ins),
             [(int_tvm_bchkbitrefs Builder:$bldr, I257:$x, I257:$y)],
             "BCHKBITREFS\t$bldr, $x, $y", "BCHKBITREFS", 0xcf3b>;

// CF3Ccc — BCHKBITSQ cc + 1 (b – ?), checks whether cc + 1 bits can be stored into b, where 0 <= cc <= 255
// TODO

// CF3D — BCHKBITSQ (b x – ?), checks whether x bits can be stored into b, 0 <= x <= 1023
defm BCHKBITSQ : I<(outs I257:$result),
             (ins Builder:$bldr, I257:$x),
             (outs), (ins),
             [(set I257:$result, (int_tvm_bchkbitsq Builder:$bldr, I257:$x))],
             "BCHKBITS\t$result, $bldr, $x", "BCHKBITS", 0xcf3d>;

// CF3E — BCHKREFSQ (b y – ?), checks whether y references can be stored into b, 0 <= y <= 7
defm BCHKREFSQ : I<(outs I257:$result),
             (ins Builder:$bldr, I257:$y),
             (outs), (ins),
             [(set I257:$result, (int_tvm_bchkrefsq Builder:$bldr, I257:$y))],
             "BCHKREFSQ\t$result, $bldr, $x", "BCHKREFSQ", 0xcf3e>;

// CF3F — BCHKBITREFSQ (b x y – ?), checks whether x bits and y references can be stored into b, 0 <= x <= 1023, 0 <= y <= 7
defm BCHKBITREFSQ : I<(outs I257:$result),
             (ins Builder:$bldr, I257:$x, I257:$y),
             (outs), (ins),
             [(set I257:$result, (int_tvm_bchkbitrefsq Builder:$bldr, I257:$x, I257:$y))],
             "BCHKBITREFS\t$result, $bldr, $x, $y", "BCHKBITREFS", 0xcf3d>;

// CF40 — STZEROES (b n – b0), stores n binary zeroes into Builder b
defm STZEROES : I<(outs Builder:$obldr), (ins Builder:$ibldr, I257:$value),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_stzeroes Builder:$ibldr, I257:$value))],
               "STZEROES\t$obldr, $ibldr, $value", "STZEROES", 0xcf40>;

// CF41 — STONES (b n – b0), stores n binary ones into Builder b

defm STONES : I<(outs Builder:$obldr), (ins Builder:$ibldr, I257:$value),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_stones Builder:$ibldr, I257:$value))],
               "STONES\t$obldr, $ibldr, $value", "STONES", 0xcf41>;

// CF42 — STSAME (b n x – b0), stores n binary xes (0 <= x <= 1) into Builder b
defm STSAME : I<(outs Builder:$obldr), (ins Builder:$ibldr, I257:$n, I257:$x),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_stsame Builder:$ibldr, I257:$n, I257:$x))],
               "STSAME\t$obldr, $ibldr, $n, $x", "STSAME", 0xcf42>;

// CFC0_xysss — STSLICECONST sss (b –	b0), stores a constant subslice sss consisting of 0 <= x <= 3 
//  references and up to 8y + 1 data bits, with 0 <= y <= 7.
// TODO

// CF81 — STSLICECONST ‘0’ or STZERO (b – b0), stores one binary zero
defm STZERO  : I<(outs Builder:$obldr), (ins Builder:$ibldr),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_stzero Builder:$ibldr))],
               "STZERO\t$obldr, $ibldr", "STZERO", 0xcf81>;

// CF83 — STSLICECONST ‘1’ or STONE (b – b0), stores one binary one
defm STONE  : I<(outs Builder:$obldr), (ins Builder:$ibldr),
               (outs), (ins),
               [(set Builder:$obldr, (int_tvm_stzero Builder:$ibldr))],
               "STONE\t$obldr, $ibldr", "STONE", 0xcf83>;
