// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s -o - | FileCheck %s
// REQUIRES: tvm-registered-target

static const char arr[] = { '\xFFFFFFFF', '\x10000000' };

// CHECK-LABEL: @foo
// CHECK: ret i257 4294967295
char foo() {
  return arr[0];
}

// CHECK-LABEL: @bar
// CHECK: ret i257 268435456
char bar() {
  return arr[1];
}

