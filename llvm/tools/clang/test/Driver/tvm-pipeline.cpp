// RUN: %clang -target tvm -c -### %s 2>&1 \
// RUN:   | FileCheck %s
// CHECK: warning: TVM doesn't support assembler; -c flag ignored
// CHECK: clang
// CHECK: llvm-link
// CHECK: opt
// CHECK: opt
// CHECK: llc
// CHECK: tvm_linker

// RUN: %clang -target tvm -o - -### %s 2>&1 \
// RUN:   | FileCheck %s -check-prefix=CHECKO
// CHECKO: clang
// CHECKO: llvm-link
// CHECKO: opt
// CHECKO: opt
// CHECKO: llc
// CHECKO: tvm_linker

// RUN: %clang -target tvm -c -emit-llvm -### %s 2>&1 \
// RUN:   | FileCheck %s -check-prefix=EMITLLVM
// EMITLLVM-NOT: warning: TVM doesn't support assembler; -c flag ignored
