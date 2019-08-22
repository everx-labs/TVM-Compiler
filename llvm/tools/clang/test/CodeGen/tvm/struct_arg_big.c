// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s -o - | FileCheck %s
// REQUIRES: tvm-registered-target

typedef struct big_struct_t {
  int a;
  int b;
  int c;
  int d;
  int e;
  int f;
  int g;
  int h;
} big_struct;

__attribute__((noinline))
static int g(big_struct v) {
  return v.a + v.b + v.c + v.d + v.e + v.f + v.g + v.h;
}

// CHECK: call fastcc i257 @g(i257 1, i257 2, i257 3, i257 4, i257 5, i257 6, i257 7, i257 8)
int main() {
  big_struct big = { 1, 2, 3, 4, 5, 6, 7, 8 };
  int rv = g(big);
  return rv;
}

