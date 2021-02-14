#pragma once

#include <tvm/cell.hpp>
#include <tvm/dictionary.hpp>
#include <tvm/slice.hpp>
#include <tvm/parser.hpp>
#include <tvm/signature_checker.hpp>
#include <tvm/persistent_data.hpp>
#include <tvm/assert.hpp>
#include <tvm/smart_contract_info.hpp>

#include <tvm/schema/make_parser.hpp>
#include <tvm/schema/make_builder.hpp>
#include <tvm/schema/message.hpp>
#include <tvm/schema/abiv1.hpp>
#include <tvm/schema/abiv2.hpp>
#include <tvm/schema/json-abi-gen.hpp>
#include <tvm/message_flags.hpp>
#include <tvm/func_id.hpp>

namespace tvm {

inline void tvm_accept() {
  __builtin_tvm_accept();
}
inline void tvm_commit() {
  __builtin_tvm_commit();
}

/* From tvm RAWRESERVE(x, y) :
Creates an output action which would reserve
  exactly x nanograms (if y = 0),
  at most x nanograms (if y = 2), or
  all but x nanograms (if y = 1 or y = 3),
from the remaining balance of the account.
It is roughly equivalent to creating an outbound message
carrying x nanograms (or b − x nanograms, where b is the remaining
balance) to oneself, so that the subsequent output actions would not
be able to spend more money than the remainder.
Bit +2 in y means that the external action does not fail if the specified amount cannot be
reserved; instead, all remaining balance is reserved. Bit +8 in y means
x ← −x before performing any further actions.
Bit +4 in y means that x is increased by the original balance of the current account (before the
compute phase), including all extra currencies, before performing any
other checks and actions. Currently x must be a non-negative integer,
and y must be in the range 0 ... 15. */

inline void tvm_rawreserve(unsigned x, rawreserve_flag y) {
  __builtin_tvm_rawreserve(x, static_cast<unsigned>(y));
}
inline void tvm_setcode(cell new_root_cell) {
  __builtin_tvm_setcode(new_root_cell);
}
inline void tvm_pop_c3(int val) {
  __builtin_tvm_setreg(3, val);
}
inline int tvm_bless(slice sl) {
  return __builtin_tvm_bless(sl);
}
inline unsigned tvm_hash(cell cl) {
  return __builtin_tvm_hashcu(cl);
}
inline unsigned tvm_hash(slice sl) {
  return __builtin_tvm_hashsu(sl);
}
inline unsigned tvm_trans_lt() {
  return __builtin_tvm_ltime();
}

inline void tvm_sendmsg(cell msg, unsigned flags) {
  __builtin_tvm_sendrawmsg(msg.get(), flags);
}
template<class T, class V>
inline static bool isa(V v) {
  return std::holds_alternative<T>(v);
}
template<class T, class V>
inline static auto cast(V v) {
  return std::get<T>(v);
}

inline static schema::MsgAddressExt get_incoming_ext_src(cell msg) {
  using namespace schema;
  auto inc_msg = parse<CommonMsgInfo>(parser(msg.ctos()), error_code::bad_incoming_msg);
  tvm_assert(isa<ext_in_msg_info>(inc_msg), error_code::bad_incoming_msg);
  return cast<ext_in_msg_info>(inc_msg).src();
}

// Default implementation of main entry functions
//  (main_external, main_internal, main_ticktock, main_split, main_merge)
#define DEFAULT_MAIN_ENTRY_FUNCTIONS(Contract, IContract, DContract, TimestampDelay)       \
__attribute__((tvm_raw_func)) int main_external(__tvm_cell msg, __tvm_slice msg_body) {    \
  return smart_switch</*Internal=*/false, Contract, IContract, DContract,                  \
                      replay_attack_protection::timestamp<TimestampDelay>>(msg, msg_body); \
}                                                                                          \
__attribute__((tvm_raw_func)) int main_internal(__tvm_cell msg, __tvm_slice msg_body) {    \
  return smart_switch</*Internal=*/true, Contract, IContract, DContract,                   \
                      replay_attack_protection::timestamp<TimestampDelay>>(msg, msg_body); \
}                                                                                          \
__attribute__((tvm_raw_func)) int main_ticktock(__tvm_cell, __tvm_slice) {                 \
  tvm_throw(error_code::unsupported_call_method);                                          \
  return 0;                                                                                \
}                                                                                          \
__attribute__((tvm_raw_func)) int main_split(__tvm_cell, __tvm_slice) {                    \
  tvm_throw(error_code::unsupported_call_method);                                          \
  return 0;                                                                                \
}                                                                                          \
__attribute__((tvm_raw_func)) int main_merge(__tvm_cell, __tvm_slice) {                    \
  tvm_throw(error_code::unsupported_call_method);                                          \
  return 0;                                                                                \
}

#define MAIN_ENTRY_FUNCTIONS_NO_REPLAY(Contract, IContract, DContract)                     \
__attribute__((tvm_raw_func)) int main_external(__tvm_cell msg, __tvm_slice msg_body) {    \
  return smart_switch</*Internal=*/false, Contract, IContract, DContract, void>(           \
    msg, msg_body);                                                                        \
}                                                                                          \
__attribute__((tvm_raw_func)) int main_internal(__tvm_cell msg, __tvm_slice msg_body) {    \
  return smart_switch</*Internal=*/true, Contract, IContract, DContract, void>(            \
    msg, msg_body);                                                                        \
}                                                                                          \
__attribute__((tvm_raw_func)) int main_ticktock(__tvm_cell, __tvm_slice) {                 \
  tvm_throw(error_code::unsupported_call_method);                                          \
  return 0;                                                                                \
}                                                                                          \
__attribute__((tvm_raw_func)) int main_split(__tvm_cell, __tvm_slice) {                    \
  tvm_throw(error_code::unsupported_call_method);                                          \
  return 0;                                                                                \
}                                                                                          \
__attribute__((tvm_raw_func)) int main_merge(__tvm_cell, __tvm_slice) {                    \
  tvm_throw(error_code::unsupported_call_method);                                          \
  return 0;                                                                                \
}

// Contract class may be simple class, or template with parameter: `bool Internal`.
// Template contract will instantiated with Internal=true for internal message pipeline,
//  and with Internal=false for external message pipeline.
#define DEFAULT_MAIN_ENTRY_FUNCTIONS_TMPL(Contract, IContract, DContract, TimestampDelay)  \
__attribute__((tvm_raw_func)) int main_external(__tvm_cell msg, __tvm_slice msg_body) {    \
  return smart_switch</*Internal=*/false, Contract<false>, IContract, DContract,           \
                      replay_attack_protection::timestamp<TimestampDelay>>(msg, msg_body); \
}                                                                                          \
__attribute__((tvm_raw_func)) int main_internal(__tvm_cell msg, __tvm_slice msg_body) {    \
  return smart_switch</*Internal=*/true, Contract<true>, IContract, DContract,             \
                      replay_attack_protection::timestamp<TimestampDelay>>(msg, msg_body); \
}                                                                                          \
__attribute__((tvm_raw_func)) int main_ticktock(__tvm_cell, __tvm_slice) {                 \
  tvm_throw(error_code::unsupported_call_method);                                          \
  return 0;                                                                                \
}                                                                                          \
__attribute__((tvm_raw_func)) int main_split(__tvm_cell, __tvm_slice) {                    \
  tvm_throw(error_code::unsupported_call_method);                                          \
  return 0;                                                                                \
}                                                                                          \
__attribute__((tvm_raw_func)) int main_merge(__tvm_cell, __tvm_slice) {                    \
  tvm_throw(error_code::unsupported_call_method);                                          \
  return 0;                                                                                \
}

// Prepare and send empty message with nanograms as transfer value.
// Only internal destination address allowed.
static void tvm_transfer(schema::MsgAddressInt dest, unsigned nanograms, bool bounce) {
  using namespace schema;

  message_relaxed<empty> out_msg;
  int_msg_info_relaxed msg_info;
  msg_info.ihr_disabled = true;
  msg_info.bounce = bounce;
  msg_info.bounced = false;
  msg_info.dest = dest;
  msg_info.value.grams = nanograms;
  msg_info.src = addr_none{}; // Will be filled by vm
  msg_info.ihr_fee = 0;
  msg_info.fwd_fee = 0;
  msg_info.created_lt = 0;
  msg_info.created_at = 0;

  out_msg.info = msg_info;
  tvm_sendmsg(build(out_msg).endc(), 0);
}
// Prepare and send empty message with nanograms as transfer value.
// Only internal destination address allowed.
static void tvm_transfer(schema::lazy<schema::MsgAddressInt> dest, unsigned nanograms, bool bounce, unsigned flags, cell payload) {
  using namespace schema;

  message_relaxed<anyval> out_msg;
  int_msg_info_relaxed msg_info;
  msg_info.ihr_disabled = true;
  msg_info.bounce = bounce;
  msg_info.bounced = false;
  msg_info.dest = dest;
  msg_info.value.grams = nanograms;
  msg_info.src = addr_none{}; // Will be filled by vm
  msg_info.ihr_fee = 0;
  msg_info.fwd_fee = 0;
  msg_info.created_lt = 0;
  msg_info.created_at = 0;

  out_msg.info = msg_info;

  builder b = build(out_msg.info);
  b = build(b, out_msg.init);
  slice payload_sl = payload.ctos();
  if (b.brembits() > payload_sl.sbits()) {
    out_msg.body = anyval{payload_sl};
    b = build(b, out_msg.body);
  } else {
    out_msg.body = ref<anyval>{payload_sl};
    b = build(b, out_msg.body);
  }
  tvm_sendmsg(b.endc(), flags);
}
static void tvm_transfer(slice dest, unsigned nanograms, unsigned bounce, unsigned flags, cell payload) {
  using namespace schema;

  message_relaxed<anyval> out_msg;
  int_msg_info_relaxed msg_info;
  msg_info.ihr_disabled = true;
  msg_info.bounce = bounce;
  msg_info.bounced = false;
  msg_info.dest = dest;
  msg_info.value.grams = nanograms;
  msg_info.src = addr_none{}; // Will be filled by vm
  msg_info.ihr_fee = 0;
  msg_info.fwd_fee = 0;
  msg_info.created_lt = 0;
  msg_info.created_at = 0;

  out_msg.info = msg_info;

  builder b = build(out_msg.info);
  b = build(b, out_msg.init);
  slice payload_sl = payload.ctos();
  if (b.brembits() > payload_sl.sbits()) {
    out_msg.body = anyval{payload_sl};
    b = build(b, out_msg.body);
  } else {
    out_msg.body = ref<anyval>{payload_sl};
    b = build(b, out_msg.body);
  }
  tvm_sendmsg(b.endc(), flags);
}
static void tvm_transfer(schema::lazy<schema::MsgAddressInt> dest, unsigned nanograms, bool bounce,
                         unsigned flags = DEFAULT_MSG_FLAGS) {
  using namespace schema;

  message_relaxed<empty> out_msg;
  int_msg_info_relaxed msg_info;
  msg_info.ihr_disabled = true;
  msg_info.bounce = bounce;
  msg_info.bounced = false;
  msg_info.dest = dest;
  msg_info.value.grams = nanograms;
  msg_info.src = addr_none{}; // Will be filled by vm
  msg_info.ihr_fee = 0;
  msg_info.fwd_fee = 0;
  msg_info.created_lt = 0;
  msg_info.created_at = 0;

  out_msg.info = msg_info;
  tvm_sendmsg(build(out_msg).endc(), flags);
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
