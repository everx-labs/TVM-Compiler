; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"  
target triple = "tvm" 

define i257 @bitsize (i257 %x) nounwind {
; CHECK-LABEL: bitsize:
; CHECK: BITSIZE
 %1 = call i257 @llvm.tvm.bitsize(i257 %x)
 ret i257 %1
}

define i257 @ubitsize (i257 %x) nounwind {
; CHECK-LABEL: ubitsize:
; CHECK: UBITSIZE
 %1 = call i257 @llvm.tvm.ubitsize(i257 %x)
 ret i257 %1
}

declare i257 @llvm.tvm.bitsize(i257) nounwind
declare i257 @llvm.tvm.ubitsize(i257) nounwind
