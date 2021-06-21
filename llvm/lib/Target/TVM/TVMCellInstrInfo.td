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

//defm STIR : I<(outs Builder : $builder),
//              (ins Builder : $builderi, I257 : $val, uimm1_256 : $precision),
//              (outs), (ins uimm1_256 : $precision), [],
//              "STIR\t$precision", "STIR\t$precision", 0xcf0a>;

//defm STUR : I<(outs Builder : $obldr),
//              (ins Builder : $bldr, I257 : $val, uimm1_256 : $precision),
//              (outs), (ins uimm1_256 : $precision), [],
//              "STUR\t$precision", "STUR\t$precision", 0xcf0b>;
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
               [(set Builder:$obldr, (int_tvm_endcst Builder:$ibldr0, Builder:$ibldr))],
               "ENDCST\t$obldr, $ibldr0, $ibldr", "ENDCST", 0xcd>;

// CF0Acc - STIR cc + 1 (b x – b0), equivalent to SWAP; STI cc + 1
let hasSideEffects = 1 in 
defm STIR : I<(outs Builder : $obldr),
             (ins Builder : $ibldr, I257 : $val, uimm1_256 : $precision),
             (outs), (ins uimm1_256 : $precision),
             [(set Builder : $obldr, (int_tvm_stir Builder : $ibldr, I257 : $val, 
               uimm1_256 : $precision))],
             "STIR\t$precision, $obldr, $ibldr, $val", "STIR\t$precision", 0xcf0a>;

// CF0Bcc - STUR cc + 1 (b x – b0), equivalent to SWAP; STU cc + 1
let hasSideEffects = 1 in
defm STUR : I<(outs Builder : $obldr),
             (ins Builder : $ibldr, I257 : $val, uimm1_256 : $precision),
             (outs), (ins uimm1_256 : $precision),
             [(set Builder : $obldr, (int_tvm_stur Builder : $ibldr, I257 : $val, 
               uimm1_256 : $precision))],
             "STUR\t$precision, $obldr, $ibldr, $val", "STUR\t$precision", 0xcf0b>;

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
                 "STSLICERQ\t$obldr, $oslice, $result, $islice, $ibldr", "STSLICERQ", 0xcf1e>;

// CF1F — STBRQ (b b0 – b b0 -1 or b00 0)
let hasSideEffects = 1 in
defm STBRQ : I<(outs Builder:$obldr, Builder:$obldr0, I257:$result), (ins Builder:$ibldr, Builder:$ibldr0),
               (outs), (ins),
               [(set Builder:$obldr, Builder:$obldr0, I257:$result, (int_tvm_stbrq Builder:$ibldr0, Builder:$ibldr))],
               "STBRQ\t$obldr, $result, $ibldr0, $ibldr", "STBRQ", 0xcf1f>;

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
             "BCHKBITSQ\t$result, $bldr, $x", "BCHKBITSQ", 0xcf3d>;

// CF3E — BCHKREFSQ (b y – ?), checks whether y references can be stored into b, 0 <= y <= 7
defm BCHKREFSQ : I<(outs I257:$result),
             (ins Builder:$bldr, I257:$y),
             (outs), (ins),
             [(set I257:$result, (int_tvm_bchkrefsq Builder:$bldr, I257:$y))],
             "BCHKREFSQ\t$result, $bldr, $y", "BCHKREFSQ", 0xcf3e>;

// CF3F — BCHKBITREFSQ (b x y – ?), checks whether x bits and y references can be stored into b, 0 <= x <= 1023, 0 <= y <= 7
defm BCHKBITREFSQ : I<(outs I257:$result),
             (ins Builder:$bldr, I257:$x, I257:$y),
             (outs), (ins),
             [(set I257:$result, (int_tvm_bchkbitrefsq Builder:$bldr, I257:$x, I257:$y))],
             "BCHKBITREFSQ\t$result, $bldr, $x, $y", "BCHKBITREFSQ", 0xcf3f>;

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
               [(set Builder:$obldr, (int_tvm_stone Builder:$ibldr))],
               "STONE\t$obldr, $ibldr", "STONE", 0xcf83>;

// ******************************************* 
//
// A.7.2. Cell deserialization primitives
//
// *******************************************/

