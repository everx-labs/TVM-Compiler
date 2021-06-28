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
        (int_tvm_dictumaxref Cell:$dict, I257:$precision))],
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
  defm DICTSETGET :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Slice:$val, Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictsetget Slice:$val, Slice:$key, Cell:$dict, I257:$precision))],
      "DICTSETGET\t$newdict, $oldval, $succ, $val, $key, $dict, $precision",
      "DICTSETGET NULLSWAPIFNOT", 0xf41e>;
  defm DICTSETGETREF :
    I<(outs Cell:$newdict, Cell:$oldval, I257:$succ),
      (ins Cell:$val, Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Cell:$oldval, I257:$succ,
        (int_tvm_dictsetgetref Cell:$val, Slice:$key, Cell:$dict, I257:$precision))],
      "DICTSETGETREF\t$newdict, $oldval, $succ, $val, $key, $dict, $precision",
      "DICTSETGETREF NULLSWAPIFNOT", 0xf41f>;

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
  defm DICTDEL :
    I<(outs Cell:$newdict, I257:$modified),
      (ins Slice:$key, Cell:$dict, I257:$w), (outs), (ins),
      [(set Cell:$newdict, I257:$modified,
        (int_tvm_dictdel Slice:$key, Cell:$dict, I257:$w))],
      "DICTDEL\t$newdict, $modified, $key, $dict, $w", "DICTDEL", 0xf459>;

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

// ******************************************* 
//
// A.10.2. Dictionary serialization and deserialization
//
// *******************************************/

// F401 — SKIPDICT or SKIPOPTREF (s – s0), equivalent to LDDICT; NIP
let mayLoad = 1 in
defm SKIPDICT : I0<(outs Slice:$dict), (ins Slice:$sl),
                 [(set Slice:$dict, (int_tvm_skipdict Slice:$sl))],
                 "SKIPDICT", 0xf401>;

// F402 — LDDICTS (s – s0 s00), loads (parses) a (Slice-represented) dictionary s0 from Slice s, and returns the remainder of s as s00
let mayLoad = 1 in
defm LDDICTS : I0<(outs Slice:$dict, Slice:$rem_sl), (ins Slice:$sl),
                 [(set Slice:$dict, Slice:$rem_sl, (int_tvm_lddicts Slice:$sl))],
                 "LDDICTS", 0xf402>;

// F403 — PLDDICTS (s – s0), preloads a (Slice - represented) dictionary s0 from Slice s. Approximately equivalent to LDDICTS; DROP
let mayLoad = 1 in
defm PLDDICTS : I0<(outs Slice:$dict), (ins Slice:$sl), 
                 [(set Slice:$dict, (int_tvm_plddicts Slice:$sl))],
                 "PLDDICTS", 0xf403>;

// F406 — LDDICTQ (s – D s0 -1 or s 0), a quiet version of LDDICT
let mayLoad = 1 in
defm LDDICTQ : I0<(outs Cell:$dict, Slice:$rem_sl, I257:$result), (ins Slice:$sl), 
                 [(set Cell:$dict, Slice:$rem_sl, I257:$result, (int_tvm_lddictq Slice:$sl))],
                 "LDDICTQ", 0xf406>;

// F407 — PLDDICTQ (s – D -1 or 0), a quiet version of PLDDICT
let mayLoad = 1 in
defm PLDDICTQ : I0<(outs Cell : $dict, I257:$result), (ins Slice : $sl),
                    [(set Cell : $dict, I257:$result, (int_tvm_plddictq Slice : $sl))],
                    "PLDDICTQ", 0xf407>;

// ******************************************* 
//
// A.10.4.	Set/Replace/Add dictionary operations
//
// *******************************************/

// F41C — DICTISETGET (x i D n – D0 y -1 or D0 0), similar to DICTSETGET, 
//  but with the key represented by a big-endian signed n-bit Integer I
// (value int slice nbits - (slice y -1) | (slice 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTISETGET :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Slice:$val, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictisetget Slice:$val, I257:$i, Cell:$dict, I257:$precision))],
      "DICTISETGET\t$newdict, $oldval, $succ, $val, $i, $dict, $precision",
      "DICTISETGET NULLSWAPIFNOT", 0xf41c>;

// F41D — DICTISETGETREF (c i D n – D0 c0 -1 or D0 0), a version of DICTSETGETREF with signed Integer i as a key
// (cell int slice nbits - (slice cell -1) | (slice 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTISETGETREF :
    I<(outs Cell:$newdict, Cell:$oldval, I257:$succ),
      (ins Cell:$val, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Cell:$oldval, I257:$succ,
        (int_tvm_dictisetgetref Cell:$val, I257:$i, Cell:$dict, I257:$precision))],
      "DICTISETGETREF\t$newdict, $oldval, $succ, $val, $i, $dict, $precision",
      "DICTISETGETREF NULLSWAPIFNOT", 0xf41d>;

// F42A — DICTREPLACEGET (x k D n – D0 y -1 or D 0), a Replace counterpart of DICTSETGET: 
//  on success, also returns the old value associated with the key in question
// (value int slice nbits - (slice value -1) | (slice 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTREPLACEGET :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Slice:$val, Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictreplaceget Slice:$val, Slice:$key, Cell:$dict, I257:$precision))],
      "DICTREPLACEGET\t$newdict, $oldval, $succ, $val, $key, $dict, $precision",
      "DICTREPLACEGET NULLSWAPIFNOT", 0xf42a>;

// F42B — DICTREPLACEGETREF (c k D n – D0 c0 -1 or D 0)
// (cell int slice nbits - (slice' cell -1) | (slice 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTREPLACEGETREF :
    I<(outs Cell:$newdict, Cell:$oldval, I257:$succ),
      (ins Cell:$val, Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Cell:$oldval, I257:$succ,
        (int_tvm_dictreplacegetref Cell:$val, Slice:$key, Cell:$dict, I257:$precision))],
      "DICTREPLACEGETREF\t$newdict, $oldval, $succ, $val, $key, $dict, $precision",
      "DICTREPLACEGETREF NULLSWAPIFNOT", 0xf42b>;

// F42C — DICTIREPLACEGET  (x i D n – D0 y -1 or D 0)
// (value int slice nbits - (slice value -1) | (slice 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTIREPLACEGET :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Slice:$val, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictireplaceget Slice:$val, I257:$i, Cell:$dict, I257:$precision))],
      "DICTIREPLACEGET\t$newdict, $oldval, $succ, $val, $i, $dict, $precision",
      "DICTIREPLACEGET NULLSWAPIFNOT", 0xf42c>;

