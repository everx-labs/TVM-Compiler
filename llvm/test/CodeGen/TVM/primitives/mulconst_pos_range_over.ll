; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"  
target triple = "tvm" 

define i257 @mulconst_pos_range_over(i257 %a1) nounwind {
; CHECK-LABEL: mulconst_pos_range_over:
; CHECK: MUL
 %1 = mul i257 128, %a1
 ret i257 %1
}