// D70Ccc — LDIQ cc + 1 (s – x s0 -1 or s 0), a quiet version of LDI
let hasSideEffects = 1 in
defm LDIQ : I<(outs I257 : $x, Slice : $osl, I257 : $result),
             (ins Slice : $isl, uimm1_256 : $precision),
             (outs), (ins uimm1_256 : $precision),
             [(set I257 : $x, Slice : $osl, I257 : $result,
               (int_tvm_ldiq Slice : $isl, uimm1_256 : $precision))],
             "LDIQ\t $precision, $x, $osl, $isl",
             "LDIQ\t $precision", 0xd70c>;

// D70Ecc — PLDIQ cc + 1 (s – x -1 or 0), a quiet version of PLDI
let hasSideEffects = 1 in
defm PLDIQ : I<(outs I257 : $x, I257 : $result),
             (ins Slice : $isl, uimm1_256 : $precision),
             (outs), (ins uimm1_256 : $precision),
             [(set I257 : $x, I257 : $result,
               (int_tvm_pldiq Slice : $isl, uimm1_256 : $precision))],
             "PLDIQ\t $precision, $x, $isl",
             "PLDIQ\t $precision", 0xd70e>;

// D70Fcc — PLDUQ cc + 1 (s – x -1 or 0), a quiet version of PLDU
let hasSideEffects = 1 in
defm PLDUQ : I<(outs I257 : $x, I257 : $result),
             (ins Slice : $isl, uimm1_256 : $precision),
             (outs), (ins uimm1_256 : $precision),
             [(set I257 : $x, I257 : $result,
               (int_tvm_plduq Slice : $isl, uimm1_256 : $precision))],
             "PLDUQ\t $precision, $x, $isl",
             "PLDUQ\t $precision", 0xd70f>;

// D714_c — PLDUZ 32(c + 1) (s - s x), preloads the first 32(c + 1) bits of Slice s into an unsigned integer x, for 0 <= c <= 7
let hasSideEffects = 1 in
defm PLDUZ : I<(outs Slice:$osl, I257 : $x),
             (ins Slice : $isl, uimm3 : $count),
             (outs), (ins uimm3 : $count),
             [(set Slice:$osl, I257 : $x, 
               (int_tvm_plduz Slice : $isl, uimm3 : $count))],
             "PLDUZ\t $count, $osl, $x, $isl",
             "PLDUZ\t $count", 0xd0714>;

// D71A — LDSLICEXQ (s l – s00 s0 -1 or s 0), a quiet version of LDSLICEX
// TODO

// D71B — PLDSLICEXQ (s l – s0 -1 or 0), a quiet version of LDSLICEXQ
// TODO

// D71Dcc — PLDSLICE cc + 1 (s - s00), returns the first 0 < cc + 1 <= 256 bits of s as s00
let hasSideEffects = 1 in
defm PLDSLICE : I<(outs Slice : $osl),
             (ins Slice : $isl, uimm1_256 : $precision),
             (outs), (ins uimm1_256 : $precision),
             [(set Slice : $osl, (int_tvm_pldslice Slice : $isl, uimm1_256 : $precision))],
             "PLDSLICE\t $precision, $osl, $isl",
             "PLDSLICE\t $precision", 0xd71f>;

// D71Ecc — LDSLICEQ cc + 1 (s – s00 s0 -1 or s 0), a quiet version of LDSLICE
let hasSideEffects = 1 in
defm LDSLICEQ : I<(outs Slice : $osl00, Slice : $osl0, I257 : $result),
             (ins Slice : $isl, uimm1_256 : $precision),
             (outs), (ins uimm1_256 : $precision),
             [(set Slice : $osl00, Slice : $osl0, I257 : $result,
               (int_tvm_ldsliceq Slice : $isl, uimm1_256 : $precision))],
             "LDSLICEQ\t $precision, $osl00, $osl0, $result, $isl",
             "LDSLICEQ\t $precision", 0xd71e>;

// D71Fcc — PLDSLICEQ cc + 1 (s – s00 -1 or 0), a quiet version of PLDSLICE
let hasSideEffects = 1 in
defm PLDSLICEQ : I<(outs Slice : $osl, I257 : $result),
             (ins Slice : $isl, uimm1_256 : $precision),
             (outs), (ins uimm1_256 : $precision),
             [(set Slice : $osl, I257 : $result,
               (int_tvm_pldsliceq Slice : $isl, uimm1_256 : $precision))],
             "PLDSLICEQ\t $precision, $osl, $result, $isl",
             "PLDSLICEQ\t $precision", 0xd71f>;

