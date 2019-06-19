; RUN: llc < %s -march=tvm -O3 | FileCheck %s
target triple = "tvm"

%struct.w = type { i64, i64 }

@w = external dso_local local_unnamed_addr global %struct.w, align 8

; CHECK-LABEL: test
define i64 @test() {
; CHECK:      PUSHINT	$w+8$
; CHECK-NEXT: CALL	$:load$
  %1 = load i64, i64* getelementptr inbounds (%struct.w, %struct.w* @w, i64 0, i32 1), align 8
  ret i64 %1
}
