// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o -
// REQUIRES: tvm-registered-target

#include "Piggybank.hpp"
#include <tvm/contract_handle.hpp>

using namespace tvm;
using namespace schema;

void foo(lazy<MsgAddressInt> addr) {
  contract_handle<IPiggybank>(addr).call<&IPiggybank::deposit>(schema::Grams(10000));
}
