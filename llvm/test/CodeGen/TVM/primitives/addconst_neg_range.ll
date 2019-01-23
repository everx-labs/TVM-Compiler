; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @addconst_neg_range(i64 %a1) nounwind {
; CHECK-LABEL: addconst_neg_range:
; CHECK: ADDCONST
 %1 = add i64 -128, %a1
 ret i64 %1
}