// RUN: %clang -std=c++17 --target=tvm -S -O3 %s -o -
// CHECK-NOT: call {{ }}

// STBREFR or ENDCST (b b00 – b), equivalent to ENDC; SWAP; STREF 
__tvm_builder endcst_test(__tvm_builder b1, __tvm_builder b2)
{
  return __builtin_tvm_endcst(b1, b2);
}

// STIR cc + 1 (b x – b0), equivalent to SWAP; STI cc + 1
__tvm_builder stir_test(__tvm_builder b1, int value)
{
  return __builtin_tvm_stir(b1, value, 7);
}

// STUR cc + 1 (b x – b0), equivalent to SWAP; STU cc + 1
__tvm_builder stur_test(__tvm_builder b1, int value)
{
  return __builtin_tvm_stur(b1, value, 8);
}


// STBREF (b0 b – b00), equivalent to SWAP; STBREFREV
__tvm_builder stbref_test(__tvm_builder b1, __tvm_builder b2)
{
  return __builtin_tvm_stbref(b1, b2);
}

// STB (b0 b – b00), appends all data from Builder b0 to Builder b
__tvm_builder stb_test(__tvm_builder b1, __tvm_builder b2)
{
  return __builtin_tvm_stb(b1, b2);
}

// STREFR (b c – b0)
__tvm_builder strefr_test(__tvm_builder b1, __tvm_cell c1)
{
  return __builtin_tvm_strefr(b1, c1);
}

// STSLICER (b s – b0)
__tvm_builder stslicer_test(__tvm_builder b1, __tvm_slice s1)
{
  return __builtin_tvm_stslicer(b1, s1);
}

// STBR (b b0 – b00), concatenates two Builder s, equivalent to SWAP; STB
__tvm_builder stbr_test(__tvm_builder b1, __tvm_builder b2)
{
  return __builtin_tvm_stbr(b1, b2);
}

// STREFQ (c b – c b -1 or b0 0)
__tvm_builder strefq_test(__tvm_cell c1, __tvm_builder b1)
{
  auto [c2, b2, result] =__builtin_tvm_strefq(c1, b1);
  return b2;
}

// STBREFQ (b0 b – b0 b -1 or b00 0)
__tvm_builder stbrefq_test(__tvm_builder b1, __tvm_builder b2)
{
  auto [b3, b4, result] = __builtin_tvm_stbrefq(b1, b2);
  return b4;
}

// STSLICEQ (s b - s b -1 or b0 0)
__tvm_builder stsliceq_test(__tvm_slice s1, __tvm_builder b1)
{
  auto [s2, b2, result] = __builtin_tvm_stsliceq(s1, b1);
  return b2;
}

// STBQ (b0 b – b0 b -1 or b00 0)
__tvm_builder stbq_test(__tvm_builder b1, __tvm_builder b2)
{
  auto [b3, b4, result] = __builtin_tvm_stbq(b1, b2);
  return b4;
}

// STBREFRQ (b b0 – b b0 -1 or b00 0)
__tvm_builder stbrefrq_test(__tvm_builder b1, __tvm_builder b2)
{
  auto [b3, b4, result] = __builtin_tvm_stbrefrq(b1, b2);
  return b4;
}

// STSLICERQ (b s – b s -1 or b00 0)
__tvm_builder stslicerq_test(__tvm_builder b1, __tvm_slice s1)
{
  auto [b2, s2, result] = __builtin_tvm_stslicerq(b1, s1);
  return b2;
}

// STBRQ (b b0 – b b0 -1 or b00 0)
__tvm_builder stbrq_test(__tvm_builder b1, __tvm_builder b2)
{
  auto [b3, b4, result] = __builtin_tvm_stbrq(b1, b2);
  return b4;
}

// ENDXC  (b x – c), if x6 = 0, creates a special or exotic cell from Builder b
__tvm_cell endxc_test(__tvm_builder b1, int x)
{
  return __builtin_tvm_endxc(b1, x);
}

// STILE4 (x b – b0), stores a little-endian signed 32-bit integer
__tvm_builder stile4_test(int x, __tvm_builder b1)
{
  return __builtin_tvm_stile4(x, b1);
}

