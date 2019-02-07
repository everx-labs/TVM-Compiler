// RUN: %clang_cc1 -Wall -Werror -triple tvm -emit-llvm -o - %s | FileCheck %s

#include <stdint.h>

int64_t div(int64_t a, int64_t b) {
  // CHECK: call {{.*}} @llvm.tvm.div
  return __builtin_tvm_div(a, b);
}
