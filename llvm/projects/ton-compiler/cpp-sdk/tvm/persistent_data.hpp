#pragma once

namespace tvm {

class persistent_data {
public:
  static inline cell get() {
    return __builtin_tvm_get_persistent_data();
  }
  static inline void set(cell cl) {
    return __builtin_tvm_set_persistent_data(cl);
  }
};

} // namespace tvm

