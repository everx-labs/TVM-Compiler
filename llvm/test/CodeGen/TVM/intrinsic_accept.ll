; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: nounwind
; CHECK-LABEL: perform_accept
define void @perform_accept() {
entry:
; CHECK: ACCEPT
  call void @llvm.tvm.accept()
  ret void
}

declare void @llvm.tvm.accept() #1
