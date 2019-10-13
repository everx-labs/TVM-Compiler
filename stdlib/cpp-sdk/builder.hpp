#pragma once

namespace tvm {

class builder {
public:
  builder() : bldr_(__builtin_tvm_newc()) {}

  builder& stu(unsigned val, unsigned len) {
    bldr_ = __builtin_tvm_stu(val, bldr_, len);
    return *this;
  }
  
  __tvm_cell make_cell() const { return __builtin_tvm_endc(bldr_); }
private:
  __tvm_builder bldr_;
};

} // namespace tvm

