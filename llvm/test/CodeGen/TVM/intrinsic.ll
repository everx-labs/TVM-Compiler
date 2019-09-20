; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: newdict
define slice @newdict() nounwind {
; CHECK: NEWDICT
  %1 = call slice @llvm.tvm.newdict()
  ret slice %1
}

; CHECK-LABEL: newc
define builder @newc() nounwind {
; CHECK: NEWC
  %1 = call builder @llvm.tvm.newc()
  ret builder %1
}

; CHECK-LABEL: persistent_root
define slice @persistent_root() nounwind {
; CHECK: PUSHROOT
; CHECK-NEXT: CTOS
  %1 = call cell @llvm.tvm.get.persistent.data()
  %2 = call slice @llvm.tvm.ctos(cell %1)
  ret slice %2
}

; CHECK-LABEL: set_persistent_root
define void @set_persistent_root() nounwind {
; CHECK: PUSHROOT
; CHECK-NEXT: POPROOT
  %1 = call cell @llvm.tvm.get.persistent.data()
  call void @llvm.tvm.set.persistent.data(cell %1)
  ret void
}

; CHECK-LABEL: getreg
define i257 @getreg() nounwind {
; CHECK: PUSH c2
  %1 = call i257 @llvm.tvm.getreg(i257 2)
  ret i257 %1
}

; CHECK-LABEL: setreg
define void @setreg() nounwind {
; CHECK: PUSH c2
; CHECK: POP c2
  %1 = call i257 @llvm.tvm.getreg(i257 2)
  call void @llvm.tvm.setreg(i257 2, i257 %1)
  ret void
}

; CHECK-LABEL: itos
define slice @itos(i257 %arg) nounwind {
; CHECK: NEWC
; CHECK-NEXT: STU 256
; CHECK-NEXT: ENDC
; CHECK-NEXT: CTOS
  %1 = call slice @llvm.tvm.inttoslice(i257 %arg)
  ret slice %1
}

; CHECK-LABEL: stoc
define cell @stoc(slice %slice) nounwind {
; CHECK: NEWC
; CHECK-NEXT: STSLICE
; CHECK-NEXT: ENDC
  %1 = call cell @llvm.tvm.stoc(slice %slice)
  ret cell %1
}

; CHECK-LABEL: stslice
define builder @stslice(slice %slice, builder %builder) nounwind {
; CHECK: STSLICE
  %1 = call builder @llvm.tvm.stslice(slice %slice, builder %builder)
  ret builder %1
}

; CHECK-LABEL: stref
define builder @stref(cell %cell, builder %builder) nounwind {
; CHECK: STREF
  %1 = call builder @llvm.tvm.stref(cell %cell, builder %builder)
  ret builder %1
}

; CHECK-LABEL: ldref
define cell @ldref(slice %slice) nounwind {
; CHECK: LDREF
  %1 = call {cell, slice} @llvm.tvm.ldref(slice %slice)
  %2 = extractvalue {cell, slice} %1, 0
  ret cell %2
}

; CHECK-LABEL: ldslicex
define slice @ldslicex(slice %slice, i257 %size) nounwind {
; CHECK: LDSLICEX
  %1 = call {slice, slice} @llvm.tvm.ldslicex(slice %slice, i257 %size)
  %2 = extractvalue {slice, slice} %1, 0
  ret slice %2
}

; CHECK-LABEL: throws
define void @throws(i257 %cond) {
; CHECK: THROWIF 42
  call void @llvm.tvm.throwif(i257 %cond, i257 42)
  ret void
}

; CHECK-LABEL: throws_neg
define void @throws_neg(i257 %cond) {
; CHECK: THROWIFNOT 42
  %1 = xor i257 %cond, -1
  call void @llvm.tvm.throwif(i257 %1, i257 42)
  ret void
}

; CHECK-LABEL: throws_uncond
define void @throws_uncond() {
; CHECK: THROW 42
  call void @llvm.tvm.throw(i257 42)
  ret void
}

; CHECK-LABEL: nop
define void @nop() {
; CHECK: NOP
  call void @llvm.tvm.nop()
  ret void
}

; CHECK-LABEL: dump
define void @dump() {
; CHECK: DUMP 1
  call void @llvm.tvm.dump(i257 1)
  ret void
}

; CHECK-LABEL: dump_value
define void @dump_value() {
; CHECK: PUSHINT 1
; CHECK: DUMP 0
; CHECK: DROP
  call void @llvm.tvm.dump.value(i257 1)
  ret void
}

; CHECK-LABEL: print
define void @print() {
; CHECK: PRINT 1
  call void @llvm.tvm.print(i257 1)
  ret void
}

; CHECK-LABEL: print_value
define void @print_value() {
; CHECK: PUSHINT 1
; CHECK: PRINT 0
; CHECK: DROP
  call void @llvm.tvm.print.value(i257 1)
  ret void
}

; CHECK-LABEL: logflush
define void @logflush() {
; CHECK: LOGFLUSH
  call void @llvm.tvm.logflush()
  ret void
}

; CHECK-LABEL: dumpstk
define void @dumpstk() {
; CHECK: DUMPSTK
  call void @llvm.tvm.dumpstk()
  ret void
}

; CHECK-LABEL: dumpstktop
define void @dumpstktop() {
; CHECK: DUMPSTKTOP 1
  call void @llvm.tvm.dumpstktop(i257 1)
  ret void
}

declare slice @llvm.tvm.newdict() nounwind
declare builder @llvm.tvm.newc() nounwind
declare cell @llvm.tvm.get.persistent.data() nounwind
declare i257 @llvm.tvm.getreg(i257 %regno) nounwind
declare void @llvm.tvm.setreg(i257 %regno, i257 %value) nounwind
declare void @llvm.tvm.set.persistent.data(cell %root) nounwind
declare slice @llvm.tvm.ctos(cell %cell) nounwind
declare slice @llvm.tvm.inttoslice(i257 %val) nounwind
declare builder @llvm.tvm.stslice(slice %slice, builder %builder) nounwind
declare cell @llvm.tvm.stoc(slice %slice) nounwind
declare {slice, slice} @llvm.tvm.ldslicex(slice %slice, i257 %size) nounwind
declare {cell, slice} @llvm.tvm.ldref(slice %slice) nounwind
declare builder @llvm.tvm.stref(cell %cell, builder %builder) nounwind
declare void @llvm.tvm.throwif(i257 %cond, i257 %exception)
declare void @llvm.tvm.throw(i257 %exception)
declare void @llvm.tvm.nop()
declare void @llvm.tvm.dumpstk()
declare void @llvm.tvm.dumpstktop(i257 %count)
declare void @llvm.tvm.dump(i257 %stack_index)
declare void @llvm.tvm.dump.value(i257 %value)
declare void @llvm.tvm.print(i257 %stack_index)
declare void @llvm.tvm.print.value(i257 %value)
declare void @llvm.tvm.logflush()
