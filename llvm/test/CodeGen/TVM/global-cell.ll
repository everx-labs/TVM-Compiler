; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

@map_persistent = global cell bitcast (i257 0 to cell)
; CHECK: map_persistent:
; CHECK-NEXT: .byte 0
; CHECK-NEXT: .size map_persistent, 1
