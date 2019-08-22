// RUN: %clang_cc1 -Wall -Werror -triple tvm-unknown-unknown -nostdsysteminc -emit-llvm -o - %s | FileCheck %s
// REQUIRES: tvm-registered-target

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

void set_persistent_data() {
  // CHECK: call {{.*}} @llvm.tvm.set.persistent.data
  __builtin_tvm_set_persistent_data(__builtin_tvm_get_persistent_data());
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

int64_t stu(int64_t slice, int64_t value, int64_t precision) {
  // CHECK: call {{.*}} @llvm.tvm.stu
  return __builtin_tvm_stu(slice, value, precision);
}

int64_t newc() {
  // CHECK: call {{.*}} @llvm.tvm.newc
  return __builtin_tvm_newc();
}

int64_t endc(int64_t slice) {
  // CHECK: call {{.*}} @llvm.tvm.endc
  return __builtin_tvm_endc(slice);
}

void sendrawmsg(int64_t cell, int64_t flags) {
  // CHECK: call {{.*}} @llvm.tvm.sendrawmsg
  __builtin_tvm_sendrawmsg(cell, flags);
}

int64_t bitsize(int64_t n) {
  // CHECK: call {{.*}} @llvm.tvm.bitsize
  return __builtin_tvm_bitsize(n);
}

int64_t ubitsize(int64_t n) {
  // CHECK: call {{.*}} @llvm.tvm.ubitsize
  return __builtin_tvm_ubitsize(n);
}

int64_t fitsx(int64_t n, int64_t c) {
  // CHECK: call {{.*}} @llvm.tvm.fitsx
  return __builtin_tvm_fitsx(n, c);
}

int64_t ufitsx(int64_t n, int64_t c) {
  // CHECK: call {{.*}} @llvm.tvm.ufitsx
  return __builtin_tvm_ufitsx(n, c);
}
