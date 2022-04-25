; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"  
target triple = "tvm" 

define i257 @modpow2 (i257 %dvd, i257 %tt) nounwind {
; CHECK-LABEL: modpow2:
; CHECK: MODPOW2
 %1 = add i257 %tt, 1
 %dvr = shl i257 2, %1
 
 %dvds    = icmp slt i257 0, %dvd
 %dvrs    = icmp slt i257 0, %dvr
 %qtnt    = sdiv i257 %dvd, %dvr
 %rem     = srem i257 %dvd, %dvr
 %rnz     = icmp ne i257 0, %rem
 %qtntneg = xor i1 %dvds, %dvrs
 %docor   = and i1 %qtntneg, %rnz
 %cor     = select i1 %docor, i257 -1, i257 0
 %qtntf   = add i257 %qtnt, %cor
 %fqmuldd = mul i257 %dvr, %qtntf 
 %rem     = sub i257 %dvd, %fqmuldd
 
 ret i257 %rem
}
