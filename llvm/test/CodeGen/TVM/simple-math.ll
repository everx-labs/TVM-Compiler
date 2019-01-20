; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; CHECK-LABEL: pow2
define i64 @pow2(i64 %x) nounwind {
; stack: [x]
; CHECK: PUSH s0
; stack: [x, x]
; CHECK-NEXT: MUL
  %1 = mul i64 %x, %x
; stack: [x * x]
  ret i64 %1
}

; CHECK-LABEL: pow3
define i64 @pow3(i64 %x) nounwind {
; stack: [x]
; CHECK: PUSH s0
; stack: [x, x]
; CHECK-NEXT: PUSH s0
; stack: [x, x, x]
; CHECK-NEXT: MUL
  %1 = mul i64 %x, %x
; stack: [x * x, x]
; CHECK-NEXT: MUL
  %2 = mul i64 %x, %1
; stack: [x * x * x]
  ret i64 %2
}

; CHECK-LABEL: kill_first
define i64 @kill_first(i64 %killed, i64 %reused, i64 %dummy) nounwind {
; stack: [dummy, reused, killed]
; CHECK: XCHG s0, s2
; stack: [killed, reused, dummy]
; CHECK-NEXT: PUSH s1
; stack: [reused, killed, reused, dummy]
; CHECK-NEXT: SUB
  %1 = sub i64 %killed, %reused
; stack: [killed - reused, reused, dummy]
; CHECK-NEXT: SUBR
  %2 = sub i64 %1, %reused
; stack: [killed - reused - reused, dummy]
; CHECK-NEXT: POP s1
; stack: [killed - reused - reused]
  ret i64 %2
}

; CHECK-LABEL: kill_first_rev
define i64 @kill_first_rev(i64 %reused, i64 %killed, i64 %dummy) nounwind {
; stack: [dummy, killed, reused]
; CHECK: XCHG s0, s1
; stack: [killed, dummy, reused]
; CHECK-NEXT: PUSH s2
; stack: [reused, killed, dummy, killed]
; CHECK-NEXT: SUB
  %1 = sub i64 %killed, %reused
; stack: [killed - reused, dummy, killed]
; CHECK-NEXT: XCHG s0, s2
; stack: [killed, dummy, killed - reused]
; CHECK-NEXT: XCHG s1, s2
; stack: [killed, killed - reused, dummy]
; CHECK-NEXT: SUB
  %2 = sub i64 %1, %reused
; stack: [killed - reused - reused, dummy]
; CHECK-NEXT: POP s1
; stack: [killed - reused - reused]
  ret i64 %2
}

; CHECK-LABEL: kill_last
define i64 @kill_last(i64 %killed, i64 %reused, i64 %dummy) nounwind {
; stack: [dummy, reused, killed]
; CHECK: PUSH s1
; stack: [reused, dummy, reused, killed]
; CHECK-NEXT: XCHG s0, s1
; stack: [dummy, reused, reused, killed]
; CHECK-NEXT: XCHG s0, s3
; stack: [killed, reused, reused, dummy]
; CHECK-NEXT: SUB
  %1 = sub i64 %reused, %killed
; stack: [reused - killed, reused, dummy]
; CHECK-NEXT: SUBR
  %2 = sub i64 %1, %reused
; stack: [reused - killed - reused, dummy]
; CHECK-NEXT: POP s1
; stack: [reused - killed - reused]
  ret i64 %2
}

; CHECK-LABEL: kill_last_rev
define i64 @kill_last_rev(i64 %reused, i64 %killed, i64 %dummy) nounwind {
; stack: [dummy, killed, reused]
; CHECK: PUSH s2
; stack: [reused, dummy, killed, reused]
; CHECK-NEXT: XCHG s0, s1
; stack: [dummy, reused, killed, reused]
; CHECK-NEXT: XCHG s0, s2
; stack: [killed, reused, dummy, reused]
; CHECK-NEXT: SUB
  %1 = sub i64 %reused, %killed
; stack: [reused - killed, dummy, reused]
; CHECK-NEXT: XCHG s0, s2
; stack: [reused, dummy, reused - killed]
; CHECK-NEXT: XCHG s1, s2
; stack: [reused, reused - killed, dummy]
; CHECK-NEXT: SUB
  %2 = sub i64 %1, %reused
; stack: [reused - killed - reused, dummy]
; CHECK-NEXT: POP s1
; stack: [reused - killed - reused]
  ret i64 %2
}

; F = G * m1 * m2 / r2
; CHECK-LABEL: gravity_force
define i64 @gravity_force(i64 %m1, i64 %m2, i64 %g, i64 %r2) nounwind {
; stack: [r2, g, m2, m1]
; CHECK: XCHG s0, s3
; stack: [m1, g, m2, r2]
; CHECK-NEXT: XCHG s1, s2
; stack: [m1, m2, g, r2]
; CHECK-NEXT: MUL
  %1 = mul i64 %m2, %m1
; stack: [m2 * m1, g, r2]
; CHECK-NEXT: MUL
  %2 = mul i64 %1, %g
; stack: [m2 * m1 * g, r2]
; CHECK-NEXT: XCHG s0, s1
; stack: [r2, m2 * m1 * g]
; CHECK-NEXT: DIV
  %3 = sdiv i64 %2, %r2
; stack: [m2 * m1 * g / r2]
  ret i64 %3
}

; CHECK-LABEL: gravity_force2
define i64 @gravity_force2(i64 %r, i64 %g, i64 %m1, i64 %m2) nounwind {
; stack: [m2, m1, g, r]
; CHECK: MUL
  %1 = mul i64 %m2, %m1
; stack: [m2 * m1, g, r]
; CHECK-NEXT: MUL
  %2 = mul i64 %1, %g
; stack: [m2 * m1 * g, r]
; CHECK-NEXT: PUSH s1
; stack: [r, m2 * m1 * g, r]
; CHECK-NEXT: DIV
  %3 = sdiv i64 %2, %r
; stack: [m2 * m1 * g / r, r]
; CHECK-NEXT: XCHG s0, s1
; stack: [r, m2 * m1 * g / r]
; CHECK-NEXT: DIV
  %4 = sdiv i64 %3, %r
; stack: [m2 * m1 * g / r / r]
  ret i64 %4
}
