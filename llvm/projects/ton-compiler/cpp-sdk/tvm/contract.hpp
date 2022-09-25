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
#include <tvm/schema/build_chain_static.hpp>

namespace tvm {

__always_inline void tvm_accept() {
  __builtin_tvm_accept();
}
__always_inline cell tvm_mycode() {
  return __builtin_tvm_mycode();
}
__always_inline slice tvm_code_salt() {
  cell code = __builtin_tvm_mycode();
  require(code.ctos().srefs() == 3, error_code::unexpected_refs_count_in_code);
  parser mycode_parser(code.ctos());
  mycode_parser.ldref();
  mycode_parser.ldref();
  return mycode_parser.ldrefrtos();
}

__always_inline cell tvm_add_code_salt_cell(cell salt, cell code) {
  require(code.ctos().srefs() == 2, error_code::unexpected_refs_count_in_code);
  return builder().stslice(code.ctos()).stref(salt).endc();
}

template<class Data>
__always_inline cell tvm_add_code_salt(Data data, cell code) {
  return tvm_add_code_salt_cell(build_chain_static(data), code);
}

template<class T>
struct prohibit_early_assert {
  static constexpr bool value = false;
};

template<class Interface, class Data>
struct preparer {
  __always_inline
  static std::pair<StateInit, uint256> execute([[maybe_unused]] Data data, [[maybe_unused]] cell code) {
    static_assert(prohibit_early_assert<Interface>::value, "preparer must be overriden for this interface");
  }
};

/// Prepare StateInit and hash (for address) for contract deploy
template<class Interface, class Data>
__always_inline
std::pair<StateInit, uint256> prepare(Data data, cell code) {
  return preparer<Interface, Data>::execute(data, code);
}

template<class Interface, class Data>
struct expecter {
  __always_inline
  static uint256 execute([[maybe_unused]] Data data, [[maybe_unused]] uint256 code_hash, [[maybe_unused]] uint16 code_depth) {
    static_assert(prohibit_early_assert<Interface>::value, "expecter must be overriden for this interface");
  }
};

/// Calculate expected hash from StateInit (contract deploy address). Using code_hash and code_depth.
template<class Interface, class Data>
__always_inline
uint256 expected(Data data, uint256 code_hash, uint16 code_depth) {
  return expecter<Interface, Data>::execute(data, code_hash, code_depth);
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
inline void tvm_setcurrentcode(cell new_root_cell) {
  __builtin_tvm_setreg(3, __builtin_tvm_bless(new_root_cell.ctos()));
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
inline unsigned tvm_sha256(slice sl) {
  return __builtin_tvm_sha256u(sl);
}
inline unsigned tvm_trans_lt() {
  return __builtin_tvm_ltime();
}

template<class T>
using opt = std::optional<T>;

inline void tvm_sendmsg(cell msg, unsigned flags) {
  __builtin_tvm_sendrawmsg(msg.get(), flags);
}

/// Calculating StateInit hash using code_hash and data_hash
__always_inline
uint256 tvm_state_init_hash(uint256 code_hash, uint256 data_hash, uint16 code_depth, uint16 data_depth) {
  builder b;
  b.stu(2, 8) // amount of refs - code + data = 2
   .stu(1, 8) // data bitlen descriptor = ((bitlen / 8) << 1) + ((bitlen % 8) ? 1 : 0)
   // data bits: optional<..> split_depth, optional<..> special,
   // optional<ref> code, optional<ref> data,
   // optional<..> library ==> b00110
   .stu(0b00110, 5)
   .stu(0b100, 3) // completion tag b100
   .stu(code_depth.get(), 16)
   .stu(data_depth.get(), 16)
   .stu(code_hash.get(), 256)
   .stu(data_hash.get(), 256);
  return uint256(tvm_sha256(b.make_slice()));
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
}

#define MAIN_ENTRY_FUNCTIONS_NO_REPLAY(Contract, IContract, DContract)                     \
__attribute__((tvm_raw_func)) int main_external(__tvm_cell msg, __tvm_slice msg_body) {    \
  return smart_switch</*Internal=*/false, Contract, IContract, DContract, void>(           \
    msg, msg_body);                                                                        \
}                                                                                          \
__attribute__((tvm_raw_func)) int main_internal(__tvm_cell msg, __tvm_slice msg_body) {    \
  return smart_switch</*Internal=*/true, Contract, IContract, DContract, void>(            \
    msg, msg_body);                                                                        \
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
}
#define MAIN_ENTRY_FUNCTIONS_NO_REPLAY_TMPL(Contract, IContract, DContract)                \
__attribute__((tvm_raw_func)) int main_external(__tvm_cell msg, __tvm_slice msg_body) {    \
  return smart_switch</*Internal=*/false, Contract<false>, IContract, DContract, void>(    \
    msg, msg_body);                                                                        \
}                                                                                          \
__attribute__((tvm_raw_func)) int main_internal(__tvm_cell msg, __tvm_slice msg_body) {    \
  return smart_switch</*Internal=*/true, Contract<true>, IContract, DContract, void>(      \
    msg, msg_body);                                                                        \
}

#define DEFAULT_MAIN_ENTRY_FUNCTIONS2(Contract, IContract, DContract, ReplayProtect)        \
__attribute__((tvm_raw_func)) int main_external(__tvm_cell msg, __tvm_slice msg_body) {     \
  return smart_switch<false, Contract, IContract, DContract, ReplayProtect>(msg, msg_body); \
}                                                                                           \
__attribute__((tvm_raw_func)) int main_internal(__tvm_cell msg, __tvm_slice msg_body) {     \
  return smart_switch<true, Contract, IContract, DContract, ReplayProtect>(msg, msg_body);  \
}

#define DEFAULT_MAIN_ENTRY_FUNCTIONS_TMPL2(Contract, IContract, DContract, ReplayProtect)          \
__attribute__((tvm_raw_func)) int main_external(__tvm_cell msg, __tvm_slice msg_body) {            \
  return smart_switch<false, Contract<false>, IContract, DContract, ReplayProtect>(msg, msg_body); \
}                                                                                                  \
__attribute__((tvm_raw_func)) int main_internal(__tvm_cell msg, __tvm_slice msg_body) {            \
  return smart_switch<true, Contract<true>, IContract, DContract, ReplayProtect>(msg, msg_body);   \
}

// Prepare and send empty message with nanograms as transfer value.
// Only internal destination address allowed.
static inline void tvm_transfer(schema::MsgAddressInt dest, unsigned nanograms, bool bounce) {
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
static inline void tvm_transfer(schema::lazy<schema::MsgAddressInt> dest, unsigned nanograms, bool bounce, unsigned flags, cell payload) {
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
static inline void tvm_transfer(slice dest, unsigned nanograms, unsigned bounce, unsigned flags, cell payload) {
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
static inline void tvm_transfer(schema::lazy<schema::MsgAddressInt> dest, unsigned nanograms, bool bounce,
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
