//===---- TVMApplicationInstrInfo.td - A11 instructions ----*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes A11 application specific instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instruction list..
defm ACCEPT       : I0<(outs), (ins), [(int_tvm_accept)],
                       "ACCEPT", 0xf800>;

defm SETGASLIMIT  : I0<(outs), (ins I257:$limit), [(int_tvm_setgaslimit I257:$limit)],
                       "SETGASLIMIT", 0xf801>;

defm BUYGAS    : I0<(outs), (ins I257:$tons), [(int_tvm_buygas I257:$tons)],
                    "BUYGAS", 0xf802>;

defm GRAMTOGAS : I0<(outs I257:$gas), (ins I257:$tons),
                    [(set I257:$gas, (int_tvm_gramtogas I257:$tons))],
                     "GRAMTOGAS", 0xf804>;

defm GASTOGRAM : I0<(outs I257:$tons), (ins I257:$gas),
                    [(set I257:$tons, (int_tvm_gastogram I257:$gas))],
                     "GASTOGRAM", 0xf805>;

defm COMMIT   : I0<(outs), (ins), [(int_tvm_commit)],
                   "COMMIT", 0xf80f>;

defm HASHCU   : I0<(outs I257 : $result), (ins Cell : $cell),
                   [(set I257 : $result, (int_tvm_hashcu Cell : $cell))],
                   "HASHCU", 0xf900>;

defm HASHSU   : I0<(outs I257 : $result), (ins Slice : $slice),
                   [(set I257 : $result, (int_tvm_hashsu Slice : $slice))],
                   "HASHSU", 0xf901>;

defm CHKSIGNU : I0<(outs I257 : $result),
                   (ins I257 : $hash, Slice : $signature, I257 : $key),
                   [(set I257 : $result, (int_tvm_chksignu I257  : $hash,
                                                           Slice : $signature,
                                                           I257  : $key))],
                   "CHKSIGNU", 0xf910>;

defm VERGRTH16   : I0<(outs I257 : $result), (ins Slice : $slice),
                   [(set I257 : $result, (int_tvm_vergrth16 Slice : $slice))],
                   "VERGRTH16", 0xf912>;

def : Pat<(int_tvm_get_temporary_data), (TO_TUPLE_COPY (PUSHC 7))>;

// A.11.4. Configuration primitives.

defm GETPARAM : I<(outs I257:$rv), (ins uimm4:$idx), (outs), (ins uimm4:$idx),
                  [(set I257:$rv, (int_tvm_getparam uimm4:$idx))],
                  "GETPARAM\t$idx", "GETPARAM\t$idx", 0xf820>;
def : Pat<(int_tvm_now), (GETPARAM 3)>;
def : Pat<(int_tvm_blocklt), (GETPARAM 4)>;
def : Pat<(int_tvm_ltime), (GETPARAM 5)>;
def : Pat<(int_tvm_myaddr), (TO_SLICE_COPY (GETPARAM 8))>;
def : Pat<(int_tvm_configroot), (TO_CELL_COPY (GETPARAM 9))>;

defm CONFIGDICT : I<(outs Cell:$dict, I257:$keylen), (ins), (outs), (ins),
                    [(set Cell:$dict, I257:$keylen, (int_tvm_configdict))],
                    "CONFIGDICT\t$dict, $keylen", "CONFIGDICT", 0xf830>;

defm CONFIGPARAM : I<(outs Cell:$param, I257:$succ), (ins I257:$idx),
                     (outs), (ins),
                     [(set Cell:$param, I257:$succ,
                       (int_tvm_configparam I257:$idx))],
                     "CONFIGPARAM\t$param, $succ, $idx", "CONFIGPARAM NULLSWAPIFNOT", 0xf832>;

defm CONFIGOPTPARAM : I<(outs Cell:$param), (ins I257:$idx), (outs), (ins),
                        [(set Cell:$param, (int_tvm_configoptparam I257:$idx))],
                        "CONFIGOPTPARAM\t$param, $idx", "CONFIGOPTPARAM",
                        0xf833>;

// A.11.5. Global variable primitives.
defm GETGLOB : I<(outs I257:$rv), (ins uimm1_31:$idx), (outs), (ins uimm1_31:$idx),
                 [(set I257:$rv, (int_tvm_getglobal uimm1_31:$idx))],
                 "GETGLOB\t$rv, $idx", "GETGLOB\t$idx", 0xf8500>;
defm GETGLOBVAR : I<(outs I257:$rv), (ins I257:$idx), (outs), (ins),
                    [(set I257:$rv, (int_tvm_getglobal I257:$idx))],
                    "GETGLOBVAR\t$rv, $idx", "GETGLOBVAR", 0xf840>;
defm SETGLOB : I<(outs), (ins I257:$val, uimm1_31:$idx), (outs), (ins uimm1_31:$idx),
                 [(int_tvm_setglobal uimm1_31:$idx, I257:$val)],
                 "SETGLOB\t$idx, $val", "SETGLOB\t$idx", 0xf8700>;
defm SETGLOBVAR : I<(outs), (ins I257:$val, I257:$idx), (outs), (ins),
                    [(int_tvm_setglobal I257:$idx, I257:$val)],
                    "SETGLOBVAR\t$idx, $val", "SETGLOBVAR", 0xf860>;

// A.11.10 Outbound message and output action primitives.
defm RAWRESERVE : I<(outs), (ins I257:$x, I257:$y), (outs), (ins),
                    [(int_tvm_rawreserve I257:$x, I257:$y)],
                    "RAWRESERVE\t$x, $y", "RAWRESERVE", 0xfb02>;

defm SETCODE : I<(outs), (ins Cell:$cell), (outs), (ins),
                 [(int_tvm_setcode Cell:$cell)],
                 "SETCODE\t$cell", "SETCODE", 0xfb04>;