// STULE4 (x b – b0), stores a little-endian unsigned 32-bit integer
__tvm_builder stule4_test(int x, __tvm_builder b1)
{
  return __builtin_tvm_stule4(x, b1);
}

// STILE8 (x b – b0), stores a little-endian signed 64-bit integer
__tvm_builder stile8_test(int x, __tvm_builder b1)
{
  return __builtin_tvm_stile8(x, b1);
}

// STULE8 (x b – b0), stores a little-endian unsigned 64-bit integer
__tvm_builder stule8_test(int x, __tvm_builder b1)
{
  return __builtin_tvm_stule8(x, b1);
}

// BDEPTH  (b – x), returns the depth of Builder b. If no cell references are stored in b, then x =  0; 
// otherwise x is one plus the maximum of depths of cells referred to from b
int bdepth_test(__tvm_builder b1)
{
  return __builtin_tvm_bdepth(b1);
}

// BBITREFS (b – x y), returns the numbers of both data bits and cell references in b
int bbitrefs_test(__tvm_builder b1)
{
  auto [x, y] = __builtin_tvm_bbitrefs(b1);
  return x + y;
}

// BREMBITREFS  (b – x0 y0)
int brembitrefs_test(__tvm_builder b1)
{
  auto [x, y] = __builtin_tvm_brembitrefs(b1);
  return x + y;
}

// BCHKBITS (b x – ), checks whether x bits can be stored into b, 0 <= x <= 1023. If there is no space for x more bits in b ... throws an exception
void bchkbits_test(__tvm_builder b1, int x)
{
  __builtin_tvm_bchkbits(b1, x);
}

// BCHKREFS (b y – ), checks whether y references can be stored into b, 0 <= y <= 7
void bchkrefs_test(__tvm_builder b1, int y)
{
  __builtin_tvm_bchkrefs(b1, y);
}

// BCHKBITREFS (b x y – ), checks whether x bits and y references can be stored into b, 0 <= x <= 1023, 0 <= y <= 7
void bchkbitrefs_test(__tvm_builder b1, int x, int y)
{
  __builtin_tvm_bchkbitrefs(b1, x, y);
}

// BCHKBITSQ (b x – ?), checks whether x bits can be stored into b, 0 <= x <= 1023
int bchkbitsq_test(__tvm_builder b1, int x)
{
  return __builtin_tvm_bchkbitsq(b1, x);
}

// BCHKREFSQ (b y – ?), checks whether y references can be stored into b, 0 <= y <= 7
int bchkrefsq_test(__tvm_builder b1, int y)
{
  return __builtin_tvm_bchkrefsq(b1, y);
}

// BCHKBITREFSQ (b x y – ?), checks whether x bits and y references can be stored into b, 0 <= x <= 1023, 0 <= y <= 7
int bchkbitrefsq_test(__tvm_builder b1, int x, int y)
{
  return __builtin_tvm_bchkbitrefsq(b1, x, y);
}

// STZEROES (b n – b0), stores n binary zeroes into Builder b
__tvm_builder stzeroes_test(__tvm_builder b1, int n)
{
  return __builtin_tvm_stzeroes(b1, n);
}

// STONES (b n – b0), stores n binary ones into Builder b
__tvm_builder stones_test(__tvm_builder b1, int n)
{
  return __builtin_tvm_stones(b1, n);
}

// STSAME (b n x – b0), stores n binary xes (0 <= x <= 1) into Builder b
__tvm_builder stsame_test(__tvm_builder b1, int n, int x)
{
  return __builtin_tvm_stsame(b1, n, x);
}

// STSLICECONST sss (b – b0), stores a constant subslice sss consisting of 0 <= x <= 3 
//  references and up to 8y + 1 data bits, with 0 <= y <= 7.
// TODO

// STSLICECONST ‘0’ or STZERO (b – b0), stores one binary zero
__tvm_builder stzero_test(__tvm_builder b1)
{
  return __builtin_tvm_stzero(b1);
}

// STSLICECONST ‘1’ or STONE (b – b0), stores one binary one
__tvm_builder stone_test(__tvm_builder b1)
{
  return __builtin_tvm_stone(b1);
}

