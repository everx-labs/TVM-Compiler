; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm"

define i64 @piece_func (i64 %x) nounwind {
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
 %q = icmp slt i64 %x, -1
 br i1 %q, label %quad, label %next
quad:
 %1 = mul i64 %x, %x
 ret i64 %1
next:
 %l = icmp sgt i64 %x, 1
 br i1 %l, label %line, label %constant
constant:
 ret i64 1
line:
 ret i64 %x
}
