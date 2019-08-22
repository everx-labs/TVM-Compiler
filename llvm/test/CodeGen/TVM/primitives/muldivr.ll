; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"  
target triple = "tvm" 

define i257 @muldivr (i257 %dvd1, i257 %dvd2, i257 %dvr) nounwind {
; CHECK-LABEL: muldivr:
; CHECK: MULDIVR
 %dvd = mul i257 %dvd1, %dvd2
 
 %1 = mul i257 %dvd, 2
 %dvdr = add i257 %1, 1
 
 %dvds1    = icmp slt i257 0, %dvdr
 %dvrs1    = icmp slt i257 0, %dvr
 %qtnt1    = sdiv i257 %dvdr, %dvr
 %rem1     = srem i257 %dvdr, %dvr
 %rnz1     = icmp ne i257 0, %rem1
 %qtntneg1 = xor i1 %dvds1, %dvrs1
 %docor1   = and i1 %qtntneg1, %rnz1
 %cor1     = select i1 %docor1, i257 -1, i257 0
 %qtntf1   = add i257 %qtnt1, %2
 
 %dvds2    = icmp slt i257 0, %qtntf1
 %dvrs2    = icmp slt i257 0, 2
 %qtnt2    = sdiv i257 %qtntf1, 2
 %rem2     = srem i257 %qtntf1, 2
 %rnz2     = icmp ne i257 0, %rem2
 %qtntneg2 = xor i1 %dvds2, %dvrs2
 %docor2   = and i1 %qtntneg2, %rnz2
 %cor2     = select i1 %docor2, i257 -1, i257 0
 %qtntf2   = add i257 %qtnt2, %cor2
 
 ret i257 %qtntf2
}
