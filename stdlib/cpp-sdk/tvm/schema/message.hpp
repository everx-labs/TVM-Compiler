#pragma once

#include <tvm/schema/basics.hpp>

namespace tvm { namespace schema {

using std::optional;
using std::variant;

struct addr_none {
  bitconst<2, 0b00> kind;

  DEFAULT_EQUAL(addr_none)
};

struct addr_extern {
  bitconst<2, 0b01> kind;

  uint_t<9> len;
  dynamic_bitfield<&addr_extern::len> external_address;

  DEFAULT_EQUAL(addr_extern)
};

struct anycast_info {
  varuint32 rewrite_pfx;

  DEFAULT_EQUAL(anycast_info)
};

struct addr_std {
  bitconst<2, 0b10> kind;
  optional<anycast_info> Anycast;
  int8 workchain_id;
  uint256 address;

  DEFAULT_EQUAL(addr_std)
};

struct addr_var {
  bitconst<2, 0b11> kind;
  optional<anycast_info> Anycast;
  uint_t<9> addr_len;
  int32 workchain_id;
  dynamic_bitfield<&addr_var::addr_len> address;

  DEFAULT_EQUAL(addr_var)
};

using MsgAddressExt = variant<addr_none, addr_extern>;
using MsgAddressInt = variant<addr_std, addr_var>;
using MsgAddress = variant<MsgAddressExt, MsgAddressInt>;

struct ExtraCurrencyCollection {
  HashmapE<32, varuint<32>> dict;
};

struct CurrencyCollection {
  Grams grams;
  ExtraCurrencyCollection other;
};

template<class SrcT>
struct int_msg_info_t {
  bitconst<1, 0b0> kind;

  bool_t ihr_disabled;
  bool_t bounce;
  bool_t bounced;
  lazy<SrcT> src;
  lazy<MsgAddressInt> dest;
  CurrencyCollection value;
  Grams ihr_fee;
  Grams fwd_fee;
  uint64 created_lt;
  uint32 created_at;
};
using int_msg_info = int_msg_info_t<MsgAddressInt>;
using int_msg_info_relaxed = int_msg_info_t<MsgAddress>;

struct ext_in_msg_info {
  bitconst<2, 0b10> kind;

  lazy<MsgAddressExt> src;
  lazy<MsgAddressInt> dest;
  Grams import_fee;
};

template<class SrcT>
struct ext_out_msg_info_t {
  bitconst<2, 0b11> kind;

  lazy<SrcT> src;
  lazy<MsgAddressExt> dest;
  uint64 created_lt;
  uint32 created_at;
};
using ext_out_msg_info = ext_out_msg_info_t<MsgAddressInt>;
using ext_out_msg_info_relaxed = ext_out_msg_info_t<MsgAddress>;

using CommonMsgInfo = variant<int_msg_info, ext_in_msg_info, ext_out_msg_info>;
using CommonMsgInfoRelaxed =
  variant<int_msg_info_relaxed, ext_in_msg_info, ext_out_msg_info_relaxed>;

struct TickTock {
  bool_t tick;
  bool_t tock;

  DEFAULT_EQUAL(TickTock)
};

struct StateInit {
  optional<uint_t<5>> split_depth;
  optional<TickTock> special;
  optional<cell> code;
  optional<cell> data;
  optional<cell> library;
};

template<typename X>
struct message {
  CommonMsgInfo info;
  optional<Either<StateInit, ref<StateInit>>> init;
  Either<X, ref<X>> body;
};

template<typename X>
struct message_relaxed {
  CommonMsgInfoRelaxed info;
  optional<Either<StateInit, ref<StateInit>>> init;
  Either<X, ref<X>> body;
};

}} // namespace tvm::schema