// F42D — DICTIREPLACEGETREF (c i D n – D0 c0 -1 or D 0)
// (cell int slice nbits - (slice' cell -1) | (slice 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTIREPLACEGETREF :
    I<(outs Cell:$newdict, Cell:$oldval, I257:$succ),
      (ins Cell:$val, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Cell:$oldval, I257:$succ,
        (int_tvm_dictireplacegetref Cell:$val, I257:$i, Cell:$dict, I257:$precision))],
      "DICTIREPLACEGETREF\t$newdict, $oldval, $succ, $val, $i, $dict, $precision",
      "DICTIREPLACEGETREF NULLSWAPIFNOT", 0xf42d>;

// F42E — DICTUREPLACEGET (x i D n – D0 y -1 or D 0)
// (value uint slice nbits - (slice value -1) | (slice 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTUREPLACEGET :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Slice:$val, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictureplaceget Slice:$val, I257:$i, Cell:$dict, I257:$precision))],
      "DICTUREPLACEGET\t$newdict, $oldval, $succ, $val, $i, $dict, $precision",
      "DICTUREPLACEGET NULLSWAPIFNOT", 0xf42e>;

// F42F — DICTUREPLACEGETREF (c i D n – D0 c0 -1 or D 0)
// (cell uint slice nbits - (slice' cell -1) | (slice 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTUREPLACEGETREF :
    I<(outs Cell:$newdict, Cell:$oldval, I257:$succ),
      (ins Cell:$val, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Cell:$oldval, I257:$succ,
        (int_tvm_dictureplacegetref Cell:$val, I257:$i, Cell:$dict, I257:$precision))],
      "DICTUREPLACEGETREF\t$newdict, $oldval, $succ, $val, $i, $dict, $precision",
      "DICTUREPLACEGETREF NULLSWAPIFNOT", 0xf42f>;

// F43A — DICTADDGET (x k D n – D0 -1 or D y 0), an Add counterpart of DICTSETGET: sets the value associated with key k in dictionary D to x, 
//   but only if key k is not already present in D. Otherwise, just returns the old value y without changing the dictionary
// (value key slice nbits - (slice -1) | (slice y 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTADDGET :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Slice:$val, Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictaddget Slice:$val, Slice:$key, Cell:$dict, I257:$precision))],
      "DICTADDGET\t$newdict, $oldval, $succ, $val, $key, $dict, $precision",
      "DICTADDGET NULLSWAPIFNOT", 0xf43a>;

// F43B	— DICTADDGETREF (c k D n – D0 -1 or D c0 0), an Add counterpart of DICTSETGETREF
// (cell key slice nbits - (slice' -1) | (slice cell 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTADDGETREF :
    I<(outs Cell:$newdict, Cell:$oldval, I257:$succ),
      (ins Cell:$val, Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Cell:$oldval, I257:$succ,
        (int_tvm_dictaddgetref Cell:$val, Slice:$key, Cell:$dict, I257:$precision))],
      "DICTADDGETREF\t$newdict, $oldval, $succ, $val, $key, $dict, $precision",
      "DICTADDGETREF NULLSWAPIFNOT", 0xf43b>;

// F43C — DICTIADDGET (x i D n – D0 -1 or D y 0)
// (value int slice nbits - (slice -1) | (slice value 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTIADDGET :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Slice:$val, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictiaddget Slice:$val, I257:$i, Cell:$dict, I257:$precision))],
      "DICTIADDGET\t$newdict, $oldval, $succ, $val, $i, $dict, $precision",
      "DICTIADDGET NULLSWAPIFNOT", 0xf43c>;

// F43D — DICTIADDGETREF (c i D n – D0 -1 or D c0 0)
let mayLoad = 1, mayStore = 1 in
  defm DICTIADDGETREF :
    I<(outs Cell:$newdict, Cell:$oldval, I257:$succ),
      (ins Cell:$val, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Cell:$oldval, I257:$succ,
        (int_tvm_dictiaddgetref Cell:$val, I257:$i, Cell:$dict, I257:$precision))],
      "DICTIADDGETREF\t$newdict, $oldval, $succ, $val, $i, $dict, $precision",
      "DICTIADDGETREF NULLSWAPIFNOT", 0xf43d>;

// F43E — DICTUADDGET	(x i D n – D0 -1 or D y 0)
// (value uint slice nbits - (slice -1) | (slice value 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTUADDGET :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Slice:$val, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictuaddget Slice:$val, I257:$i, Cell:$dict, I257:$precision))],
      "DICTUADDGET\t$newdict, $oldval, $succ, $val, $i, $dict, $precision",
      "DICTUADDGET NULLSWAPIFNOT", 0xf43e>;

// F43F — DICTUADDGETREF (c i D n – D0 -1 or D c0 0)
// (cell uint slice nbits - (slice -1) | (slice cell 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTUADDGETREF :
    I<(outs Cell:$newdict, Cell:$oldval, I257:$succ),
      (ins Cell:$val, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Cell:$oldval, I257:$succ,
        (int_tvm_dictuaddgetref Cell:$val, I257:$i, Cell:$dict, I257:$precision))],
      "DICTUADDGETREF\t$newdict, $oldval, $succ, $val, $i, $dict, $precision",
      "DICTUADDGETREF NULLSWAPIFNOT", 0xf43d>;

// ******************************************* 
//
// A.10.5. Builder-accepting variants of Set dictionary operations.
//
// *******************************************/

// F441 — DICTSETB (b k D n – D0)
// (builder key slice nbits - slice)
defm DICTSETB : I<(outs Cell:$odict), (ins Builder:$bldr, Slice:$key, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, (int_tvm_dictsetb Builder:$bldr, Slice:$key, Cell:$idict, I257:$precision))],
                 "DICTSETB\t$odict, $bldr, $key, $idict, $precision","DICTSETB", 0xf441>;

// F442 — DICTISETB (b i D n – D0)
// (builder int slice nbits - slice)
defm DICTISETB : I<(outs Cell:$odict), (ins Builder:$bldr, I257:$i, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, (int_tvm_dictisetb Builder:$bldr, I257:$i, Cell:$idict, I257:$precision))],
                 "DICTISETB\t$odict, $bldr, $i, $idict, $precision","DICTISETB", 0xf442>;

