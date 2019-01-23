; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @addconst_neg_range_over(i64 %a1) nounwind {
; CHECK-LABEL: addconst_neg_range_over:
; CHECK: PUSHINT
; CHECK-NEXT: ADD
 %1 = add i64 %a1, -129
 ret i64 %1
}

