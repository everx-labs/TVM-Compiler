#pragma once

#include <tvm/msg_address.hpp>
#include <tvm/slice.hpp>
#include <tvm/cell.hpp>

namespace tvm {

class parser {
public:
  parser(){}
  explicit parser(slice sl) : sl_(sl.get()) {}
  explicit parser(__tvm_slice sl) : sl_(sl) {}
  explicit parser(__tvm_cell cell) : sl_(__builtin_tvm_ctos(cell)) {}

  slice ldslice(unsigned len) {
    auto [val, sl] = __builtin_tvm_ldslice(sl_, len);
    sl_ = sl;
    return val;
  }

  unsigned ldu(unsigned len) {
    auto [val, sl] = __builtin_tvm_ldu(sl_, len);
    sl_ = sl;
    return val;
  }
  int ldi(unsigned len) {
    auto [val, sl] = __builtin_tvm_ldi(sl_, len);
    sl_ = sl;
    return val;
  }
  unsigned pldu(unsigned len) const {
    return __builtin_tvm_pldu(sl_, len);
  }
  unsigned pldi(unsigned len) const {
    return __builtin_tvm_pldi(sl_, len);
  }
  std::optional<unsigned> lduq(unsigned len) {
    auto [val, sl, succ] = __builtin_tvm_lduq(sl_, len);
    if (succ) {
      sl_ = sl;
      return val;
    }
    return {};
  }

  unsigned ldvaruint16() {
    auto [val, sl] = __builtin_tvm_ldvaruint16(sl_);
    sl_ = sl;
    return val;
  }
  unsigned ldvaruint32() {
    auto [val, sl] = __builtin_tvm_ldvaruint32(sl_);
    sl_ = sl;
    return val;
  }

  int ldvarint16() {
    auto [val, sl] = __builtin_tvm_ldvarint16(sl_);
    sl_ = sl;
    return val;
  }
  int ldvarint32() {
    auto [val, sl] = __builtin_tvm_ldvarint32(sl_);
    sl_ = sl;
    return val;
  }

  cell lddict() {
    auto [val, sl] = __builtin_tvm_lddict(sl_);
    sl_ = sl;
    return val;
  }

  cell ldref() {
    auto [val, sl] = __builtin_tvm_ldref(sl_);
    sl_ = sl;
    return val;
  }

  slice ldrefrtos() {
    auto [sl, val] = __builtin_tvm_ldrefrtos(sl_);
    sl_ = sl;
    return val;
  }

  parser& skip(unsigned len) {
    auto [to_skip, remain_slice] = __builtin_tvm_ldslice(sl_, len);
    sl_ = remain_slice;
    return *this;
  }

  // load msg address as slice
  slice ldmsgaddr() {
    auto [msg_slice, remain_slice] = __builtin_tvm_ldmsgaddr(sl_);
    sl_ = remain_slice;
    return msg_slice;
  }
  msg_address parsemsgaddr() {
    return msg_address(__builtin_tvm_parsemsgaddr(ldmsgaddr()));
  }
  
  slice get_cur_slice() const { return sl_; }
  slice sl() const { return sl_; }

  // Ensure that slice is now empty and throw exception if not
  void ends() const {
    __builtin_tvm_ends(sl_);
  }
private:
  __tvm_slice sl_;
};

} // namespace tvm

