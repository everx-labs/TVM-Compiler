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
  defm DICTIGET : I<(outs I64 : $result, I64 : $status),
                    (ins I64 : $key, I64 : $slice, I64 : $precision),
                    (outs), (ins),
                    [(set I64 : $result, I64 : $status,
                        (int_tvm_dictuget I64 : $key, I64 : $slice, I64 : $precision))],
                    "DICTIGET\t$result, $status, $key, $slice, $precision",
                    "DICTIGET", 0xf40c>;
  defm DICTUGET : I<(outs I64 : $result, I64 : $status),
                    (ins I64 : $key, slice_op : $slice, I64 : $precision),
                    (outs), (ins), [],
                    "DICTUGET\t$result, $status, $key, $slice, $precision",
                    "DICTUGET", 0xf40e>;
}

let mayStore = 1 in {
  defm DICTISET : I<(outs slice_op : $sliceo),
                    (ins I64 : $val, I64 : $key, slice_op : $slicei, I64 : $precision),
                    (outs), (ins), [],
                    "DICTISET\t$sliceo, $val, $key, $slicei, $precision",
                    "DICTISET", 0xf414>;
  defm DICTUSET : I<(outs slice_op : $sliceo),
                    (ins I64 : $val, I64 : $key, slice_op : $slicei, I64 : $precision),
                    (outs), (ins), [],
                    "DICTUSET\t$sliceo, $val, $key, $slicei, $precision",
                    "DICTUSET", 0xf416>;
  defm NEWDICT : I<(outs I64 : $slice), (ins), (outs), (ins),
                   [(set I64 : $slice, (int_tvm_newdict))],
                   "NEWDICT\t$slice", "NEWDICT", 0x8b04>;
}
