// RUN: %clang -std=c++17 --target=tvm -S -O3 %s -o -
// CHECK-NOT: call {{ }}

// ******************************************* 
//
// A.10	Dictionary manipulation primitives
//
// *******************************************/

// ******************************************* 
//
// A.10.2. Dictionary serialization and deserialization
//
// *******************************************/
/*
__tvm_slice lddict_test(__tvm_slice s)
{
  auto [c1, s1] = __builtin_tvm_lddict(s);
  return s1;
}
*/

// SKIPDICT or SKIPOPTREF (s – s0), equivalent to LDDICT; NIP
__tvm_slice skipdict_test(__tvm_slice s)
{
  return __builtin_tvm_skipdict(s);
}

// LDDICTS (s – s0 s00), loads (parses) a (Slice-represented) dictionary s0 from Slice s, and returns the remainder of s as s00
__tvm_slice lddicts_test(__tvm_slice s)
{
  auto [s1, s2] = __builtin_tvm_lddicts(s);
  return s2;
}

// PLDDICTS (s – s0), preloads a (Slice - represented) dictionary s0 from Slice s. Approximately equivalent to LDDICTS; DROP
__tvm_slice plddicts_test(__tvm_slice s)
{
  return __builtin_tvm_plddicts(s);
}

// LDDICTQ (s – D s0 -1 or s 0), a quiet version of LDDICT
int lddictq_test(__tvm_slice s)
{
  auto [c1, s1, result] = __builtin_tvm_lddictq(s);
  return result;
}

// PLDDICTQ (s – D -1 or 0), a quiet version of PLDDICT
int plddictq_test(__tvm_slice s)
{
  auto [c1, result] = __builtin_tvm_plddictq(s);
  return result;
}

// ******************************************* 
//
// A.10.4.	Set/Replace/Add dictionary operations
//
// *******************************************/

// DICTISETGET (x i D n – D0 y -1 or D0 0), similar to DICTSETGET, 
//  but with the key represented by a big-endian signed n-bit Integer I
// (value int slice nbits - (slice y -1) | (slice 0))
int dictisetget_test(__tvm_slice x, int i, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictisetget(x, i, d, n);
  return result;
}

// DICTISETGETREF (c i D n – D0 c0 -1 or D0 0), a version of DICTSETGETREF with signed Integer i as a key
// (cell int slice nbits - (slice cell -1) | (slice 0))
int dictisetgetref_test(__tvm_cell c, int i, __tvm_cell d, int n)
{
  auto [c1, c2, result] = __builtin_tvm_dictisetgetref(c, i, d, n);
  return result;
}

// DICTREPLACEGET (x k D n – D0 y -1 or D 0), a Replace counterpart of DICTSETGET: 
//  on success, also returns the old value associated with the key in question
// (value int slice nbits - (slice value -1) | (slice 0))
int dictreplaceget_test(__tvm_slice x, __tvm_slice k, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictreplaceget(x, k, d, n);
  return result;
}

// DICTREPLACEGETREF (c k D n – D0 c0 -1 or D 0)
// (cell int slice nbits - (slice' cell -1) | (slice 0))
int dictreplacegetref_test(__tvm_cell c, __tvm_slice k, __tvm_cell d, int n)
{
  auto [c1, c2, result] = __builtin_tvm_dictreplacegetref(c, k, d, n);
  return result;
}

// DICTIREPLACEGET  (x i D n – D0 y -1 or D 0)
// (value int slice nbits - (slice value -1) | (slice 0))
int dictireplaceget_test(__tvm_slice x, int i, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictireplaceget(x, i, d, n);
  return result;
}

// DICTIREPLACEGETREF (c i D n – D0 c0 -1 or D 0)
// (cell int slice nbits - (slice' cell -1) | (slice 0))
int dictireplacegetref_test(__tvm_cell c, int i, __tvm_cell d, int n)
{
  auto [c1, c2, result] = __builtin_tvm_dictireplacegetref(c, i, d, n);
  return result;
}

// DICTUREPLACEGET (x i D n – D0 y -1 or D 0)
// (value uint slice nbits - (slice value -1) | (slice 0))
int dictureplaceget_test(__tvm_slice x, int i, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictureplaceget(x, i, d, n);
  return result;
}

