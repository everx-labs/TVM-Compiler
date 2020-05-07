#pragma once

#include <tvm/tuple.hpp>
#include <tvm/incoming_msg.hpp>

namespace tvm {

class __attribute__((tvm_tuple)) smart_contract_info {
public:
  static inline tuple<smart_contract_info> get();

  static inline int magic() {
    return __builtin_tvm_index(smart_contract_info::get().get(), 0);
  }
  static inline int actions() {
    return __builtin_tvm_index(smart_contract_info::get().get(), 1);
  }
  static inline int msgs_sent() {
    return __builtin_tvm_index(smart_contract_info::get().get(), 2);
  }

  static inline int now() {
    return __builtin_tvm_now();
  }
  static inline int blocklt() {
    return __builtin_tvm_blocklt();
  }
  static inline int ltime() {
    return __builtin_tvm_ltime();
  }
  static inline int rand_seed() {
    return __builtin_tvm_index(smart_contract_info::get().get(), 6);
  }
  static inline int balance_remaining() {
    auto balance_tup = (__tvm_tuple)__builtin_tvm_index(smart_contract_info::get().get(), 7);
    return __builtin_tvm_index(balance_tup, 0);
  }
  static inline __tvm_slice myaddr() {
    return __builtin_tvm_myaddr();
  }
  static inline __tvm_cell configroot() {
    return __builtin_tvm_configroot();
  }
  static inline auto /*{ __tvm_cell, int keylen }*/ configdict() {
    return __builtin_tvm_configdict();
  }
  inline auto get_magic() const { return magic_; }
  inline auto get_actions() const { return actions_; }
  inline auto get_msgs_sent() const { return msgs_sent_; }
  inline auto get_now() const { return now_; }
  inline auto get_blocklt() const { return blocklt_; }
  inline auto get_ltime() const { return ltime_; }
  inline auto get_rand_seed() const { return rand_seed_; }
  inline auto get_balance_remaining() const { return balance_remaining_; }
  inline auto get_myaddr() const { return myaddr_; }
  inline auto get_configroot() const { return configroot_; }
private:
  int magic_;
  int actions_;
  int msgs_sent_;
  // Returns the current Unix time as an Integer.
  int now_;
  // Returns the starting logical time of the current block.
  int blocklt_;
  // Returns the logical time of the current transaction.
  int ltime_;
  int rand_seed_;
  int balance_remaining_;
  // Returns the internal address of the current smart contract
  //  as a Slice with a MsgAddressInt. If necessary, it can be parsed further
  //  using primitives such as PARSESTDADDR or REWRITESTDADDR.
  __tvm_slice myaddr_;
  // Returns the Maybe Cell D with the current global configuration dictionary.
  __tvm_cell configroot_;
};

// c7 = globals tuple { smart_contract_info, global1, ..., globalN }
class __attribute__((tvm_tuple)) temporary_data {
public:
  static inline tuple<temporary_data> get() {
    return tuple<temporary_data>(__builtin_tvm_get_temporary_data());
  }

  inline tuple<smart_contract_info> contract_info() const { return contract_info_; }

  static int getglob(unsigned idx) { return __builtin_tvm_getglobal(idx); }
  static void setglob(unsigned idx, int val) { __builtin_tvm_setglobal(idx, val); }
private:
  tuple<smart_contract_info> contract_info_;
  tuple<incoming_msg> msg_;
};

inline tuple<smart_contract_info> smart_contract_info::get() {
  // variant: return temporary_data::get().unpackfirst().contract_info();
  return tuple<smart_contract_info>(__builtin_tvm_cast_to_tuple(
    __builtin_tvm_index(temporary_data::get().get(), 0)));
}

static inline void set_msg(cell msg) {
  temporary_data::setglob(1, __builtin_tvm_cast_from_cell(msg.get()));
}

static tuple<incoming_msg> parse_incoming_msg(cell msg) {
  auto parsed_v = schema::parse<schema::CommonMsgInfo>(parser(msg.ctos()),
                                                       error_code::bad_incoming_msg);
  return tuple<incoming_msg>::create(incoming_msg(parsed_v));
}

// For only internal message
static tuple<incoming_msg> parse_incoming_int_msg(cell msg) {
  auto parsed_v = schema::parse<schema::int_msg_info>(parser(msg.ctos()),
                                                      error_code::bad_incoming_msg);
  return tuple<incoming_msg>::create(incoming_msg(schema::CommonMsgInfo{parsed_v}));
}
// For only external message
static tuple<incoming_msg> parse_incoming_ext_msg(cell msg) {
  auto parsed_v = schema::parse<schema::ext_in_msg_info>(parser(msg.ctos()),
                                                         error_code::bad_incoming_msg);
  return tuple<incoming_msg>::create(incoming_msg(parsed_v));
}

static inline tuple<incoming_msg> msg() {
  auto v = temporary_data::getglob(1);
  if (__builtin_tvm_istuple(__builtin_tvm_cast_to_tuple(v))) // already parsed into tuple
    return tuple<incoming_msg>(__builtin_tvm_cast_to_tuple(v));
  
  auto tp = parse_incoming_msg(__builtin_tvm_cast_to_cell(v));
  temporary_data::setglob(1, __builtin_tvm_cast_from_tuple(tp.get()));
  return tp;
}

static inline tuple<incoming_msg> int_msg() {
  auto v = temporary_data::getglob(1);
  if (__builtin_tvm_istuple(__builtin_tvm_cast_to_tuple(v))) // already parsed into tuple
    return tuple<incoming_msg>(__builtin_tvm_cast_to_tuple(v));

  auto tp = parse_incoming_int_msg(__builtin_tvm_cast_to_cell(v));
  temporary_data::setglob(1, __builtin_tvm_cast_from_tuple(tp.get()));
  return tp;
}
static inline tuple<incoming_msg> ext_msg() {
  auto v = temporary_data::getglob(1);
  if (__builtin_tvm_istuple(__builtin_tvm_cast_to_tuple(v))) // already parsed into tuple
    return tuple<incoming_msg>(__builtin_tvm_cast_to_tuple(v));

  auto tp = parse_incoming_ext_msg(__builtin_tvm_cast_to_cell(v));
  temporary_data::setglob(1, __builtin_tvm_cast_from_tuple(tp.get()));
  return tp;
}

static inline int tvm_now() { return smart_contract_info::now(); }
static inline slice tvm_myaddr() { return smart_contract_info::myaddr(); }
static inline int tvm_balance() { return smart_contract_info::balance_remaining(); }

} // namespace tvm

