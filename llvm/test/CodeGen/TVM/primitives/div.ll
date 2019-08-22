; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"  
target triple = "tvm" 

define i257 @div (i257 %dvd, i257 %dvr) nounwind {
; CHECK-LABEL: div:
; CHECK: DIV
 %1 = call i257 @llvm.tvm.div(i257 %dvd, i257 %dvr)
 ret i257 %1
}

declare i257 @llvm.tvm.div(i257, i257) nounwind
