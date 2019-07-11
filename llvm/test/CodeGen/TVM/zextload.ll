; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

@X = global i8 zeroinitializer, align 1

; CHECK-LABEL: zext_i8_i64
define i64 @zext_i8_i64() {
  %a = load i8, i8* @X, align 1
;CHECK: CALL  $:load$
  %ext = zext i8 %a to i64
  ret i64 %ext
}

