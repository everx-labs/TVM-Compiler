#pragma once

namespace tvm {

class parser {
public:
  explicit parser(__tvm_slice sl) : sl_(sl) {}
  explicit parser(__tvm_cell cell) : sl_(__builtin_tvm_ctos(cell)) {}

  unsigned ldu(unsigned len) {
    auto [val, sl] = __builtin_tvm_ldu(sl_, len);
    sl_ = sl;
    return val;
  }
  __tvm_slice get_cur_slice() const { return sl_; }
private:
  __tvm_slice sl_;
};

} // namespace tvm

