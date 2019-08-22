; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257" 
target triple = "tvm"

define i257 @reciprocal_nz (i257 %x) nounwind {
; CHECK-LABEL: reciprocal_nz:
; CHECK-NEXT: PUSHINT 0
; CHECK-NEXT: DUP
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:    PUSHINT 1
; CHECK-NEXT:    SWAP
; CHECK-NEXT:    DIV
; CHECK-NEXT:    RET
; CHECK-NEXT: }
; CHECK-NEXT: IF
 switch i257 %x, label %normal [ i257 0, label %over ]
normal:
 %1 = sdiv i257 1, %x
 ret i257 %1
over:
 ret i257 0
}
