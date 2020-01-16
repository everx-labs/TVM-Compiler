// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s -o - | FileCheck %s
// REQUIRES: tvm-registered-target

__tvm_cell map_persistent;

// CHECK: @map_persistent = weak dso_local local_unnamed_addr global cell bitcast (i257 0 to cell), align 1
