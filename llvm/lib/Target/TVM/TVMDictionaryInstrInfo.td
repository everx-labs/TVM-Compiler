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

// DICTGET-like instructions should use NULLSWAPIFNOT after, to insert null
//  result value.
// Otherwise instruction has variable results number: {val, succeeded} or {failed}.
// NULLSWAPIFNOT will correct it to {val, succeeded} or {null, failed}
let mayLoad = 1 in {
  defm DICTGET :
    I<(outs Slice:$result, I257:$status),
      (ins Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$result, I257:$status,
        (int_tvm_dictget Slice:$key, Cell:$dict, I257:$precision))],
      "DICTGET\t$result, $status, $key, $dict, $precision",
      "DICTGET NULLSWAPIFNOT", 0xf40a>;
  defm DICTGETREF :
    I<(outs Cell:$result, I257:$status),
      (ins Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$result, I257:$status,
        (int_tvm_dictgetref Slice:$key, Cell:$dict, I257:$precision))],
      "DICTGETREF\t$result, $status, $key, $dict, $precision",
      "DICTGETREF NULLSWAPIFNOT", 0xf40b>;
  defm DICTIGET :
    I<(outs Slice:$result, I257:$status),
      (ins I257:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$result, I257:$status,
        (int_tvm_dictiget I257:$key, Cell:$dict, I257:$precision))],
      "DICTIGET\t$result, $status, $key, $dict, $precision",
      "DICTIGET NULLSWAPIFNOT", 0xf40c>;
  defm DICTIGETREF :
    I<(outs Cell:$result, I257:$status),
      (ins I257:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$result, I257:$status,
        (int_tvm_dictigetref I257:$key, Cell:$dict, I257:$precision))],
      "DICTIGET\t$result, $status, $key, $dict, $precision",
      "DICTIGET NULLSWAPIFNOT", 0xf40d>;
  defm DICTUGET :
    I<(outs Slice:$result, I257:$status),
      (ins I257:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$result, I257:$status,
        (int_tvm_dictuget I257:$key, Cell:$dict, I257:$precision))],
      "DICTUGET\t$result, $status, $key, $dict, $precision",
      "DICTUGET NULLSWAPIFNOT", 0xf40e>;
  defm DICTUGETREF :
    I<(outs Cell:$result, I257:$status),
      (ins I257:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$result, I257:$status,
        (int_tvm_dictugetref I257:$key, Cell:$dict, I257:$precision))],
      "DICTUGETREF\t$result, $status, $key, $dict, $precision",
      "DICTUGETREF NULLSWAPIFNOT", 0xf40f>;

  defm DICTMIN :
    I<(outs Slice:$result, Slice:$key, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$result, Slice:$key, I257:$status,
        (int_tvm_dictmin Cell:$dict, I257:$precision))],
      "DICTMIN\t$result, $key, $status, $dict, $precision",
      "DICTMIN NULLSWAPIFNOT NULLSWAPIFNOT", 0xf482>;
  defm DICTGETNEXT :
    I<(outs Slice:$result, Slice:$next, I257:$status),
      (ins Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$result, Slice:$next, I257:$status,
        (int_tvm_dictgetnext Slice:$key, Cell:$dict, I257:$precision))],
      "DICTGETNEXT\t$result, $next, $status, $key, $dict, $precision",
      "DICTGETNEXT NULLSWAPIFNOT NULLSWAPIFNOT", 0xf474>;

  defm DICTUMIN :
    I<(outs Slice:$result, I257:$key, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$result, I257:$key, I257:$status,
        (int_tvm_dictumin Cell:$dict, I257:$precision))],
      "DICTUMIN\t$result, $key, $status, $dict, $precision",
      "DICTUMIN NULLSWAPIFNOT NULLSWAPIFNOT", 0xf486>;
  defm DICTUMINREF :
    I<(outs Cell:$result, I257:$key, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$result, I257:$key, I257:$status,
        (int_tvm_dictuminref Cell:$dict, I257:$precision))],
      "DICTUMINREF\t$result, $key, $status, $dict, $precision",
      "DICTUMINREF NULLSWAPIFNOT NULLSWAPIFNOT", 0xf487>;
  defm DICTUGETNEXT :
    I<(outs Slice:$result, I257:$next, I257:$status),
      (ins I257:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$result, I257:$next, I257:$status,
        (int_tvm_dictugetnext I257:$key, Cell:$dict, I257:$precision))],
      "DICTUGETNEXT\t$result, $next, $status, $key, $dict, $precision",
      "DICTUGETNEXT NULLSWAPIFNOT NULLSWAPIFNOT", 0xf47c>;
  defm DICTUMAX :
    I<(outs Slice:$result, I257:$key, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$result, I257:$key, I257:$status,
        (int_tvm_dictumax Cell:$dict, I257:$precision))],
      "DICTUMAX\t$result, $key, $status, $dict, $precision",
      "DICTUMAX NULLSWAPIFNOT NULLSWAPIFNOT", 0xf48e>;
  defm DICTUMAXREF :
    I<(outs Cell:$result, I257:$key, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$result, I257:$key, I257:$status,
        (int_tvm_dictumax Cell:$dict, I257:$precision))],
      "DICTUMAXREF\t$result, $key, $status, $dict, $precision",
      "DICTUMAXREF NULLSWAPIFNOT NULLSWAPIFNOT", 0xf48f>;
  defm DICTUGETPREV :
    I<(outs Slice:$result, I257:$next, I257:$status),
      (ins I257:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$result, I257:$next, I257:$status,
        (int_tvm_dictugetprev I257:$key, Cell:$dict, I257:$precision))],
      "DICTUGETPREV\t$result, $next, $status, $key, $dict, $precision",
      "DICTUGETPREV NULLSWAPIFNOT NULLSWAPIFNOT", 0xf47a>;
  defm DICTUREMMIN :
    I<(outs Cell:$newdict, Slice:$result, I257:$idx, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$result, I257:$idx, I257:$status,
        (int_tvm_dicturemmin Cell:$dict, I257:$precision))],
      "DICTUREMMIN\t$result, $idx, $status, $dict, $precision",
      "DICTUREMMIN NULLSWAPIFNOT NULLSWAPIFNOT", 0xf496>;
  defm DICTUREMMINREF :
    I<(outs Cell:$newdict, Cell:$result, I257:$idx, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Cell:$result, I257:$idx, I257:$status,
        (int_tvm_dicturemminref Cell:$dict, I257:$precision))],
      "DICTUREMMINREF\t$result, $idx, $status, $dict, $precision",
      "DICTUREMMINREF NULLSWAPIFNOT NULLSWAPIFNOT", 0xf497>;
  defm DICTUREMMAX :
    I<(outs Cell:$newdict, Slice:$result, I257:$idx, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$result, I257:$idx, I257:$status,
        (int_tvm_dicturemmax Cell:$dict, I257:$precision))],
      "DICTUREMMAX\t$result, $idx, $status, $dict, $precision",
      "DICTUREMMAX NULLSWAPIFNOT NULLSWAPIFNOT", 0xf496>;
  defm DICTUREMMAXREF :
    I<(outs Cell:$newdict, Cell:$result, I257:$idx, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Cell:$result, I257:$idx, I257:$status,
        (int_tvm_dicturemmaxref Cell:$dict, I257:$precision))],
      "DICTUREMMAXREF\t$result, $idx, $status, $dict, $precision",
      "DICTUREMMAXREF NULLSWAPIFNOT NULLSWAPIFNOT", 0xf49f>;
}

