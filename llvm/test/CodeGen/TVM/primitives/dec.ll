; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @dec (i64 %a1) nounwind {
; CHECK-LABEL: dec:
; CHECK: DEC
 %1 = sub i64 %a1, 1
 ret i64 %1
}