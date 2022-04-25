// RUN: %clang_cc1 -O3 -triple tvm %s -isysroot %S/../../../../../projects/ton-compiler/ -internal-isystem %S/../../../../../projects/ton-compiler/cpp-sdk -internal-isystem %S/../../../../../projects/ton-compiler/cpp-sdk/std -internal-isystem %S/../../../../../projects/ton-compiler/cpp-sdk/std/target -fno-decomposition-binding-override -fms-extensions -fsyntax-only -verify %s
// REQUIRES: tvm-registered-target

struct pair { int a, b; };

pair foo() {
  return { 1, 2 };
}

int test1() {
  unsigned v1 = 200;
  auto [v0, =v1] = foo(); // expected-error{{Decomposition binding overrides are not enabled; use '-fdecomposition-binding-override' to enable support}}
  return v0 + v1;
}

