// RUN: %clang -O3 -S -c -emit-llvm -std=c++17 -target tvm %s -o - | FileCheck %s
// REQUIRES: tvm-registered-target

struct most_big {
  int a_v0;
  bool a_v1_bool;
  unsigned a_v2;
  __tvm_slice a_v3;
  __tvm_builder a_v4;
  __tvm_cell a_v5;
  __tvm_tuple a_v6;
  unsigned a_v7;
  int a_v8;
};

struct beta {
  int b_v0;
  int b_v1;
  unsigned b_v2;
};

struct gamma {
  int g_v0;
  unsigned g_v1;
  unsigned g_v2;
  int g_v3;
};

struct delta {
  int d_v0;
  __tvm_slice d_v1;
};

union test_union {
  gamma g;
  beta b;
  most_big a;
  delta d;
};

// CHECK: @_Z18test_argument_beta10test_union(i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}})
__attribute__((noinline)) int test_argument_beta(test_union v) {
  return v.b.b_v0 + v.b.b_v1 + (int)v.b.b_v2;
}

// CHECK: @_Z19test_argument_gamma10test_union(i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}})
__attribute__((noinline)) int test_argument_gamma(test_union v) {
  return v.g.g_v0 + (int)v.g.g_v1 + (int)v.g.g_v2 + v.g.g_v3;
}

// CHECK: @_Z19test_argument_delta10test_union(i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}}, i257{{( %.*)?}})
__attribute__((noinline)) int test_argument_delta(test_union v) {
  auto [val, new_sl] = __builtin_tvm_ldu(v.d.d_v1, 8);
  return v.d.d_v0 + val;
}

int test_argument_beta_call() {
  test_union v;
  beta b = { 111, 222, 333 };
  v.b = b;
  // CHECK: call i257 @_Z18test_argument_beta10test_union(i257 111, i257 222, i257 333
  return test_argument_beta(v);
}

int test_argument_gamma_call() {
  test_union v;
  gamma g = { 111, 222, 333, 444 };
  v.g = g;
  // CHECK: call i257 @_Z19test_argument_gamma10test_union(i257 111, i257 222, i257 333, i257 444
  return test_argument_gamma(v);
}

int test_argument_delta_call(__tvm_slice sl) {
  test_union v;
  delta d = { 111, sl };
  v.d = d;
  // CHECK: call i257 @_Z19test_argument_delta10test_union(i257 111, i257 %
  return test_argument_delta(v);
}

// most_big structure test
int test_most_big(test_union v) {
  return v.a.a_v1_bool ? v.a.a_v0 : v.a.a_v8;
}

