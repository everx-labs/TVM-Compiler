// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s -o - | FileCheck %s
// REQUIRES: tvm-registered-target

typedef struct str {
  int x;
  int y;
} str_t;

// CHECK-LABEL: @foo
// CHECK: ret i257 1
int foo() {
  str_t v;
  if (&v.y > &v.x)
    return 1;
  return -1;
}

