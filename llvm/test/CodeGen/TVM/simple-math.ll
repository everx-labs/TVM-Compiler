; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; F = G * m1 * m2 / r2
; CHECK-LABEL: gravity_force
define i64 @gravity_force(i64 %g, i64 %r2, i64 %m1, i64 %m2) nounwind {
; CHECK: MUL
; CHECK: MUL
; CHECK: DIV
  %1 = mul i64 %m2, %m1
  %2 = mul i64 %1, %r2
  %3 = sdiv i64 %2, %g
  ret i64 %3
}

; CHECK-LABEL: gravity_force2
define i64 @gravity_force2(i64 %m1, i64 %m2, i64 %g, i64 %r2) nounwind {
; CHECK: MUL
; CHECK: MUL
; CHECK: DIV
  %1 = mul i64 %m2, %m1
  %2 = mul i64 %1, %r2
  %3 = sdiv i64 %2, %g
  ret i64 %3
}