// F443 — DICTUSETB (b i D n – D0)
// (builder uint slice nbits - slice)
defm DICTUSETB : I<(outs Cell:$odict), (ins Builder:$bldr, I257:$i, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, (int_tvm_dictusetb Builder:$bldr, I257:$i, Cell:$idict, I257:$precision))],
                 "DICTUSETB\t$odict, $bldr, $i, $idict, $precision","DICTUSETB", 0xf443>;

// F445 — DICTSETGETB (b k D n – D0 y -1 or D0 0)
// (builder key slice nbits - (slice value -1) | (slice 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTSETGETB :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Builder:$bldr, Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictsetgetb Builder:$bldr, Slice:$key, Cell:$dict, I257:$precision))],
      "DICTSETGETB\t$newdict, $oldval, $succ, $bldr, $key, $dict, $precision",
      "DICTSETGETB NULLSWAPIFNOT", 0xf445>;

// F446 — DICTISETGETB (b i D n – D0 y -1 or D0 0)
// (builder int slice nbits - (slice' y -1) | (slice' 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTISETGETB :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Builder:$bldr, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictisetgetb Builder:$bldr, I257:$i, Cell:$dict, I257:$precision))],
      "DICTISETGETB\t$newdict, $oldval, $succ, $bldr, $i, $dict, $precision",
      "DICTISETGETB NULLSWAPIFNOT", 0xf446>;

// F447 — DICTUSETGETB (b i D n – D0 y -1 or D0 0)
// (builder uint slice nbits - (slice' y -1) | (slice' 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTUSETGETB :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Builder:$bldr, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictusetgetb Builder:$bldr, I257:$i, Cell:$dict, I257:$precision))],
      "DICTUSETGETB\t$newdict, $oldval, $succ, $bldr, $i, $dict, $precision",
      "DICTUSETGETB NULLSWAPIFNOT", 0xf447>;

// F449 — DICTREPLACEB (b k D n – D0 -1 or D 0)
// (builder key slice nbits - (slice -1) | (slice 0))
defm DICTREPLACEB : I<(outs Cell:$odict, I257:$result), (ins Builder:$bldr, Slice:$key, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, I257:$result, (int_tvm_dictreplaceb Builder:$bldr, Slice:$key, Cell:$idict, I257:$precision))],
                 "DICTREPLACEB\t$odict, $result, $bldr, $key, $idict, $precision","DICTREPLACEB", 0xf449>;

// F44A — DICTIREPLACEB (b i D n – D0 -1 or D 0)
// (builder uint slice nbits - (slice -1) | (slice 0))
defm DICTIREPLACEB : I<(outs Cell:$odict, I257:$result), (ins Builder:$bldr, I257:$i, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, I257:$result, (int_tvm_dictireplaceb Builder:$bldr, I257:$i, Cell:$idict, I257:$precision))],
                 "DICTIREPLACEB\t$odict, $result, $bldr, $i, $idict, $precision","DICTIREPLACEB", 0xf44a>;

// F44B — DICTUREPLACEB (b i D n – D0 -1 or D 0)
// (builder uint slice nbits - (slice -1) | (slice 0))
defm DICTUREPLACEB : I<(outs Cell:$odict, I257:$result), (ins Builder:$bldr, I257:$i, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, I257:$result, (int_tvm_dictureplaceb Builder:$bldr, I257:$i, Cell:$idict, I257:$precision))],
                 "DICTUREPLACEB\t$odict, $result, $bldr, $i, $idict, $precision","DICTUREPLACEB", 0xf44a>;

// F44D — DICTREPLACEGETB (b k D n – D0 y -1 or D 0)
// (builder key slice nbits - (slice value -1) | (slice 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTREPLACEGETB :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Builder:$bldr, Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictreplacegetb Builder:$bldr, Slice:$key, Cell:$dict, I257:$precision))],
      "DICTREPLACEGETB\t$newdict, $oldval, $succ, $bldr, $key, $dict, $precision",
      "DICTREPLACEGETB NULLSWAPIFNOT", 0xf44d>;


// F44E — DICTIREPLACEGETB (b i D n – D0 y -1 or D 0)
// (builder int slice nbits - (slice value -1) | (slice 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTIREPLACEGETB :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Builder:$bldr, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictireplacegetb Builder:$bldr, I257:$i, Cell:$dict, I257:$precision))],
      "DICTIREPLACEGETB\t$newdict, $oldval, $succ, $bldr, $i, $dict, $precision",
      "DICTIREPLACEGETB NULLSWAPIFNOT", 0xf44e>;

// F44F — DICTUREPLACEGETB	(b i D n – D0 y -1 or D 0)
// (builder uint slice nbits - (slice value -1) | (slice 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTUREPLACEGETB :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Builder:$bldr, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictureplacegetb Builder:$bldr, I257:$i, Cell:$dict, I257:$precision))],
      "DICTUREPLACEGETB\t$newdict, $oldval, $succ, $bldr, $i, $dict, $precision",
      "DICTUREPLACEGETB NULLSWAPIFNOT", 0xf44f>;


// F451 — DICTADDB (b k D n – D0 -1 or D 0)
// (builder key slice nbits - (slice -1) | (slice 0))
defm DICTADDB : I<(outs Cell:$odict, I257:$result), (ins Builder:$bldr, Slice:$key, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, I257:$result, (int_tvm_dictaddb Builder:$bldr, Slice:$key, Cell:$idict, I257:$precision))],
                 "DICTADDB\t$odict,  $result, $bldr, $key, $idict, $precision","DICTADDB", 0xf451>;

// F452 — DICTIADDB (b i D n – D0 -1 or D 0)
// (builder int slice nbits - (slice -1) | (slice 0))
defm DICTIADDB : I<(outs Cell:$odict, I257:$result), (ins Builder:$bldr, I257:$i, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, I257:$result, (int_tvm_dictiaddb Builder:$bldr, I257:$i, Cell:$idict, I257:$precision))],
                 "DICTIADDB\t$odict, $result, $bldr, $i, $idict, $precision","DICTIADDB", 0xf452>;


