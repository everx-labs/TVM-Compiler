; RUN: llc < %s -march=tvm -O0 -asm-verbose=false | FileCheck %s
target triple = "tvm"

; CHECK-LABEL: test
define i257 @test() {
; CHECK:      GETGLOB 5
; CHECK-NEXT: ADDCONST -1
; CHECK-NEXT: SETGLOB 5
; CHECK-NEXT: PUSH c0
; CHECK-NEXT: GETGLOB 5
; CHECK-NEXT: PUSHINT 18234
; CHECK-NEXT: GETGLOB 14 CALLX
; CHECK-NEXT: GETGLOB 5
; CHECK-NEXT: GETGLOB 13 CALLX
; CHECK-NEXT: GETGLOB 5
; CHECK-NEXT: ADDCONST 1
; CHECK-NEXT: SETGLOB 5
  %v = alloca i257, align 1
  store i257 18234, i257* %v, align 1
  %1 = load i257, i257* %v, align 1
  ret i257 %1
}