// DICTUREPLACEGETREF (c i D n – D0 c0 -1 or D 0)
// (cell uint slice nbits - (slice' cell -1) | (slice 0))
int dictureplacegetref_test(__tvm_cell c, int i, __tvm_cell d, int n)
{
  auto [c1, c2, result] = __builtin_tvm_dictureplacegetref(c, i, d, n);
  return result;
}

// DICTADD (x k D n – D0 -1 or D 0), an Add counterpart of DICTSET: sets the value associated with key k in dictionary D to x, but
//   only if it is not already present in D
// (value key slice nbits - (slice -1) | (slice 0))
int dictadd_test(__tvm_slice x, __tvm_slice k, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_dictadd(x, k, d, n);
  return result;
}

// DICTADDREF (c k D n – D0 -1 or D 0)
// (value key slice nbits - (slice -1) | (slice y 0))
int dictaddref_test(__tvm_cell c, __tvm_slice k, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_dictaddref(c, k, d, n);
  return result;
}

// DICTIADD (x i D n – D0 -1 or D 0)
// (value int slice nbits - (slice -1) | (slice 0))
int dictiadd_test(__tvm_slice x, int i, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_dictiadd(x, i, d, n);
  return result;
}

// DICTIADDREF (c i D n – D0 -1 or D 0)
// (cell int slice nbits - (slice -1) | (slice 0))
int dictiaddref_test(__tvm_cell c, int i, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_dictiaddref(c, i, d, n);
  return result;
}

// DICTUADD  (x i D n – D0 -1 or D 0)
// (value uint slice nbits - (slice -1) | (slice 0))
int dictuadd_test(__tvm_slice x, int i, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_dictuadd(x, i, d, n);
  return result;
}

// DICTUADDREF (c i D n – D0 -1 or D 0)
// (cell uint slice nbits - (slice -1) | (slice 0))
int dictuaddref_test(__tvm_cell c, int i, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_dictuaddref(c, i, d, n);
  return result;
}

// DICTADDGET (x k D n – D0 -1 or D y 0), an Add counterpart of DICTSETGET: sets the value associated with key k in dictionary D to x, 
//   but only if key k is not already present in D. Otherwise, just returns the old value y without changing the dictionary
// (value key slice nbits - (slice -1) | (slice y 0))
int dictaddget_test(__tvm_slice x, __tvm_slice k, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictaddget(x, k, d, n);
  return result;
}

// DICTADDGETREF (c k D n – D0 -1 or D c0 0), an Add counterpart of DICTSETGETREF
// (cell key slice nbits - (slice' -1) | (slice cell 0))
int dictaddgetref_test(__tvm_cell c, __tvm_slice k, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictaddgetref(c, k, d, n);
  return result;
}

// DICTIADDGET (x i D n – D0 -1 or D y 0)
// (value int slice nbits - (slice -1) | (slice value 0))
int dictiaddget_test(__tvm_slice x, int i, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictiaddget(x, i, d, n);
  return result;
}

// DICTIADDGETREF (c i D n – D0 -1 or D c0 0)
// (cell int slice nbits - (slice -1) | (slice cell 0))
int dictiaddgetref_test(__tvm_cell c, int i, __tvm_cell d, int n)
{
  auto [c1, c2, result] = __builtin_tvm_dictiaddgetref(c, i, d, n);
  return result;
}

// DICTUADDGET	(x i D n – D0 -1 or D y 0)
// (value uint slice nbits - (slice -1) | (slice value 0))
int dictuaddget_test(__tvm_slice x, int i, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictuaddget(x, i, d, n);
  return result;
}

// DICTUADDGETREF (c i D n – D0 -1 or D c0 0)
// (cell uint slice nbits - (slice -1) | (slice cell 0))
int dictuaddgetref_test(__tvm_cell c, int i, __tvm_cell d, int n)
{
  auto [c1, c2, result] = __builtin_tvm_dictuaddgetref(c, i, d, n);
  return result;
}


// ******************************************* 
//
// A.10.5. Builder-accepting variants of Set dictionary operations.
//
// *******************************************/

// DICTSETB (b k D n – D0)
// (builder key slice nbits - slice)
__tvm_cell dictsetb_test(__tvm_builder b, __tvm_slice k, __tvm_cell d, int n)
{
  return __builtin_tvm_dictsetb(b, k, d, n);
}

