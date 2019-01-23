; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @not (i64 %v) nounwind {
; CHECK-LABEL: not:
; CHECK: NOT
 %1 = xor i64 -1, %v
 ret i64 %1
}