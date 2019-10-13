#pragma once

#include "untuple_caller.hpp"

namespace tvm {

template<class T>
class __attribute__((tvm_tuple)) tuple {
public:
  explicit tuple(__tvm_tuple tp) : tp_(tp) {}
  explicit tuple(T tpVal) : tp_(__builtin_tvm_tuple(tpVal)) {}
  T unpack() const { return tvm::untuple_caller<sizeof(T)>::untuple(tp_); }
  void pack(const T &tpVal) { tp_ = __builtin_tvm_tuple(tpVal); }
  
  __tvm_tuple get() const { return tp_; }
private:
  __tvm_tuple tp_;
};

} // namespace tvm

