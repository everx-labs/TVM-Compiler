#pragma once

#include <tvm/cell.hpp>
#include <tvm/slice.hpp>

namespace tvm {

class __attribute((tvm_tuple)) builder {
public:
  builder() : bldr_(__builtin_tvm_newc()) {}
  explicit builder(__tvm_builder bldr) : bldr_(bldr) {}

  builder& stslice(slice sl) {
    bldr_ = __builtin_tvm_stslice(sl, bldr_);
    return *this;
  }
  builder& stu(unsigned val, unsigned len) {
    bldr_ = __builtin_tvm_stu(val, bldr_, len);
    return *this;
  }
  builder& sti(int val, unsigned len) {
    bldr_ = __builtin_tvm_sti(val, bldr_, len);
    return *this;
  }
  builder& stvaruint16(unsigned val) {
    bldr_ = __builtin_tvm_stvaruint16(bldr_, val);
    return *this;
  }
  builder& stvaruint32(unsigned val) {
    bldr_ = __builtin_tvm_stvaruint32(bldr_, val);
    return *this;
  }
  builder& stvarint16(int val) {
    bldr_ = __builtin_tvm_stvarint16(bldr_, val);
    return *this;
  }
  builder& stvarint32(int val) {
    bldr_ = __builtin_tvm_stvarint32(bldr_, val);
    return *this;
  }

  builder& stdict(cell dict) {
    bldr_ = __builtin_tvm_stdict(dict, bldr_);
    return *this;
  }
  builder& stdicts(slice dict) {
    bldr_ = __builtin_tvm_stdicts(dict, bldr_);
    return *this;
  }
  builder& stref(cell cl) {
    bldr_ = __builtin_tvm_stref(cl, bldr_);
    return *this;
  }

  unsigned bbits() const { return __builtin_tvm_bbits(bldr_); }
  unsigned brembits() const { return __builtin_tvm_brembits(bldr_); }

  cell make_cell() const { return  { __builtin_tvm_endc(bldr_) }; }
  cell endc() const { return make_cell(); }
  slice make_slice() const { return make_cell().ctos(); }

  __tvm_builder get() const { return bldr_; }
private:
  __tvm_builder bldr_;
};

} // namespace tvm

