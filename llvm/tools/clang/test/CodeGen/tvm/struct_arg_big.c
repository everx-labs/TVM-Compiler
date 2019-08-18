// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s -o - | FileCheck %s

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

// CHECK: call fastcc i64 @g(i64 1, i64 2, i64 3, i64 4, i64 5, i64 6, i64 7, i64 8)
int main() {
  big_struct big = { 1, 2, 3, 4, 5, 6, 7, 8 };
  int rv = g(big);
  return rv;
}

