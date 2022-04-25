; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s

target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: zero:
define i257 @zero() {
; CHECK:       GETGLOB 5
; CHECK-NEXT:  ADDCONST -1
; CHECK-NEXT:  SETGLOB 5
; CHECK-NEXT:  GETGLOB 5
; CHECK-NEXT:  GETGLOB 13 CALLX
; CHECK-NEXT:  GETGLOB 5
; CHECK-NEXT:  ADDCONST 1
; CHECK-NEXT:  SETGLOB 5
entry:
  %var = alloca i257, align 1
  %rv = load i257, i257* %var, align 1
  ret i257 %rv
}

; CHECK-LABEL: small:
define i257 @small() {
; CHECK:       GETGLOB 5
; CHECK-NEXT:  ADDCONST -128
; CHECK-NEXT:  SETGLOB 5
; CHECK-NEXT:  GETGLOB 5
; CHECK-NEXT:  ADDCONST 127
; CHECK-NEXT:  GETGLOB 13 CALLX
; CHECK-NEXT:  GETGLOB 5
; CHECK-NEXT:  PUSHINT 128
; CHECK-NEXT:  ADD
; CHECK-NEXT:  SETGLOB 5
entry:
  %fill = alloca i257, i32 127
  %var = alloca i257, align 1
  %rv = load i257, i257* %var, align 1
  ret i257 %rv
}

; CHECK-LABEL: large:
define i257 @large() {
; CHECK:       GETGLOB 5
; CHECK-NEXT:  PUSHINT 129
; CHECK-NEXT:  SUB
; CHECK-NEXT:  SETGLOB 5
; CHECK-NEXT:  GETGLOB 5
; CHECK-NEXT:  PUSHINT 128
; CHECK-NEXT:  ADD
; CHECK-NEXT:  GETGLOB 13 CALLX
; CHECK-NEXT:  GETGLOB 5
; CHECK-NEXT:  PUSHINT 129
; CHECK-NEXT:  ADD
; CHECK-NEXT:  SETGLOB 5
entry:
  %fill = alloca i257, i32 128
  %var = alloca i257, align 1
  %rv = load i257, i257* %var, align 1
  ret i257 %rv
}
