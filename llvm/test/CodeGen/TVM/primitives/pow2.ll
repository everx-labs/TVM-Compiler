; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"  
target triple = "tvm" 

define i257 @pow2 (i257 %v) nounwind {
; CHECK-LABEL: pow2:
; CHECK: POW2
 %1 = shl i257 2, %v
 ret i257 %1
}
