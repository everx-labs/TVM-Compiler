; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @lshift1 (i64 %v) nounwind {
; CHECK-LABEL: lshift1:
; CHECK: LSHIFT
 %1 = shl i64 %v, 1
 ret i64 %1
}