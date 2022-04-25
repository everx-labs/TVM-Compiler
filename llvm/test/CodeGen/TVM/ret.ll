; RUN: llc < %s -march=tvm | FileCheck %s
; RUN: llc < %s -march=tvm --disable-tvm-return-opt | FileCheck %s -check-prefix=NOOPT
; XFAIL:*
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: retv:
; NOOPT-LABEL: retv:
define void @retv() nounwind {
; CHECK-NOT: {{\tRET}}
; NOOPT: RET
  ret void
}

; CHECK-LABEL: reti8:
; NOOPT-LABEL: reti8:
define i8 @reti8() nounwind {
; CHECK: PUSHINT 42
; CHECK-NOT: {{\tRET}}
; NOOPT: RET
  ret i8 42
}

; CHECK-LABEL: reti16:
; NOOPT-LABEL: reti16:
define i16 @reti16() nounwind {
; CHECK: PUSHINT 42
; CHECK-NOT: {{\tRET}}
; NOOPT: RET
  ret i16 42
}

; CHECK-LABEL: reti32:
; NOOPT-LABEL: reti32:
define i32 @reti32() nounwind {
; CHECK: PUSHINT 42
; CHECK-NOT: {{\tRET}}
; NOOPT: RET
  ret i32 42
}

; CHECK-LABEL: reti64:
; NOOPT-LABEL: reti64:
define i257 @reti64() nounwind {
; CHECK: PUSHINT 42
; CHECK-NOT: {{\tRET}}
; NOOPT: RET
  ret i257 42
}

; CHECK-LABEL: retai64:
; NOOPT-LABEL: retai64:
define i257 @retai64(i257 %arg) nounwind {
; CHECK-NOT: {{\tRET}}
; NOOPT: RET
  ret i257 %arg
}

; CHECK-LABEL: retraai64:
; NOOPT-LABEL: retraai64:
define i257 @retraai64(i257 %arg1, i257 %arg2, i257 %arg3) nounwind {
; CHECK: POP s0
; CHECK: POP s0
; CHECK-NOT: {{\tRET}}
; NOOPT: POP s0
; NOOPT: POP s0
; NOOPT: RET
  ret i257 %arg1
}

; CHECK-LABEL: retarai64:
; NOOPT-LABEL: retarai64:
define i257 @retarai64(i257 %arg1, i257 %arg2, i257 %arg3) nounwind {
; CHECK: POP s0
; CHECK: POP s1
; NOOPT: POP s0
; NOOPT: POP s1
; NOOPT: RET
  ret i257 %arg2
}

; CHECK-LABEL: retaari64:
; NOOPT-LABEL: retaari64:
define i257 @retaari64(i257 %arg1, i257 %arg2, i257 %arg3) nounwind {
; CHECK: POP s1
; CHECK: POP s1
; NOOPT: POP s1
; NOOPT: POP s1
; NOOPT: RET
  ret i257 %arg3
}