// DICTISETB (b i D n – D0)
// (builder int slice nbits - slice)
__tvm_cell dictisetb_test(__tvm_builder b, int i, __tvm_cell d, int n)
{
  return __builtin_tvm_dictisetb(b, i, d, n);
}

// DICTUSETB (b i D n – D0)
// (builder uint slice nbits - slice)
__tvm_cell dictusetb_test(__tvm_builder b, int i, __tvm_cell d, int n)
{
  return __builtin_tvm_dictusetb(b, i, d, n);
}

// DICTSETGETB (b k D n – D0 y -1 or D0 0)
// (builder key slice nbits - (slice value -1) | (slice 0))

int dictsetgetb_test(__tvm_builder b, __tvm_slice k, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictsetgetb(b, k, d, n);
  return result;
}

// DICTISETGETB (b i D n – D0 y -1 or D0 0)
// (builder int slice nbits - (slice' y -1) | (slice' 0))
int dictisetgetb_test(__tvm_builder b, int i, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictisetgetb(b, i, d, n);
  return result;
}

// DICTUSETGETB (b i D n – D0 y -1 or D0 0)
// (builder uint slice nbits - (slice' y -1) | (slice' 0))
int dictusetgetb_test(__tvm_builder b, int i, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictusetgetb(b, i, d, n);
  return result;
}

// DICTREPLACEB (b k D n – D0 -1 or D 0)
// (builder int slice nbits - (slice -1) | (slice 0))
int dictreplaceb_test(__tvm_builder b, __tvm_slice k, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_dictreplaceb(b, k, d, n);
  return result;
}

// DICTIREPLACEB (b i D n – D0 -1 or D 0)
// (builder uint slice nbits - (slice -1) | (slice 0))
int dictireplaceb_test(__tvm_builder b, int  i, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_dictireplaceb(b, i, d, n);
  return result;
}

// DICTUREPLACEB (b i D n – D0 -1 or D 0)
// (builder uint slice nbits - (slice -1) | (slice 0))
int dictureplaceb_test(__tvm_builder b, int  i, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_dictureplaceb(b, i, d, n);
  return result;
}

// DICTREPLACEGETB (b k D n – D0 y -1 or D 0)
// (builder key slice nbits - (slice value -1) | (slice 0))
int dictreplacegetb_test(__tvm_builder b, __tvm_slice k, __tvm_cell d, int n)
{
	auto[c1, s1, result] = __builtin_tvm_dictreplacegetb(b, k, d, n);
	return result;
}

// DICTIREPLACEGETB (b i D n – D0 y -1 or D 0)
// (builder int slice nbits - (slice value -1) | (slice 0))
int dictireplacegetb_test(__tvm_builder b, int i, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictireplacegetb(b, i, d, n);
  return result;
}

// DICTUREPLACEGETB	(b i D n – D0 y -1 or D 0)
// (builder uint slice nbits - (slice value -1) | (slice 0))
int dictureplacegetb_test(__tvm_builder b, int i, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictureplacegetb(b, i, d, n);
  return result;
}

// DICTADDB (b k D n – D0 -1 or D 0)
// (builder key slice nbits - (slice -1) | (slice 0))
int dictaddb_test(__tvm_builder b, __tvm_slice k, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_dictaddb(b, k, d, n);
  return result;
}

// DICTIADDB (b i D n – D0 -1 or D 0)
// (builder int slice nbits - (slice -1) | (slice 0))
int dictiaddb_test(__tvm_builder b, int i, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_dictiaddb(b, i, d, n);
  return result;
}

// DICTUADDB  (b i D n – D0 -1 or D 0)
// (builder uint slice nbits - (slice -1) | (slice 0))
int dictuaddb_test(__tvm_builder b, int i, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_dictuaddb(b, i, d, n);
  return result;
}

// DICTADDGETB (b k D n – D0 -1 or D y 0)
// (builder key slice nbits - (slice -1) | (slice value 0))
int dictaddgetb_test(__tvm_builder b, __tvm_slice k, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictaddgetb(b, k, d, n);
  return result;
}

// DICTIADDGETB (b i D n – D0 -1 or D y 0)
// (builder int slice nbits - (slice' -1) | (slice y 0))
int dictiaddgetb_test(__tvm_builder b, int i, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictiaddgetb(b, i, d, n);
  return result;
}

