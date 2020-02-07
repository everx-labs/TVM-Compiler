// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../../stdlib -o -
// REQUIRES: tvm-registered-target

#include <tvm/schema/message.hpp>
#include <tvm/tuple.hpp>

using namespace tvm;
using namespace schema;

tuple<int_msg_info> foo(slice src, slice dest) {
  int_msg_info rv;
  rv.ihr_disabled = true;
  rv.bounce = false;
  rv.bounced = true;
  rv.src = src;
  rv.dest = dest;
  rv.value.grams = 777;
  rv.ihr_fee = 111;
  rv.fwd_fee = 222;
  rv.created_lt = 333;
  rv.created_at = 444;

  return tuple<int_msg_info>::create(rv);
}