// F453 — DICTUADDB  (b i D n – D0 -1 or D 0)
// (builder uint slice nbits - (slice -1) | (slice 0))
defm DICTUADDB : I<(outs Cell:$odict, I257:$result), (ins Builder:$bldr, I257:$i, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, I257:$result, (int_tvm_dictuaddb Builder:$bldr, I257:$i, Cell:$idict, I257:$precision))],
                 "DICTUADDB\t$odict, $result, $bldr, $i, $idict, $precision","DICTUADDB", 0xf453>;

// F455 — DICTADDGETB (b k D n – D0 -1 or D y 0)
// (builder key slice nbits - (slice -1) | (slice value 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTADDGETB :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Builder:$bldr, Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictaddgetb Builder:$bldr, Slice:$key, Cell:$dict, I257:$precision))],
      "DICTADDGETB\t$newdict, $oldval, $succ, $bldr, $key, $dict, $precision",
      "DICTADDGETB NULLSWAPIFNOT", 0xf455>;
	  
// F456 — DICTIADDGETB (b i D n – D0 -1 or D y 0)
// (builder int slice nbits - (slice' -1) | (slice y 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTIADDGETB :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Builder:$bldr, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictiaddgetb Builder:$bldr, I257:$i, Cell:$dict, I257:$precision))],
      "DICTIADDGETB\t$newdict, $oldval, $succ, $bldr, $i, $dict, $precision",
      "DICTIADDGETB NULLSWAPIFNOT", 0xf456>;

// F457 — DICTUADDGETB	(b i D n – D0 -1 or D y 0)
// (builder uint slice nbits - (slice' -1) | (slice y 0))
let mayLoad = 1, mayStore = 1 in
  defm DICTUADDGETB :
    I<(outs Cell:$newdict, Slice:$oldval, I257:$succ),
      (ins Builder:$bldr, I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$oldval, I257:$succ,
        (int_tvm_dictuaddgetb Builder:$bldr, I257:$i, Cell:$dict, I257:$precision))],
      "DICTUADDGETB\t$newdict, $oldval, $succ, $bldr, $i, $dict, $precision",
      "DICTUADDGETB NULLSWAPIFNOT", 0xf457>;


// ******************************************* 
//
// A.10.6. Delete dictionary operations
//
// *******************************************/

// F45A	— DICTIDEL (i D n – D0 ?), a version of DICTDEL with the key represented by a signed n-bit Integer i. If i does not fit into n bits,
//   simply returns D 0 (“key not found, dictionary unmodified”)
// (int slice nbits - (slice' -1) | (slice 0))
let mayStore = 1 in
  defm DICITDEL :
    I<(outs Cell:$newdict, I257:$succ),
      (ins I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, I257:$succ,
        (int_tvm_dictidel I257:$i, Cell:$dict, I257:$precision))],
      "DICTDEL\t$newdict, $succ, $i, $dict, $precision",
      "DICTDEL", 0xf45a>;

// F464 — DICTIDELGET (i D n – D0 x -1 or D 0), a variant of primitive DICTDELGET with signed n-bit integer i as a key
// (int slice nbits - (slice value -1) | (slice 0))
let mayLoad = 1, mayStore = 1 in
  defm DICITDELGET :
    I<(outs Cell:$newdict, Slice:$x, I257:$succ),
      (ins I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$x, I257:$succ,
        (int_tvm_dictidelget I257:$i, Cell:$dict, I257:$precision))],
      "DICTDELGET\t$newdict, $x, $succ, $i, $dict, $precision",
      "DICTDELGET NULLSWAPIFNOT", 0xf464>;

// F465	— DICTIDELGETREF (i D n – D0 c -1 or  D 0), a variant of primitive DICTIDELGET returning a Cell instead of a Slice
// (int slice nbits - (slice cell -1) | (slice 0))
let mayLoad = 1, mayStore = 1 in
  defm DICITDELGETREF :
    I<(outs Cell:$newdict, Cell:$c, I257:$succ),
      (ins I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Cell:$c, I257:$succ,
        (int_tvm_dictidelgetref I257:$i, Cell:$dict, I257:$precision))],
      "DICTDELGETREF\t$newdict, $c, $succ, $i, $dict, $precision",
      "DICTDELGETREF NULLSWAPIFNOT", 0xf465>;

// F466 — DICTUDELGET (i D n – D0x -1 or D 0), a variant of primitive DICTDELGET with unsigned n-bit integer i as a key
// (uint slice nbits - (slice value -1) | (slice 0))
let mayLoad = 1, mayStore = 1 in
  defm DICUTDELGET :
    I<(outs Cell:$newdict, Slice:$x, I257:$succ),
      (ins I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Slice:$x, I257:$succ,
        (int_tvm_dictudelget I257:$i, Cell:$dict, I257:$precision))],
      "DICUDELGET\t$newdict, $x, $succ, $i, $dict, $precision",
      "DICUDELGET NULLSWAPIFNOT", 0xf466>;

// F467	— DICTUDELGETREF (i D n – D0 c -1 or D 0), a variant of primitive DICTUDELGET returning a Cell instead of a Slice
let mayLoad = 1, mayStore = 1 in
  defm DICUTDELGETREF :
    I<(outs Cell:$newdict, Cell:$c, I257:$succ),
      (ins I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$newdict, Cell:$c, I257:$succ,
        (int_tvm_dictudelgetref I257:$i, Cell:$dict, I257:$precision))],
      "DICUDELGETREF\t$newdict, $c, $succ, $i, $dict, $precision",
      "DICUDELGETREF NULLSWAPIFNOT", 0xf467>;


// ******************************************* 
//
// A.10.7. “Maybe reference” dictionary operations
//
// *******************************************/

// F469 — DICTGETOPTREF (k D n – c?), a variant of DICTGETREF that returns Null	
//   instead of the value c? if the key k is absent from dictionary D
// (key slice nbits - (slice -1) | (slice 0))
let mayLoad = 1 in
defm DICTGETOPTREF : I<(outs Cell:$c, I257:$result), (ins Slice:$key, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$c, I257:$result, 
				   (int_tvm_dictgetoptref Slice:$key, Cell:$idict, I257:$precision))],
                 "DICTGETOPTREF\t$c,  $result, $idict, $precision","DICTGETOPTREF", 0xf469>;

