// RUN: %clang_cc1 -Wall -Werror -triple tvm-unknown-unknown -nostdsysteminc -emit-llvm -o - %s | FileCheck %s

#include <stdint.h>

int64_t div(int64_t a, int64_t b) {
  // CHECK: call {{.*}} @llvm.tvm.div
  return __builtin_tvm_div(a, b);
}

int64_t mod(int64_t a, int64_t b) {
  // CHECK: call {{.*}} @llvm.tvm.mod
  return __builtin_tvm_mod(a, b);
}

int64_t newdict() {
  // CHECK: call {{.*}} @llvm.tvm.newdict
  return __builtin_tvm_newdict();
}

int64_t get_persistent_data() {
  // CHECK: call {{.*}} @llvm.tvm.get.persistent.data
  return __builtin_tvm_get_persistent_data();
}

int64_t inttoslice(int64_t n) {
  // CHECK: call {{.*}} @llvm.tvm.inttoslice
  return __builtin_tvm_inttoslice(n);
}

int64_t ctos(int64_t n) {
  // CHECK: call {{.*}} @llvm.tvm.ctos
  return __builtin_tvm_ctos(n);
}

int64_t stslice(int64_t n, int64_t slice) {
  // CHECK: call {{.*}} @llvm.tvm.stslice
  return __builtin_tvm_stslice(n, slice);
}

void throws(int64_t cond) {
  // CHECK: call void @llvm.tvm.throwif
  __builtin_tvm_throwif(cond, 42);
}
