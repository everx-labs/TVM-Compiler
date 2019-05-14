; RUN: llc < %s -O0 -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @min (i64 %x, i64 %y) nounwind {
; CHECK-LABEL: min:
; CHECK: MIN
 %cmp = icmp sgt i64 %x, %y
 %1 = select i1 %cmp, i64 %y, i64 %x
 ret i64 %1
}

define i64 @max (i64 %x, i64 %y) nounwind {
; CHECK-LABEL: max:
; CHECK: MAX
 %cmp = icmp sgt i64 %x, %y
 %1 = select i1 %cmp, i64 %x, i64 %y
 ret i64 %1
}
