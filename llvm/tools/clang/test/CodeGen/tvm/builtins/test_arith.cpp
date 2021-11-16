// RUN: %clang -std=c++17 --target=tvm -S -O3 %s -o -
// CHECK-NOT: call {{ }}

// ******************************************* 
//
// A.5.2. Division
//
// *******************************************/

// DIVR (x y – q0:= bx/y + 1/2c)
int divr_test(int x, int y)
{
  return __builtin_tvm_divr(x, y);
}

// DIVC (x y – q00:= dx/ye)
int divc_test(int x, int y)
{
  return __builtin_tvm_divc(x, y);
}

// DIVMOD (x y - q r), where q := bx/yc, r := x - yq
int divmod_test(int x, int y)
{
  auto [q, r] = __builtin_tvm_divmod(x, y);
  return q + r;
}

// DIVMODR (x y – q0 r0), where q0:= bx/y + 1/2c, r0:= x - yq0
int divmodr_test(int x, int y)
{
  auto [q, r] = __builtin_tvm_divmodr(x, y);
  return q + r;
}

// DIVMODC	(x y – q00 r00), where q00:= dx/ye, r00:= x - yq00
int divmodc_test(int x, int y)
{
  auto [q, r] = __builtin_tvm_divmodc(x, y);
  return q + r;
}

// MODPOW2 tt + 1: (x – x mod 2tt+1)
int modpow2_test(int x)
{
  return __builtin_tvm_modpow2(x, 5);
}

// MULDIVMOD (x y z – q r), where q := bx · y/zc, r := x · y mod z (same as */MOD in Forth)
int muldivmod_test(int x, int y, int z)
{
  auto [q, r] = __builtin_tvm_muldivmod(x, y, z);
  return q + r;
}

// MULRSHIFT (x y z – bxy * 2-zc) for 0 <= z <= 256
int mulrshift_test(int x, int y, int z)
{
  return __builtin_tvm_mulrshift(x, y, z);
}

// MULRSHIFT tt + 1 (x y - bxy · 2-tt-1c)
int mulrshift_tt_test(int x, int y)
{
  return __builtin_tvm_mulrshift_tt(x, y, 3);
}

// MULRSHIFTR	tt + 1 (x y - bxy · 2-tt-1+ 1/2c)
int mulrshiftr_tt_test(int x, int y)
{
  return __builtin_tvm_mulrshiftr_tt(x, y, 4);
}

// MULRSHIFTR (x y z – bxy * 2-z+ 1/2c) for 0 <= z <= 256
int mulrshiftr_test(int x, int y, int z)
{
  return __builtin_tvm_mulrshiftr(x, y, z);
}

// LSHIFTDIV (x y z - b2zx/yc) for 0 <= z <= 256
int lshiftdiv_test(int x, int y, int z)
{
  return __builtin_tvm_lshiftdiv(x, y, z);
}

// LSHIFTDIVR (x y z – b2zx/y + 1/2c) for 0 <= z <= 256
int lshiftdivr_test(int x, int y, int z)
{
	return __builtin_tvm_lshiftdivr(x, y, z);
}

// LSHIFTDIV tt + 1 (x y – b2tt+1x/yc)
int lshiftdiv_tt_test(int x, int y)
{
  return __builtin_tvm_lshiftdiv_tt(x, y, 2);
}

// LSHIFTDIVR tt + 1 (x y – b2tt+1x/y+ 1/2c)
int lshiftdivr_tt_test(int x, int y)
{
  return __builtin_tvm_lshiftdivr_tt(x, y, 4);
}

// ******************************************* 
//
// A.5.3. Shifts, logical operations
//
// *******************************************/ 

// POW2 (y – 2**y), 0 <= y <= 1023, equivalent to ONE; SWAP; LSHIFT
int pow2_test(int y)
{
  return __builtin_tvm_pow2(y);
}

// MINMAX or INTSORT2 (x y – x y or y x), sorts two integer
int minmax_test(int x, int y)
{
  auto [x2, y2] = __builtin_tvm_minmax(x, y);
  return x2 - y2;
}

// ******************************************* 
//
// A.5.4. Quiet arithmetic primitives
//
// *******************************************/ 

// QADD (x y – x + y), always works if x and y are Integer s, but returns a NaN if the addition cannot be performed
int qadd_test(int x, int y)
{
  return __builtin_tvm_qadd(x, y);
}

// QDIV (x y – bx/yc), returns a NaN if y = 0, or if y = -1 and
//   x = -2256, or if either of x or y is a NaN
int qdiv_test(int x, int y)
{
  return __builtin_tvm_qdiv(x, y);
}

// QAND (x y – x&y), bitwise “and” (similar to AND), but returns a NaN if either x or y is a NaN
int qand_test(int x, int y)
{
  return __builtin_tvm_qand(x, y);
}

// QOR (x y – x|y), bitwise “or”. If  x = -1 or y = -1, the result is always -1,
//  even if the other argument is a NaN
int qor_test(int x, int y)
{
  return __builtin_tvm_qor(x, y);
}

// ******************************************* 
//
// A.6	Comparison primitives
//
// *******************************************/ 

// ******************************************* 
//
// A.6	Comparison primitives
//
// *******************************************/ 

// SGN (x – sgn(x)), computes the sign of an integer x:	-1 if x < 0, 0 if x = 0, 1 if x > 0
int sgn_test(int x)
{
  return __builtin_tvm_sgn(x);
}

// ISNAN (x – x = NaN), checks whether x is a NaN
int isnan_test(int x)
{
  return __builtin_tvm_isnan(x);
}

// CHKNAN (x - x), throws an arithmetic overflow exception if x is a NaN
int chknan_test(int x)
{
  return __builtin_tvm_chknan(x);
}
