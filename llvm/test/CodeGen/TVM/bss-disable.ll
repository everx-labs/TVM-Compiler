; RUN: llc -march=tvm -asm-verbose=false < %s | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK:        .type   gv,@object
; CHECK-NEXT:   .data
; CHECK-NEXT:   .globl  gv
; CHECK-NEXT: gv:
; CHECK-NEXT:   .byte   0
; CHECK-NEXT:   .size   gv, 1
@gv = global i257 0, align 1
