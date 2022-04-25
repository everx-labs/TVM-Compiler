; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
; XFAIL:*
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: pow2
define i257 @pow2(i257 %x) nounwind {
; stack: [x]
; CHECK: PUSH s0
; stack: [x, x]
; CHECK-NEXT: MUL
  %1 = mul i257 %x, %x
; stack: [x * x]
  ret i257 %1
}

; CHECK-LABEL: pow3
define i257 @pow3(i257 %x) nounwind {
; stack: [x]
; CHECK: PUSH s0
; stack: [x, x]
; CHECK-NEXT: PUSH s0
; stack: [x, x, x]
; CHECK-NEXT: MUL
  %1 = mul i257 %x, %x
; stack: [x * x, x]
; CHECK-NEXT: MUL
  %2 = mul i257 %x, %1
; stack: [x * x * x]
  ret i257 %2
}

; CHECK-LABEL: kill_first
define i257 @kill_first(i257 %killed, i257 %reused, i257 %dummy) nounwind {
; stack: [dummy, reused, killed]
; CHECK: XCHG s0, s2
; stack: [killed, reused, dummy]
; CHECK-NEXT: PUSH s1
; stack: [reused, killed, reused, dummy]
; CHECK-NEXT: SUB
  %1 = sub i257 %killed, %reused
; stack: [killed - reused, reused, dummy]
; CHECK-NEXT: SUBR
  %2 = sub i257 %1, %reused
; stack: [killed - reused - reused, dummy]
; CHECK-NEXT: POP s1
; stack: [killed - reused - reused]
  ret i257 %2
}

; CHECK-LABEL: kill_first_rev
define i257 @kill_first_rev(i257 %reused, i257 %killed, i257 %dummy) nounwind {
; stack: [dummy, killed, reused]
; CHECK: XCHG s0, s1
; stack: [killed, dummy, reused]
; CHECK-NEXT: PUSH s2
; stack: [reused, killed, dummy, killed]
; CHECK-NEXT: SUB
  %1 = sub i257 %killed, %reused
; stack: [killed - reused, dummy, killed]
; CHECK-NEXT: XCHG s0, s2
; stack: [killed, dummy, killed - reused]
; CHECK-NEXT: XCHG s1, s2
; stack: [killed, killed - reused, dummy]
; CHECK-NEXT: SUB
  %2 = sub i257 %1, %reused
; stack: [killed - reused - reused, dummy]
; CHECK-NEXT: POP s1
; stack: [killed - reused - reused]
  ret i257 %2
}

; CHECK-LABEL: kill_last
define i257 @kill_last(i257 %killed, i257 %reused, i257 %dummy) nounwind {
; stack: [dummy, reused, killed]
; CHECK: PUSH s1
; stack: [reused, dummy, reused, killed]
; CHECK-NEXT: XCHG s0, s1
; stack: [dummy, reused, reused, killed]
; CHECK-NEXT: XCHG s0, s3
; stack: [killed, reused, reused, dummy]
; CHECK-NEXT: SUB
  %1 = sub i257 %reused, %killed
; stack: [reused - killed, reused, dummy]
; CHECK-NEXT: SUBR
  %2 = sub i257 %1, %reused
; stack: [reused - killed - reused, dummy]
; CHECK-NEXT: POP s1
; stack: [reused - killed - reused]
  ret i257 %2
}

; CHECK-LABEL: kill_last_rev
define i257 @kill_last_rev(i257 %reused, i257 %killed, i257 %dummy) nounwind {
; stack: [dummy, killed, reused]
; CHECK: PUSH s2
; stack: [reused, dummy, killed, reused]
; CHECK-NEXT: XCHG s0, s1
; stack: [dummy, reused, killed, reused]
; CHECK-NEXT: XCHG s0, s2
; stack: [killed, reused, dummy, reused]
; CHECK-NEXT: SUB
  %1 = sub i257 %reused, %killed
; stack: [reused - killed, dummy, reused]
; CHECK-NEXT: XCHG s0, s2
; stack: [reused, dummy, reused - killed]
; CHECK-NEXT: XCHG s1, s2
; stack: [reused, reused - killed, dummy]
; CHECK-NEXT: SUB
  %2 = sub i257 %1, %reused
; stack: [reused - killed - reused, dummy]
; CHECK-NEXT: POP s1
; stack: [reused - killed - reused]
  ret i257 %2
}

; F = G * m1 * m2 / r2
; CHECK-LABEL: gravity_force
define i257 @gravity_force(i257 %m1, i257 %m2, i257 %g, i257 %r2) nounwind {
; stack: [r2, g, m2, m1]
; CHECK: XCHG s0, s3
; stack: [m1, g, m2, r2]
; CHECK-NEXT: XCHG s1, s2
; stack: [m1, m2, g, r2]
; CHECK-NEXT: MUL
  %1 = mul i257 %m2, %m1
; stack: [m2 * m1, g, r2]
; CHECK-NEXT: MUL
  %2 = mul i257 %1, %g
; stack: [m2 * m1 * g, r2]
; CHECK-NEXT: XCHG s0, s1
; stack: [r2, m2 * m1 * g]
; CHECK-NEXT: DIV
  %3 = call i257 @llvm.tvm.div(i257 %2, i257 %r2)
; stack: [m2 * m1 * g / r2]
  ret i257 %3
}

; CHECK-LABEL: gravity_force2
define i257 @gravity_force2(i257 %r, i257 %g, i257 %m1, i257 %m2) nounwind {
; stack: [m2, m1, g, r]
; CHECK: MUL
  %1 = mul i257 %m2, %m1
; stack: [m2 * m1, g, r]
; CHECK-NEXT: MUL
  %2 = mul i257 %1, %g
; stack: [m2 * m1 * g, r]
; CHECK-NEXT: PUSH s1
; stack: [r, m2 * m1 * g, r]
; CHECK-NEXT: DIV
  %3 = call i257 @llvm.tvm.div(i257 %2, i257 %r)
; stack: [m2 * m1 * g / r, r]
; CHECK-NEXT: XCHG s0, s1
; stack: [r, m2 * m1 * g / r]
; CHECK-NEXT: DIV
  %4 = call i257 @llvm.tvm.div(i257 %3, i257 %r)
; stack: [m2 * m1 * g / r / r]
  ret i257 %4
}

declare i257 @llvm.tvm.div(i257, i257) nounwind
