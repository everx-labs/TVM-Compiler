#pragma once

#include <tvm/cell.hpp>
#include <tvm/dictionary.hpp>
#include <tvm/slice.hpp>
#include <tvm/parser.hpp>
#include <tvm/signature_checker.hpp>
#include <tvm/persistent_data.hpp>

#include <tvm/schema/make_parser.hpp>
#include <tvm/schema/make_builder.hpp>
#include <tvm/schema/message.hpp>
#include <tvm/schema/abiv1.hpp>
#include <tvm/schema/json-abi-gen.hpp>

namespace tvm {

inline void tvm_accept() {
  __builtin_tvm_accept();
}
inline void tvm_sendmsg(cell msg, unsigned flags) {
  __builtin_tvm_sendrawmsg(msg.get(), flags);
}
inline void tvm_assert(bool cond, unsigned exc_code) {
  if (!cond)
    __builtin_tvm_throw(exc_code);
}
// tvm_assert copy to match solidity name
inline void require(bool cond, unsigned exc_code) {
  if (!cond)
    __builtin_tvm_throw(exc_code);
}
inline void tvm_throw(unsigned exc_code) {
  __builtin_tvm_throw(exc_code);
}
template<class T, class V>
inline static bool isa(V v) {
  return std::holds_alternative<T>(v);
}
template<class T, class V>
inline static auto cast(V v) {
  return std::get<T>(v);
}
inline static unsigned check_signature(slice msg_body, unsigned err) {
  signature_checker sig_check(msg_body);
  tvm_assert(sig_check.verified(), err);
  return sig_check.public_key();
}

inline static schema::MsgAddressExt get_incoming_ext_src(cell msg) {
  using namespace schema;
  auto inc_msg = parse<CommonMsgInfo>(parser(msg.ctos()), error_code::bad_incoming_msg);
  tvm_assert(isa<ext_in_msg_info>(inc_msg), error_code::bad_incoming_msg);
  return cast<ext_in_msg_info>(inc_msg).src();
}

// Prepare and send empty message with nanograms as transfer value.
// Only internal destination address allowed.
static void tvm_transfer(schema::MsgAddressInt dest, unsigned nanograms, bool bounce) {
  using namespace schema;

  message_relaxed<empty> out_msg;
  int_msg_info_relaxed msg_info;
  msg_info.ihr_disabled = false;
  msg_info.bounce = bounce;
  msg_info.bounced = false;
  msg_info.dest = dest;
  msg_info.value.grams = nanograms;
  msg_info.ihr_disabled = 0;
  msg_info.src = addr_none{}; // Will be filled by vm
  msg_info.ihr_fee = 0;
  msg_info.fwd_fee = 0;
  msg_info.created_lt = 0;
  msg_info.created_at = 0;

  out_msg.info = msg_info;
  tvm_sendmsg(build(out_msg).endc(), 0);
}
// The same for lazy MsgAddressInt (not parsed from slice)
static void tvm_transfer(schema::lazy<schema::MsgAddressInt> dest, unsigned nanograms, bool bounce) {
  using namespace schema;

  message_relaxed<empty> out_msg;
  int_msg_info_relaxed msg_info;
  msg_info.ihr_disabled = false;
  msg_info.bounce = bounce;
  msg_info.bounced = false;
  msg_info.dest = dest;
  msg_info.value.grams = nanograms;
  msg_info.ihr_disabled = 0;
  msg_info.src = addr_none{}; // Will be filled by vm
  msg_info.ihr_fee = 0;
  msg_info.fwd_fee = 0;
  msg_info.created_lt = 0;
  msg_info.created_at = 0;

  out_msg.info = msg_info;
  tvm_sendmsg(build(out_msg).endc(), 0);
}

class contract {
public:
  using persistent_offset = int;

  static slice get_persistent(persistent_offset offset, unsigned err = error_code::no_persistent_data) {
    auto dict = dictionary::get_persistent();
    auto [val_slice, succ] = dict.dictiget(static_cast<int>(offset), 64); {}
    tvm_assert(succ, err);
    return val_slice;
  }
  template<class Format>
  static Format get_persistent(persistent_offset offset, unsigned err = error_code::no_persistent_data) {
    auto dict = dictionary::get_persistent();
    auto [val_slice, succ] = dict.dictiget(static_cast<int>(offset), 64); {}
    tvm_assert(succ, err);
    return schema::parse<Format>(parser(val_slice), err, true);
  }
  static std::optional<parser> get_persistent_opt(persistent_offset offset) {
    auto dict = dictionary::get_persistent();
    auto [val_slice, succ] = dict.dictiget(static_cast<int>(offset), 64); {}
    if (succ)
      return parser(val_slice);
    return {};
  }
  template<class Format>
  static std::optional<Format> get_persistent_opt(persistent_offset offset) {
    auto dict = dictionary::get_persistent();
    auto [val_slice, succ] = dict.dictiget(static_cast<int>(offset), 64); {}
    if (succ)
      return schema::parse_if<Format>(parser(val_slice));
    return {};
  }
  static void set_persistent(persistent_offset offset, slice sl) {
    auto dict = dictionary::get_persistent();
    dict.dictiset(sl, static_cast<int>(offset), 64);
    dictionary::set_persistent(dict);
  }
  template<class Format>
  static void set_persistent(persistent_offset offset, Format val) {
    auto sl = schema::build<Format>(val).make_slice();
    auto dict = dictionary::get_persistent();
    dict.dictiset(sl, static_cast<int>(offset), 64);
    dictionary::set_persistent(dict);
  }
  template<class ArgsFmt>
  static auto parse_args(slice msg_body, unsigned err) {
    parser p(msg_body);
    auto [parsed_args_opt, new_p] = schema::parse_continue<ArgsFmt>(p);
    tvm_assert(!!parsed_args_opt, err);
    // Ensures slice is now empty
    // TODO: disabled for testing
    // new_p.ends();
    return *parsed_args_opt;
  }
};

} // namespace tvm
