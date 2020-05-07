#pragma once

namespace tvm {

__always_inline void tvm_assert(bool cond, unsigned exc_code) {
  if (!cond)
    __builtin_tvm_throw(exc_code);
}
// tvm_assert copy to match solidity name
__always_inline void require(bool cond, unsigned exc_code) {
  if (!cond)
    __builtin_tvm_throw(exc_code);
}
__always_inline void tvm_throw(unsigned exc_code) {
  __builtin_tvm_throw(exc_code);
}

} // namespace tvm