// D720 — SDCUTFIRST (s l – s0), returns the first 0 <= l <= 1023 bits of s. It is equivalent to PLDSLICEX
let hasSideEffects = 1 in
defm SDCUTFIRST : I<(outs Slice : $osl),
             (ins Slice : $isl, uimm10 : $l),
             (outs), (ins uimm10 : $l),
             [(set Slice : $osl, 
               (int_tvm_sdcutfirst Slice : $isl, uimm10 : $l))],
             "SDCUTFIRST\t $osl, $isl, $l",
             "SDCUTFIRST", 0xd720>;

// D721 — SDSKIPFIRST (s l – s0), returns all but the first 0 <= l <= 1023 bits of s. It is equivalent to LDSLICEX; NIP
let hasSideEffects = 1 in
defm SDSKIPFIRST : I<(outs Slice : $osl),
             (ins Slice : $isl, uimm10 : $l),
             (outs), (ins uimm10 : $l),
             [(set Slice : $osl, 
               (int_tvm_sdskipfirst Slice : $isl, uimm10 : $l))],
             "SDSKIPFIRST\t $osl, $isl, $l",
             "SDSKIPFIRST", 0xd721>;

// D722 — SDCUTLAST (s l – s0), returns the last 0 <= l <= 1023 bits of s
let hasSideEffects = 1 in
defm SDCUTLAST : I<(outs Slice : $osl),
             (ins Slice : $isl, uimm10 : $l),
             (outs), (ins uimm10 : $l),
             [(set Slice : $osl, 
               (int_tvm_sdcutlast Slice : $isl, uimm10 : $l))],
             "SDCUTLAST\t $osl, $isl, $l",
             "SDCUTLAST", 0xd722>;

// D723	— SDSKIPLAST (s l – s0), returns all but the last 0 <= l <=	1023 bits of s
let hasSideEffects = 1 in
defm SDSKIPLAST : I<(outs Slice : $osl),
             (ins Slice : $isl, uimm10 : $l),
             (outs), (ins uimm10 : $l),
             [(set Slice : $osl, 
               (int_tvm_sdskiplast Slice : $isl, uimm10 : $l))],
             "SDSKIPLAST\t $osl, $isl, $l",
             "SDSKIPLAST", 0xd723>;

// D724 — SDSUBSTR (s l l0 – s0), returns 0 <= l <= 1023 bits of s starting from offset 0 <= l <= 1023, 
//  thus extracting a bit substring out of the data of s
let hasSideEffects = 1 in
defm SDSUBSTR : I<(outs Slice : $osl),
             (ins Slice : $isl, uimm10 : $l, uimm10 : $l0),
             (outs), (ins uimm10 : $l, uimm10 : $l0),
             [(set Slice : $osl, 
               (int_tvm_sdsubstr Slice : $isl, uimm10 : $l, uimm10 : $l0))],
             "SDSUBSTR\t $osl, $isl, $l, $l0",
             "SDSUBSTR", 0xd724>;

// D726 — SDBEGINSX (s s0 – s00), checks whether s begins with (the data bits of) s0, and removes s0 from s	on success. 
//  On failure throws a cell deserialization exception
let hasSideEffects = 1 in
defm SDBEGINSX : I<(outs Slice : $osl),
             (ins Slice : $isl, Slice : $isl0),
             (outs), (ins),
             [(set Slice : $osl, 
               (int_tvm_sdbeginsx Slice : $isl, Slice : $isl0))],
             "SDBEGINSX\t $osl, $isl, $isl0",
             "SDBEGINSX", 0xd726>;

// D727 — SDBEGINSXQ (s s0 – s00 -1 or s 0), a quiet version of SDBEGINSX
defm SDBEGINSXQ : I<(outs Slice : $osl, I257 : $result),
             (ins Slice : $isl, Slice : $isl0),
             (outs), (ins),
             [(set Slice : $osl, I257 : $result,
               (int_tvm_sdbeginsxq Slice : $isl, Slice : $isl0))],
             "SDBEGINSXQ\t $osl, $result, $isl, $isl0",
             "SDBEGINSXQ", 0xd727>;

