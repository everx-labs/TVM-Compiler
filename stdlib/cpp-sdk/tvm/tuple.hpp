#pragma once

#include <tvm/untuple_caller.hpp>
#include <tvm/unpackfirst_caller.hpp>

namespace tvm {

template<class T>
class __attribute__((tvm_tuple)) tuple {
public:
  tuple() {}
  explicit tuple(__tvm_tuple tp) : tp_(tp) {}
  explicit tuple(T tpVal) : tp_(__builtin_tvm_tuple(tpVal)) {}
  T unpack() const { return tvm::untuple_caller<sizeof(T)>::untuple(tp_); }
  void pack(const T &tpVal) { tp_ = __builtin_tvm_tuple(tpVal); }

  static tuple<T> create(T val) {
    tuple<T> rv;
    rv.pack(val);
    return rv;
  }

  T unpackfirst() const { return tvm::unpackfirst_caller<sizeof(T)>::unpack(tp_); }

  __tvm_tuple get() const { return tp_; }
private:
  __tvm_tuple tp_;
};

} // namespace tvm

