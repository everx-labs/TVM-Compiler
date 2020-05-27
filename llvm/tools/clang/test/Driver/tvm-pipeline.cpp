// RUN: %clang -target tvm -c -### %s 2>&1 \
// RUN:   | FileCheck %s
// CHECK: warning: TVM doesn't support assembler; -c flag ignored
// CHECK: clang
// CHECK-NEXT: tvm_linker
