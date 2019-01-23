; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @mulconst_neg_range(i64 %a1) nounwind {
; CHECK-LABEL: mulconst_neg_range:
; CHECK: MULCONST
 %1 = mul i64 -128, %a1
 ret i64 %1
}