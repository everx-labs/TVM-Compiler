; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define void @nop() nounwind {
  ret void
}

; CHECK-LABEL: test0
define void @test0() nounwind {
; CHECK: PUSHINT $nop$
; CHECK: CALL 1
  call void @nop()
  ret void
}

; CHECK-LABEL: test00
define void @test00() nounwind {
; CHECK: PUSHINT $nop$
; CHECK: DUP
; CHECK: CALL 1
; CHECK: CALL 1
  call void @nop()
  call void @nop()
  ret void
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Testing return value
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

define i64 @one() nounwind {
  ret i64 1
}

; CHECK-LABEL: test11
define i64 @test11() nounwind {
; CHECK: PUSHINT $one$
; CHECK: DUP
; CHECK: CALL 1
  %1 = call i64 @one()
; CHECK: CALL 1
  %2 = call i64 @one()
; CHECK: ADD
  %3 = add i64 %1, %2
  ret i64 %3
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Testing parameters
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

define void @nop1(i64 %x) nounwind {
  ret void
}

; CHECK-LABEL: test21
define i64 @test21(i64 %x) nounwind {
; CHECK: PUSHINT $nop$
; CHECK: CALL 1
  call void @nop()
; CHECK: PUSH s1
; CHECK: XCHG s1, s2
; CHECK: ADD
  %1 = add i64 %x, %x
; CHECK: PUSHINT $nop1$
; CHECK: PUSH s1
; CHECK: CALL 1
  call void @nop1(i64 %1)
  ret i64 %1
}


; CHECK-LABEL: pow2
define i64 @pow2(i64 %x) nounwind {
; CHECK: DUP
; CHECK: MUL
  %1 = mul i64 %x, %x
  ret i64 %1
}

; CHECK-LABEL: test22
define i64 @test22(i64 %x) nounwind {
; CHECK: PUSH s1
; CHECK: XCHG s1, s2
; CHECK-NEXT: ADD
  %1 = add i64 %x, %x
; CHECK: PUSHINT $pow2$
; CHECK: XCHG s0, s1
; CHECK: CALL 1
  %2 = call i64 @pow2(i64 %1)
  ret i64 %2
}

; CHECK-LABEL: test23
define i64 @test23(i64 %x, i64 %y) nounwind {
; CHECK: PUSH c0
; CHECK: PUSHINT $pow2$
; CHECK: DUP
; CHECK: XCHG s0, s4
; CHECK: XCHG s2, s4
; CHECK: XCHG s3, s4
; CHECK: CALL 1
  %1 = call i64 @pow2(i64 %x)
; CHECK: XCHG s0, s3
; CHECK: XCHG s2, s3
; CHECK: CALL 1
  %2 = call i64 @pow2(i64 %y)
; CHECK: XCHG s0, s2
; CHECK: XCHG s1, s2
; CHECK: POP c0
; CHECK: ADD
  %3 = add i64 %1, %2
  ret i64 %3
}

define i64 @sum(i64 %x, i64 %y) nounwind {
  %1 = add i64 %x, %y
  ret i64 %1
}

; CHECK-LABEL: test24
define i64 @test24(i64 %x, i64 %y) nounwind {
; [x, y]
; CHECK: PUSH c0
; [x, y, c0]
; CHECK: PUSHINT $sum$
; [x, y, c0, $sum]
; CHECK: PUSH s2
; [x, y, c0, $sum, y]
; CHECK: PUSH s4
; [x, y, c0, $sum, y, x]
; CHECK: PUSH s2
; [x, y, c0, $sum, y, x, $sum]
; CHECK: XCHG s0, s2
; [x, y, c0, $sum, $sum, x, y]
; CHECK-NEXT: CALL 1
; [x, y, c0, $sum, x + y]
  %1 = call i64 @sum(i64 %x, i64 %y)
; CHECK: XCHG s0, s4
; [x + y, y, c0, $sum, x]
; CHECK: XCHG s1, s3
; [x + y, $sum, c0, y, x]
; CHECK: XCHG s2, s3
; [x + y, c0, $sum, y, x]
; CHECK: XCHG s3, s4
; [c0, x + y, $sum, y, x]
; CHECK: CALL 1
; [c0, x + y, y + x]
  %2 = call i64 @sum(i64 %y, i64 %x)
; CHECK: XCHG s0, s2
; [y + x, x + y, c0]
; CHECK: XCHG s1, s2
; [x + y, y + x, c0]
; CHECK: POP c0
; [x + y, y + x]
  %3 = mul i64 %1, %2
; CHECK: MUL
  ret i64 %3
}

; CHECK-LABEL: test25
define i64 @test25(i64 %x, i64 %y, i64 %z) nounwind {
<<<<<<< HEAD
; [z, y, x]
; CHECK: XCHG s0, s2
; CHECK-NEXT: XCHG s1, s3
; CHECK-NEXT: PUSHINT $sum$
; [@sum, y, x, z]
; CHECK-NEXT: CALL 1
  %1 = call i64 @sum(i64 %x, i64 %y)
; [x + y, z]
; CHECK-NEXT: XCHG s0, s2
; CHECK-NEXT: XCHG s1, s2
; CHECK-NEXT: PUSHINT $sum$
; [@sum, z, x + y]
; CHECK-NEXT: CALL 1
=======
; CHECK: PUSH c0
; CHECK: PUSHINT $sum$
; CHECK: DUP
; CHECK: XCHG s0, s4
; CHECK: XCHG s1, s5
; CHECK: XCHG s2, s4
; CHECK: XCHG s3, s5
; CHECK: CALL 1
  %1 = call i64 @sum(i64 %x, i64 %y)
; CHECK: XCHG s0, s3
; CHECK: XCHG s1, s3
; CHECK: XCHG s2, s3
; CHECK: CALL 1
>>>>>>> Fix of code generation for global address and external symbol operands
  %2 = call i64 @sum(i64 %1, i64 %z)
  ret i64 %2
}
