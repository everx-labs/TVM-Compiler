; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257" 
target triple = "tvm"

define i257 @fabs (i257 %val) nounwind {
; CHECK-LABEL: fabs:
 %neg = icmp slt i257 %val, 0
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
 ret i257 %val
Negative:
 %1 = mul i257 %val, -1
 ret i257 %1
}
