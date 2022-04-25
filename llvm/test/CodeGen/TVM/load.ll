; RUN: llc -march=tvm -asm-verbose=false < %s | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: __tvm_load_global
define i257 @__tvm_load_global(i257* nocapture readonly %p) {
entry:
; CHECK:        GETGLOB    1
; CHECK:        PUSHINT    64
; CHECK:        DICTIGET NULLSWAPIFNOT
; CHECK-NEXT:   THROWIFNOT 60
; CHECK-NEXT:   PUSHINT    257
; CHECK-NEXT:   LDIX
; CHECK-NEXT:   ENDS
  %address = ptrtoint i257* %p to i257

  %getglob = call i257 @llvm.tvm.getglobal(i257 1)
  %dict = call cell @llvm.tvm.cast.to.cell(i257 %getglob)

  %dictiget = call {slice, i257} @llvm.tvm.dictiget(i257 %address, cell %dict, i257 64)
  %flag = extractvalue {slice, i257} %dictiget, 1
  %slice = extractvalue {slice, i257} %dictiget, 0

  %flag1 = trunc i257 %flag to i1
  br i1 %flag1, label %ok, label %do_throw
ok:
  %ldi = call {i257, slice} @llvm.tvm.ldi(slice %slice, i257 257)
  %slice_rem = extractvalue {i257, slice} %ldi, 1
  %value = extractvalue {i257, slice} %ldi, 0

  call void @llvm.tvm.ends(slice %slice_rem)
  ret i257 %value
do_throw:
  call void @llvm.tvm.throw(i257 60)
  unreachable
}

; CHECK-LABEL: __tvm_load_persistent
define i257 @__tvm_load_persistent(i257* nocapture readonly %p) {
entry:
; CHECK:        PUSH       c4
; CHECK:        CTOS
; CHECK-NEXT:   PLDDICT
; CHECK:        PUSHINT    64
; CHECK:        DICTIGET NULLSWAPIFNOT
; CHECK-NEXT:   THROWIFNOT 60
; CHECK-NEXT:   PUSHINT    257
; CHECK-NEXT:   LDIX
; CHECK-NEXT:   ENDS
  %address = ptrtoint i257* %p to i257

  %getreg = call i257 @llvm.tvm.getreg(i257 4)
  %cont = call cell @llvm.tvm.cast.to.cell(i257 %getreg)
  %ctos = call slice @llvm.tvm.ctos(cell %cont)
  %dict = call cell @llvm.tvm.plddict(slice %ctos)

  %dictiget = call {slice, i257} @llvm.tvm.dictiget(i257 %address, cell %dict, i257 64)
  %flag = extractvalue {slice, i257} %dictiget, 1
  %slice = extractvalue {slice, i257} %dictiget, 0

  %flag1 = trunc i257 %flag to i1
  br i1 %flag1, label %ok, label %do_throw
ok:
  %ldi = call {i257, slice} @llvm.tvm.ldi(slice %slice, i257 257)
  %slice_rem = extractvalue {i257, slice} %ldi, 1
  %value = extractvalue {i257, slice} %ldi, 0

  call void @llvm.tvm.ends(slice %slice_rem)
  ret i257 %value
do_throw:
  call void @llvm.tvm.throw(i257 60)
  unreachable
}

define i257 @__tvm_load(i257* nocapture readonly %p) {
entry:
  %address = ptrtoint i257* %p to i257
  %is_persistent = icmp slt i257 %address, 10000
  br i1 %is_persistent, label %persistent, label %globl
persistent:
  %0 = call i257 @__tvm_load_persistent(i257* %p)
  ret i257 %0
globl:
  %1 = call i257 @__tvm_load_global(i257* %p)
  ret i257 %1
}

declare i257 @llvm.tvm.getglobal(i257 %idx)
declare i257 @llvm.tvm.getreg(i257 %regno)
declare tuple @llvm.tvm.cast.to.tuple(i257 %v)
declare cell @llvm.tvm.cast.to.cell(i257 %v)
declare slice @llvm.tvm.cast.to.slice(i257 %v)
declare slice @llvm.tvm.ctos(cell %cell)
declare void @llvm.tvm.ends(slice %slice)
declare {slice, i257} @llvm.tvm.dictiget(i257 %address, cell %dict, i257 %precision)
declare void @llvm.tvm.throw(i257 %errcode) noreturn
declare {i257, slice} @llvm.tvm.ldi(slice %slice, i257 %precision)
declare cell @llvm.tvm.plddict(slice %dict)