// D72A_xsss — SDBEGINS (s – s00), checks whether s begins with constant bitstring sss of length 8x + 3 
//   (with continuation bit assumed), where 0 <= x <= 127, and removes sss from s on success
let hasSideEffects = 1 in
defm SDBEGINS : I<(outs Slice : $osl),
             (ins Slice : $isl, uimm7 : $x),
             (outs), (ins uimm7 : $x),
             [(set Slice : $osl, 
               (int_tvm_sdbegins Slice : $isl, uimm7 : $x))],
             "SDBEGINS$x\t $osl, $isl",
             "SDBEGINS$x", 0xd72a>;

// D72802 — SDBEGINS ‘0’ (s – s00), checks whether s begins with a binary zero
let hasSideEffects = 1 in
defm SDBEGINS0 : I<(outs Slice : $osl),
             (ins Slice : $isl),
             (outs), (ins),
             [(set Slice : $osl, 
               (int_tvm_sdbegins0 Slice : $isl))],
             "SDBEGINS0\t $osl, $isl",
             "SDBEGINS0", 0xd72802>;

// D72806 — SDBEGINS ‘1’ (s – s00), checks whether s begins with a binary one
let hasSideEffects = 1 in
defm SDBEGINS1 : I<(outs Slice : $osl),
             (ins Slice : $isl),
             (outs), (ins),
             [(set Slice : $osl, 
               (int_tvm_sdbegins1 Slice : $isl))],
             "SDBEGINS1\t $osl, $isl",
             "SDBEGINS1", 0xd72806>;

// D72E_xsss — SDBEGINSQ (s – s00 -1 or s 0), a quiet version of SDBEGINS
defm SDBEGINSQ : I<(outs Slice : $osl, I257 : $result),
             (ins Slice : $isl, uimm7 : $x),
             (outs), (ins uimm7 : $x),
             [(set Slice : $osl, I257 : $result,
               (int_tvm_sdbeginsq Slice : $isl, uimm7 : $x))],
             "SDBEGINSQ$x\t $osl, $result, $isl",
             "SDBEGINSQ$x", 0xd72a>;

// D730 — SCUTFIRST (s l r – s0), returns the first 0 <= l <= 1023 bits and first 0 <= r <= 4 references of s
let hasSideEffects = 1 in
defm SCUTFIRST : I<(outs Slice : $osl),
             (ins Slice : $isl, uimm10 : $l, uimm2 : $r),
             (outs), (ins uimm10 : $l, uimm2 : $r),
             [(set Slice : $osl, 
               (int_tvm_scutfirst Slice : $isl, uimm10 : $l, uimm2 : $r))],
             "SCUTFIRST\t $osl, $isl, $l, $r",
             "SCUTFIRST", 0xd730>;

// D731 — SSKIPFIRST (s l r – s0)
let hasSideEffects = 1 in
defm SSKIPFIRST : I<(outs Slice : $osl),
             (ins Slice : $isl, uimm10 : $l, uimm2 : $r),
             (outs), (ins uimm10 : $l, uimm2 : $r),
             [(set Slice : $osl, 
               (int_tvm_sskipfirst Slice : $isl, uimm10 : $l, uimm2 : $r))],
             "SSKIPFIRST\t $osl, $isl, $l, $r",
             "SSKIPFIRST", 0xd731>;

// D732 — SCUTLAST (s l r – s0), returns the last 0 <= l <= 1023 data bits and last 0 <= r <= 4 references of s
let hasSideEffects = 1 in
defm SCUTLAST : I<(outs Slice : $osl),
             (ins Slice : $isl, uimm10 : $l, uimm2 : $r),
             (outs), (ins uimm10 : $l, uimm2 : $r),
             [(set Slice : $osl, 
               (int_tvm_scutlast Slice : $isl, uimm10 : $l, uimm2 : $r))],
             "SCUTLAST\t $osl, $isl, $l, $r",
             "SCUTLAST", 0xd732>;

// D733 — SSKIPLAST (s l r – s0)
let hasSideEffects = 1 in
defm SSKIPLAST : I<(outs Slice : $osl),
             (ins Slice : $isl, uimm10 : $l, uimm2 : $r),
             (outs), (ins uimm10 : $l, uimm2 : $r),
             [(set Slice : $osl, 
               (int_tvm_sskiplast Slice : $isl, uimm10 : $l, uimm2 : $r))],
             "SSKIPLAST\t $osl, $isl, $l, $r",
             "SSKIPLAST", 0xd733>;

