// RUN: %clang -O3 -S -c -target tvm %s -o - | FileCheck %s
// RUN: %clang -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry main | FileCheck %s -check-prefix=CHECK-RUN

__attribute__((noinline))
int g(int a, int b) {
  return a + b;
}

__attribute__((noinline))
int triple_xxx(int a, int b, int c, int d) {
// CHECK-LABEL: triple_xxx
// CHECK: XCHG3
  int rv = g(0, 1);
  return rv + g(a, b) * d;
}

__attribute__((noinline))
int triple_xxp(int a, int b, int c, int d) {
// CHECK-LABEL: triple_xxp
// CHECK: XC2PU
  int rv = g(0, 1);
  rv += g(a, b) * d;
  return rv + g(1, 2);
}

__attribute__((noinline))
int triple_xpx(int a, int b, int c, int d) {
// CHECK-LABEL: triple_xpx
// CHECK: XCPUXC
  int rv = g(0, 1);
  rv += g(a, b) * d;
  return rv - b;
}

__attribute__((noinline))
int triple_xpp(int a, int b, int c, int d) {
// CHECK-LABEL: triple_xpp
// CHECK: XCPU2
  int rv = g(0, 1);
  rv += g(a, b) * d;
  return rv + b + g(1, 2);
}

__attribute__((noinline))
int triple_pxx(int a, int b, int c, int d, volatile int *e) {
// CHECK-LABEL: triple_pxx
// CHECK: PUXC2
  int rv = g(0, 1);
  int diff = rv - *e;
  rv += g(a, b) * d;
  return rv * *e - a + diff;
}

__attribute__((noinline))
int triple_pxp(int a, int b, int c, int d) {
// CHECK-LABEL: triple_pxp
// CHECK: PUXCPU
  int rv = g(0, 1);
  rv += g(a, b) * d;
  return rv + a + g(1, 2);
}

__attribute__((noinline))
int triple_ppx(int a, int b, int c, int d) {
// CHECK-LABEL: triple_ppx
// CHECK: XCPUXC
  int rv = g(0, 1);
  rv += g(a, b) * d;
  return (rv + a) * b;
}

__attribute__((noinline))
int triple_ppp(int a, int b, int c, int d) {
// CHECK-LABEL: triple_ppp
// CHECK: PUSH3
  int rv = g(0, 1);
  rv += g(a, b) * d;
  return (rv + a + g(1, 2)) * b;
}

// CHECK-RUN-NOT: custom error
int main() {
  int ret = triple_xxx(10, 20, 1, 7);
  __builtin_tvm_throwif(ret != 211, 13);

  ret = triple_xxp(10, 20, 4, 8);
  __builtin_tvm_throwif(ret != 244, 13);
  
  ret = triple_xpx(10, 20, 4, 9);
  __builtin_tvm_throwif(ret != 251, 13);

  ret = triple_xpp(10, 20, 4, 10);
  __builtin_tvm_throwif(ret != 324, 13);
  
  int val = 22;
  ret = triple_pxx(10, 20, 4, 11, &val);
  __builtin_tvm_throwif(ret != 7251, 13);

  ret = triple_pxp(10, 20, 4, 12);
  __builtin_tvm_throwif(ret != 374, 13);

  ret = triple_ppx(10, 20, 4, 13);
  __builtin_tvm_throwif(ret != 8020, 13);

  ret = triple_ppp(10, 20, 4, 14);
  __builtin_tvm_throwif(ret != 8680, 13);
  
  return ret;
}