// F46A — DICTIGETOPTREF (i D n – c?), similar to DICTGETOPTREF, but also returns Null
let mayLoad = 1 in
defm DICTIGETOPTREF : I<(outs Cell:$c), (ins I257:$i, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$c,  
				   (int_tvm_dictigetoptref I257:$i, Cell:$idict, I257:$precision))],
                 "DICTIGETOPTREF\t$c, $i, $idict, $precision","DICTIGETOPTREF", 0xf46a>;

// F46B — DICTUGETOPTREF (i D n – c?), similar to DICTGETOPTREF, but with the key given by unsigned n-bit Integer
let mayLoad = 1 in
defm DICTUGETOPTREF : I<(outs Cell:$c, I257:$result), (ins I257:$i, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$c, I257:$result, 
				   (int_tvm_dictugetoptref I257:$i, Cell:$idict, I257:$precision))],
                 "DICTUGETOPTREF\t$c,  $result, $idict, $precision","DICTUGETOPTREF", 0xf46b>;

// F46D	— DICTSETGETOPTREF	(c? k D n – D0 ~?), a variant of both DICTGETOPTREF and DICTSETGETREF that sets the value corresponding to key k 
//   in dictionary D to c? (if c? Is Null, then the key is deleted instead), and returns the old value ~? (if the key k was absent before, returns Null instead)
let mayLoad = 1, mayStore = 1 in
defm DICTSETGETOPTREF : I<(outs Cell:$odict, Cell:$oc), (ins Cell:$ic, Slice:$key, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, Cell:$oc, 
				   (int_tvm_dictsetgetoptref Cell:$ic, Slice:$key, Cell:$idict, I257:$precision))],
                 "DICTSETGETOPTREF\t$odict, $oc, $ic, $key, $idict, $precision","DICTSETGETOPTREF", 0xf46d>;

// F46E — DICTISETGETOPTREF (c? i D n – D0 ~?), similar to primitive DICTSETGETOPTREF, but using signed n-bit Integer i as a key. 
//   If i does not fit into n bits, throws a range checking exception
let mayLoad = 1, mayStore = 1 in
defm DICTISETGETOPTREF : I<(outs Cell:$odict, Cell:$oc), (ins Cell:$ic, I257:$i, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, Cell:$oc, 
				   (int_tvm_dictisetgetoptref Cell:$ic, I257:$i, Cell:$idict, I257:$precision))],
                 "DICTISETGETOPTREF\t$odict, $oc, $ic, $i, $idict, $precision","DICTISETGETOPTREF", 0xf46e>;

// F46F	— DICTUSETGETOPTREF (c? i D n –	D0 ~?), similar to primitive DICTSETGETOPTREF, but using unsigned n-bit Integer i as a key
let mayLoad = 1, mayStore = 1 in
defm DICTUSETGETOPTREF : I<(outs Cell:$odict, Cell:$oc), (ins Cell:$ic, I257:$i, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, Cell:$oc, 
				   (int_tvm_dictusetgetoptref Cell:$ic, I257:$i, Cell:$idict, I257:$precision))],
                 "DICTUSETGETOPTREF\t$odict, $oc, $ic, $i, $idict, $precision","DICTUSETGETOPTREF", 0xf46F>;

// ******************************************* 
//
// A.10.8. Prefix code dictionary operations
//
// *******************************************/

// F470 — PFXDICTSET (x k D n – D0 -1 or D 0)
// (value key slice nbits - slice -1|0)
let mayStore = 1 in
defm PFXDICTSET : I<(outs Cell:$odict, I257:$result), (ins Slice:$value, Slice:$key, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, I257:$result, (int_tvm_pfxdictset Slice:$value, Slice:$key, Cell:$idict, I257:$precision))],
                 "PFXDICTSET\t$odict, $result, $value, $key, $idict, $precision","PFXDICTSET", 0xf470>;

// F471 — PFXDICTREPLACE (x k D n – D0 -1 or D 0)
// (value key slice nbits - slice -1|0)
let mayLoad = 1, mayStore = 1 in
defm PFXDICTREPLACE : I<(outs Cell:$odict, I257:$result), (ins Slice:$value, Slice:$key, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, I257:$result, (int_tvm_pfxdictreplace Slice:$value, Slice:$key, Cell:$idict, I257:$precision))],
                 "PFXDICTREPLACE\t$odict, $result, $value, $key, $idict, $precision","PFXDICTREPLACE", 0xf471>;

// F472 — PFXDICTADD (x k D n – D0 -1 or D 0)
// (value key slice nbits - slice -1|0)
let mayStore = 1 in
defm PFXDICTADD : I<(outs Cell:$odict, I257:$result), (ins Slice:$value, Slice:$key, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, I257:$result, (int_tvm_pfxdictadd Slice:$value, Slice:$key, Cell:$idict, I257:$precision))],
                 "PFXDICTADD\t$odict, $result, $value, $key, $idict, $precision","PFXDICTADD", 0xf472>;

// F473 — PFXDICTDEL (k D n – D0 -1 or D 0)
// (key slice nbits - slice -1|0)
let mayStore = 1 in
defm PFXDICTDEL : I<(outs Cell:$odict, I257:$result), (ins Slice:$key, Cell:$idict, I257:$precision), (outs), (ins),
                 [(set Cell:$odict, I257:$result, (int_tvm_pfxdictdel Slice:$key, Cell:$idict, I257:$precision))],
                 "PFXDICTDEL\t$odict, $result, $key, $idict, $precision","PFXDICTDEL", 0xf473>;

// ******************************************* 
//
// A.10.9. Variants of GetNext and GetPrev operations
//
// *******************************************/

// F475 — DICTGETNEXTEQ (k D n – x0 k0 -1 or 0), similar to DICTGETNEXT, but computes the minimal key k0 
//  that is lexicographically greater than or equal to k
// (key slice nbits - (value key -1) | (0))
let mayLoad = 1 in
  defm DICTGETNEXTQ :
    I<(outs Slice:$value, Slice:$next, I257:$status),
      (ins Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$value, Slice:$next, I257:$status,
        (int_tvm_dictgetnexteq Slice:$key, Cell:$dict, I257:$precision))],
      "DICTGETNEXTQ\t$value, $next, $status, $key, $dict, $precision",
      "DICTGETNEXTQ NULLSWAPIFNOT2", 0xf475>;

