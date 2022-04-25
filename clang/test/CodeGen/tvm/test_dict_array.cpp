// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o -
// REQUIRES: tvm-registered-target

#include <tvm/dict_array.hpp>
#include <numeric>

using namespace tvm;
using namespace schema;

uint32 test1(dict_array<uint32, 32> dict) {
  uint32 sum(0);
  for (auto elem : dict) {
    sum += elem;
  }
  return sum;
}

int32 test2(dict_array<int32, 32> dict) {
  int32 sum(0);
  auto [key, val, success] = dict.min();
  sum += val;
  while (success) {
    auto [=key, =val, =success] = dict.next(key);
    sum += val;
  }
  return sum;
}

int32 test3(dict_array<int32, 32> dict) {
  std::reverse(dict.begin(), dict.end());
  const auto& cdict = dict;
  return std::accumulate(cdict.begin(), cdict.end(), int32(1000),
                         [](int32 l, int32 r){ return l - r; });
}

int32 test4(dict_array<int32, 32> dict) {
  std::reverse(dict.begin(), dict.end());
  int32 idx(0);
  int32 sum(0);
  for (auto elem : dict) {
    sum += elem * idx++;
  }
  return sum;
}
int32 test5(dict_array<int32, 32> dict) {
  int32 idx(0);
  int32 sum(0);
  for (auto elem : dict) {
    sum += elem * idx++;
  }
  return sum;
}

unsigned foo() {
  dict_array<schema::uint32, 32> dict { 1, 2, 3, 4, 10, 20 };
  return test1(dict).get();
}

unsigned bar() {
  dict_array<schema::int32, 32> dict { -19, 2, 3, 4, 10, 20 };
  return test2(dict).get();
}

unsigned baz() {
  dict_array<schema::int32, 32> dict { -19, 2, 3, 4, 10, 20 };
  return test3(dict).get();
}
unsigned baz2() {
  dict_array<schema::int32, 32> dict { -19, 2, 3, 4, 10, 20 };
  return test4(dict).get();
}
unsigned baz3() {
  dict_array<schema::int32, 32> dict { -19, 2, 3, 4, 10, 20 };
  return test5(dict).get();
}


// ----------------- main entry functions ---------------------- //

__attribute__((tvm_raw_func)) int main_external(__tvm_cell msg, __tvm_slice msg_body) {
  cell msg_v(msg);
  slice msg_body_v(msg_body);
  parser msg_parser(msg_body_v);
  auto func_id = msg_parser.ldu(32);
  switch (func_id) {
  case 1:
    return foo();
  case 2:
    return bar();
  case 3:
    return baz();
  case 4:
    return baz2();
  case 5:
    return baz3();
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