// LDIQ cc + 1 (s – x s0 -1 or s 0), a quiet version of LDI
int ldiq_test(__tvm_slice s1)
{
  auto [x, s2, result] = __builtin_tvm_ldiq(s1, 15);
  return result;
}

// PLDIQ cc + 1 (s – x -1 or 0), a quiet version of PLDI
int pldiq_test(__tvm_slice s1)
{
  auto [x, result] = __builtin_tvm_pldiq(s1, 17);
  return result;
}

// PLDUQ cc + 1 (s – x -1 or 0), a quiet version of PLDU
int plduq_test(__tvm_slice s1)
{
  auto [x, result] = __builtin_tvm_plduq(s1, 18);
  return result;
}

// PLDUZ 32(c + 1) (s - s x), preloads the first 32(c + 1) bits of Slice s into an unsigned integer x, for 0 <= c <= 7
__tvm_slice plduz_test(__tvm_slice s1)
{
  auto [s2, x] = __builtin_tvm_plduz(s1, 3);
  return s2;
}

// PLDSLICE cc + 1 (s - s00), returns the first 0 < cc + 1 <= 256 bits of s as s00
__tvm_slice pldslice_test(__tvm_slice s1)
{
  return __builtin_tvm_pldslice(s1, 4);
}

// LDSLICEQ cc + 1 (s – s00 s0 -1 or s 0), a quiet version of LDSLICE
__tvm_slice ldsliceq_test(__tvm_slice s1)
{
  auto [s2, s3, result] = __builtin_tvm_ldsliceq(s1, 5);
  return s3;
}

// PLDSLICEQ cc + 1 (s – s00 -1 or 0), a quiet version of PLDSLICE
__tvm_slice pldsliceq_test(__tvm_slice s1)
{
  auto [s2, result] = __builtin_tvm_pldsliceq(s1, 6);
  return s2;
}

// LDSLICEXQ (s l – s00 s0 -1 or s 0), a quiet version of LDSLICEX
__tvm_slice ldslicexq_test(__tvm_slice s1, int l)
{
  auto [s2, s3, result] = __builtin_tvm_ldslicexq(s1, l);
  return s3;
}

// PLDSLICEXQ (s l – s0 -1 or 0), a quiet version of LDSLICEXQ
__tvm_slice pldslice_test(__tvm_slice s1, int l)
{
  auto [s2, result] = __builtin_tvm_pldslicexq(s1, l);
  return s2;
}

// SDCUTFIRST (s l – s0), returns the first 0 <= l <= 1023 bits of s. It is equivalent to PLDSLICEX
__tvm_slice sdcutfirst_test(__tvm_slice s1, int l)
{
  return __builtin_tvm_sdcutfirst(s1, l);
}

// SDSKIPFIRST (s l – s0), returns all but the first 0 <= l <= 1023 bits of s. It is equivalent to LDSLICEX; NIP
__tvm_slice sdskipfirst_test(__tvm_slice s1, int l)
{
  return __builtin_tvm_sdskipfirst(s1, l);
}

// SDCUTLAST (s l – s0), returns the last 0 <= l <= 1023 bits of s
__tvm_slice sdcutlast_test(__tvm_slice s1, int l)
{
  return __builtin_tvm_sdcutlast(s1, l);
}

// SDSKIPLAST (s l – s0), returns all but the last 0 <= l <= 1023 bits of s
__tvm_slice sdskiplast_test(__tvm_slice s1, int l)
{
  return __builtin_tvm_sdskiplast(s1, l);
}

// SUBSLICE (s l r l0 r0 – s0), returns 0 <= l0 <= 1023 bits and 0 <= r0 <= 4 references from Slice s
__tvm_slice subslice_test(__tvm_slice s1, int l, int r, int l0, int r0)
{
  return __builtin_tvm_subslice(s1, l, r, l0, r0);
}

// SPLIT (s l r – s0 s00), splits the first 0 <= l <= 1023 data bits and first 0 <= r <= 4 
//  references from s into s0, returning the remainder of s as s00
__tvm_slice split_test(__tvm_slice s1, int l, int r)
{
  auto [s2, s3] = __builtin_tvm_split(s1, l, r);
  return s3;
}

