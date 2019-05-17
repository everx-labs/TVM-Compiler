; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @bitsize (i64 %x) nounwind {
; CHECK-LABEL: bitsize:
; CHECK: BITSIZE
 %1 = call i64 @llvm.tvm.bitsize(i64 %x)
 ret i64 %1
}

define i64 @ubitsize (i64 %x) nounwind {
; CHECK-LABEL: ubitsize:
; CHECK: UBITSIZE
 %1 = call i64 @llvm.tvm.ubitsize(i64 %x)
 ret i64 %1
}

declare i64 @llvm.tvm.bitsize(i64) nounwind
declare i64 @llvm.tvm.ubitsize(i64) nounwind
