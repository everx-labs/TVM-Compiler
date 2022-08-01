#pragma once

#include <tvm/from_dictionary.hpp>
#include <tvm/to_dictionary.hpp>

namespace tvm {

// strategy to keep persistent data in dictionary
template<class Contract>
struct persistent_strategy_in_dictionary {
  static int __attribute__((always_inline))
  process_constructor(int (Contract::*func)(cell, slice), cell msg, slice msg_body) {
    auto dict = dictionary::get_persistent();
    Contract c;
    c.p = from_dictionary(c.p, dict);
    int rv = (c.*func)(msg, msg_body);
    dict = to_dictionary(c.p, dict);
    dictionary::set_persistent(dict);
    return rv;
  }
  static int __attribute__((always_inline))
  process_external_call(int (Contract::*func)(cell, slice), cell msg, slice msg_body) {
    auto dict = dictionary::get_persistent();
    Contract c;
    c.p = from_dictionary(c.p, dict);
    int rv = (c.*func)(msg, msg_body);
    dict = to_dictionary(c.p, dict);
    dictionary::set_persistent(dict);
    return rv;
  }
};

// strategy to keep persistent data in one slice
template<class Contract>
struct persistent_strategy_in_slice {
  static int __attribute__((always_inline))
  process_constructor(int (Contract::*func)(cell, slice), cell msg, slice msg_body) {
    Contract c;
    int rv = c.constructor(msg, msg_body);
    persistent_data::set(schema::build(c.p).make_cell());
    return rv;
  }
  static int __attribute__((always_inline))
  process_external_call(int (Contract::*func)(cell, slice), cell msg, slice msg_body) {
    auto cl = persistent_data::get();
    Contract c;
    c.p = schema::parse<decltype(c.p)>(parser(cl), error_code::persistent_data_parse_error, true);
    int rv = (c.*func)(msg, msg_body);
    persistent_data::set(schema::build(c.p).make_cell());
    return rv;
  }
};

template<class Contract, class PersistentStrategy>
struct call_wrapper {
  static int __attribute__((always_inline))
  process_constructor(int (Contract::*func)(cell, slice), cell msg, slice msg_body) {
    return PersistentStrategy::process_constructor(func, msg, msg_body);
  }
  static int __attribute__((always_inline))
  process_external_call(int (Contract::*func)(cell, slice), cell msg, slice msg_body) {
    return PersistentStrategy::process_external_call(func, msg, msg_body);
  }
};

} // namespace tvm
