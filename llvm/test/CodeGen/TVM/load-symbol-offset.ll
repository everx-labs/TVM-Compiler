; RUN: llc < %s -march=tvm -O3 | FileCheck %s
target triple = "tvm"

%struct.w = type { i257, i257 }

@w = external dso_local local_unnamed_addr global %struct.w, align 1

; CHECK-LABEL: test
define i257 @test() {
; CHECK: PUSHINT $w+1$
; CHECK: GETGLOB 13 CALLX
  %1 = load i257, i257* getelementptr inbounds (%struct.w, %struct.w* @w, i257 0, i32 1), align 1
  ret i257 %1
}
