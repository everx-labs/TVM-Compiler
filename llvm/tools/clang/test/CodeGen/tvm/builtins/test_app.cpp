// RUN: %clang -std=c++17 --target=tvm -S -O3 %s -o -
// CHECK-NOT: call {{ }}

// ******************************************* 
//
// A.11.3. Pseudo-random number generator primitives
//
// *******************************************/

// RANDU256 ( –	x), generates a new pseudo-random unsigned 256-bit Integer x
int randu256_test()
{
  return __builtin_tvm_randu256();
}

// RAND (y – z), generates a new pseudo-random integer z in the range 0...y-1 (or y...-1, if y < 0)
int rand_test(int y)
{
  return __builtin_tvm_rand(y);
}


// SETRAND  (x	– ), sets the random seed to unsigned 256-bit Integer x
void setrand_test(int x)
{
  __builtin_tvm_setrand(x);
}

// ADDRAND (x – ), mixes unsigned 256-bit Integer x into the random seed r
void addrand_test(int x)
{
  __builtin_tvm_addrand(x);
}

// ******************************************* 
//
// A.11.4. Configuration primitives
//
// *******************************************/

// RANDSEED ( –	x), returns the current random seed as an unsigned 256-bit Integer. Equivalent to GETPARAM 6
int randseed_test()
{
  return __builtin_tvm_randseed();
}

// BALANCE ( –	t), returns the remaining balance of the smart contract as a Tuple consisting of an Integer (the remaining Gram balance in nanograms) 
//   and a Maybe Cell (a dictionary with 32-bit keys representing the balance of “extra currencies”). Equivalent to GETPARAM 7
__tvm_tuple balance_test()
{
  return __builtin_tvm_balance();
}


// ******************************************* 
//
// A.11.6. Hashing and cryptography primitives
//
// *******************************************/

// SHA256U (s – x), computes sha256 of the data bits of Slice s.
//  If the bit length of s is not divisible by eight, throws a cell underflow exception. The hash value is returned as a 256-bit unsigned integer x
int sha256u_test(__tvm_slice s)
{
  return __builtin_tvm_sha256u(s);
}

// CHKSIGNS  (d s k - ?) checks whether	s is a valid Ed25519-signature of the data portion of Slice	d using public key k, similarly to CHKSIGNU
int chksigns_test(__tvm_slice d, int s, int k)
{
  return __builtin_tvm_chksigns(d, s, k);
}

// ******************************************* 
//
// A.11.7. Miscellaneous primitives
//
// *******************************************/

// CDATASIZEQ (c n – x y z -1 or 0), recursively computes the count of distinct cells x, data bits y, 
//   and cell references z in the dag rooted at Cell c
int cdatasizeq_test(__tvm_cell c, int n)
{
  auto [x, y, z, result] = __builtin_tvm_cdatasizeq(c, n);
  return result;
}

// ******************************************* 
//
// A.11.10. Outbound message and output action primitives
//
// *******************************************/

// RAWRESERVEX  (x D y – ), similar to RAWRESERVE, but also accepts a dictionary D (represented by a Cell or Null ) with extra currencies
void rawreservex_test(int x, __tvm_cell d, int y)
{
  __builtin_tvm_rawreservex(x, d, y);
}

// SETLIBCODE (c x – ), creates an output action that would modify the collection of this smart contract libraries by adding or removing
//   library with code given in	Cell c
void setlibcode_test(__tvm_cell c, int x)
{
  __builtin_tvm_setlibcode(c, x);
}

// CHANGELIB (h x – ), creates an output action similarly to SETLIBCODE, 
//   but instead of the library code accepts its hash as an unsigned 256-bit integer
void changelib_test(int h, int x)
{
  __builtin_tvm_changelib(h, x);
}
