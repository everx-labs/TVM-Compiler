; XFAIL: *

; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define i64 @addi64(i64 %par) nounwind {
; CHECK: PUSHINT 1
; CHECK: ADD
  %1 = add i64 %par, 1
	ret i64 %1
}
