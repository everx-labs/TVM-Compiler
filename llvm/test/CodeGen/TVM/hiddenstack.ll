; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

declare i257 @llvm.tvm.hiddenstack(i257 %index)

; CHECK-LABEL: hstack:
define i257 @hstack() nounwind {
; CHECK: PUSH s1
  %1 = call i257 @llvm.tvm.hiddenstack(i257 1)
  ret i257 %1
}

; CHECK-LABEL: hstack_args:
define i257 @hstack_args(i257 %a, i257 %b, i257 %c) nounwind {
; CHECK: { %{{[0-9]+}} | %{{[0-9]+}} | %{{[0-9]+}} | - }
; CHECK: PUSH s10
; CHECK: { %{{[0-9]+}} | %{{[0-9]+}} | %{{[0-9]+}} | %{{[0-9]+}} | - }
  %1 = call i257 @llvm.tvm.hiddenstack(i257 7)
; CHECK: ADD
; CHECK: ADD
; CHECK: ADD
  %2 = add i257 %1, %a
  %3 = add i257 %2, %b
  %4 = add i257 %3, %c
  ret i257 %4
}
