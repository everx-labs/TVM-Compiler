; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @mulconst_pos_range(i64 %a1) nounwind {
; CHECK-LABEL: mulconst_pos_range:
; CHECK: MULCONST
 %1 = mul i64 127, %a1
 ret i64 %1
}