// D734	— SUBSLICE (s l r l0 r0 – s0), returns 0 <= l0 <= 1023 bits and 0 <= r0 <= 4 references from Slice s
let hasSideEffects = 1 in
defm SUBSLICE : I<(outs Slice : $osl),
             (ins Slice : $isl, uimm10 : $l, uimm2 : $r, uimm10 : $l0, uimm2 : $r0),
             (outs), (ins uimm10 : $l, uimm2 : $r, uimm10 : $l0, uimm2 : $r0),
             [(set Slice : $osl, 
               (int_tvm_subslice Slice : $isl, uimm10 : $l, uimm2 : $r, uimm10 : $l0, uimm2 : $r0))],
             "SUBSLICE\t $osl, $isl, $l, $r, $l0, $r0",
             "SUBSLICE", 0xd734>;

// D736 — SPLIT (s l r – s0 s00), splits the first 0 <= l <= 1023 data bits and first 0 <= r <= 4 
//  references from s into s0, returning the remainder of s as s00
let hasSideEffects = 1 in
defm SPLIT : I<(outs Slice : $osl0, Slice : $osl00),
             (ins Slice : $isl, uimm10 : $l, uimm2 : $r),
             (outs), (ins uimm10 : $l, uimm2 : $r),
             [(set Slice : $osl0, Slice : $osl00, 
               (int_tvm_split Slice : $isl, uimm10 : $l, uimm2 : $r))],
             "SPLIT\t $osl0, $osl00, $isl, $l, $r",
             "SPLIT", 0xd736>;

// D737 — SPLITQ (s l r – s0 s00 -1 or s 0), a quiet version of SPLIT
defm SPLITQ : I<(outs Slice : $osl0, Slice : $osl00, I257 : $result),
             (ins Slice : $isl, uimm10 : $l, uimm2 : $r),
             (outs), (ins uimm10 : $l, uimm2 : $r),
             [(set Slice : $osl0, Slice : $osl00, I257 : $result, 
               (int_tvm_splitq Slice : $isl, uimm10 : $l, uimm2 : $r))],
             "SPLITQ\t $osl0, $osl00, $result, $isl, $l, $r",
             "SPLITQ", 0xd737>;

// D739 — XCTOS (c – s ?), transforms an ordinary or exotic cell into a Slice, as if it were an ordinary cell. 
//  A flag is returned indicating whether c is exotic
let hasSideEffects = 1 in
defm XCTOS : I<(outs Slice : $osl, I257 : $result),
             (ins Cell : $icell),
             (outs), (ins),
             [(set Slice : $osl, I257 : $result,
               (int_tvm_xctos Cell : $icell))],
             "XCTOS\t $osl, $result, $icell",
             "XCTOS", 0xd739>;

// D73A — XLOAD (c – c0), loads an exotic cell c and returns an ordinary cell c0. If c is already ordinary, does nothing. 
//  If c cannot be loaded, throws an exception
let hasSideEffects = 1 in
defm XLOAD : I<(outs Cell : $ocell),
             (ins Cell : $icell),
             (outs), (ins),
             [(set Cell : $ocell,
               (int_tvm_xload Cell : $icell))],
             "XLOAD\t $ocell, $icell",
             "XLOAD", 0xd73a>;

// D73B — XLOADQ (c – c0 -1 or c 0), loads an exotic cell c as XLOAD, but returns 0 on failure
let hasSideEffects = 1 in
defm XLOADQ : I<(outs Cell : $ocell, I257 : $result),
             (ins Cell : $icell),
             (outs), (ins),
             [(set Cell : $ocell, I257 : $result,
               (int_tvm_xloadq Cell : $icell))],
             "XLOADQ\t $ocell, $result, $icell",
             "XLOADQ", 0xd73b>;

// D741 — SCHKBITS (s l – ), checks whether there are at least l data bits in Slice s. If this is not the case, 
//  throws a cell deserialisation (i.e., cell underflow) exception
let hasSideEffects = 1 in
defm SCHKBITS : I<(outs),
             (ins Slice : $isl, I257 : $l),
             (outs), (ins),
             [(int_tvm_schkbits Slice : $isl, I257 : $l)],
             "SCHKBITS\t $isl, $l",
             "SCHKBITS", 0xd741>;

