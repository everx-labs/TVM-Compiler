; RUN: llc -march=tvm -asm-verbose=false < %s | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: foo
define i257 @foo(i257 %index, i257 %mask) {
entry:
  %shl = shl i257 1, %index
  %neg = xor i257 %shl, -1
  %and = and i257 %neg, %mask
  ret i257 %and
; CHECK:      ONE
; CHECK-NEXT: XCHG2 s0, s2
; CHECK-NEXT: LSHIFT
; CHECK-NEXT: NOT
; CHECK-NEXT: AND
}
