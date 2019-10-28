#pragma once

#include <tvm/cell.hpp>
#include <tvm/dictionary.hpp>
#include <tvm/slice.hpp>
#include <tvm/parser.hpp>
#include <tvm/signature_checker.hpp>

#include <tvm/schema/make_parser.hpp>
#include <tvm/schema/make_builder.hpp>
#include <tvm/schema/message.hpp>

namespace tvm {

class contract {
public:
  using error_code = unsigned;
  using persistent_offset = int;

  static constexpr unsigned no_persistent_data = 90;
  static constexpr unsigned bad_incoming_msg = 91;

  inline static void accept() { __builtin_tvm_accept(); }
  inline static void sendmsg(cell msg, unsigned flags) {
    __builtin_tvm_sendrawmsg(msg.get(), flags);
  }

  inline static void assert(bool cond, error_code exc_code) {
    if (!cond)
      __builtin_tvm_throw(static_cast<unsigned>(exc_code));
  }
  inline static void tvm_throw(error_code exc_code) {
    __builtin_tvm_throw(static_cast<unsigned>(exc_code));
  }
  template<class T, class V>
  inline static bool isa(V v) {
    return std::holds_alternative<T>(v);
  }
  template<class T, class V>
  inline static auto cast(V v) {
    return std::get<T>(v);
  }

  static slice get_persistent(persistent_offset offset, error_code err = no_persistent_data) {
    auto dict = dictionary::get_persistent();
    auto [val_slice, succ] = dict.dictiget(static_cast<int>(offset), 64); {}
    assert(succ, err);
    return val_slice;
  }
  template<class Format>
  static Format get_persistent(persistent_offset offset, error_code err = no_persistent_data) {
    auto dict = dictionary::get_persistent();
    auto [val_slice, succ] = dict.dictiget(static_cast<int>(offset), 64); {}
    assert(succ, err);
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
  inline static schema::MsgAddressExt get_incoming_ext_src(cell msg) {
    using namespace schema;
    auto inc_msg = parse<CommonMsgInfo>(parser(msg.ctos()), bad_incoming_msg);
    assert(isa<ext_in_msg_info>(inc_msg), bad_incoming_msg);
    return cast<ext_in_msg_info>(inc_msg).src;
  }
  template<class ArgsFmt>
  static auto parse_args(slice msg_body, error_code err) {
    parser p(msg_body);
    auto [parsed_args_opt, new_p] = schema::parse_continue<ArgsFmt>(p);
    assert(!!parsed_args_opt, err);
    // Ensures slice is now empty
    // TODO: disabled for testing
    // new_p.ends();
    return *parsed_args_opt;
  }
  static unsigned check_signature(slice msg_body, error_code err) {
    signature_checker sig_check(msg_body);
    assert(sig_check.verified(), err);
    return sig_check.public_key();
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
    sendmsg(build(out_msg).endc(), 0);
  }
};

} // namespace tvm
