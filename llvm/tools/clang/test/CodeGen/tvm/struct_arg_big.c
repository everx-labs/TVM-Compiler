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

// CHECK: call fastcc i257 @g(i257 %{{.*}}, i257 %{{.*}}, i257 %{{.*}}, i257 %{{.*}}, i257 %{{.*}}, i257 %{{.*}}, i257 %{{.*}}, i257 %{{.*}})
int main_func(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8) {
  big_struct big = { v1, v2, v3, v4, v5, v6, v7, v8 };
  int rv = g(big);
  return rv;
}

