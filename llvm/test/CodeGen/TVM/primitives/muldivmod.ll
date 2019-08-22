; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"  
target triple = "tvm" 

define { i257, i257 } @muldivmod (i257 %dvd1, i257 %dvd2, i257 %dvr) nounwind {
; CHECK-LABEL: muldivmod:
; CHECK: MULDIVMOD
 %dvd = mul i257 %dvd1, %dvd2
 %dvds    = icmp slt i257 0, %dvd
 %dvrs    = icmp slt i257 0, %dvr
 %qtnt    = sdiv i257 %dvd, %dvr
 %rem     = srem i257 %dvd, %dvr
 %rnz     = icmp ne i257 0, %rem
 %qtntneg = xor i1 %dvds, %dvrs
 %docor   = and i1 %qtntneg, %rnz
 %cor     = select i1 %docor, i257 -1, i257 0
 %qtntfin   = add i257 %qtnt, %cor
 
 %qmuldd1 = mul i257 %qtntfin, %dvr
 %remfin  = sub i257 %dvd, %qmuldd1
 
 %agg1 = insertvalue {i257, i257} undef, i257 %remfin, 0
 %agg2 = insertvalue {i257, i257} %agg1, i257 %qtntfin, 1
 ret { i257, i257 } %agg2
}
