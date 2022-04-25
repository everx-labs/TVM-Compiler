; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"  
target triple = "tvm" 

define i257 @xor (i257 %a1, i257 %a2) nounwind {
; CHECK-LABEL: xor:
; CHECK: XOR
 %1 = xor i257 %a1, %a2
 ret i257 %1
}
