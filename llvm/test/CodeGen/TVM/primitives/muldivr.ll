; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @muldivr (i64 %dvd1, i64 %dvd2, i64 %dvr) nounwind {
; CHECK-LABEL: muldivr:
; CHECK: MULDIVR
 %dvd = mul i64 %dvd1, %dvd2
 
 %1 = mul i64 %dvd, 2
 %dvdr = add i64 %1, 1
 
 %dvds1    = icmp slt i64 0, %dvdr
 %dvrs1    = icmp slt i64 0, %dvr
 %qtnt1    = sdiv i64 %dvdr, %dvr
 %rem1     = srem i64 %dvdr, %dvr
 %rnz1     = icmp ne i64 0, %rem1
 %qtntneg1 = xor i1 %dvds1, %dvrs1
 %docor1   = and i1 %qtntneg1, %rnz1
 %cor1     = select i1 %docor1, i64 -1, i64 0
 %qtntf1   = add i64 %qtnt1, %2
 
 %dvds2    = icmp slt i64 0, %qtntf1
 %dvrs2    = icmp slt i64 0, 2
 %qtnt2    = sdiv i64 %qtntf1, 2
 %rem2     = srem i64 %qtntf1, 2
 %rnz2     = icmp ne i64 0, %rem2
 %qtntneg2 = xor i1 %dvds2, %dvrs2
 %docor2   = and i1 %qtntneg2, %rnz2
 %cor2     = select i1 %docor2, i64 -1, i64 0
 %qtntf2   = add i64 %qtnt2, %cor2
 
 ret i64 %qtntf2
}