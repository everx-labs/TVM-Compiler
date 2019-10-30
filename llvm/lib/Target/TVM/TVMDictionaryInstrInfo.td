//===-- TVMDictionaryInstrInfo.td - TVM Instruction defs ---*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
/// \file
/// This file describes Dictionary TVM instructions (A.9) in TableGen format.
//
//===----------------------------------------------------------------------===//

let mayLoad = 1 in {
  defm DICTIGET : I<(outs Slice:$result, I257:$status),
                    (ins I257:$key, Cell:$dict, I257:$precision),
                    (outs), (ins),
                    [(set Slice:$result, I257:$status,
                        (int_tvm_dictiget I257:$key, Cell:$dict, I257:$precision))],
                    "DICTIGET\t$result, $status, $key, $dict, $precision",
                    "DICTIGET", 0xf40c>;
  defm DICTUGET : I<(outs Slice:$result, I257:$status),
                    (ins I257:$key, Cell:$dict, I257:$precision),
                    (outs), (ins),
                    [(set Slice:$result, I257:$status,
                        (int_tvm_dictuget I257:$key, Cell:$dict, I257:$precision))],
                    "DICTUGET\t$result, $status, $key, $dict, $precision",
                    "DICTUGET", 0xf40e>;
}

let mayStore = 1 in {
  defm DICTISET : I<(outs Cell:$dicto),
                    (ins I257:$val, I257:$key, Cell:$dicti, I257:$precision),
                    (outs), (ins),
                    [(set Cell:$dicto,
                        (int_tvm_dictiset I257:$val, I257:$key, Cell:$dicti, I257:$precision))],
                    "DICTISET\t$dicto, $val, $key, $dicti, $precision",
                    "DICTISET", 0xf414>;
  defm DICTUSET : I<(outs Cell:$dicto),
                    (ins I257:$val, I257:$key, Cell:$dicti, I257:$precision),
                    (outs), (ins),
                    [(set Cell:$dicto,
                        (int_tvm_dictuset I257:$val, I257:$key, Cell:$dicti, I257:$precision))],
                    "DICTUSET\t$dicto, $val, $key, $dicti, $precision",
                    "DICTUSET", 0xf416>;
  defm NEWDICT  : I<(outs Cell:$dict), (ins), (outs), (ins),
                    [(set Cell:$dict, (int_tvm_newdict))],
                    "NEWDICT\t$dict", "NEWDICT", 0x8b04>;
}

// TODO: Implement in terms of LDDICT.
defm PLDDICT    : I0<(outs Cell : $dict), (ins Slice : $slice),
                     [(set Cell : $dict, (int_tvm_plddict Slice : $slice))],
                     "PLDDICT", 0xf405>;

// A.10.2 Dictionary serialization and deserialization
defm STDICTS    : I0<(outs Builder : $builder),
                     (ins Slice : $slice, Builder : $builderi),
                     [(set Builder : $builder, (int_tvm_stdicts Slice : $slice,
                                                Builder : $builderi))],
                     "STDICTS", 0xce>;
defm STDICT     : I0<(outs Builder : $builder),
                     (ins Cell : $dict, Builder : $builderi),
                     [(set Builder : $builder, (int_tvm_stdict Cell : $dict,
                                                Builder : $builderi))],
                     "STDICT", 0xf400>;
