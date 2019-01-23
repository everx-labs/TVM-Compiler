; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define i64 @addi64(i64 %par) nounwind {
; CHECK: {{PUSHINT (%[0-9], )*2345}}
; CHECK: ADD
  %1 = add i64 %par, 2345
  ret i64 %1
}