// D742 — SCHKREFS (s r – ), checks whether there are at least r references in Slice s
let hasSideEffects = 1 in
defm SCHKREFS : I<(outs),
             (ins Slice : $isl, I257 : $r),
             (outs), (ins),
             [(int_tvm_schkrefs Slice : $isl, I257 : $r)],
             "SCHKREFS\t $isl, $r",
             "SCHKREFS", 0xd742>;

// D743	— SCHKBITREFS (s l r – ), checks whether there are at least l data bits and r references in Slice s 
let hasSideEffects = 1 in
defm SCHKBITREFS : I<(outs),
             (ins Slice : $isl, I257 : $l, I257 : $r),
             (outs), (ins),
             [(int_tvm_schkbitrefs Slice : $isl, I257 : $l, I257 : $r)],
             "SCHKBITREFS\t $isl, $l",
             "SCHKBITREFS", 0xd743>;

// D745 — SCHKBITSQ (s l – ?), checks whether there are at least l data bits in Slice s
defm SCHKBITSQ : I<(outs I257 : $result),
             (ins Slice : $isl, I257 : $l),
             (outs), (ins),
             [(set I257 : $result, 
			   (int_tvm_schkbitsq Slice : $isl, I257 : $l))],
             "SCHKBITSQ\t $result, $isl, $l",
             "SCHKBITSQ", 0xd745>;

// D746 — SCHKREFSQ (s r – ?), checks whether there are at least r references in Slice s
defm SCHKREFSQ : I<(outs I257 : $result),
             (ins Slice : $isl, I257 : $r),
             (outs), (ins),
             [(set I257 : $result, 
			   (int_tvm_schkrefsq Slice : $isl, I257 : $r))],
             "SCHKREFSQ\t $result, $isl, $r",
             "SCHKREFSQ", 0xd746>;

// D747 — SCHKBITREFSQ (s l r – ?), checks whether there are at least l data bits and r references in Slice s
defm SCHKBITREFSQ : I<(outs I257 : $result),
             (ins Slice : $isl, I257 : $l, I257 : $r),
             (outs), (ins),
             [(set I257 : $result, 
			   (int_tvm_schkbitrefsq Slice : $isl, I257 : $l, I257 : $r))],
             "SCHKBITREFSQ\t $result, $isl, $l, $r",
             "SCHKBITREFSQ", 0xd747>;

// D748 — PLDREFVAR (s n – c), returns the n-th cell reference of Slice s for 0 <= n <= 3
let hasSideEffects = 1 in
defm PLDREFVAR : I<(outs Cell: $ocell),
             (ins Slice : $isl, uimm2 : $n),
             (outs), (ins uimm2 : $n),
             [(set Cell: $ocell, 
			   (int_tvm_pldrefvar Slice : $isl, uimm2 : $n))],
             "PLDREFVAR\t $ocell, $isl, $n",
             "PLDREFVAR", 0xd748>;

// D74E_n — PLDREFIDX n	(s – c), returns the n-th cell reference of Slice s, where 0 <= n <= 3
let hasSideEffects = 1 in
defm PLDREFIDX : I<(outs Cell: $ocell),
             (ins Slice : $isl, uimm2 : $n),
             (outs), (ins uimm2 : $n),
             [(set Cell: $ocell, 
			   (int_tvm_pldrefidx Slice : $isl, uimm2 : $n))],
             "PLDREFIDX$n\t $ocell, $isl, $n",
             "PLDREFIDX", 0xd74e>;

// D74C — PLDREF (s – c), preloads the first cell reference of a Slice
defm PLDREF : I<(outs Cell: $ocell),
             (ins Slice : $isl),
             (outs), (ins),
             [(set Cell: $ocell, 
			   (int_tvm_pldref Slice : $isl))],
             "PLDREF\t $ocell, $isl",
             "PLDREF", 0xd74c>;

// D750 — LDILE4 (s – x s0), loads a little-endian signed 32-bit integer
let hasSideEffects = 1 in
defm LDILE4 : I<(outs I257 : $x, Slice : $osl),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x, Slice : $osl, 
			   (int_tvm_ldile4 Slice : $isl))],
             "LDILE4\t $x, $osl, $isl",
             "LDILE4", 0xd750>;

