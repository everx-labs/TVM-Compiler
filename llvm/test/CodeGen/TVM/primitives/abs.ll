; RUN: llc < %s -O0 -march=tvm | FileCheck %s 
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257" 
target triple = "tvm" 

define i257 @abs (i257 %x) nounwind {
; CHECK-LABEL: abs:
; CHECK: ABS
 %cmp = icmp sgt i257 %x, 0
 %negx = sub nsw i257 0, %x
 %1 = select i1 %cmp, i257 %x, i257 %negx
 ret i257 %1
}
