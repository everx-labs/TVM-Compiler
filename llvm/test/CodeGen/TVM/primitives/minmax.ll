; RUN: llc < %s -O0 -march=tvm | FileCheck %s 
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"  
target triple = "tvm" 

define i257 @min (i257 %x, i257 %y) nounwind {
; CHECK-LABEL: min:
; CHECK: MIN
 %cmp = icmp sgt i257 %x, %y
 %1 = select i1 %cmp, i257 %y, i257 %x
 ret i257 %1
}

define i257 @max (i257 %x, i257 %y) nounwind {
; CHECK-LABEL: max:
; CHECK: MAX
 %cmp = icmp sgt i257 %x, %y
 %1 = select i1 %cmp, i257 %x, i257 %y
 ret i257 %1
}