// DICTUADDGETB	(b i D n – D0 -1 or D y 0)
// (builder uint slice nbits - (slice' -1) | (slice y 0))
int dictuaddgetb_test(__tvm_builder b, int i, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictuaddgetb(b, i, d, n);
  return result;
}


// ******************************************* 
//
// A.10.6. Delete dictionary operations
//
// *******************************************/

// DICTIDEL (i D n – D0 ?), a version of DICTDEL with the key represented by a signed n-bit Integer i. If i does not fit into n bits,
//   simply returns D 0 (“key not found, dictionary unmodified”)
// (int slice nbits - (slice' -1) | (slice 0))
int dictidel_test(int i, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_dictidel(i, d, n);
  return result;
}

// DICTIDELGET (i D n – D0 x -1 or D 0), a variant of primitive DICTDELGET with signed n-bit integer i as a key
// (int slice nbits - (slice value -1) | (slice 0))
int dictidelget_test(int i, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictidelget(i, d, n);
  return result;
}

// DICTIDELGETREF (i D n – D0 c -1 or  D 0), a variant of primitive DICTIDELGET returning a Cell instead of a Slice
// (int slice nbits - (slice cell -1) | (slice 0))
int dictidelgetref_test(int i, __tvm_cell d, int n)
{
  auto [c1, c2, result] = __builtin_tvm_dictidelgetref(i, d, n);
  return result;
}

// DICTUDELGET (i D n – D0x -1 or D 0), a variant of primitive DICTDELGET with unsigned n-bit integer i as a key
// (uint slice nbits - (slice value -1) | (slice 0))
int dictudelget_test(int i, __tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictudelget(i, d, n);
  return result;
}

// DICTUDELGETREF (i D n – D0 c -1 or D 0), a variant of primitive DICTUDELGET returning a Cell instead of a Slice
// (uint slice nbits - (slice cell -1) | (slice 0))
int dictudelgetref_test(int i, __tvm_cell d, int n)
{
  auto [c1, c2, result] = __builtin_tvm_dictudelgetref(i, d, n);
  return result;
}


// ******************************************* 
//
// A.10.7. “Maybe reference” dictionary operations
//
// *******************************************/

// DICTGETOPTREF (k D n – c?), a variant of DICTGETREF that returns Null	
//   instead of the value c? if the key k is absent from dictionary D
__tvm_cell dictgetoptref_test(__tvm_slice k, __tvm_cell d, int n)
{
  return __builtin_tvm_dictgetoptref(k, d, n);
}

// DICTIGETOPTREF (i D n – c?), similar to DICTGETOPTREF, but also returns Null
__tvm_cell dictigetoptref_test(int i, __tvm_cell d, int n)
{
  return __builtin_tvm_dictigetoptref(i, d, n);
}

// DICTUGETOPTREF (i D n – c?), similar to DICTGETOPTREF, but with the key given by unsigned n-bit Integer
__tvm_cell _dictugetoptref_test(int i, __tvm_cell d, int n)
{
  return __builtin_tvm_dictugetoptref(i, d, n);
}

// DICTSETGETOPTREF	(c? k D n – D0 ~?), a variant of both DICTGETOPTREF and DICTSETGETREF that sets the value corresponding to key k 
//   in dictionary D to c? (if c? Is Null, then the key is deleted instead), and returns the old value ~? (if the key k was absent before, returns Null instead)
__tvm_cell dictsetgetoptref_test(__tvm_cell c, __tvm_slice k, __tvm_cell d, int n)
{
  auto [c1, c2] = __builtin_tvm_dictsetgetoptref(c, k, d, n);
  return c2;
}

// DICTISETGETOPTREF (c? i D n – D0 ~?), similar to primitive DICTSETGETOPTREF, but using signed n-bit Integer i as a key. 
//   If i does not fit into n bits, throws a range checking exception
__tvm_cell dictisetgetoptref_test(__tvm_cell c, int i, __tvm_cell d, int n)
{
  auto [c1, c2] = __builtin_tvm_dictisetgetoptref(c, i, d, n);
  return c2;
}

