// RUN: %clang -target tvm -S -emit-llvm %s -o - | FileCheck %s
// REQUIRES: tvm-registered-target

// CHECK: @.str = private unnamed_addr constant [4 x i257] [i257 98, i257 97, i257 114, i257 0], align 1
char foo(int n) { return "bar"[n]; }

// CHECK: @str = dso_local local_unnamed_addr global [8 x i257] [i257 115, i257 116, i257 114, i257 0, i257 0, i257 0, i257 0, i257 0], align 1
char str[8] = "str";
