; RUN: llc < %s -march=tvm -O0 -asm-verbose=false | FileCheck %s
target triple = "tvm"

; CHECK-LABEL: test
define i257 @test() {
; CHECK: PUSHINT 1
; CHECK-NEXT: CALL $:enter$
; CHECK-NEXT: PUSH c0
; CHECK-NEXT: PUSHINT 0
; CHECK-NEXT: CALL $:frameidx$
; CHECK-NEXT: PUSHINT 18234
; CHECK-NEXT: CALL $:store$
; CHECK-NEXT: PUSHINT 0
; CHECK-NEXT: CALL $:frameidx$
; CHECK-NEXT: CALL $:load$
; CHECK-NEXT: PUSHINT 1
; CHECK-NEXT: CALL $:leave$
  %v = alloca i257, align 1
  store i257 18234, i257* %v, align 1
  %1 = load i257, i257* %v, align 1
  ret i257 %1
}