let mayStore = 1 in {
  defm DICTSET :
    I<(outs Cell:$newdict),
      (ins Slice:$val, Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict,
        (int_tvm_dictset Slice:$val, Slice:$key, Cell:$dict, I257:$precision))],
      "DICTSET\t$newdict, $val, $key, $dict, $precision",
      "DICTSET", 0xf412>;
  defm DICTSETREF :
    I<(outs Cell:$newdict),
      (ins Cell:$c, Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict,
        (int_tvm_dictsetref Cell:$c, Slice:$key, Cell:$dict, I257:$precision))],
      "DICTSETREF\t$newdict, $c, $key, $dict, $precision",
      "DICTSETREF", 0xf413>;
  defm DICTISET :
    I<(outs Cell:$newdict),
      (ins Slice:$val, I257:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict,
        (int_tvm_dictiset Slice:$val, I257:$key, Cell:$dict, I257:$precision))],
      "DICTISET\t$newdict, $val, $key, $dict, $precision",
      "DICTISET", 0xf414>;
  defm DICTISETREF :
    I<(outs Cell:$newdict),
      (ins Cell:$c, I257:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict,
        (int_tvm_dictisetref Cell:$c, I257:$key, Cell:$dict, I257:$precision))],
      "DICTISETREF\t$newdict, $c, $key, $dict, $precision",
      "DICTISETREF", 0xf415>;
  defm DICTUSET :
    I<(outs Cell:$newdict),
      (ins Slice:$val, I257:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict,
        (int_tvm_dictuset Slice:$val, I257:$key, Cell:$dict, I257:$precision))],
      "DICTUSET\t$newdict, $val, $key, $dict, $precision",
      "DICTUSET", 0xf416>;
  defm DICTUSETREF :
    I<(outs Cell:$newdict),
      (ins Cell:$c, I257:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict,
        (int_tvm_dictusetref Cell:$c, I257:$key, Cell:$dict, I257:$precision))],
      "DICTUSETREF\t$newdict, $c, $key, $dict, $precision",
      "DICTUSETREF", 0xf417>;
  defm DICTUSETGET :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Slice:$val, I257:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictusetget Slice:$val, I257:$key, Cell:$dict, I257:$precision))],
      "DICTUSETGET\t$newdict, $oldval, $succ, $val, $key, $dict, $precision",
      "DICTUSETGET NULLSWAPIFNOT", 0xf41e>;
  defm DICTUSETGETREF :
    I<(outs Cell:$newdict, Cell:$oldval, I257:$succ),
      (ins Cell:$val, I257:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Cell:$oldval, I257:$succ,
        (int_tvm_dictusetgetref Cell:$val, I257:$key, Cell:$dict, I257:$precision))],
      "DICTUSETGETREF\t$newdict, $oldval, $succ, $val, $key, $dict, $precision",
      "DICTUSETGETREF NULLSWAPIFNOT", 0xf41f>;

  defm DICTREPLACE :
    I<(outs Cell:$newdict, I257:$succ),
      (ins Slice:$s, Slice:$key, Cell:$D, I257:$key_len), (outs), (ins),
      [(set Cell:$newdict, I257:$succ,
        (int_tvm_dictreplace Slice:$s, Slice:$key, Cell:$D, I257:$key_len))],
      "DICTREPLACE\t$newdict, $succ, $s, $key, $D, $key_len",
      "DICTREPLACE", 0xf422>;
  defm DICTREPLACEREF :
    I<(outs Cell:$newdict, I257:$succ),
      (ins Cell:$c, Slice:$key, Cell:$D, I257:$key_len), (outs), (ins),
      [(set Cell:$newdict, I257:$succ,
        (int_tvm_dictreplaceref Cell:$c, Slice:$key, Cell:$D, I257:$key_len))],
      "DICTREPLACEREF\t$newdict, $succ, $c, $key, $D, $key_len",
      "DICTREPLACEREF", 0xf423>;
  defm DICTIREPLACE :
    I<(outs Cell:$newdict, I257:$succ),
      (ins Slice:$s, I257:$key, Cell:$D, I257:$key_len), (outs), (ins),
      [(set Cell:$newdict, I257:$succ,
        (int_tvm_dictireplace Slice:$s, I257:$key, Cell:$D, I257:$key_len))],
      "DICTIREPLACE\t$newdict, $succ, $s, $key, $D, $key_len",
      "DICTIREPLACE", 0xf424>;
  defm DICTIREPLACEREF :
    I<(outs Cell:$newdict, I257:$succ),
      (ins Cell:$c, I257:$key, Cell:$D, I257:$key_len), (outs), (ins),
      [(set Cell:$newdict, I257:$succ,
        (int_tvm_dictireplaceref Cell:$c, I257:$key, Cell:$D, I257:$key_len))],
      "DICTIREPLACEREF\t$newdict, $succ, $c, $key, $D, $key_len",
      "DICTIREPLACEREF", 0xf425>;
  defm DICTUREPLACE :
    I<(outs Cell:$newdict, I257:$succ),
      (ins Slice:$s, I257:$key, Cell:$D, I257:$key_len), (outs), (ins),
      [(set Cell:$newdict, I257:$succ,
        (int_tvm_dictureplace Slice:$s, I257:$key, Cell:$D, I257:$key_len))],
      "DICTUREPLACE\t$newdict, $succ, $s, $key, $D, $key_len",
      "DICTUREPLACE", 0xf426>;
  defm DICTUREPLACEREF :
    I<(outs Cell:$newdict, I257:$succ),
      (ins Cell:$c, I257:$key, Cell:$D, I257:$key_len), (outs), (ins),
      [(set Cell:$newdict, I257:$succ,
        (int_tvm_dictureplaceref Cell:$c, I257:$key, Cell:$D, I257:$key_len))],
      "DICTUREPLACEREF\t$newdict, $succ, $c, $key, $D, $key_len",
      "DICTUREPLACEREF", 0xf427>;

  // TODO: think how to implement DICTREPLACEGET-like operations
  //  with 2 or 3 return values (D old_val true) | (D false)

  defm DICTADD :
    I<(outs Cell:$newdict, I257:$succ),
      (ins Slice:$s, Slice:$key, Cell:$D, I257:$key_len), (outs), (ins),
      [(set Cell:$newdict, I257:$succ,
        (int_tvm_dictadd Slice:$s, Slice:$key, Cell:$D, I257:$key_len))],
      "DICTADD\t$newdict, $succ, $s, $key, $D, $key_len",
      "DICTADD", 0xf432>;
  defm DICTADDREF :
    I<(outs Cell:$newdict, I257:$succ),
      (ins Cell:$c, Slice:$key, Cell:$D, I257:$key_len), (outs), (ins),
      [(set Cell:$newdict, I257:$succ,
        (int_tvm_dictaddref Cell:$c, Slice:$key, Cell:$D, I257:$key_len))],
      "DICTADDREF\t$newdict, $succ, $c, $key, $D, $key_len",
      "DICTADDREF", 0xf433>;
  defm DICTIADD :
    I<(outs Cell:$newdict, I257:$succ),
      (ins Slice:$s, I257:$key, Cell:$D, I257:$key_len), (outs), (ins),
      [(set Cell:$newdict, I257:$succ,
        (int_tvm_dictiadd Slice:$s, I257:$key, Cell:$D, I257:$key_len))],
      "DICTIADD\t$newdict, $succ, $s, $key, $D, $key_len",
      "DICTIADD", 0xf434>;
  defm DICTIADDREF :
    I<(outs Cell:$newdict, I257:$succ),
      (ins Cell:$c, I257:$key, Cell:$D, I257:$key_len), (outs), (ins),
      [(set Cell:$newdict, I257:$succ,
        (int_tvm_dictiaddref Cell:$c, I257:$key, Cell:$D, I257:$key_len))],
      "DICTIADDREF\t$newdict, $succ, $c, $key, $D, $key_len",
      "DICTIADDREF", 0xf435>;
  defm DICTUADD :
    I<(outs Cell:$newdict, I257:$succ),
      (ins Slice:$s, I257:$key, Cell:$D, I257:$key_len), (outs), (ins),
      [(set Cell:$newdict, I257:$succ,
        (int_tvm_dictuadd Slice:$s, I257:$key, Cell:$D, I257:$key_len))],
      "DICTUADD\t$newdict, $succ, $s, $key, $D, $key_len",
      "DICTUADD", 0xf436>;
  defm DICTUADDREF :
    I<(outs Cell:$newdict, I257:$succ),
      (ins Cell:$c, I257:$key, Cell:$D, I257:$key_len), (outs), (ins),
      [(set Cell:$newdict, I257:$succ,
        (int_tvm_dictuaddref Cell:$c, I257:$key, Cell:$D, I257:$key_len))],
      "DICTUADDREF\t$newdict, $succ, $c, $key, $D, $key_len",
      "DICTUADDREF", 0xf437>;
  defm DICTDELGET :
    I<(outs Cell:$newdict, Slice:$result, I257:$status),
      (ins Slice:$key, Cell:$dict, I257:$precision),
      (outs), (ins),
      [(set Cell:$newdict, Slice:$result, I257:$status,
        (int_tvm_dictdelget Slice:$key, Cell:$dict, I257:$precision))],
      "DICTDELGET\t$newdict, $result, $status, $key, $dict, $precision",
      "DICTDELGET NULLSWAPIFNOT", 0xf462>;
  defm DICTDELGETREF :
    I<(outs Cell:$newdict, Cell:$result, I257:$status),
      (ins Slice:$key, Cell:$dict, I257:$precision),
      (outs), (ins),
      [(set Cell:$newdict, Cell:$result, I257:$status,
        (int_tvm_dictdelgetref Slice:$key, Cell:$dict, I257:$precision))],
      "DICTDELGETREF\t$newdict, $result, $status, $key, $dict, $precision",
      "DICTDELGETREF NULLSWAPIFNOT", 0xf463>;
  defm DICTUDEL :
    I<(outs Cell:$newdict, I257:$modified),
      (ins I257:$key, Cell:$dict, I257:$w), (outs), (ins),
      [(set Cell:$newdict, I257:$modified,
        (int_tvm_dictudel I257:$key, Cell:$dict, I257:$w))],
      "DICTUDEL\t$newdict, $modified, $key, $dict, $w", "DICTUDEL", 0xf45b>;

  // TODO: think how to implement DICTADDGET-like operations
  //  with 2 or 3 return values (D old_val true) | (D false)

  defm NEWDICT  : I<(outs Cell:$dict), (ins), (outs), (ins),
                    [(set Cell:$dict, (int_tvm_newdict))],
                    "NEWDICT\t$dict", "NEWDICT", 0x8b04>;
}

// TODO: Implement in terms of LDDICT.
defm PLDDICT    : I0<(outs Cell : $dict), (ins Slice : $slice),
                     [(set Cell : $dict, (int_tvm_plddict Slice : $slice))],
                     "PLDDICT", 0xf405>;

defm LDDICT : I0<(outs Cell:$dict, Slice:$rem_sl), (ins Slice:$sl),
                 [(set Cell:$dict, Slice:$rem_sl, (int_tvm_lddict Slice:$sl))],
                 "LDDICT", 0xf404>;

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
