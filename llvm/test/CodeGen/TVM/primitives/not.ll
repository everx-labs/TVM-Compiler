; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"  
target triple = "tvm" 

define i257 @not (i257 %v) nounwind {
; CHECK-LABEL: not:
; CHECK: NOT
 %1 = xor i257 -1, %v
 ret i257 %1
}
