#pragma once

#include <tvm/slice.hpp>

namespace tvm {

class __attribute__((tvm_tuple)) cell {
public:
  static constexpr unsigned max_bits = 1023;
  static constexpr unsigned max_refs = 4;

  cell() : cl_((__tvm_cell)__builtin_tvm_pushnull()) {}
  cell(__tvm_cell cl) : cl_(cl) {}
  slice ctos() const { return __builtin_tvm_ctos(cl_); }
  bool isnull() const { return __builtin_tvm_isnull_cell(cl_); }

  __tvm_cell get() const { return cl_; }

  operator __tvm_cell() const { return get(); }
private:
  __tvm_cell cl_;
};

} // namespace tvm
