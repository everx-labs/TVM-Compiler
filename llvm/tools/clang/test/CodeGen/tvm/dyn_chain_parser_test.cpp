// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o -
// REQUIRES: tvm-registered-target

#include <tvm/schema/chain_builder.hpp>
#include <tvm/assert.hpp>
#include <tvm/schema/json-abi-gen.hpp>

using namespace tvm;
using namespace schema;

__interface IDynContract {
  __attribute__((external))
  void constructor(bytes name, bytes symbol, uint8 decimals, uint256 root_public_key, cell wallet_code, uint256 total_supply) = 1;
};
struct DDynContract {};
struct EDynContract {};

DEFINE_JSON_ABI(IDynContract, DDynContract, EDynContract);

struct Args {
  bytes name;
  bytes symbol;
  uint8 decimals;
  uint256 root_public_key;
  cell wallet_code;
  uint256 total_supply;
};

__always_inline uint256 test1(slice sl) {
  parser p(sl);
  Args args = parse_chain_dynamic<Args>(p);
  return args.total_supply;
}

__attribute__((tvm_raw_func))
int main_external(__tvm_cell msg, __tvm_slice msg_body) {
  parser p(msg_body);
  require(p.ldu(32) == 1, error_code::wrong_public_call);
  unsigned timestamp = p.ldu(64); // reading timestamp
  p.ldref(); // reading signature reference

  return test1(p.sl()).get();
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

