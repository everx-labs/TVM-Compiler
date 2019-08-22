; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"  
target triple = "tvm" 

define i257 @chkbool (i257 %x) nounwind {
; CHECK-LABEL: chkbool:
; CHECK: CHKBOOL
 %1 = call i257 @llvm.tvm.fitsx(i257 %x, i257 1)
 ret i257 %1
}

define i257 @chkbit (i257 %x) nounwind {
; CHECK-LABEL: chkbit:
; CHECK: CHKBIT
 %1 = call i257 @llvm.tvm.ufitsx(i257 %x, i257 1)
 ret i257 %1
}

define i257 @fitsx (i257 %x, i257 %c) nounwind {
; CHECK-LABEL: fitsx:
; CHECK: FITSX
 %1 = call i257 @llvm.tvm.fitsx(i257 %x, i257 %c)
 ret i257 %1
}

define i257 @ufitsx (i257 %x, i257 %c) nounwind {
; CHECK-LABEL: ufitsx:
; CHECK: UFITSX
 %1 = call i257 @llvm.tvm.ufitsx(i257 %x, i257 %c)
 ret i257 %1
}

define i257 @fits (i257 %x) nounwind {
; CHECK-LABEL: fits:
; CHECK: FITS
 %1 = call i257 @llvm.tvm.fitsx(i257 %x, i257 32)
 ret i257 %1
}

define i257 @ufits (i257 %x) nounwind {
; CHECK-LABEL: ufits:
; CHECK: UFITS
 %1 = call i257 @llvm.tvm.ufitsx(i257 %x, i257 32)
 ret i257 %1
}

declare i257 @llvm.tvm.fitsx(i257, i257) nounwind
declare i257 @llvm.tvm.ufitsx(i257, i257) nounwind