// SPLITQ (s l r – s0 s00 -1 or s 0), a quiet version of SPLIT
__tvm_slice splitq_test(__tvm_slice s1, int l, int r)
{
  auto [s2, s3, result] = __builtin_tvm_splitq(s1, l, r);
  return s3;
}

// XCTOS (c – s ?), transforms an ordinary or exotic cell into a Slice, as if it were an ordinary cell. 
//  A flag is returned indicating whether c is exotic
__tvm_slice xctos_test(__tvm_cell c1)
{
  auto [s2, result] = __builtin_tvm_xctos(c1);
  return s2;
}

// XLOAD (c – c0), loads an exotic cell c and returns an ordinary cell c0. If c is already ordinary, does nothing. 
//  If c cannot be loaded, throws an exception
__tvm_cell xload_test(__tvm_cell c1)
{
  return __builtin_tvm_xload(c1);
}

// XLOADQ (c – c0 -1 or c 0), loads an exotic cell c as XLOAD, but returns 0 on failure
__tvm_cell xloadq_test(__tvm_cell c1)
{
  auto [c2, result]	= __builtin_tvm_xloadq(c1);
  return c2;
}


// SCHKBITS (s l – ), checks whether there are at least l data bits in Slice s. If this is not the case, 
//  throws a cell deserialisation (i.e., cell underflow) exception
void schkbits_test(__tvm_slice s1, int l)
{
  __builtin_tvm_schkbits(s1, l);
}

// SCHKREFS (s r – ), checks whether there are at least r references in Slice s
void schkrefs_test(__tvm_slice s1, int r)
{
  __builtin_tvm_schkrefs(s1, r);
}

// SCHKBITREFS	(s l r – ), checks whether there are at least l data bits and r references in Slice s
void schkbitrefs_test(__tvm_slice s1, int l, int r)
{
  __builtin_tvm_schkbitrefs(s1, l, r);
}

// SCHKBITSQ (s l – ?), checks whether there are at least l data bits in Slice s
int schkbitsq_test(__tvm_slice s1, int l)
{
  return __builtin_tvm_schkbitsq(s1, l);
}

// SCHKREFSQ (s r – ?), checks whether there are at least r references in Slice s
int schkrefsq_test(__tvm_slice s1, int r)
{
  return __builtin_tvm_schkrefsq(s1, r);
}

// SCHKBITREFSQ (s l r – ?), checks whether there are at least l data bits and r references in Slice s
int schkbitrefsq_test(__tvm_slice s1, int l, int r)
{
  return __builtin_tvm_schkbitrefsq(s1, l, r);
}

// PLDREFVAR (s n – c), returns the n-th cell reference of Slice s for 0 <= n <= 3
__tvm_cell pldrefvar_test(__tvm_slice s1)
{
  return __builtin_tvm_pldrefvar(s1, 2);
}

// PLDREFIDX n	(s – c), returns the n-th cell reference of Slice s, where 0 <= n <= 3
__tvm_cell pldrefidx_test(__tvm_slice s1)
{
  return __builtin_tvm_pldrefidx(s1, 2);
}

// PLDREF (s – c), preloads the first cell reference of a Slice
__tvm_cell pldref_test(__tvm_slice s1)
{
  return __builtin_tvm_pldref(s1);
}

// LDILE4 (s – x s0), loads a little-endian signed 32-bit integer
__tvm_slice ldile4_test(__tvm_slice s1)
{
  auto [x, s2] = __builtin_tvm_ldile4(s1);
  return s2;
}

// LDULE4 (s – x s0), loads a little-endian unsigned 32-bit integer
__tvm_slice ldule4_test(__tvm_slice s1)
{
  auto [x, s2] = __builtin_tvm_ldule4(s1);
  return s2;
}

// LDILE8 (s – x s0), loads a little-endian signed 64-bit integer
__tvm_slice ldile8_test(__tvm_slice s1)
{
  auto [x, s2] = __builtin_tvm_ldile8(s1);
  return s2;
}

// LDULE8 (s – x s0), loads a little-endian unsigned 64-bit integer
__tvm_slice ldule8_test(__tvm_slice s1)
{
  auto[x, s2] = __builtin_tvm_ldule8(s1);
  return s2;
}

