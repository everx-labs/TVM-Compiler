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
                    (ins I257:$key, Slice:$slice, I257:$precision),
                    (outs), (ins),
                    [(set Slice:$result, I257:$status,
                        (int_tvm_dictiget I257:$key, Slice:$slice, I257:$precision))],
                    "DICTIGET\t$result, $status, $key, $slice, $precision",
                    "DICTIGET", 0xf40c>;
  defm DICTUGET : I<(outs Slice:$result, I257:$status),
                    (ins I257:$key, Slice:$slice, I257:$precision),
                    (outs), (ins),
                    [(set Slice:$result, I257:$status,
                        (int_tvm_dictuget I257:$key, Slice:$slice, I257:$precision))],
                    "DICTUGET\t$result, $status, $key, $slice, $precision",
                    "DICTUGET", 0xf40e>;
}

let mayStore = 1 in {
  defm DICTISET : I<(outs Slice:$sliceo),
                    (ins I257:$val, I257:$key, Slice:$slicei, I257:$precision),
                    (outs), (ins),
                    [(set Slice:$sliceo,
                        (int_tvm_dictiset I257:$val, I257:$key, Slice:$slicei, I257:$precision))],
                    "DICTISET\t$sliceo, $val, $key, $slicei, $precision",
                    "DICTISET", 0xf414>;
  defm DICTUSET : I<(outs Slice:$sliceo),
                    (ins I257:$val, I257:$key, Slice:$slicei, I257:$precision),
                    (outs), (ins),
                    [(set Slice:$sliceo,
                        (int_tvm_dictuset I257:$val, I257:$key, Slice:$slicei, I257:$precision))],
                    "DICTUSET\t$sliceo, $val, $key, $slicei, $precision",
                    "DICTUSET", 0xf416>;
  defm NEWDICT  : I<(outs Slice:$slice), (ins), (outs), (ins),
                    [(set Slice:$slice, (int_tvm_newdict))],
                    "NEWDICT\t$slice", "NEWDICT", 0x8b04>;
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
