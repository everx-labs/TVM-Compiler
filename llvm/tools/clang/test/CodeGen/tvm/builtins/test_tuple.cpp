// RUN: %clang -std=c++17 --target=tvm -S -O3 %s -o -
// CHECK-NOT: call {{ }}

// ******************************************* 
//
// A.3  Tuple, List, and Null primitives
//
// *******************************************/

// NIL ( – t), pushes the only Tuple t = () of length zero
__tvm_tuple nil_test()
{
  return __builtin_tvm_nil();
}

// SINGLE (x – t), creates a singleton t := (x), i.e., a Tuple of length one
__tvm_tuple single_test(int x)
{
  return __builtin_tvm_single(x);
}

// PAIR or CONS (x y – t), creates pair t := (x, y)
__tvm_tuple pair_test(int x, int y)
{
  return __builtin_tvm_pair(x, y);
}

// TRIPLE (x y z – t), creates triple t := (x, y, z)
__tvm_tuple triple_test(int x, int y, int z)
{
  return __builtin_tvm_triple(x, y, z);
}

// FIRST or CAR (t – x), returns the fist element of a Tuple
int first_test(__tvm_tuple t)
{
  return __builtin_tvm_first(t);
}

// SECOND or CDR (t – y), returns the second element of a Tuple
int second_test(__tvm_tuple t)
{
  return __builtin_tvm_second(t);
}

// THIRD (t – z), returns the third element of a Tuple
int third_test(__tvm_tuple t)
{
  return __builtin_tvm_third(t);
}

// UNSINGLE (t – x), unpacks a singleton t = (x)
int unsingle_test(__tvm_tuple t)
{
  return __builtin_tvm_unsingle(t);
}

// UNPAIR or UNCONS (t – x y), unpacks a pair t = (x, y)
int unpair_test(__tvm_tuple t)
{
  auto [x, y] = __builtin_tvm_unpair(t);
  return y;
}

// UNTRIPLE (t – x y z), unpacks a triple t = (x, y, z)
int untriple_test(__tvm_tuple t)
{
  auto [x, y, z] = __builtin_tvm_untriple(t);
  return z;
}

// EXPLODE	n (t – x1. . . xmm), unpacks a Tuple t = (x1, . . . , xm) and returns its length m, 
//  but only if m <= n <= 15. Otherwise throws a type check exception
// TODO

// SETFIRST (t x – t0), sets the first component of Tuple t to x and returns the resulting Tuple t0
__tvm_tuple setfirst_test(__tvm_tuple t, int x)
{
  return __builtin_tvm_setfirst(t, x);
}

// SETSECOND (t x – t0), sets the second component of Tuple t to x and returns the resulting Tuple t0
__tvm_tuple setsecond_test(__tvm_tuple t, int x)
{
  return __builtin_tvm_setsecond(t, x);
}

// SETTHIRD (t x – t0), sets the third component of Tuple t to x and returns the resulting Tuple t0
__tvm_tuple setthird_test(__tvm_tuple t, int x)
{
  return __builtin_tvm_setthird(t, x);
}

// UNPACKFIRSTVAR (t n – x1. . . xn), similar to UNPACKFIRST n, but with 0 <= n <= 255 taken from the stack
// TODO

// EXPLODEVAR (t n – x1. . . xmm), similar to EXPLODE n, but with 0 <= n <= 255 taken from the stack
// TODO

// INDEXVARQ (t k – x), similar to INDEXQ n, but with 0 <= k <= 254 taken from the stack
int indexvarq_test(__tvm_tuple t, int k)
{
  return __builtin_tvm_indexvarq(t, k);
}

// SETINDEXVARQ (t x k – t0), similar to SETINDEXQ	k, but with 0 <= k <= 254 taken from the stack
__tvm_tuple setindexvarq_test(__tvm_tuple t, int x, int k)
{
  return __builtin_tvm_setindexvarq(t, x, k);
}