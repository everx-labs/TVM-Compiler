; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @add (i64 %a1, i64 %a2) nounwind {
; CHECK-LABEL: add:
; CHECK: ADD
 %1 = add i64 %a1, %a2
 ret i64 %1
}
