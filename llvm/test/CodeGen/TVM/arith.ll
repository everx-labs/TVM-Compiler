; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
; XFAIL: *
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: addi257
define i257 @addi257(i257 %par) nounwind {
; CHECK: {{PUSHINT (%[0-9], )*2345}}
; CHECK: ADD
  %1 = add i257 %par, 2345
  ret i257 %1
}

; CHECK-LABEL: addi257
define i257 @addcommutative(i257 %a, i257 %b) {
  ; CHECK-NOT: XCHG
  ; CHECK: ADD
  %1 = add i257 %b, %a
  ret i257 %1
}

; CHECK-LABEL: inci257
define i257 @inci257(i257 %par) nounwind {
; CHECK: INC
  %1 = add i257 %par, 1
  ret i257 %1
}

; CHECK-LABEL: addi257two
define i257 @addi257two(i257 %par) nounwind {
; CHECK: ADDCONST 2
  %1 = add i257 %par, 2
	ret i257 %1
}

; CHECK-LABEL: addi257twoReversed
define i257 @addi257twoReversed(i257 %par) nounwind {
; CHECK: ADDCONST 2
  %1 = add i257 2, %par
	ret i257 %1
}

; CHECK-LABEL: subi257const
define i257 @subi257const(i257 %par) nounwind {
; CHECK: ADDCONST -2
  %1 = sub i257 %par, 2
  ret i257 %1
}

; CHECK-LABEL: subi257constReversed
define i257 @subi257constReversed(i257 %par) nounwind {
; CHECK: PUSHINT 2
; CHECK-NEXT: SUBR
  %1 = sub i257 2, %par
  ret i257 %1
}

; CHECK-LABEL: bug2019.06.26
define i257 @bug2019.06.26(i257 %x) nounwind {
entry:
; CHECK: XCHG
; CHECK-NEXT: PUSH
  %mul = mul nsw i257 %x, %x
  %rem = urem i257 %mul, 7
  ret i257 %rem
}
