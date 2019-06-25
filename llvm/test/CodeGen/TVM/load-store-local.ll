; RUN: llc < %s -march=tvm -O0 -asm-verbose=false | FileCheck %s
target triple = "tvm"

; CHECK-LABEL: test
define i64 @test() {
; CHECK: PUSHINT 8
; CHECK-NEXT: CALL $:enter$
; CHECK-NEXT: PUSHINT 0
; CHECK-NEXT: CALL $:frameidx$
; CHECK-NEXT: PUSHINT 18234
; CHECK-NEXT: CALL $:store$
; CHECK-NEXT: PUSHINT 0
; CHECK-NEXT: CALL $:frameidx$
; CHECK-NEXT: CALL $:load$
; CHECK-NEXT: PUSHINT 8
; CHECK-NEXT: CALL $:leave$
  %v = alloca i64, align 8
  store i64 18234, i64* %v, align 8
  %1 = load i64, i64* %v, align 8
  ret i64 %1
}
