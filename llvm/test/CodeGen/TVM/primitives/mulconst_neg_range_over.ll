; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @mulconst_neg_range_over(i64 %a1) nounwind {
; CHECK-LABEL: mulconst_neg_range_over:
; CHECK: PUSHINT
; CHECK-NEXT: MUL
 %1 = mul i64 -129, %a1
 ret i64 %1
}