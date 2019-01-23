; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @rshift_s (i64 %v, i64 %s) nounwind {
; CHECK-LABEL: rshift_s:
; CHECK: RSHIFT
 %1 = lshr i64 %v, %s
 ret i64 %1
}