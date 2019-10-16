// RUN: %clang -O3 -S -c -target tvm %s -std=c++17 -I%S/../../../../../../../stdlib/cpp-sdk/ -o - | tvm-testrun --no-trace --entry main | FileCheck %s

#include "untuple_caller.hpp"
#include "builder.hpp"
#include "parser.hpp"
#include "tuple.hpp"

struct __attribute__((tvm_tuple)) pair {
  int x;
  int y;
};

struct __attribute__((tvm_tuple)) x_and_pair {
  int x;
  tvm::tuple<pair> tpPair;
};

tvm::tuple<x_and_pair> make_x_and_pair() {
  pair p = { 200, 300 };
  x_and_pair tp = { 100, tvm::tuple<pair>(p) };
  return tvm::tuple<x_and_pair>(tp);
}

int sum_x_and_pair(tvm::tuple<x_and_pair> tpHandle) {
  x_and_pair tp = tpHandle.unpack();
  pair p = tp.tpPair.unpack();
  return tp.x + p.x + p.y;
}

struct __attribute__((tvm_tuple)) single_tuple {
  tvm::tuple<x_and_pair> tp;
};

int sum_single_tuple(tvm::tuple<single_tuple> tp) {
  auto unTp1 = tp.unpack();
  auto unTp2 = unTp1.tp.unpack();
  auto [v0, v1] = unTp2.tpPair.unpack();
  return unTp2.x + v0 + v1;
}

// CHECK-NOT: Unhandled exception
int main() {
  auto tp1 = make_x_and_pair();
  int v0 = sum_x_and_pair(tp1);
  __builtin_tvm_throwif(v0 != 600, 11);

  single_tuple single_tp = { tp1 };
  tvm::tuple<single_tuple> tp2(single_tp);
  int v1 = sum_single_tuple(tp2);
  __builtin_tvm_throwif(v1 != 600, 12);
  
  return 0;
}

