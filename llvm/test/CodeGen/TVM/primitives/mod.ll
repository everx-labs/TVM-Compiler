; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @mod (i64 %dvd, i64 %dvr) nounwind {
; CHECK-LABEL: mod:
; CHECK: MOD
  %1 = call i64 @llvm.tvm.mod(i64 %dvd, i64 %dvr)
  ret i64 %1
}

declare i64 @llvm.tvm.mod(i64 %dvd, i64 %dvr) nounwind