// DICTUSETGETOPTREF (c? i D n – D0 ~?), similar to primitive DICTSETGETOPTREF, but using unsigned n-bit Integer i as a key
__tvm_cell dictusetgetoptref_test(__tvm_cell c, int i, __tvm_cell d, int n)
{
  auto [c1, c2] = __builtin_tvm_dictusetgetoptref(c, i, d, n);
  return c2;
}

// ******************************************* 
//
// A.10.8. Prefix code dictionary operations
//
// *******************************************/

// PFXDICTSET (x k D n – D0 -1 or D 0)
// (value key slice nbits - slice -1|0)
int pfxdictset_test(__tvm_slice x, __tvm_slice k, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_pfxdictset(x, k, d, n);
  return result;
}

// PFXDICTREPLACE (x k D n – D0 -1 or D 0)
// (value key slice nbits - slice -1|0)
int pfxdictreplace_test(__tvm_slice x, __tvm_slice k, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_pfxdictreplace(x, k, d, n);
  return result;
}

// PFXDICTADD (x k D n – D0 -1 or D 0)
// (value key slice nbits - slice -1|0)
int pfxdictadd_test(__tvm_slice x, __tvm_slice k, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_pfxdictadd(x, k, d, n);
  return result;
}

// PFXDICTDEL (k D n – D0 -1 or D 0)
// (key slice nbits - slice -1|0)
int pfxdictdel_test(__tvm_slice k, __tvm_cell d, int n)
{
  auto [c1, result] = __builtin_tvm_pfxdictdel(k, d, n);
  return result;
}

// ******************************************* 
//
// A.10.9. Variants of GetNext and GetPrev operations
//
// *******************************************/

// DICTGETNEXTEQ (k D n – x0 k0 -1 or 0), similar to DICTGETNEXT, but computes the minimal key k0 
//  that is lexicographically greater than or equal to k
// (key slice nbits - (value key -1) | (0))
int dictgetnexteq_test(__tvm_slice k, __tvm_cell d, int n)
{
  auto [s1, s2, result] = __builtin_tvm_dictgetnexteq(k, d, n);
  return result;
}

// DICTGETPREV (k D n – x0 k0 -1 or 0), similar to DICTGETNEXT, but computes the maximal key k0 
//   lexicographically smaller than k
// (key slice nbits - (value key -1) | (0))
int dictgetprev_test(__tvm_slice k, __tvm_cell d, int n)
{
  auto [s1, s2, result] = __builtin_tvm_dictgetprev(k, d, n);
  return result;
}

// DICTGETPREVEQ (k D n – x0 k0 -1 or 0), similar to DICTGETPREV, but computes the maximal key k0
//  lexicographically smaller than or equal to k
// (key slice nbits - (value key -1) | (0))
int dictgetpreveq_test(__tvm_slice k, __tvm_cell d, int n)
{
  auto [s1, s2, result] = __builtin_tvm_dictgetpreveq(k, d, n);
  return result;
}

// DICTIGETNEXT (i D n – x0 i0 -1 or 0), similar to DICTGETNEXT, but interprets all keys in dictionary D as big-endian signed n-bit integers, 
//   and computes the minimal key i0 that is larger than Integer i (which does not necessarily fit into n bits)
// (int slice nbits - (value key -1) | (0))
int dictigetnext_test(int i, __tvm_cell d, int n)
{
  auto [s1, s2, result] = __builtin_tvm_dictigetnext(i, d, n);
  return result;
}

// DICTIGETNEXTEQ (i D n – x0 i0 -1 or 0)
// (int slice nbits - (value key -1) | (0))
int dictigetnexteq_test(int i, __tvm_cell d, int n)
{
  auto [s1, s2, result] = __builtin_tvm_dictigetnexteq(i, d, n);
  return result;
}

// DICTIGETPREV (i D n – x0 i0 -1 or 0)
// (int slice nbits - (value key -1) | (0))
int dictigetprev_test(int i, __tvm_cell d, int n)
{
  auto [s1, s2, result] = __builtin_tvm_dictigetprev(i, d, n);
  return result;
}

// DICTIGETPREVEQ (i D n – x0 i0 -1 or 0)
// (int slice nbits - (value key -1) | (0))
int dictigetpreveq_test(int i, __tvm_cell d, int n)
{
  auto [s1, s2, result] = __builtin_tvm_dictigetpreveq(i, d, n);
  return result;
}

