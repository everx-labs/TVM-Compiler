; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define void @nop() nounwind {
  ret void
}

; CHECK-LABEL: test0
define void @test0() nounwind {
; CHECK: PUSHCONT nop
; CHECK-NEXT: CALLX
  call void @nop()
  ret void
}

; CHECK-LABEL: test00
define void @test00() nounwind {
; CHECK: PUSHCONT nop
; CHECK-NEXT: CALLX
; CHECK-NEXT: PUSHCONT nop
; CHECK-NEXT: CALLX
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
; CHECK: PUSHCONT one
; CHECK-NEXT: CALLX
  %1 = call i64 @one()
; CHECK-NEXT: PUSHCONT one
; CHECK-NEXT: CALLX
  %2 = call i64 @one()
; CHECK-NEXT: ADD
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
; CHECK: PUSHCONT nop
; CHECK-NEXT: CALLX
  call void @nop()
; CHECK-NEXT: PUSH s0
; CHECK-NEXT: ADD
  %1 = add i64 %x, %x
; CHECK: PUSHCONT nop1
; CHECK-NEXT: CALLX
  call void @nop1(i64 %1)
  ret i64 %1
}


; CHECK-LABEL: pow2
define i64 @pow2(i64 %x) nounwind {
; CHECK: PUSH s0
; CHECK-NEXT: MUL
  %1 = mul i64 %x, %x
  ret i64 %1
}

; CHECK-LABEL: test22
define i64 @test22(i64 %x) nounwind {
; CHECK: PUSH s0
; CHECK-NEXT: ADD
  %1 = add i64 %x, %x
; CHECK: PUSHCONT pow2
; CHECK-NEXT: CALLX
  %2 = call i64 @pow2(i64 %1)
  ret i64 %2
}

; CHECK-LABEL: test23
define i64 @test23(i64 %x, i64 %y) nounwind {
; CHECK: XCHG s0, s1
; CHECK-NEXT: PUSHCONT pow2
; CHECK-NEXT: CALLX
  %1 = call i64 @pow2(i64 %x)
; CHECK-NEXT: XCHG s0, s1
; CHECK-NEXT: PUSHCONT pow2
; CHECK-NEXT: CALLX
  %2 = call i64 @pow2(i64 %y)
; CHECK-NEXT: ADD
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
; CHECK: PUSH s1
; [x, y, x]
; CHECK-NEXT: PUSH s1
; [x, y, x, y]
; CHECK-NEXT: PUSHCONT sum
; CHECK-NEXT: CALLX
; [x, y, x + y]
  %1 = call i64 @sum(i64 %x, i64 %y)
; CHECK-NEXT: XCHG s0, s2
; [x + y, y, x]
; CHECK-NEXT: PUSHCONT sum
; CHECK-NEXT: CALLX
; [x + y, y + x]
  %2 = call i64 @sum(i64 %y, i64 %x)
  %3 = mul i64 %1, %2
; CHECK-NEXT: MUL
  ret i64 %3
}

; CHECK-LABEL: test25
define i64 @test25(i64 %x, i64 %y, i64 %z) nounwind {
; [z, y, x]
; CHECK: XCHG s0, s1
; CHECK-NEXT: XCHG s1, s2
; CHECK-NEXT: PUSHCONT sum
; [@sum, y, x, z]
; CHECK-NEXT: CALLX
  %1 = call i64 @sum(i64 %x, i64 %y)
; [x + y, z]
; CHECK-NEXT: XCHG s0, s1
; CHECK-NEXT: PUSHCONT sum
; [@sum, z, x + y]
; CHECK-NEXT: CALLX
  %2 = call i64 @sum(i64 %1, i64 %z)
  ret i64 %2
}
