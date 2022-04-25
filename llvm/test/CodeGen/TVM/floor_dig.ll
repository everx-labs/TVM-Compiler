; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257" 
target triple = "tvm" 

define i257 @floor_dig (i257 %val, i257 %dig) nounwind {
; CHECK-LABEL: floor_dig:
 %dvr = mul i257 %dig, 10
 %rem = srem i257 %val, %dvr
 %no_rem = icmp eq i257 %rem, 0
 br i1 %no_rem, label %Return, label %Round
Round:
 %1 = sdiv i257 %val, %dvr
 %res = mul i257 %1, %dvr
 ret i257 %res
Return:
 ret i257 %val
}
