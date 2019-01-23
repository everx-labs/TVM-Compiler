; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @divc (i64 %dvd, i64 %dvr) nounwind {
; CHECK-LABEL: divc:
; CHECK: DIVC
 %dvd_neg = icmp slt i64 0, %dvd
 %dvr_neg = icmp slt i64 0, %dvr
 %res_neg = xor i1 %dvd_neg, %dvr_neg
 %pr_rem  = srem i64 %dvd, %dvr
 %no_rem  = icmp eq i64 0, %pr_rem
 %n_cor   = or i1 %res_neg, %no_rem
 %cor = select i1 %n_cor, i64 0, i64 1
 %qtnt_pr = sdiv i64 %dvd, %dvr
 %qtnt = add i64 %qtnt_pr, cor
 ret i64 %qtnt
}