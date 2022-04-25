; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

@X = global i256 zeroinitializer, align 1

; CHECK-LABEL: zext_i256_i257
define i257 @zext_i256_i257() {
  %a = load i256, i256* @X, align 1
; CHECK: GETGLOB 13 CALLX
  %ext = zext i256 %a to i257
  ret i257 %ext
}