// PLDILE4 (s – x), preloads a little-endian signed 32-bit integer
int pldile4_test(__tvm_slice s1)
{
  return __builtin_tvm_pldile4(s1);
}

// PLDULE4 (s – x), preloads a little-endian unsigned 32-bit integer
int pldule4_test(__tvm_slice s1)
{
  return __builtin_tvm_pldule4(s1);
}

// PLDILE8 (s – x), preloads a little-endian signed 64-bit integer
int pldile8_test(__tvm_slice s1)
{
  return __builtin_tvm_pldile8(s1);
}

// PLDULE8 (s – x), preloads a little-endian unsigned 64-bit integer
int pldule8_test(__tvm_slice s1)
{
  return __builtin_tvm_pldule8(s1);
}

// LDILE4Q (s – x s0 -1 or s 0), quietly loads a little-endian signed 32-bit integer
__tvm_slice ldile4q_test(__tvm_slice s1)
{
  auto [x, s2, result] = __builtin_tvm_ldile4q(s1);
  return s2;
}

// LDULE4Q (s – x s0 -1 or s 0), quietly loads a little-endian unsigned 32-bit integer
__tvm_slice ldule4q_test(__tvm_slice s1)
{
  auto [x, s2, result] = __builtin_tvm_ldule4q(s1);
  return s2;
}

// LDILE8Q	(s – x s0 -1 or	s 0), quietly loads a little-endian signed 64-bit integer
__tvm_slice ldile8q_test(__tvm_slice s1)
{
  auto [x, s2, result] = __builtin_tvm_ldile8q(s1);
  return s2;
}

// LDULE8Q	(s – x s0 -1 or	s 0), quietly loads a little-endian unsigned 64-bit integer
__tvm_slice ldule8q_test(__tvm_slice s1)
{
	auto[x, s2, result] = __builtin_tvm_ldule8q(s1);
	return s2;
}

// PLDILE4Q (s – x -1 or 0), quietly preloads a little-endian signed 32-bit integer
int pldile4q_test(__tvm_slice s1)
{
  auto [x, result] = __builtin_tvm_pldile4q(s1);
  return result;
}

// PLDULE4Q (s – x -1 or	0), quietly preloads a little-endian unsigned 32-bit integer
int pldule4q_test(__tvm_slice s1)
{
  auto [x, result] = __builtin_tvm_pldule4q(s1);
  return result;
}

// PLDILE8Q (s – x -1 or 0), quietly preloads a little-endian signed 64-bit integer
int pldile8q_test(__tvm_slice s1)
{
  auto [x, result] = __builtin_tvm_pldile8q(s1);
  return result;
}

// PLDULE8Q (s – x -1 or 0), quietly preloads a little-endian unsigned 64-bit integer
int pldule8q_test(__tvm_slice s1)
{
  auto [x, result] = __builtin_tvm_pldule8q(s1);
  return result;
}

// LDZEROES (s – n s0), returns the count n of leading zero bits in s, and removes these bits from s
__tvm_slice ldzeroes_test(__tvm_slice s1)
{
  auto [n, s2] = __builtin_tvm_ldzeroes(s1);
  return s2;
}

// LDONES (s – n s0), returns the count n of leading one bits in s, and removes these bits from s
__tvm_slice ldones_test(__tvm_slice s1)
{
  auto [n, s2] = __builtin_tvm_ldones(s1);
  return s2;
}

// LDSAME (s x – n s0), returns the count	n of leading bits equal to 0 <= x <= 1 in s, and removes these bits from s
__tvm_slice ldsame_test(__tvm_slice s1, int x)
{
  auto [n, s2] = __builtin_tvm_ldsame(s1, x);
  return s2;
}

// SDEPTH (s – x), returns the depth of Slice s. If s has no references, then x = 0; 
//  otherwise x is one plus the maximum of depths of cells referred to from s
int sdepth_test(__tvm_slice s1)
{
  return __builtin_tvm_sdepth(s1);
}

// CDEPTH (c – x), returns the depth of Cell c. If c has no references, then x = 0; 
//  otherwise x is one plus the maximum of depths of cells referred to from c
int cdepth_test(__tvm_cell c1)
{
  return __builtin_tvm_cdepth(c1);
}

