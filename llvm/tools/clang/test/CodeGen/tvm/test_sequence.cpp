// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o -
// REQUIRES: tvm-registered-target

#include <tvm/sequence.hpp>
#include <numeric>

using namespace tvm;
using namespace schema;

uint32 test1_func(sequence<uint32> arr) {
  uint32 sum(0);
  arr.enumerate([&](uint32 v){ sum += v; });
  return sum;
}

uint32 test2_func(sequence<uint32> arr) {
  uint32 sum(0);
  for (auto val : arr) {
    sum += val;
  }
  return sum;
}

int test1() {
  sequence<uint32> arr { 3, 4, 5, 6, 7, 8, 9, 10 };
  return test1_func(arr).get();
}

int test2() {
  sequence<uint32> arr { 3, 4, 5, 6, 7, 8, 9, 10 };
  return test2_func(arr).get();
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