// F476 — DICTGETPREV (k D n – x0 k0 -1 or 0), similar to DICTGETNEXT, but computes the maximal key k0 
//   lexicographically smaller than k
// (key slice nbits - (value key -1) | (0))
let mayLoad = 1 in
  defm DICTGETPREV :
    I<(outs Slice:$value, Slice:$prev, I257:$status),
      (ins Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$value, Slice:$prev, I257:$status,
        (int_tvm_dictgetprev Slice:$key, Cell:$dict, I257:$precision))],
      "DICTGETPREV\t$value, $prev, $status, $key, $dict, $precision",
      "DICTGETPREV NULLSWAPIFNOT2", 0xf476>;

// F477 — DICTGETPREVEQ (k D n – x0 k0 -1 or 0), similar to DICTGETPREV, but computes the maximal key k0
//  lexicographically smaller than or equal to k
// (key slice nbits - (value key -1) | (0))
let mayLoad = 1 in
  defm DICTGETPREVQ :
    I<(outs Slice:$value, Slice:$prev, I257:$status),
      (ins Slice:$key, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$value, Slice:$prev, I257:$status,
        (int_tvm_dictgetpreveq Slice:$key, Cell:$dict, I257:$precision))],
      "DICTGETPREVQ\t$value, $prev, $status, $key, $dict, $precision",
      "DICTGETPREVQ NULLSWAPIFNOT2", 0xf477>;

// F478 — DICTIGETNEXT (i D n – x0 i0 -1 or 0), similar to DICTGETNEXT, but interprets all keys in dictionary D as big-endian signed n-bit integers, 
//   and computes the minimal key i0 that is larger than Integer i (which does not necessarily fit into n bits)
// (int slice nbits - (value key -1) | (0))
let mayLoad = 1 in
  defm DICTIGETNEXT :
    I<(outs Slice:$value, Slice:$next, I257:$status),
      (ins I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$value, Slice:$next, I257:$status,
        (int_tvm_dictigetnext I257:$i, Cell:$dict, I257:$precision))],
      "DICTIGETNEXT\t$value, $next, $status, $i, $dict, $precision",
      "DICTIGETNEXT NULLSWAPIFNOT2", 0xf478>;

// F479 — DICTIGETNEXTEQ (i D n – x0 i0 -1 or 0)
// (int slice nbits - (value key -1) | (0))
let mayLoad = 1 in
  defm DICTIGETNEXTEQ :
    I<(outs Slice:$value, Slice:$next, I257:$status),
      (ins I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$value, Slice:$next, I257:$status,
        (int_tvm_dictigetnexteq I257:$i, Cell:$dict, I257:$precision))],
      "DICTIGETNEXTEQ\t$value, $next, $status, $i, $dict, $precision",
      "DICTIGETNEXTEQ NULLSWAPIFNOT2", 0xf479>;

// F47A — DICTIGETPREV (i D n – x0 i0 -1 or 0)
// (int slice nbits - (value key -1) | (0))
let mayLoad = 1 in
  defm DICTIGETPREV :
    I<(outs Slice:$value, Slice:$prev, I257:$status),
      (ins I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$value, Slice:$prev, I257:$status,
        (int_tvm_dictigetprev I257:$i, Cell:$dict, I257:$precision))],
      "DICTIGETPREV\t$value, $prev, $status, $i, $dict, $precision",
      "DICTIGETPREV NULLSWAPIFNOT2", 0xf47a>;

// F47B — DICTIGETPREVEQ (i D n – x0 i0 -1 or 0)
// (int slice nbits - (value key -1) | (0))
let mayLoad = 1 in
  defm DICTIGETPREVEQ :
    I<(outs Slice:$value, Slice:$prev, I257:$status),
      (ins I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$value, Slice:$prev, I257:$status,
        (int_tvm_dictigetpreveq I257:$i, Cell:$dict, I257:$precision))],
      "DICTIGETPREVEQ\t$value, $prev, $status, $i, $dict, $precision",
      "DICTIGETPREVEQ NULLSWAPIFNOT2", 0xf47b>;

// F47D — DICTUGETNEXTEQ (i D n – x0 i0 -1 or 0)
// (uint slice nbits - (value key -1) | (0))
let mayLoad = 1 in
  defm DICTUGETNEXTEQ :
    I<(outs Slice:$value, Slice:$prev, I257:$status),
      (ins I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$value, Slice:$prev, I257:$status,
        (int_tvm_dictugetnexteq I257:$i, Cell:$dict, I257:$precision))],
      "DICTUGETNEXTEQ\t$value, $prev, $status, $i, $dict, $precision",
      "DICTUGETNEXTEQ NULLSWAPIFNOT2", 0xf47d>;

// F47F — DICTUGETPREVEQ (i D n – x0 i0 -1 or 0)
// (uint slice nbits - (value key -1) | (0))
let mayLoad = 1 in
  defm DICTUGETPREVEQ :
    I<(outs Slice:$value, Slice:$prev, I257:$status),
      (ins I257:$i, Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$value, Slice:$prev, I257:$status,
        (int_tvm_dictugetpreveq I257:$i, Cell:$dict, I257:$precision))],
      "DICTUGETPREVEQ\t$value, $prev, $status, $i, $dict, $precision",
      "DICTUGETPREVEQ NULLSWAPIFNOT2", 0xf47f>;


// ******************************************* 
//
// A.10.10. GetMin, GetMax, RemoveMin, RemoveMax operations
//
// *******************************************/

// F483	— DICTMINREF (D n – c k -1 or 0), similar to DICTMIN, but returns the only reference in the value as a Cell c
// (slice nbits - (cell key -1) | (0))
let mayLoad = 1 in
  defm DICTMINREF :
    I<(outs Cell:$c, Slice:$key, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$c, Slice:$key, I257:$status,
        (int_tvm_dictminref Cell:$dict, I257:$precision))],
      "DICTMINREF\t$c, $key, $status, $dict, $precision",
      "DICTMINREF NULLSWAPIFNOT2", 0xf483>;

// F484 — DICTIMIN (D n – x i -1 or 0), somewhat similar to DICTMIN, but computes the minimal key I under the assumption that all keys are big-endian signed n-bit integers. 
//   Notice that the key and value returned may differ from those computed by DICTMIN and DICTUMIN
// (slice nbits - (value int -1) | (0))
let mayLoad = 1 in
  defm DICTIMIN :
    I<(outs Slice:$x, I257:$i, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$x, I257:$i, I257:$status,
        (int_tvm_dictimin Cell:$dict, I257:$precision))],
      "DICTIMIN\t$x, $i, $status, $dict, $precision",
      "DICTIMIN NULLSWAPIFNOT2", 0xf484>;

