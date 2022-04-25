; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"  
target triple = "tvm" 

define i257 @rshift_s (i257 %v, i257 %s) nounwind {
; CHECK-LABEL: rshift_s:
; CHECK: RSHIFT
 %1 = lshr i257 %v, %s
 ret i257 %1
}
