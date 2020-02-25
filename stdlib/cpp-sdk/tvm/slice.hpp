#pragma once

#include <tvm/to_std_tuple.hpp>

namespace tvm {

class __attribute((tvm_tuple)) slice {
public:
  slice() : sl_((__tvm_slice)__builtin_tvm_pushnull()) {}
  slice(__tvm_slice sl) : sl_(sl) {}

  unsigned sbits() const { return __builtin_tvm_sbits(sl_); }
  bool empty() const { return __builtin_tvm_sdempty(sl_); }

  __tvm_slice get() const { return sl_; }

  bool operator == (slice v) const {
    return __builtin_tvm_sdeq(sl_, v);
  }
  operator __tvm_slice() const { return get(); }
private:
  __tvm_slice sl_;
};

} // namespace tvm
