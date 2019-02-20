; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @floor_dig (i64 %val, i64 %dig) nounwind {
; CHECK-LABEL: floor_dig:
 %dvr = mul i64 %dig, 10
 %rem = srem i64 %val, %dvr
 %no_rem = icmp eq i64 %rem, 0
 br i1 %no_rem, label %Return, label %Round
Round:
 %1 = sdiv i64 %val, %dvr
 %res = mul i64 %1, %dvr
 ret i64 %res
Return:
 ret i64 %val
}
