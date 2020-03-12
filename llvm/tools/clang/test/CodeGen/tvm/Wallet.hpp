#pragma once

#include <tvm/schema/message.hpp>

namespace tvm { namespace schema {

struct IWallet {
  void constructor() = 1;
  void set_subscription_account(MsgAddress addr) = 2;
  MsgAddress get_subscription_account() = 3;
  void send_transaction(MsgAddress dest, uint_t<128> value, bool_t bounce) = 4;
};

struct DWallet{};
struct EWallet{};

}} // namespace tvm::schema