// D751 — LDULE4 (s – x s0), loads a little-endian unsigned 32-bit integer
let hasSideEffects = 1 in
defm LDULE4 : I<(outs I257 : $x, Slice : $osl),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x, Slice : $osl, 
			   (int_tvm_ldule4 Slice : $isl))],
             "LDULE4\t $x, $osl, $isl",
             "LDULE4", 0xd751>;

// D752 — LDILE8 (s – x s0), loads a little-endian signed 64-bit integer
let hasSideEffects = 1 in
defm LDILE8 : I<(outs I257 : $x, Slice : $osl),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x, Slice : $osl, 
			   (int_tvm_ldile8 Slice : $isl))],
             "LDILE8\t $x, $osl, $isl",
             "LDILE8", 0xd752>;

// D753 — LDULE8 (s – x s0), loads a little-endian unsigned 64-bit integer
let hasSideEffects = 1 in
defm LDULE8 : I<(outs I257 : $x, Slice : $osl),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x, Slice : $osl, 
			   (int_tvm_ldule8 Slice : $isl))],
             "LDILE8\t $x, $osl, $isl",
             "LDILE8", 0xd753>;

// D754 — PLDILE4 (s – x), preloads a little-endian signed 32-bit integer
let hasSideEffects = 1 in
defm PLDILE4 : I<(outs I257 : $x),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x,  
			   (int_tvm_pldile4 Slice : $isl))],
             "PLDILE4\t $x, $isl",
             "PLDILE4", 0xd754>;

// D755 — PLDULE4 (s – x), preloads a little-endian unsigned 32-bit integer
let hasSideEffects = 1 in
defm PLDULE4 : I<(outs I257 : $x),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x,  
			   (int_tvm_pldule4 Slice : $isl))],
             "PLDULE4\t $x, $isl",
             "PLDULE4", 0xd755>;

// D756 — PLDILE8 (s – x), preloads a little-endian signed 64-bit integer
let hasSideEffects = 1 in
defm PLDILE8 : I<(outs I257 : $x),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x,  
			   (int_tvm_pldile8 Slice : $isl))],
             "PLDILE8\t $x, $isl",
             "PLDILE8", 0xd756>;

// D757 — PLDULE8 (s – x), preloads a little-endian unsigned 64-bit integer
let hasSideEffects = 1 in
defm PLDULE8 : I<(outs I257 : $x),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x,  
			   (int_tvm_pldule8 Slice : $isl))],
             "PLDULE8\t $x, $isl",
             "PLDULE8", 0xd756>;

// D758	— LDILE4Q (s – x s0 -1 or s 0), quietly loads a little-endian signed 32-bit integer
let hasSideEffects = 1 in
defm LDILE4Q : I<(outs I257 : $x, Slice : $osl, I257 : $result),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x, Slice : $osl, I257 : $result,
			   (int_tvm_ldile4q Slice : $isl))],
             "LDILE4Q\t $x, $osl, $result, $isl",
             "LDILE4Q", 0xd758>;

// D759	— LDULE4Q (s – x s0 -1 or s 0), quietly loads a little-endian unsigned 32-bit integer
let hasSideEffects = 1 in
defm LDULE4Q : I<(outs I257 : $x, Slice : $osl, I257 : $result),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x, Slice : $osl, I257 : $result,
			   (int_tvm_ldule4q Slice : $isl))],
             "LDULE4Q\t $x, $osl, $result, $isl",
             "LDULE4Q", 0xd759>;

// D75A	— LDILE8Q	(s – x s0 -1 or	s 0), quietly loads a little-endian signed 64-bit integer
let hasSideEffects = 1 in
defm LDILE8Q : I<(outs I257 : $x, Slice : $osl, I257 : $result),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x, Slice : $osl, I257 : $result,
			   (int_tvm_ldile8q Slice : $isl))],
             "LDILE8Q\t $x, $osl, $result, $isl",
             "LDILE8Q", 0xd75a>;

