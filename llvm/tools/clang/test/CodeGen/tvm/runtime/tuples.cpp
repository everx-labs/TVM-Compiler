// RUN: %clang -O3 -S -c -target tvm %s -std=c++17 -o - | tvm-testrun --no-trace --entry main | FileCheck %s -check-prefix=CHECK-RUN
// RUN: %clang -DNOINLINE -O3 -S -c -target tvm %s -std=c++17 -o - | tvm-testrun --no-trace --entry main | FileCheck %s -check-prefix=CHECK-RUN2

#ifdef NOINLINE
__attribute__((noinline))
#endif
__tvm_tuple make_tuple2(unsigned x, unsigned y) {
  return __builtin_tvm_tuple(x, y);
}

#ifdef NOINLINE
__attribute__((noinline))
#endif
int sum2(__tvm_tuple tp) {
  auto [x, y] = __builtin_tvm_untuple2(tp);
  return x + y;
}

#ifdef NOINLINE
__attribute__((noinline))
#endif
int sub2(__tvm_tuple tp) {
  auto [x, y] = __builtin_tvm_untuple2(tp);
  return x - y;
}

#ifdef NOINLINE
__attribute__((noinline))
#endif
__tvm_tuple make_tuple3(unsigned x, unsigned y, unsigned z) {
  return __builtin_tvm_tuple(x, y, z);
}

#ifdef NOINLINE
__attribute__((noinline))
#endif
int muldiv(__tvm_tuple tp) {
  auto [x, y, z] = __builtin_tvm_untuple3(tp);
  return (x * y) / z;
}

#ifdef NOINLINE
__attribute__((noinline))
#endif
__tvm_tuple make_tuple16(int v0, int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8, int v9, int v10, int v11, int v12, int v13, int v14, int v15) {
  return __builtin_tvm_tuple(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15);
}

#ifdef NOINLINE
__attribute__((noinline))
#endif
int sum16(__tvm_tuple tp) {
  auto [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15] = __builtin_tvm_untuple16(tp);
  return v0 + v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9 + v10 + v11 + v12 + v13 + v14 + v15;
}

// CHECK-RUN-NOT: Unhandled exception
// CHECK-RUN2-NOT: Unhandled exception
int main() {
  int v = sum2(make_tuple2(156, 44));
  if (v != 200)
    __builtin_tvm_throw(11);

  v = sub2(make_tuple2(47, 747));
  if (v != -700)
    __builtin_tvm_throw(12);
  
  v = muldiv(make_tuple3(60, 100, 10));
  if (v != 600)
    __builtin_tvm_throw(13);

  auto tup16 = make_tuple16(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  v = sum16(tup16);
  if (v != 136)
    __builtin_tvm_throw(14);

  __builtin_tvm_throwif(__builtin_tvm_index(tup16, 6) != 7, 15);
  __builtin_tvm_throwif(__builtin_tvm_indexq(tup16, 15) != 16, 16);

  __builtin_tvm_setindex(tup16, 7, 102);
  __builtin_tvm_setindexq(tup16, 8, -992);
  __builtin_tvm_throwif(__builtin_tvm_index(tup16, 7) == 102, 17);
  __builtin_tvm_throwif(__builtin_tvm_index(tup16, 8) == -992, 18);

  __builtin_tvm_chktuple(tup16);

  __builtin_tvm_throwif(__builtin_tvm_tlen(tup16) != 16, 19);
  __builtin_tvm_throwif(__builtin_tvm_tlen(__builtin_tvm_tuple(1)) != 1, 20);
  __builtin_tvm_throwif(__builtin_tvm_tlen(__builtin_tvm_tuple(1, 2)) != 2, 21);
  __builtin_tvm_throwif(__builtin_tvm_qtlen(__builtin_tvm_tuple(1, 2, 3)) != 3, 22);
  __builtin_tvm_throwif(!__builtin_tvm_istuple(tup16), 23);
  __builtin_tvm_throwif(__builtin_tvm_last(tup16) != 16, 24);

  tup16 = __builtin_tvm_tpush(tup16, 198);
  __builtin_tvm_throwif(__builtin_tvm_tlen(tup16) != 17, 25);
  __builtin_tvm_throwif(__builtin_tvm_last(tup16) != 198, 26);
  
  auto [x, y, z] = __builtin_tvm_unpackfirst3(tup16);
  __builtin_tvm_throwif(x != 1, 27);
  __builtin_tvm_throwif(y != 2, 28);
  __builtin_tvm_throwif(z != 3, 29);

  auto [tupP, val17] = __builtin_tvm_tpop(tup16);
  tup16 = tupP;
  __builtin_tvm_throwif(__builtin_tvm_tlen(tup16) != 16, 30);
  __builtin_tvm_throwif(__builtin_tvm_last(tup16) != 16, 31);
  __builtin_tvm_throwif(val17 != 198, 32);

  auto do_tpop = [&tup16]() {
    auto [newTup, val] = __builtin_tvm_tpop(tup16);
    tup16 = newTup;
  };

  for (unsigned i = 0; i < 10; ++i) {
    do_tpop();
  }

  __builtin_tvm_throwif(__builtin_tvm_tlen(tup16) != 6, 33);

  return 0;
}

