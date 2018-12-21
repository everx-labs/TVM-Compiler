; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; F = G * m1 * m2 / r2
; CHECK-LABEL: gravity_force
define i64 @gravity_force(i64 %m1, i64 %m2, i64 %g, i64 %r2) nounwind {
; CHECK: MUL
; CHECK: MUL
; CHECK: DIV
  %1 = mul i64 %m2, %m1
  %2 = mul i64 %1, %g
  %3 = sdiv i64 %2, %r2
  ret i64 %3
}

; CHECK-LABEL: gravity_force2
define i64 @gravity_force2(i64 %g, i64 %r, i64 %m1, i64 %m2) nounwind {
; CHECK: MUL
; CHECK: DIV
; CHECK: DIV
; CHECK: MUL
  %1 = mul i64 %m2, %m1
  %2 = sdiv i64 %1, %r
  %3 = sdiv i64 %2, %r
  %4 = mul i64 %3, %g
  ret i64 %4
}
