; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @chkbool (i64 %x) nounwind {
; CHECK-LABEL: chkbool:
; CHECK: CHKBOOL
 %1 = call i64 @llvm.tvm.fitsx(i64 %x, i64 1)
 ret i64 %1
}

define i64 @chkbit (i64 %x) nounwind {
; CHECK-LABEL: chkbit:
; CHECK: CHKBIT
 %1 = call i64 @llvm.tvm.ufitsx(i64 %x, i64 1)
 ret i64 %1
}

define i64 @fitsx (i64 %x, i64 %c) nounwind {
; CHECK-LABEL: fitsx:
; CHECK: FITSX
 %1 = call i64 @llvm.tvm.fitsx(i64 %x, i64 %c)
 ret i64 %1
}

define i64 @ufitsx (i64 %x, i64 %c) nounwind {
; CHECK-LABEL: ufitsx:
; CHECK: UFITSX
 %1 = call i64 @llvm.tvm.ufitsx(i64 %x, i64 %c)
 ret i64 %1
}

define i64 @fits (i64 %x) nounwind {
; CHECK-LABEL: fits:
; CHECK: FITS
 %1 = call i64 @llvm.tvm.fitsx(i64 %x, i64 32)
 ret i64 %1
}

define i64 @ufits (i64 %x) nounwind {
; CHECK-LABEL: ufits:
; CHECK: UFITS
 %1 = call i64 @llvm.tvm.ufitsx(i64 %x, i64 32)
 ret i64 %1
}

declare i64 @llvm.tvm.fitsx(i64, i64) nounwind
declare i64 @llvm.tvm.ufitsx(i64, i64) nounwind