// DICTUGETNEXTEQ (i D n – x0 i0 -1 or 0)
// (uint slice nbits - (value key -1) | (0))
int dictugetnexteq_test(int i, __tvm_cell d, int n)
{
  auto [s1, s2, result] = __builtin_tvm_dictugetnexteq(i, d, n);
  return result;
}

// DICTUGETPREVEQ (i D n – x0 i0 -1 or 0)
// (uint slice nbits - (value key -1) | (0))
int dictugetpreveq_test(int i, __tvm_cell d, int n)
{
  auto [s1, s2, result] = __builtin_tvm_dictugetpreveq(i, d, n);
  return result;
}


// ******************************************* 
//
// A.10.10. GetMin, GetMax, RemoveMin, RemoveMax operations
//
// *******************************************/

// DICTMINREF (D n – c k -1 or 0), similar to DICTMIN, but returns the only reference in the value as a Cell c
// (slice nbits - (cell key -1) | (0))
int dictminref_test(__tvm_cell d, int n)
{
  auto [c1, s1, result] = __builtin_tvm_dictminref(d, n);
  return result;
}

// DICTIMIN (D n – x i -1 or 0), somewhat similar to DICTMIN, but computes the minimal key I under the assumption that all keys are big-endian signed n-bit integers. 
//   Notice that the key and value returned may differ from those computed by DICTMIN and DICTUMIN
// (slice nbits - (value int -1) | (0))
int dictimin_test(__tvm_cell d, int n)
{
  auto [s1, i1, result] = __builtin_tvm_dictimin(d, n);
  return result;
}

// DICTIMINREF (D n – c i -1 or 0)
// (slice nbits - (cell int -1) | (0))
int dictiminref_test(__tvm_cell d, int n)
{
  auto [c1, i1, result] = __builtin_tvm_dictiminref(d, n);
  return result;
}

// DICTMAX (D n – x k -1 or 0), computes the maximal key k (represented by a Slice with n data bits) 
//   in dictionary D, and returns k along with the associated value x
// (slice nbits - (value key -1) | (0))
int dictmax_test(__tvm_cell d, int n)
{
  auto [s1, s2, result] = __builtin_tvm_dictmax(d, n);
  return result;
}

// DICTMAXREF (D n – c k -1 or 0)
// (slice nbits - (cell key -1) | (0))
int dictmaxref_test(__tvm_cell d, int n)
{
  auto [c1, s2, result] = __builtin_tvm_dictmaxref(d, n);
  return result;
}

// DICTIMAX (D n – x i -1 or 0)
// (slice nbits - (value int -1) | (0))
int dictimax_test(__tvm_cell d, int n)
{
  auto [s1, i1, result] = __builtin_tvm_dictimax(d, n);
  return result;
}

// DICTIMAXREF	(D n – c i -1 or 0)
// (slice nbits - (cell int -1) | (0))
int dictimaxref_test(__tvm_cell d, int n)
{
  auto [c1, i1, result] = __builtin_tvm_dictimaxref(d, n);
  return result;
}

// DICTREMMIN (D n – D0 x k -1 or D 0), computes the minimal key k (represented by a Slice with n data bits) in dictionary D, 
//   removes k from the dictionary, and returns k along with the associated value x and the modified dictionary D0
// (slice nbits - (slice value key -1) | (0))
int dictremmin_test(__tvm_cell d, int n)
{
  auto [c1, s1, s2, result] = __builtin_tvm_dictremmin(d, n);
  return result;
}

//  DICTREMMINREF (D n – D0 c k -1 or D 0), similar to DICTREMMIN,
//  but returns the only reference in the value as a Cell c
// (slice nbits - (slice cell key -1) | (0))
int dictremminref_test(__tvm_cell d, int n)
{
  auto [c1, c2, s1, result] = __builtin_tvm_dictremminref(d, n);
  return result;
}

// DICTIREMMIN	(D n – D0 x i -1 or D 0), somewhat similar to DICTREMMIN, but computes the minimal key i under the assumption 
//  that all keys are big-endian signed n-bit integers. Notice that the key and value returned may differ from those computed by DICTREMMIN and
//  DICTUREMMIN
// (slice nbits - (slice cell key -1) | (0))
int dictiremmin_test(__tvm_cell d, int n)
{
  auto [c1, s1, i1, result] = __builtin_tvm_dictiremmin(d, n);
  return result;
}

