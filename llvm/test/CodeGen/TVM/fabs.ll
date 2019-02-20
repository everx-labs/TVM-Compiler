; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm"

define i64 @fabs (i64 %val) nounwind {
; CHECK-LABEL: fabs:
 %neg = icmp slt i64 %val, 0
; CHECK-NEXT: DUP
; CHECK-NEXT: PUSHINT 0
; CHECK-NEXT: LESS
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT: PUSHINT -1
; CHECK-NEXT: MUL
; CHECK-NEXT: }
; CHECK-NEXT: IF
 br i1 %neg, label %Negative, label %Positive
Positive:
 ret i64 %val
Negative:
 %1 = mul i64 %val, -1
 ret i64 %1
}
