// RUN: %clang -O3 -S -target tvm %s -o - | FileCheck %s
// REQUIRES: tvm-registered-target

void setcode_Impl(__tvm_cell c) {
  __builtin_tvm_setcode(c);
}

// CHECK-LABEL: setcode_Impl
// CHECK: SETCODE
