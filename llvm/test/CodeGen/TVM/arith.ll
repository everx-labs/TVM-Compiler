; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; CHECK-LABEL: addi64
define i64 @addi64(i64 %par) nounwind {
; CHECK: {{PUSHINT (%[0-9], )*2345}}
; CHECK: ADD
  %1 = add i64 %par, 2345
  ret i64 %1
}

; CHECK-LABEL: inci64
define i64 @inci64(i64 %par) nounwind {
; CHECK: INC
  %1 = add i64 %par, 1
  ret i64 %1
}

; CHECK-LABEL: addi64two
define i64 @addi64two(i64 %par) nounwind {
; CHECK: ADDCONST 2
  %1 = add i64 %par, 2
	ret i64 %1
}

; CHECK-LABEL: addi64twoReversed
define i64 @addi64twoReversed(i64 %par) nounwind {
; CHECK: ADDCONST 2
  %1 = add i64 2, %par
	ret i64 %1
}

; CHECK-LABEL: subi64const
define i64 @subi64const(i64 %par) nounwind {
; CHECK: ADDCONST -2
  %1 = sub i64 %par, 2
  ret i64 %1
}

; CHECK-LABEL: subi64constReversed
define i64 @subi64constReversed(i64 %par) nounwind {
; CHECK: PUSHINT 2
; CHECK-NEXT: SUBR
  %1 = sub i64 2, %par
  ret i64 %1
}

; CHECK-LABEL: bug2019.06.26
define i64 @bug2019.06.26(i64 %x) nounwind {
entry:
; CHECK: XCHG
; CHECK-NEXT: PUSH
  %mul = mul nsw i64 %x, %x
  %rem = urem i64 %mul, 7
  ret i64 %rem
}
