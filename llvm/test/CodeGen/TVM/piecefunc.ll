; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257" 
target triple = "tvm"

define i257 @piece_func (i257 %x) nounwind {
; CHECK-LABEL: piece_func:
; CHECK-NEXT: DUP
; CHECK-NEXT: PUSHINT -1
; CHECK-NEXT: LESS
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:    DUP
; CHECK-NEXT:    MUL
; CHECK-NEXT:    RET
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:    DUP
; CHECK-NEXT:    PUSHINT 1
; CHECK-NEXT:    GREATER
; CHECK-NEXT:    SWAP
; CHECK-NEXT:    PUSHINT 1
; CHECK-NEXT:    CONDSEL
; CHECK-NEXT: }
; CHECK-NEXT: IFELSE
 %q = icmp slt i257 %x, -1
 br i1 %q, label %quad, label %next
quad:
 %1 = mul i257 %x, %x
 ret i257 %1
next:
 %l = icmp sgt i257 %x, 1
 br i1 %l, label %line, label %constant
constant:
 ret i257 1
line:
 ret i257 %x
}
