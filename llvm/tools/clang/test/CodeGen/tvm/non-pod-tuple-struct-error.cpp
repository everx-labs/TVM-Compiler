// RUN: not %clang -O3 -S -c -emit-llvm -target tvm %s -std=c++17 -I%S/../../../../../projects/ton-compiler/cpp-sdk/ -o - 2>&1 | FileCheck %s
// REQUIRES: tvm-registered-target

void f(int x);

// CHECK: error: 'tvm_tuple' attribute can only be used on a POD
class __attribute__((tvm_tuple)) NonPod {
public:
  ~NonPod() { f(x); }
  int x;
};

int g() {
  NonPod t;
  return 0;
}
