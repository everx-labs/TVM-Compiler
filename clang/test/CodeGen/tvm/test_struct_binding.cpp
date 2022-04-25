// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o - | FileCheck %s
// REQUIRES: tvm-registered-target

#include <tuple>

std::tuple<int, unsigned> foo() {
  return { 1, 2 };
}

std::tuple<int, unsigned> foo2() {
  return { 2, 3 };
}

struct my_pair { int x; unsigned y; };
my_pair foo3() {
  return { 3, 4 };
}

// CHECK: test1
// CHECK: ret i257 3
int test1() {
  int v0 = 100;
  unsigned v1 = 200;
  auto [=v0, =v1] = foo();
  return v0 + v1;
}

// CHECK: test2
// CHECK: ret i257 5
int test2() {
  auto [v0, v1] = foo();
  auto [=v0, =v1] = foo2();
  return v0 + v1;
}

// CHECK: test3
// CHECK: ret i257 7
int test3() {
  int v0 = 100;
  unsigned v1 = 200;
  auto [=v0, =v1] = foo3();
  return v0 + v1;
}

// CHECK: test4
// CHECK: ret i257 9
int test4() {
  int v0 = 100;
  unsigned v1 = 200;
  int foo4[2] { 4, 5 };
  auto [=v0, =v1] = foo4;
  return v0 + v1;
}

