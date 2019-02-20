; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm"

define i64 @reciprocal_nz (i64 %x) nounwind {
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
 switch i64 %x, label %normal [ i64 0, label %over ]
normal:
 %1 = sdiv i64 1, %x
 ret i64 %1
over:
 ret i64 0
}
