; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define i64 @sum(i64 %x, i64 %y) nounwind {
  %1 = add i64 %x, %y
  ret i64 %1
}

; CHECK-LABEL: test24
define i64 @test24(i64 %x, i64 %y, i64 %z) nounwind {
  %1 = call i64 @sum(i64 %x, i64 %y)
  %2 = call i64 @sum(i64 %1, i64 %z)
  ret i64 %2
}
