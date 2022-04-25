//===----- TVMComparisonInstrInfo.td - A11 instructions ----*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes A.6 comparison instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instruction list..
defm SEMPTY  : I0<(outs I257:$result), (ins Slice:$slice),
                  [(set I257:$result, (int_tvm_sempty Slice:$slice))],
                  "SEMPTY", 0xc700>;
defm SDEMPTY : I0<(outs I257:$result), (ins Slice:$slice),
                  [(set I257:$result, (int_tvm_sdempty Slice:$slice))],
                  "SDEMPTY", 0xc701>;
defm SREMPTY : I0<(outs I257:$result), (ins Slice:$slice),
                  [(set I257:$result, (int_tvm_srempty Slice:$slice))],
                  "SREMPTY", 0xc702>;
defm SDFIRST : I<(outs I257:$firstbit), (ins Slice:$sl), (outs), (ins),
                 [(set I257:$firstbit, (int_tvm_sdfirst Slice:$sl))],
                 "SDFIRST\t$firstbit, $sl", "SDFIRST", 0xc703>;
defm SDLEXCMP : I<(outs I257:$cmp), (ins Slice:$sl1, Slice:$sl2),
                  (outs), (ins),
                  [(set I257:$cmp, (int_tvm_sdlexcmp Slice:$sl1, Slice:$sl2))],
                  "SDLEXCMP\t$cmp, $sl1, $sl2", "SDLEXCMP", 0xc704>;
defm SDEQ : I<(outs I257:$eq), (ins Slice:$sl1, Slice:$sl2), (outs), (ins),
              [(set I257:$eq, (int_tvm_sdeq Slice:$sl1, Slice:$sl2))],
              "SDEQ\t$eq, $sl1, $sl2", "SDEQ", 0xc705>;

defm SDPFX : I<(outs I257:$rv), (ins Slice:$sl1, Slice:$sl2), (outs), (ins),
               [(set I257:$rv, (int_tvm_sdpfx Slice:$sl1, Slice:$sl2))],
               "SDPFX\t$rv, $sl1, $sl2", "SDPFX", 0xc708>;

defm SDPFXREV : I<(outs I257:$rv), (ins Slice:$sl1, Slice:$sl2), (outs), (ins),
                  [(set I257:$rv, (int_tvm_sdpfxrev Slice:$sl1, Slice:$sl2))],
                  "SDPFXREV\t$rv, $sl1, $sl2", "SDPFXREV", 0xc709>;

defm SDPPFX : I<(outs I257:$rv), (ins Slice:$sl1, Slice:$sl2), (outs), (ins),
                [(set I257:$rv, (int_tvm_sdppfx Slice:$sl1, Slice:$sl2))],
                "SDPPFX\t$rv, $sl1, $sl2", "SDPPFX", 0xc70a>;

defm SDPPFXREV : I<(outs I257:$rv), (ins Slice:$sl1, Slice:$sl2), (outs), (ins),
                   [(set I257:$rv, (int_tvm_sdppfxrev Slice:$sl1, Slice:$sl2))],
                   "SDPPFXREV\t$rv, $sl1, $sl2", "SDPPFXREV", 0xc70b>;

defm SDSFX : I<(outs I257:$rv), (ins Slice:$sl1, Slice:$sl2), (outs), (ins),
               [(set I257:$rv, (int_tvm_sdsfx Slice:$sl1, Slice:$sl2))],
               "SDSFX\t$rv, $sl1, $sl2", "SDSFX", 0xc70c>;

defm SDSFXREV : I<(outs I257:$rv), (ins Slice:$sl1, Slice:$sl2), (outs), (ins),
                  [(set I257:$rv, (int_tvm_sdsfxrev Slice:$sl1, Slice:$sl2))],
                  "SDSFXREV\t$rv, $sl1, $sl2", "SDSFXREV", 0xc70d>;

defm SDPSFX : I<(outs I257:$rv), (ins Slice:$sl1, Slice:$sl2), (outs), (ins),
                [(set I257:$rv, (int_tvm_sdpsfx Slice:$sl1, Slice:$sl2))],
                "SDPSFX\t$rv, $sl1, $sl2", "SDPSFX", 0xc70e>;

defm SDPSFXREV : I<(outs I257:$rv), (ins Slice:$sl1, Slice:$sl2), (outs), (ins),
                   [(set I257:$rv, (int_tvm_sdpsfxrev Slice:$sl1, Slice:$sl2))],
                   "SDPSFXREV\t$rv, $sl1, $sl2", "SDPSFXREV", 0xc70f>;

defm SDCNTLEAD0 : I<(outs I257:$cntlead0), (ins Slice:$sl), (outs), (ins),
                    [(set I257:$cntlead0, (int_tvm_sdcntlead0 Slice:$sl))],
                    "SDCNTLEAD0\t$cntlead0, $sl", "SDCNTLEAD0", 0xc710>;

defm SDCNTLEAD1 : I<(outs I257:$cntlead1), (ins Slice:$sl), (outs), (ins),
                    [(set I257:$cntlead1, (int_tvm_sdcntlead1 Slice:$sl))],
                    "SDCNTLEAD1\t$cntlead1, $sl", "SDCNTLEAD1", 0xc711>;

defm SDCNTTRAIL0 : I<(outs I257:$cnttrail0), (ins Slice:$sl), (outs), (ins),
                     [(set I257:$cnttrail0, (int_tvm_sdcnttrail0 Slice:$sl))],
                     "SDCNTTRAIL0\t$cnttrail0, $sl", "SDCNTTRAIL0", 0xc712>;

defm SDCNTTRAIL1 : I<(outs I257:$cnttrail1), (ins Slice:$sl), (outs), (ins),
                     [(set I257:$cnttrail1, (int_tvm_sdcnttrail1 Slice:$sl))],
                     "SDCNTTRAIL1\t$cnttrail1, $sl", "SDCNTTRAIL1", 0xc713>;

// A.3.1. Null primitives
defm ISNULL : I0<(outs I257:$result), (ins I257:$val),
                 [(set I257:$result, (int_tvm_isnull I257:$val))],
                 "ISNULL", 0x6E>;
def : Pat<(int_tvm_isnull_slice Slice:$sl),
          (ISNULL (FROM_SLICE_COPY Slice:$sl))>;
def : Pat<(int_tvm_isnull_builder Builder:$b),
          (ISNULL (FROM_BUILDER_COPY Builder:$b))>;
def : Pat<(int_tvm_isnull_cell Cell:$cell),
          (ISNULL (FROM_CELL_COPY Cell:$cell))>;
def : Pat<(int_tvm_isnull_tuple Tuple:$tp),
          (ISNULL (FROM_TUPLE_COPY Tuple:$tp))>;

let isMoveImm = 1, isAsCheapAsAMove = 1, isReMaterializable = 1 in
defm PUSHNULL : I<(outs I257:$res), (ins), (outs), (ins),
                  [(set I257:$res, (int_tvm_pushnull))],
                  "PUSHNULL\t$res", "PUSHNULL", 0x6d>;