// F485 — DICTIMINREF (D n – c i -1 or 0)
// (slice nbits - (cell int -1) | (0))
let mayLoad = 1 in
  defm DICTIMINREF :
    I<(outs Cell:$c, I257:$i, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$c, I257:$i, I257:$status,
        (int_tvm_dictiminref Cell:$dict, I257:$precision))],
      "DICTIMINREF\t$c, $i, $status, $dict, $precision",
      "DICTIMINREF NULLSWAPIFNOT2", 0xf485>;

// F48A — DICTMAX (D n – x k -1 or 0), computes the maximal key k (represented by a Slice with n data bits) 
//   in dictionary D, and returns k along with the associated value x
// (slice nbits - (value key -1) | (0))
let mayLoad = 1 in
  defm DICTMAX :
    I<(outs Slice:$x, Slice:$key, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$x, Slice:$key, I257:$status,
        (int_tvm_dictmax Cell:$dict, I257:$precision))],
      "DICTMAX\t$x, $key, $status, $dict, $precision",
      "DICTMAX NULLSWAPIFNOT2", 0xf48a>;

// F48B — DICTMAXREF (D n – c k -1 or 0)
// (slice nbits - (cell key -1) | (0))
let mayLoad = 1 in
  defm DICTMAXREF :
    I<(outs Cell:$c, Slice:$key, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$c, Slice:$key, I257:$status,
        (int_tvm_dictmaxref Cell:$dict, I257:$precision))],
      "DICTMAXREF\t$c, $key, $status, $dict, $precision",
      "DICTMAXREF NULLSWAPIFNOT2", 0xf48b>;

// F48C — DICTIMAX (D n – x i -1 or 0)
// (slice nbits - (value int -1) | (0))
let mayLoad = 1 in
  defm DICTIMAX :
    I<(outs Slice:$x, I257:$i, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Slice:$x, I257:$i, I257:$status,
        (int_tvm_dictimax Cell:$dict, I257:$precision))],
      "DICTIMAX\t$x, $i, $status, $dict, $precision",
      "DICTIMAX NULLSWAPIFNOT2", 0xf48c>;

// F48D — DICTIMAXREF	(D n – c i -1 or 0)
// (slice nbits - (cell int -1) | (0))
let mayLoad = 1 in
  defm DICTIMAXREF :
    I<(outs Cell:$c, I257:$i, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$c, I257:$i, I257:$status,
        (int_tvm_dictimaxref Cell:$dict, I257:$precision))],
      "DICTIMAXREF\t$c, $i, $status, $dict, $precision",
      "DICTIMAXREF NULLSWAPIFNOT2", 0xf48d>;

// F492 — DICTREMMIN (D n – D0 x k -1 or D 0), computes the minimal key k (represented by a Slice with n data bits) in dictionary D, 
//   removes k from the dictionary, and returns k along with the associated value x and the modified dictionary D0
// (slice nbits - (slice value key -1) | (0))
let mayLoad = 1 in
  defm DICTREMMIN :
    I<(outs Cell:$odict, Slice:$x, Slice:$key, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$odict, Slice:$x, Slice:$key, I257:$status,
        (int_tvm_dictremmin Cell:$dict, I257:$precision))],
      "DICTREMMIN\t$odict, $x, $key, $status, $dict, $precision",
      "DICTREMMIN NULLSWAPIFNOT2", 0xf492>;

// F493 — DICTREMMINREF (D n – D0 c k -1 or D 0), similar to DICTREMMIN,
//  but returns the only reference in the value as a Cell c
// (slice nbits - (slice cell key -1) | (0))
let mayLoad = 1 in
  defm DICTREMMINREF :
    I<(outs Cell:$odict, Cell:$c, Slice:$key, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$odict, Cell:$c, Slice:$key, I257:$status,
        (int_tvm_dictremminref Cell:$dict, I257:$precision))],
      "DICTREMMINREF\t$odict, $c, $key, $status, $dict, $precision",
      "DICTREMMINREF NULLSWAPIFNOT2", 0xf493>;

// F494	— DICTIREMMIN	(D n – D0 x i -1 or D 0), somewhat similar to DICTREMMIN, but computes the minimal key i under the assumption 
//  that all keys are big-endian signed n-bit integers. Notice that the key and value returned may differ from those computed by DICTREMMIN and
//  DICTUREMMIN
// (slice nbits - (slice cell key -1) | (0))
let mayLoad = 1 in
  defm DICITREMMIN :
    I<(outs Cell:$odict, Slice:$x, I257:$i, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$odict, Slice:$x, I257:$i, I257:$status,
        (int_tvm_dictiremmin Cell:$dict, I257:$precision))],
      "DICTIREMMIN\t$odict, $x, $i, $status, $dict, $precision",
      "DICTIREMMIN NULLSWAPIFNOT2", 0xf494>;

// F495 — DICTIREMMINREF (D n – D0 c i -1 or D 0)
// (slice nbits - (slice cell int -1) | (0))
let mayLoad = 1 in
  defm DICITREMMINREF :
    I<(outs Cell:$odict, Cell:$c, I257:$i, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$odict, Cell:$c, I257:$i, I257:$status,
        (int_tvm_dictiremminref Cell:$dict, I257:$precision))],
      "DICTIREMMINREF\t$odict, $c, $i, $status, $dict, $precision",
      "DICTIREMMINREF NULLSWAPIFNOT2", 0xf495>;

// F49A — DICTREMMAX (D n – D0 x k -1 or D 0), computes the maximal key k (represented by a Slice with n data bits) in dictionary D, 
//   removes k from the dictionary, and returns k along with the associated value x and the modified dictionary D0
// (slice nbits - (slice value key -1) | (0))
let mayLoad = 1 in
  defm DICTREMMAX :
    I<(outs Cell:$odict, Slice:$x, Slice:$key, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$odict, Slice:$x, Slice:$key, I257:$status,
        (int_tvm_dictremmax Cell:$dict, I257:$precision))],
      "DICTREMMAX\t$odict, $x, $key, $status, $dict, $precision",
      "DICTREMMAX NULLSWAPIFNOT2", 0xf49a>;

