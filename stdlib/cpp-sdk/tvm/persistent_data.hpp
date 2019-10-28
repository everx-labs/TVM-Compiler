#pragma once

namespace tvm {

class __attribute__((tvm_tuple)) persistent_data {
public:
  static inline persistent_data get() { return persistent_data(__builtin_tvm_get_persistent_data()); }
protected:
  persistent_data(__tvm_cell cell) : cell_(cell) {}
private:
  __tvm_cell cell_;
};

} // namespace tvm

