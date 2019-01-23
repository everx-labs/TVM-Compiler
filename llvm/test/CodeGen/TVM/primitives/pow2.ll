; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @pow2 (i64 %v) nounwind {
; CHECK-LABEL: pow2:
; CHECK: POW2
 %1 = shl i64 2, %v
 ret i64 %1
}