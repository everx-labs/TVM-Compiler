// RUN: %clang -target tvm -export-json-abi %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o - | FileCheck %s
// REQUIRES: tvm-registered-target

// CHECK: "functions":
// CHECK: "name": "init"
// CHECK: "name": "set_subscription_account"
// CHECK: "name": "get_subscription_account"
// CHECK: "name": "send_transaction"

#include <tvm/schema/json-abi-gen.hpp>

namespace tvm { namespace schema {

struct IWallet {
  void init() = 1;
  void set_subscription_account(MsgAddress addr) = 2;
  MsgAddress get_subscription_account() = 3;
  void send_transaction(MsgAddress dest, uint_t<128> value, bool_t bounce) = 4;
};

struct DWallet {};

struct EWallet {};

}} // namespace tvm::schema

DEFINE_JSON_ABI(tvm::schema::IWallet, tvm::schema::DWallet, tvm::schema::EWallet);

