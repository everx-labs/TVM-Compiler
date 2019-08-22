; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"  
target triple = "tvm" 

define { i257, i257 } @divmod (i257 %dvd, i257 %dvr) nounwind {
; CHECK-LABEL: divmod:
; CHECK: DIVMOD
 %dvds    = icmp slt i257 0, %dvd
 %dvrs    = icmp slt i257 0, %dvr
 %qtnt    = sdiv i257 %dvd, %dvr
 %rem     = srem i257 %dvd, %dvr
 %rnz     = icmp ne i257 0, %rem
 %qtntneg = xor i1 %dvds, %dvrs
 %docor   = and i1 %qtntneg, %rnz
 %cor     = select i1 %docor, i257 -1, i257 0
 %qtntf   = add i257 %qtnt, %cor
 
 %qmuldd1 = mul i257 %qtntf, %dvr
 %remfin  = sub i257 %dvd, %qmuldd1
 
 %agg1 = insertvalue {i257, i257} undef, i257 %remfin, 0
 %agg2 = insertvalue {i257, i257} %agg1, i257 %qtntf, 1
 ret { i257, i257 } %agg2
}

