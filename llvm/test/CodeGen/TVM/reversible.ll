; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
; XFAIL: *
target datalayout = "e-S1024-i256:256:256"
target triple = "tvm"

; CHECK-LABEL: subr1
define i64 @subr1(i64 %x, i64 %y) {
; CHECK: SUBR
  %res = sub i64 %y, %x
  ret i64 %res
}

; CHECK-LABEL: subr2
define i64 @subr2(i64 %x, i64 %y, i64 %z) {
; CHECK: SUBR
  %res.0 = sub i64 %y, %x
  %res.1 = sub i64 %z, %res.0
  ret i64 %res.1
}

; CHECK-LABEL: stir
define i64 @stir(i64 %builder, i64 %val) {
; CHECK: STIR 37
  %res = call i64 @llvm.tvm.sti(i64 %val, i64 %builder, i64 37)
  ret i64 %res
}

; CHECK-LABEL: stur
define i64 @stur(i64 %builder, i64 %val) {
; CHECK: STUR 42
  %res = call i64 @llvm.tvm.stu(i64 %val, i64 %builder, i64 42)
  ret i64 %res
}

; CHECK-LABEL: stixr
define i64 @stixr(i64 %builder, i64 %val, i64 %precision) {
; CHECK: STIXR
  %res = call i64 @llvm.tvm.sti(i64 %val, i64 %builder, i64 %precision)
  ret i64 %res
}

; CHECK-LABEL: stuxr
define i64 @stuxr(i64 %builder, i64 %val, i64 %precision) {
; CHECK: STUXR
  %res = call i64 @llvm.tvm.stu(i64 %val, i64 %builder, i64 %precision)
  ret i64 %res
}

declare i64 @llvm.tvm.sti(i64 %val, i64 %buider, i64 %precision)
declare i64 @llvm.tvm.stu(i64 %val, i64 %buider, i64 %precision)
