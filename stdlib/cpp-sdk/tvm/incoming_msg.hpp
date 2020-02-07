#pragma once

#include <tvm/contract.hpp>

namespace tvm {

struct __attribute__((tvm_tuple)) incoming_msg final {
  incoming_msg(schema::CommonMsgInfo v) : impl_(v) {}
  schema::int_msg_info internal() const {
    return std::get_known<schema::int_msg_info>(impl_);
  }
  schema::ext_in_msg_info external() const {
    return std::get_known<schema::ext_in_msg_info>(impl_);
  }
  int value() const {
    return internal().value.grams();
  }
  schema::lazy<schema::MsgAddressInt> int_sender() const {
    return internal().src;
  }
  schema::lazy<schema::MsgAddressExt> ext_sender() const {
    return external().src;
  }

  schema::CommonMsgInfo impl_;
};

} // namespace tvm

