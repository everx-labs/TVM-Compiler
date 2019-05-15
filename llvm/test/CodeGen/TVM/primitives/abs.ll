; RUN: llc < %s -O0 -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @abs (i64 %x) nounwind {
; CHECK-LABEL: abs:
; CHECK: ABS
 %cmp = icmp sgt i64 %x, 0
 %negx = sub nsw i64 0, %x
 %1 = select i1 %cmp, i64 %x, i64 %negx
 ret i64 %1
}
