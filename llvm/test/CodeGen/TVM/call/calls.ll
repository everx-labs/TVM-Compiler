; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
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

define i257 @one() nounwind {
  ret i257 1
}

; CHECK-LABEL: test11
define i257 @test11() nounwind {
; CHECK: PUSHINT $one$
; CHECK: DUP
; CHECK: CALL 1
  %1 = call i257 @one()
; CHECK: CALL 1
  %2 = call i257 @one()
; CHECK: ADD
  %3 = add i257 %1, %2
  ret i257 %3
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Testing parameters
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

define void @nop1(i257 %x) nounwind {
  ret void
}

; CHECK-LABEL: test21
define i257 @test21(i257 %x) nounwind {
; CHECK: PUSHINT $nop$
; CHECK: CALL 1
  call void @nop()
; CHECK: ADD
  %1 = add i257 %x, %x
; CHECK: PUSHINT $nop1$
; CHECK: CALL 1
  call void @nop1(i257 %1)
  ret i257 %1
}


; CHECK-LABEL: pow2
define i257 @pow2(i257 %x) nounwind {
; CHECK: DUP
; CHECK: MUL
  %1 = mul i257 %x, %x
  ret i257 %1
}

; CHECK-LABEL: test22
define i257 @test22(i257 %x) nounwind {
; CHECK: ADD
  %1 = add i257 %x, %x
; CHECK: PUSHINT $pow2$
; CHECK: CALL 1
  %2 = call i257 @pow2(i257 %1)
  ret i257 %2
}

; CHECK-LABEL: test23
define i257 @test23(i257 %x, i257 %y) nounwind {
; CHECK: PUSHINT $pow2$
; CHECK: CALL 1
  %1 = call i257 @pow2(i257 %x)
; CHECK: CALL 1
  %2 = call i257 @pow2(i257 %y)
; CHECK: ADD
  %3 = add i257 %1, %2
  ret i257 %3
}

define i257 @sum(i257 %x, i257 %y) nounwind {
  %1 = add i257 %x, %y
  ret i257 %1
}

; CHECK-LABEL: test24
define i257 @test24(i257 %x, i257 %y) nounwind {
; [x, y]
; CHECK: PUSHINT $sum$
; [x, y, $sum]
; [x, y, $sum, x, y, $sum]
; CHECK: CALL 1
; [x, y, $sum, x + y]
  %1 = call i257 @sum(i257 %x, i257 %y)
; CHECK: CALL 1
; [x + y, y + x]
  %2 = call i257 @sum(i257 %y, i257 %x)
; [x + y, y + x]
  %3 = mul i257 %1, %2
; CHECK: MUL
  ret i257 %3
}

; CHECK-LABEL: test25
define i257 @test25(i257 %x, i257 %y, i257 %z) nounwind {
; CHECK: PUSHINT $sum$
; CHECK: CALL 1
  %1 = call i257 @sum(i257 %x, i257 %y)
; CHECK: CALL 1
  %2 = call i257 @sum(i257 %1, i257 %z)
  ret i257 %2
}
