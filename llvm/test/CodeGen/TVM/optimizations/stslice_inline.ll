; RUN: llc < %s -march=tvm -filetype=asm | FileCheck %s
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: stslice
define builder @stslice() {
; CHECK: PUSHINT
; CHECK: STUR
; CHECK-NOT: STSLICE
  %b1 = tail call builder @llvm.tvm.newc()
  %b2 = tail call builder @llvm.tvm.stu(i257 0, builder %b1, i257 2)
  %b3 = tail call builder @llvm.tvm.sti(i257 -1, builder %b2, i257 2)
  %c1 = tail call cell @llvm.tvm.endc(builder %b3)
  %s1 = tail call slice @llvm.tvm.ctos(cell %c1)
  %b4 = tail call builder @llvm.tvm.newc()
  %b5 = tail call builder @llvm.tvm.stslice(slice %s1, builder %b4)
  ret builder %b5
}

; CHECK-LABEL: stslice_varint
define builder @stslice_varint() {
; CHECK-NOT: STSLICE
; CHECK: PUSHINT	275010290703
; CHECK: STUR 42
  %b1 = tail call builder @llvm.tvm.newc()
  %b2 = tail call builder @llvm.tvm.stvarint16(builder %b1, i257 0)
  %b3 = tail call builder @llvm.tvm.stvarint16(builder %b2, i257 -7)
  %b4 = tail call builder @llvm.tvm.stvaruint32(builder %b3, i257 2)
  %b5 = tail call builder @llvm.tvm.sti(i257 -1, builder %b4, i257 2)
  %c1 = tail call cell @llvm.tvm.endc(builder %b5)
  %s1 = tail call slice @llvm.tvm.ctos(cell %c1)
  %b6 = tail call builder @llvm.tvm.newc()
  %b7 = tail call builder @llvm.tvm.stslice(slice %s1, builder %b6)
  ret builder %b7
}

; CHECK-LABEL: stslice_neg
define builder @stslice_neg() {
; CHECK: STSLICE
; CHECK: STSLICE
  %b1 = tail call builder @llvm.tvm.newc()
  %b2 = tail call builder @llvm.tvm.stu(i257 0, builder %b1, i257 2)
  %b3 = tail call builder @llvm.tvm.sti(i257 -1, builder %b2, i257 2)
  %c1 = tail call cell @llvm.tvm.endc(builder %b3)
  %s1 = tail call slice @llvm.tvm.ctos(cell %c1)
  %b4 = tail call builder @llvm.tvm.newc()
  %b5 = tail call builder @llvm.tvm.stslice(slice %s1, builder %b4)
  %b6 = tail call builder @llvm.tvm.stslice(slice %s1, builder %b5)
  ret builder %b6
}

; CHECK-LABEL: stslice_rec
define builder @stslice_rec() {
; CHECK: PUSHINT
; CHECK: STUR
; CHECK-NOT: STSLICE
  %b1 = tail call builder @llvm.tvm.newc()
  %b2 = tail call builder @llvm.tvm.stu(i257 0, builder %b1, i257 2)
  %b3 = tail call builder @llvm.tvm.sti(i257 -1, builder %b2, i257 2)
  %c1 = tail call cell @llvm.tvm.endc(builder %b3)
  %s1 = tail call slice @llvm.tvm.ctos(cell %c1)
  %b4 = tail call builder @llvm.tvm.newc()
  %b5 = tail call builder @llvm.tvm.stslice(slice %s1, builder %b4)
  %c2 = tail call cell @llvm.tvm.endc(builder %b5)
  %s2 = tail call slice @llvm.tvm.ctos(cell %c2)
  %b7 = tail call builder @llvm.tvm.newc()
  %b8 = tail call builder @llvm.tvm.stslice(slice %s2, builder %b7)
  ret builder %b8
}
declare builder @llvm.tvm.stu(i257, builder, i257)
declare builder @llvm.tvm.sti(i257, builder, i257)
declare builder @llvm.tvm.stvarint16(builder, i257)
declare builder @llvm.tvm.stvaruint32(builder, i257)
declare builder @llvm.tvm.newc()
declare cell @llvm.tvm.endc(builder)
declare builder @llvm.tvm.stslice(slice, builder)
declare slice @llvm.tvm.ctos(cell)
