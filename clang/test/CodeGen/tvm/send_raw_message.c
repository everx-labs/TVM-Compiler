// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s -o - | FileCheck %s
// REQUIRES: tvm-registered-target

__tvm_cell make_internal_msg_cell(unsigned to_addr, unsigned value) {
  __tvm_builder bldr = __builtin_tvm_newc();
  bldr = __builtin_tvm_stu(0, bldr, 5);
  bldr = __builtin_tvm_stu(-1, bldr, 8);
  bldr = __builtin_tvm_stu(to_addr, bldr, 256);
  bldr = __builtin_tvm_stu(value, bldr, 32);
  __tvm_cell cell = __builtin_tvm_endc(bldr);
  return cell;
}

void send_raw_message(unsigned to_addr, unsigned value) {
// CHECK: void @send_raw_message
// CHECK: @llvm.tvm.newc
// CHECK: @llvm.tvm.stu
// CHECK: @llvm.tvm.stu
// CHECK: @llvm.tvm.stu
// CHECK: @llvm.tvm.stu
// CHECK: @llvm.tvm.endc
// CHECK: @llvm.tvm.sendrawmsg

  __tvm_cell cell = make_internal_msg_cell(to_addr, value);
  __builtin_tvm_sendrawmsg(cell, 0);
}