// DICTIREMMINREF (D n – D0 c i -1 or D 0)
// (slice nbits - (slice cell int -1) | (0))
int dictiremminref_test(__tvm_cell d, int n)
{
  auto [c1, c2, i1, result] = __builtin_tvm_dictiremminref(d, n);
  return result;
}

// DICTREMMAX (D n – D0 x k -1 or D 0), computes the maximal key k (represented by a Slice with n data bits) in dictionary D, 
//   removes k from the dictionary, and returns k along with the associated value x and the modified dictionary D0
// (slice nbits - (slice value key -1) | (0))
int dictremmax_test(__tvm_cell d, int n)
{
  auto [c1, s1, s2, result] = __builtin_tvm_dictremmax(d, n);
  return result;
}

// DICTREMMAXREF (D n – D0c k -1 or D 0)
// (slice nbits - (slice cell key -1) | (0))
int dictremmaxref_test(__tvm_cell d, int n)
{
	auto[c1, c2, s1, result] = __builtin_tvm_dictremminref(d, n);
	return result;
}

// DICTIREMMAX (D n – D0x i -1 or D 0)
// (slice nbits - (slice value int -1) | (0))
int dictiremmax_test(__tvm_cell d, int n)
{
  auto [c1, s1, i1, result] = __builtin_tvm_dictiremmax(d, n);
  return result;
}

// DICTIREMMAXREF (D n – D0c i -1 or D 0)
// (slice nbits - (slice cell int -1) | (0))
int dictiremmaxref_test(__tvm_cell d, int n)
{
  auto [c1, c2, i1, result] = __builtin_tvm_dictiremmaxref(d, n);
  return result;
}

// ******************************************* 
//
// A.10.12. SubDictdictionary operations
//
// *******************************************/

// SUBDICTGET (k l D n – D0), constructs a subdictionary consisting of all keys beginning with prefix k 
//  (represented by a Slice, the first 0 <= l <= n <= 1023 data bits of which are used as a key) of length l in dictionary D of type HashmapE(n, X) 
//  with n-bit keys. On success, returns the new subdictionary of the same type HashmapE(n, X) as a Slice D0
// (prefix lbits dict nbits - dict)
__tvm_cell subdictget_test(__tvm_slice k, int l, __tvm_cell d, int n)
{
  return __builtin_tvm_subdictget(k, l, d, n);
}

// SUBDICTIGET (x l D n – D0), variant of SUBDICTGET with the prefix represented by a signed big-endian l-bit Integer x, where necessarily l <= 257
// (prefix lbits dict nbits - dict')
__tvm_cell subdictiget_test(int x, int l, __tvm_cell d, int n)
{
  return __builtin_tvm_subdictiget(x, l, d, n);
}

// SUBDICTUGET (x l D n – D0), variant of SUBDICTGET with the prefix represented by an unsigned big-endian l-bit	Integer	x, where necessarily l <= 256
// (prefix lbits dict nbits - dict')
__tvm_cell subdictuget_test(int x, int l, __tvm_cell d, int n)
{
  return __builtin_tvm_subdictuget(x, l, d, n);
}

// SUBDICTRPGET (k l D n – D0), similar to SUBDICTGET, but removes the common prefix	k from all keys of the new dictionary D0, 
//   which becomes of type HashmapE(n - l, X)
// (prefix lbits dict nbits - dict')
__tvm_cell subdictrpget_test(__tvm_slice k, int l, __tvm_cell d, int n)
{
  return __builtin_tvm_subdictrpget(k, l, d, n);
}

// SUBDICTIRPGET (x l D n – D0), variant of SUBDICTRPGET with the prefix represented by a signed big-endian l-bit Integer x, 
//  where necessarily l <= 257
// (prefix lbits dict nbits - dict')
__tvm_cell subdictirpget_test(int x, int l, __tvm_cell d, int n)
{
	return __builtin_tvm_subdictirpget(x, l, d, n);
}

// SUBDICTURPGET (x l D n – D0), variant of SUBDICTRPGET with the prefix represented by an unsigned big-endian l-bit Integer x, 
//   where necessarily l <= 256
// (prefix lbits dict nbits - dict')
__tvm_cell subdicturpget_test(int x, int l, __tvm_cell d, int n)
{
  return __builtin_tvm_subdicturpget(x, l, d, n);
}
