; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"  
target triple = "tvm" 

define i257 @negate (i257 %a1) nounwind {
; CHECK-LABEL: negate:
; CHECK: NEGATE
 %1 = mul i257 -1, %a1
 ret i257 %1
}
