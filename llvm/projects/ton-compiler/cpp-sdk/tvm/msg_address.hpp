#pragma once

#include <tvm/tuple.hpp>
#include <tvm/schema/message.hpp>

namespace tvm {

// LDMSGADDR support
// TODO: implement as variant_tuple<addr_none, addr_extern, addr_std, addr_var>
class msg_address {
public:
  enum class msg_addr_kind {
    kind_none = 0,
    kind_extern = 1,
    kind_std = 2,
    kind_var = 3
  };

  // addr_none is represented by t = (0), i.e., a Tuple containing exactly
  //  one Integer equal to zero.
  class __attribute__((tvm_tuple)) addr_none {
  public:
    static bool isa(__tvm_tuple tp) {
      return __builtin_tvm_tlen(tp) == sizeof(addr_none) &&
        msg_addr_kind(__builtin_tvm_index(tp, 0)) == msg_addr_kind::kind_none;
    }
    int kind() const { return kind_; }
  private:
    int kind_;
  };

  // addr_extern is represented by t = (1, s), where Slice s contains the
  //  field external_address.
  class __attribute__((tvm_tuple)) addr_extern {
  public:
    static bool isa(__tvm_tuple tp) {
      return __builtin_tvm_tlen(tp) == sizeof(addr_extern) &&
        msg_addr_kind(__builtin_tvm_index(tp, 0)) == msg_addr_kind::kind_extern;
    }
    int kind() const { return kind_; }
    __tvm_slice external_address() const { return external_address_; }
  private:
    int kind_;
    __tvm_slice external_address_;
  };

  // addr_std is represented by t = (2, u, x, s),
  //  where u is either a Null (if anycast is absent)
  //  or a Slice s containing rewrite_pfx (if anycast is present).
  //  Next, Integer x is the workchain_id, and Slice s contains the address.
  class __attribute__((tvm_tuple)) addr_std {
  public:
    static bool isa(__tvm_tuple tp) {
      return __builtin_tvm_tlen(tp) == sizeof(addr_std) &&
        msg_addr_kind(__builtin_tvm_index(tp, 0)) == msg_addr_kind::kind_std;
    }
    int kind() const { return kind_; }
    __tvm_slice rewrite_pfx() const { return rewrite_pfx_; }
    int workchain_id() const { return workchain_id_; }
    __tvm_slice address() const { return address_; }
  private:
    int kind_;
    __tvm_slice rewrite_pfx_;
    int workchain_id_;
    __tvm_slice address_;
  };

  // addr_var is represented by t = (3, u, x, s), where u, x, and s have the
  //  same meaning as for addr_std.
  class __attribute__((tvm_tuple)) addr_var {
  public:
    static bool isa(__tvm_tuple tp) {
      return __builtin_tvm_tlen(tp) == sizeof(addr_var) &&
        msg_addr_kind(__builtin_tvm_index(tp, 0)) == msg_addr_kind::kind_var;
    }
    int kind() const { return kind_; }
    __tvm_slice rewrite_pfx() const { return rewrite_pfx_; }
    int workchain_id() const { return workchain_id_; }
    __tvm_slice address() const { return address_; }
  private:
    int kind_;
    __tvm_slice rewrite_pfx_;
    int workchain_id_;
    __tvm_slice address_;
  };

  explicit msg_address(__tvm_tuple tup) : tup_(tup) {}
  int kind() const { return __builtin_tvm_index(tup_, 0); }
  inline schema::MsgAddress unpack() const;
private:
  __tvm_tuple tup_;
};

} // namespace tvm

