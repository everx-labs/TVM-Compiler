// RUN: not %clang -target tvm -S %s -o - 2>&1 | FileCheck %s
// REQUIRES: tvm-registered-target

// CHECK: float is not supported on this target
void foo(float x);
// CHECK: double is not supported on this target
void bar(double x);
