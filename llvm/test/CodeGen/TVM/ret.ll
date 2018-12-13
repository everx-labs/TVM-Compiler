; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; CHECK-LABEL: retv:
define void @retv() nounwind {
	ret void
}

; CHECK-LABEL: reti8:
; CHECK: PUSHINT %{{[0-9]+}}, 42
define i8 @reti8() nounwind {
	ret i8 42
}

; CHECK-LABEL: reti16:
; CHECK: PUSHINT %{{[0-9]+}}, 42
define i16 @reti16() nounwind {
	ret i16 42
}

; CHECK-LABEL: reti32:
; CHECK: PUSHINT %{{[0-9]+}}, 42
define i32 @reti32() nounwind {
	ret i32 42
}

; CHECK-LABEL: reti64:
; CHECK: PUSHINT %{{[0-9]+}}, 42
define i64 @reti64() nounwind {
	ret i64 42
}
