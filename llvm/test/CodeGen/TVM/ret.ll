; RUN: llc < %s -march=tvm | FileCheck %s
; RUN: llc < %s -march=tvm --disable-tvm-return-opt | FileCheck %s -check-prefix=NOOPT
target datalayout = "E-S1024-i256:256:256"
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
define i64 @reti64() nounwind {
; CHECK: PUSHINT 42
; CHECK-NOT: {{\tRET}}
; NOOPT: RET
  ret i64 42
}

; CHECK-LABEL: retai64:
; NOOPT-LABEL: retai64:
define i64 @retai64(i64 %arg) nounwind {
; CHECK-NOT: {{\tRET}}
; NOOPT: RET
  ret i64 %arg
}

; CHECK-LABEL: retraai64:
; NOOPT-LABEL: retraai64:
define i64 @retraai64(i64 %arg1, i64 %arg2, i64 %arg3) nounwind {
; CHECK: POP s0
; CHECK: POP s0
; CHECK-NOT: {{\tRET}}
; NOOPT: POP s0
; NOOPT: POP s0
; NOOPT: RET
  ret i64 %arg1
}

; CHECK-LABEL: retarai64:
; NOOPT-LABEL: retarai64:
define i64 @retarai64(i64 %arg1, i64 %arg2, i64 %arg3) nounwind {
; CHECK: POP s0
; CHECK: POP s1
; NOOPT: POP s0
; NOOPT: POP s1
; NOOPT: RET
  ret i64 %arg2
}

; CHECK-LABEL: retaari64:
; NOOPT-LABEL: retaari64:
define i64 @retaari64(i64 %arg1, i64 %arg2, i64 %arg3) nounwind {
; CHECK: POP s1
; CHECK: POP s1
; NOOPT: POP s1
; NOOPT: POP s1
; NOOPT: RET
  ret i64 %arg3
}
