#pragma once

namespace tvm {

class __attribute__((tvm_tuple)) cell {
public:
  cell() : cl_((__tvm_cell)0) {}
  cell(__tvm_cell cl) : cl_(cl) {}
  slice ctos() const { return __builtin_tvm_ctos(cl_); }
  bool isnull() const { return __builtin_tvm_isnull_cell(cl_); }

  __tvm_cell get() const { return cl_; }

  operator __tvm_cell() const { return get(); }
private:
  __tvm_cell cl_;
};

} // namespace tvm
