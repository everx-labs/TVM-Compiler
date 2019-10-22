#pragma once

#include "tuple.hpp"

namespace tvm {

class __attribute__((tvm_tuple)) smart_contract_info {
public:
  static inline tuple<smart_contract_info> get();
  
  static inline int now() {
    return __builtin_tvm_now();
  }
  static inline int blocklt() {
    return __builtin_tvm_blocklt();
  }
  static inline int ltime() {
    return __builtin_tvm_ltime();
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
  inline auto get_now() { return now_; }
  inline auto get_blocklt() { return blocklt_; }
  inline auto get_ltime() { return ltime_; }
  inline auto get_myaddr() { return myaddr_; }
  inline auto get_configroot() { return configroot_; }
private:
  int v0;
  int v1;
  int v2;
  // Returns the current Unix time as an Integer.
  int now_;
  // Returns the starting logical time of the current block.
  int blocklt_;
  // Returns the logical time of the current transaction.
  int ltime_;
  
  int v6;
  int v7;
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
};

inline tuple<smart_contract_info> smart_contract_info::get() {
  // variant: return temporary_data::get().unpackfirst().contract_info();
  return tuple<smart_contract_info>(__builtin_tvm_cast_to_tuple(
    __builtin_tvm_index(temporary_data::get().get(), 0)));
}

} // namespace tvm

