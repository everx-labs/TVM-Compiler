; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @div (i64 %dvd, i64 %dvr) nounwind {
; CHECK-LABEL: div:
; CHECK: DIV
 %1 = call i64 @llvm.tvm.div(i64 %dvd, i64 %dvr)
 ret i64 %1
}

declare i64 @llvm.tvm.div(i64, i64) nounwind
