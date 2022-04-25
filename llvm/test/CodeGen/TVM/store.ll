; RUN: llc -march tvm -asm-verbose=false < %s | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: __tvm_store_global
define void @__tvm_store_global(i257* nocapture %p, i257 %v) {
entry:
; CHECK:        NEWC
; CHECK-NEXT:   PUSHINT 257
; CHECK-NEXT:   STIX
; CHECK-NEXT:   ENDC
; CHECK-NEXT:   CTOS
  %address = ptrtoint i257* %p to i257
  %builder = call builder @llvm.tvm.newc()
  %builder.1 = call builder @llvm.tvm.sti(i257 %v, builder %builder, i257 257)
  %cell = call cell @llvm.tvm.endc(builder %builder.1)
  %slice = call slice @llvm.tvm.ctos(cell %cell)

; CHECK-NEXT:   GETGLOB 1
; CHECK-NEXT:   PUSHINT 64
; CHECK-NEXT:   ROLL    2
; CHECK-NEXT:   ROLL    3
; CHECK-NEXT:   ROLL    3
; CHECK-NEXT:   ROLL    3
; CHECK-NEXT:   DICTISET
; CHECK-NEXT:   SETGLOB 1
  %c7 = call i257 @llvm.tvm.getglobal(i257 1)
  %c7.dict = call cell @llvm.tvm.cast.to.cell(i257 %c7)
  %c7.dict.new = call cell @llvm.tvm.dictiset(slice %slice, i257 %address, cell %c7.dict, i257 64)
  %c7.dict.new.i = call i257 @llvm.tvm.cast.from.cell(cell %c7.dict.new)
  call void @llvm.tvm.setglobal(i257 1, i257 %c7.dict.new.i)
  ret void
}

; CHECK-LABEL: __tvm_store_persistent
define void @__tvm_store_persistent(i257* nocapture %p, i257 %v) {
entry:
; CHECK:        NEWC
; CHECK-NEXT:   PUSHINT 257
; CHECK-NEXT:   STIX
; CHECK-NEXT:   ENDC
; CHECK-NEXT:   CTOS
  %address = ptrtoint i257* %p to i257
  %builder = call builder @llvm.tvm.newc()
  %builder.1 = call builder @llvm.tvm.sti(i257 %v, builder %builder, i257 257)
  %cell = call cell @llvm.tvm.endc(builder %builder.1)
  %slice = call slice @llvm.tvm.ctos(cell %cell)

; CHECK-NEXT:   PUSH    c4
; CHECK-NEXT:   CTOS
; CHECK-NEXT:   PLDDICT
; CHECK-NEXT:   PUSHINT 64
; CHECK-NEXT:   ROLL    2
; CHECK-NEXT:   ROLL    3
; CHECK-NEXT:   ROLL    3
; CHECK-NEXT:   ROLL    3
; CHECK-NEXT:   DICTISET
; CHECK-NEXT:   NEWC
; CHECK-NEXT:   STDICT
; CHECK-NEXT:   ENDC
; CHECK-NEXT:   POP     c4
  %c4 = call i257 @llvm.tvm.getreg(i257 4)
  %c4.cell = call cell @llvm.tvm.cast.to.cell(i257 %c4)
  %c4.slice = call slice @llvm.tvm.ctos(cell %c4.cell)
  %c4.dict = call cell @llvm.tvm.plddict(slice %c4.slice)
  %c4.dict.1 = call cell @llvm.tvm.dictiset(slice %slice, i257 %address, cell %c4.dict, i257 64)
  %builder.2 = call builder @llvm.tvm.newc()
  %builder.3 = call builder @llvm.tvm.stdict(cell %c4.dict.1, builder %builder.2)
  %cell.1 = call cell @llvm.tvm.endc(builder %builder.3)
  %c4.new = call i257 @llvm.tvm.cast.from.cell(cell %cell.1)
  call void @llvm.tvm.setreg(i257 4, i257 %c4.new)
  ret void
}

define void @__tvm_store(i257* nocapture %p, i257 %v) {
entry:
  %address = ptrtoint i257* %p to i257
  %is_persistent = icmp slt i257 %address, 10000
  br i1 %is_persistent, label %persistent, label %globl
persistent:
  call void @__tvm_store_persistent(i257* %p, i257 %v)
  ret void
globl:
  call void @__tvm_store_global(i257* %p, i257 %v)
  ret void
}

declare i257 @llvm.tvm.getglobal(i257 %idx)
declare void @llvm.tvm.setglobal(i257 %v, i257 %idx)
declare i257 @llvm.tvm.getreg(i257 %regno)
declare void @llvm.tvm.setreg(i257 %regno, i257 %v)
declare tuple @llvm.tvm.cast.to.tuple(i257 %v)
declare cell @llvm.tvm.cast.to.cell(i257 %v)
declare slice @llvm.tvm.cast.to.slice(i257 %v)
declare i257 @llvm.tvm.cast.from.cell(cell %cell)
declare i257 @llvm.tvm.cast.from.slice(slice %slice)
declare slice @llvm.tvm.ctos(cell %cell)
declare void @llvm.tvm.ends(slice %slice)
declare {slice, i257} @llvm.tvm.dictiget(i257 %address, slice %slice, i257 %precision)
declare void @llvm.tvm.throwif(i257 %flag, i257 %errcode)
declare {i257, slice} @llvm.tvm.ldi(slice %slice, i257 %precision)
declare cell @llvm.tvm.plddict(slice %dict)
declare builder @llvm.tvm.newc()
declare cell @llvm.tvm.endc(builder %b)
declare builder @llvm.tvm.sti(i257 %value, builder %builder, i257 %size)
declare builder @llvm.tvm.stdict(cell %dict, builder %builder)
declare cell @llvm.tvm.dictiset(slice %value, i257 %key, cell %dict, i257 %precision)
