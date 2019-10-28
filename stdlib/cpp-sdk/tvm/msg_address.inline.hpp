#pragma once

#include <tvm/msg_address.hpp>
#include <tvm/parser.hpp>
#include <tvm/error_code.hpp>

namespace tvm {

inline schema::MsgAddress msg_address::unpack() const {
  switch (static_cast<msg_addr_kind>(kind())) {
  case msg_addr_kind::kind_none:
    return schema::MsgAddressExt { schema::addr_none{} };
  case msg_addr_kind::kind_extern: {
    schema::addr_extern rv;
    rv.external_address.sl_ = __builtin_tvm_cast_to_slice(__builtin_tvm_index(tup_, 1));
    rv.external_address.bitlen_ = rv.external_address.sl_.sbits();
    return schema::MsgAddressExt { rv };
  }
  case msg_addr_kind::kind_std: {
    tvm::tuple<addr_std> tup(tup_);
    auto unpacked = tup.unpack();
    schema::addr_std rv;
    if (!__builtin_tvm_isnull(__builtin_tvm_cast_from_slice(unpacked.rewrite_pfx())))
      rv.Anycast = schema::anycast_info { parser(unpacked.rewrite_pfx()).ldvaruint32() };
    rv.workchain_id = unpacked.workchain_id();
    rv.address = parser(unpacked.address()).ldu(256);
    return schema::MsgAddressInt { rv };
  }
  case msg_addr_kind::kind_var: {
    tvm::tuple<addr_var> tup(tup_);
    auto unpacked = tup.unpack();
    schema::addr_var rv;
    if (!__builtin_tvm_isnull(__builtin_tvm_cast_from_slice(unpacked.rewrite_pfx())))
      rv.Anycast = schema::anycast_info { parser(unpacked.rewrite_pfx()).ldvaruint32() };
    rv.workchain_id = unpacked.workchain_id();
    rv.address.sl_ = unpacked.address();
    rv.address.bitlen_ = rv.address.sl_.sbits();
    return schema::MsgAddressInt { rv };
  }
  }
  __builtin_tvm_throw(error_code::bad_tupled_variant_kind);
}

} // namespace tvm