// F49B — DICTREMMAXREF  (D n – D0 c k -1 or D 0)
// (slice nbits - (slice cell key -1) | (0))
let mayLoad = 1 in
  defm DICTREMMAXREF :
    I<(outs Cell:$odict, Cell:$c, Slice:$key, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$odict, Cell:$c, Slice:$key, I257:$status,
        (int_tvm_dictremmaxref Cell:$dict, I257:$precision))],
      "DICTREMMAXREF\t$odict, $c, $key, $status, $dict, $precision",
      "DICTREMMAXREF NULLSWAPIFNOT2", 0xf49b>;

// F49C — DICTIREMMAX	(D n – D0 x i -1 or D 0)
// (slice nbits - (slice value int -1) | (0))
let mayLoad = 1 in
  defm DICITREMMAX :
    I<(outs Cell:$odict, Slice:$x, I257:$i, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$odict, Slice:$x, I257:$i, I257:$status,
        (int_tvm_dictiremmax Cell:$dict, I257:$precision))],
      "DICTIREMMAX\t$odict, $x, $i, $status, $dict, $precision",
      "DICTIREMMAX NULLSWAPIFNOT2", 0xf49c>;

// F49D — DICTIREMMAXREF (D n – D0 c i -1 or D 0)
// (slice nbits - (slice cell int -1) | (0))
let mayLoad = 1 in
  defm DICITREMMAXREF :
    I<(outs Cell:$odict, Cell:$c, I257:$i, I257:$status),
      (ins Cell:$dict, I257:$precision), (outs), (ins),
      [(set Cell:$odict, Cell:$c, I257:$i, I257:$status,
        (int_tvm_dictiremmaxref Cell:$dict, I257:$precision))],
      "DICTIREMMAXREF\t$odict, $c, $i, $status, $dict, $precision",
      "DICTIREMMAXREF NULLSWAPIFNOT2", 0xf49d>;


// ******************************************* 
//
// A.10.12. SubDictdictionary operations
//
// *******************************************/

// F4B1 — SUBDICTGET (k l D n – D0), constructs a subdictionary consisting of all keys beginning with prefix k 
//  (represented by a Slice, the first 0 <= l <= n <= 1023 data bits of which are used as a key) of length l in dictionary D of type HashmapE(n, X) 
//  with n-bit keys. On success, returns the new subdictionary of the same type HashmapE(n, X) as a Slice D0
// (prefix lbits dict nbits - dict)
  defm SUBDICTGET :
    I<(outs Cell:$odict),
      (ins Slice:$prefix, I257:$lbits, Cell:$idict, I257:$precision), (outs), (ins),
      [(set Cell:$odict,
        (int_tvm_subdictget Slice:$prefix, I257:$lbits, Cell:$idict, I257:$precision))],
      "SUBDICTGET\t$odict, $prefix, $lbits, $idict, $precision",
      "SUBDICTGET", 0xf4b1>;

// F4B2	— SUBDICTIGET (x l D n – D0), variant of SUBDICTGET with the prefix represented by a signed big-endian l-bit Integer x, where necessarily l <= 257
// (prefix lbits dict nbits - dict')
  defm SUBDICTIGET :
    I<(outs Cell:$odict),
      (ins I257:$prefix, I257:$lbits, Cell:$idict, I257:$precision), (outs), (ins),
      [(set Cell:$odict,
        (int_tvm_subdictiget I257:$prefix, I257:$lbits, Cell:$idict, I257:$precision))],
      "SUBDICTIGET\t$odict, $prefix, $lbits, $idict, $precision",
      "SUBDICTIGET", 0xf4b2>;

// F4B3 — SUBDICTUGET (x l D n – D0), variant of SUBDICTGET with the prefix represented by an unsigned big-endian l-bit	Integer	x, where necessarily l <= 256
// (prefix lbits dict nbits - dict')
  defm SUBDICTUGET :
    I<(outs Cell:$odict),
      (ins I257:$prefix, I257:$lbits, Cell:$idict, I257:$precision), (outs), (ins),
      [(set Cell:$odict,
        (int_tvm_subdictuget I257:$prefix, I257:$lbits, Cell:$idict, I257:$precision))],
      "SUBDICTUGET\t$odict, $prefix, $lbits, $idict, $precision",
      "SUBDICTUGET", 0xf4b3>;

// F4B5	— SUBDICTRPGET (k l D n – D0), similar to SUBDICTGET, but removes the common prefix	k from all keys of the new dictionary D0, 
//   which becomes of type HashmapE(n - l, X)
// (prefix lbits dict nbits - dict')
  defm SUBDICTRPGET :
    I<(outs Cell:$odict),
      (ins Slice:$prefix, I257:$lbits, Cell:$idict, I257:$precision), (outs), (ins),
      [(set Cell:$odict,
        (int_tvm_subdictrpget Slice:$prefix, I257:$lbits, Cell:$idict, I257:$precision))],
      "SUBDICTRPGET\t$odict, $prefix, $lbits, $idict, $precision",
      "SUBDICTRPGET", 0xf4b5>;

// F4B6 — SUBDICTIRPGET (x l D n – D0), variant of SUBDICTRPGET with the prefix represented by a signed big-endian l-bit Integer x, 
//  where necessarily l <= 257
// (prefix lbits dict nbits - dict')
  defm SUBDICTIRPGET :
    I<(outs Cell:$odict),
      (ins I257:$prefix, I257:$lbits, Cell:$idict, I257:$precision), (outs), (ins),
      [(set Cell:$odict,
        (int_tvm_subdictirpget I257:$prefix, I257:$lbits, Cell:$idict, I257:$precision))],
      "SUBDICTIRPGET\t$odict, $prefix, $lbits, $idict, $precision",
      "SUBDICTIRPGET", 0xf4b6>;

// F4B7 — SUBDICTURPGET (x l D n – D0), variant of SUBDICTRPGET with the prefix represented by an unsigned big-endian l-bit Integer x, 
//   where necessarily l <= 256
// (prefix lbits dict nbits - dict')
  defm SUBDICTURPGET :
    I<(outs Cell:$odict),
      (ins I257:$prefix, I257:$lbits, Cell:$idict, I257:$precision), (outs), (ins),
      [(set Cell:$odict,
        (int_tvm_subdicturpget I257:$prefix, I257:$lbits, Cell:$idict, I257:$precision))],
      "SUBDICTURPGET\t$odict, $prefix, $lbits, $idict, $precision",
      "SUBDICTURPGET", 0xf4b7>;
