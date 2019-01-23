; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @negate (i64 %a1) nounwind {
; CHECK-LABEL: negate:
; CHECK: NEGATE
 %1 = mul i64 -1, %a1
 ret i64 %1
}