// D75B — LDULE8Q	(s – x s0 -1 or	s 0), quietly loads a little-endian unsigned 64-bit integer
let hasSideEffects = 1 in
defm LDULE8Q : I<(outs I257 : $x, Slice : $osl, I257 : $result),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x, Slice : $osl, I257 : $result,
			   (int_tvm_ldule8q Slice : $isl))],
             "LDULE8Q\t $x, $osl, $result, $isl",
             "LDULE8Q", 0xd75b>;

// D75C	— PLDILE4Q (s – x -1 or 0), quietly preloads a little-endian signed 32-bit integer
let hasSideEffects = 1 in
defm PLDILE4Q : I<(outs I257 : $x, I257 : $result),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x, I257 : $result,
			   (int_tvm_pldile4q Slice : $isl))],
             "PLDILE4Q\t $x, $result, $isl",
             "PLDILE4Q", 0xd75c>;

// D75D	— PLDULE4Q (s – x -1 or	0), quietly preloads a little-endian unsigned 32-bit integer
let hasSideEffects = 1 in
defm PLDULE4Q : I<(outs I257 : $x, I257 : $result),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x, I257 : $result,
			   (int_tvm_pldule4q Slice : $isl))],
             "PLDULE4Q\t $x, $result, $isl",
             "PLDULE4Q", 0xd75d>;

// D75E	— PLDILE8Q (s – x -1 or 0), quietly preloads a little-endian signed 64-bit integer
let hasSideEffects = 1 in
defm PLDILE8Q : I<(outs I257 : $x, I257 : $result),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x, I257 : $result,
			   (int_tvm_pldile8q Slice : $isl))],
             "PLDILE8Q\t $x, $result, $isl",
             "PLDILE8Q", 0xd75e>;

// D75F	— PLDULE8Q (s –	x -1 or 0), quietly preloads a little-endian unsigned 64-bit integer
let hasSideEffects = 1 in
defm PLDULE8Q : I<(outs I257 : $x, I257 : $result),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x, I257 : $result,
			   (int_tvm_pldule8q Slice : $isl))],
             "PLDULE8Q\t $x, $result, $isl",
             "PLDULE8Q", 0xd75e>;

// D760	— LDZEROES (s – n s0), returns the count n of leading zero bits in s, and removes these bits from s
let hasSideEffects = 1 in
defm LDZEROES : I<(outs I257 : $n, Slice : $osl),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $n, Slice : $osl,
			   (int_tvm_ldzeroes Slice : $isl))],
             "LDZEROES\t $n, $osl, $isl",
             "LDZEROES", 0xd760>;

// D761 — LDONES (s – n s0), returns the count n of leading one bits in s, and removes these bits from s
let hasSideEffects = 1 in
defm LDONES : I<(outs I257 : $n, Slice : $osl),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $n, Slice : $osl,
			   (int_tvm_ldones Slice : $isl))],
             "LDONES\t $n, $osl, $isl",
             "LDONES", 0xd761>;

// D762 — LDSAME (s x – n s0), returns the count n of leading bits equal to 0 <= x <= 1 in s, and removes these bits from s
let hasSideEffects = 1 in
defm LDSAME : I<(outs I257 : $n, Slice : $osl),
             (ins Slice : $isl, I257 : $x),
             (outs), (ins),
             [(set I257 : $n, Slice : $osl,
			   (int_tvm_ldsame Slice : $isl, I257 : $x))],
             "LDSAME\t $n, $osl, $isl, $x",
             "LDSAME", 0xd762>;

// D764	— SDEPTH (s – x), returns the depth of Slice s. If s has no references, then x = 0; 
//  otherwise x is one plus the maximum of depths of cells referred to from s
defm SDEPTH : I<(outs I257 : $x),
             (ins Slice : $isl),
             (outs), (ins),
             [(set I257 : $x, 
			   (int_tvm_sdepth Slice : $isl))],
             "SDEPTH\t $x, $isl",
             "SDEPTH", 0xd764>;

// D765	— CDEPTH (c – x), returns the depth of Cell c. If c has no references, then x = 0; 
//  otherwise x is one plus the maximum of depths of cells referred to from c
defm CDEPTH : I<(outs I257 : $x),
             (ins Cell : $icell),
             (outs), (ins),
             [(set I257 : $x, 
			   (int_tvm_cdepth Cell : $icell))],
             "CDEPTH\t $x, $icell",
             "CDEPTH", 0xd765>;
