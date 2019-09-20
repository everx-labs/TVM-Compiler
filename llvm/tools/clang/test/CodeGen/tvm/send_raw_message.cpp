// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s -o - | FileCheck %s
// REQUIRES: tvm-registered-target

class Builder {
public:
  Builder() : bldr_(__builtin_tvm_newc()) {}

  Builder& stu(unsigned val, unsigned len) {
    bldr_ = __builtin_tvm_stu(val, bldr_, len);
    return *this;
  }
  
  __tvm_cell make_cell() const { return __builtin_tvm_endc(bldr_); }
private:
  __tvm_builder bldr_;
};

__tvm_cell make_internal_msg_cell(unsigned to_addr, unsigned value) {
  Builder bldr;
  bldr.stu(0, 5).stu(-1, 8).stu(to_addr, 256).stu(value, 32);
  return bldr.make_cell();
}

void send_raw_message(unsigned to_addr, unsigned value) {
// CHECK: send_raw_message
// CHECK: @llvm.tvm.newc
// CHECK: @llvm.tvm.stu
// CHECK: @llvm.tvm.stu
// CHECK: @llvm.tvm.stu
// CHECK: @llvm.tvm.stu
// CHECK: @llvm.tvm.endc
// CHECK: @llvm.tvm.sendrawmsg

  auto cell = make_internal_msg_cell(to_addr, value);
  __builtin_tvm_sendrawmsg(cell, 0);
}

