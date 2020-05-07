// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o -
// REQUIRES: tvm-registered-target

#include <tvm/dict_map.hpp>
#include <numeric>

using namespace tvm;
using namespace schema;

uint128 test1_func(dict_map<uint32, uint128> dict) {
  uint128 sum(0);
  for (auto pair : dict) {
    sum += pair.second;
  }
  return sum;
}

uint128 test2_func(dict_map<uint32, uint128> dict) {
  uint128 sum(0);
  for (auto pair : dict) {
    sum += (pair.second - (uint128)pair.first);
  }
  return sum;
}

unsigned test1() {
  dict_map<uint32, uint128> dict{ {10, 100}, {100, 200}, {1, 2001}, {6, 11}, {7, 300}, {0, 800} };
  return test1_func(dict).get();
}

unsigned test2() {
  dict_map<uint32, uint128> dict{ {10, 100}, {100, 200}, {1, 2001}, {6, 11}, {7, 300}, {0, 800} };
  return test2_func(dict).get();
}

// ----------------- main entry functions ---------------------- //

__attribute__((tvm_raw_func)) int main_external(__tvm_cell msg, __tvm_slice msg_body) {
  cell msg_v(msg);
  slice msg_body_v(msg_body);
  parser msg_parser(msg_body_v);
  auto func_id = msg_parser.ldu(32);
  switch (func_id) {
  case 1:
    return test1();
  case 2:
    return test2();
  }
  tvm_throw(error_code::wrong_public_call);
  return 0;
}

__attribute__((tvm_raw_func)) int main_internal(__tvm_cell msg, __tvm_slice msg_body) {
  tvm_throw(error_code::unsupported_call_method);
  return 0;
}

__attribute__((tvm_raw_func)) int main_ticktock(__tvm_cell msg, __tvm_slice msg_body) {
  tvm_throw(error_code::unsupported_call_method);
  return 0;
}

__attribute__((tvm_raw_func)) int main_split(__tvm_cell msg, __tvm_slice msg_body) {
  tvm_throw(error_code::unsupported_call_method);
  return 0;
}

__attribute__((tvm_raw_func)) int main_merge(__tvm_cell msg, __tvm_slice msg_body) {
  tvm_throw(error_code::unsupported_call_method);
  return 0;
}

