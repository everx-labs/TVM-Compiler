; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; CHECK-LABEL: newdict
define i64 @newdict() nounwind {
; CHECK: NEWDICT
  %1 = call i64 @llvm.tvm.newdict()
  ret i64 %1
}

; CHECK-LABEL: newc
define i64 @newc() nounwind {
; CHECK: NEWC
  %1 = call i64 @llvm.tvm.newc()
  ret i64 %1
}

; CHECK-LABEL: persistent_root
define i64 @persistent_root() nounwind {
; CHECK: PUSH c4
; CHECK-NEXT: CTOS
  %1 = call i64 @llvm.tvm.get.persistent.data()
  ret i64 %1
}

; CHECK-LABEL: set_persistent_root
define void @set_persistent_root() nounwind {
; CHECK: PUSH c4
; CHECK-NEXT: CTOS
; CHECK-NEXT: POPROOT
  %1 = call i64 @llvm.tvm.get.persistent.data()
  call void @llvm.tvm.set.persistent.data(i64 %1)
  ret void
}

; CHECK-LABEL: getreg
define i64 @getreg() nounwind {
; CHECK: PUSH c2
  %1 = call i64 @llvm.tvm.getreg(i64 2)
  ret i64 %1
}

; CHECK-LABEL: setreg
define void @setreg() nounwind {
; CHECK: PUSH c2
; CHECK: POP c2
  %1 = call i64 @llvm.tvm.getreg(i64 2)
  call void @llvm.tvm.setreg(i64 2, i64 %1)
  ret void
}

; CHECK-LABEL: itos
define i64 @itos(i64 %arg) nounwind {
; CHECK: NEWC
; CHECK-NEXT: STU 256
; CHECK-NEXT: ENDC
; CHECK-NEXT: CTOS
  %1 = call i64 @llvm.tvm.inttoslice(i64 %arg)
  ret i64 %1
}

; CHECK-LABEL: stoc
define i64 @stoc(i64 %cell) nounwind {
; CHECK: NEWC
; CHECK-NEXT: STSLICE
; CHECK-NEXT: ENDC
  %1 = call i64 @llvm.tvm.stoc(i64 %cell)
  ret i64 %1
}

; CHECK-LABEL: stslice
define i64 @stslice(i64 %slice, i64 %builder) nounwind {
; CHECK: STSLICE
  %1 = call i64 @llvm.tvm.stslice(i64 %slice, i64 %builder)
  ret i64 %1
}

; CHECK-LABEL: stref
define i64 @stref(i64 %cell, i64 %builder) nounwind {
; CHECK: STREF
  %1 = call i64 @llvm.tvm.stref(i64 %cell, i64 %builder)
  ret i64 %1
}

; CHECK-LABEL: ldref
define i64 @ldref(i64 %cell) nounwind {
; CHECK: LDREF
  %1 = call {i64, i64} @llvm.tvm.ldref(i64 %cell)
  %2 = extractvalue {i64, i64} %1, 0
  ret i64 %2
}

; CHECK-LABEL: ldslicex
define i64 @ldslicex(i64 %slice, i64 %size) nounwind {
; CHECK: LDSLICEX
  %1 = call {i64, i64} @llvm.tvm.ldslicex(i64 %slice, i64 %size)
  %2 = extractvalue {i64, i64} %1, 0
  ret i64 %2
}

; CHECK-LABEL: throws
define void @throws(i64 %cond) {
; CHEC: THROWIF 42
  call void @llvm.tvm.throwif(i64 %cond, i64 42)
  ret void
}

; CHECK-LABEL: throws_neg
define void @throws_neg(i64 %cond) {
; CHEC: THROWIFNOT 42
  %1 = xor i64 %cond, -1
  call void @llvm.tvm.throwif(i64 %1, i64 42)
  ret void
}

; CHECK-LABEL: throws_uncond
define void @throws_uncond() {
; CHECK: THROW 42
  call void @llvm.tvm.throw(i64 42)
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
  call void @llvm.tvm.dump(i64 1)
  ret void
}

; CHECK-LABEL: dump_value
define void @dump_value() {
; CHECK: PUSHINT 1
; CHECK: DUMP 0
; CHECK: DROP
  call void @llvm.tvm.dump.value(i64 1)
  ret void
}

; CHECK-LABEL: print
define void @print() {
; CHECK: PRINT 1
  call void @llvm.tvm.print(i64 1)
  ret void
}

; CHECK-LABEL: print_value
define void @print_value() {
; CHECK: PUSHINT 1
; CHECK: PRINT 0
; CHECK: DROP
  call void @llvm.tvm.print.value(i64 1)
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
  call void @llvm.tvm.dumpstktop(i64 1)
  ret void
}

declare i64 @llvm.tvm.newdict() nounwind
declare i64 @llvm.tvm.newc() nounwind
declare i64 @llvm.tvm.get.persistent.data() nounwind
declare i64 @llvm.tvm.getreg(i64 %regno) nounwind
declare void @llvm.tvm.setreg(i64 %regno, i64 %value) nounwind
declare void @llvm.tvm.set.persistent.data(i64 %root) nounwind
declare i64 @llvm.tvm.inttoslice(i64 %val) nounwind
declare i64 @llvm.tvm.stslice(i64 %slice, i64 %builder) nounwind
declare i64 @llvm.tvm.stoc(i64 %slice) nounwind
declare {i64, i64} @llvm.tvm.ldslicex(i64 %slice, i64 %size) nounwind
declare {i64, i64} @llvm.tvm.ldref(i64 %cell) nounwind
declare i64 @llvm.tvm.stref(i64 %cell, i64 %builder) nounwind
declare void @llvm.tvm.throwif(i64 %cond, i64 %exception)
declare void @llvm.tvm.throw(i64 %exception)
declare void @llvm.tvm.nop()
declare void @llvm.tvm.dumpstk()
declare void @llvm.tvm.dumpstktop(i64 %count)
declare void @llvm.tvm.dump(i64 %stack_index)
declare void @llvm.tvm.dump.value(i64 %value)
declare void @llvm.tvm.print(i64 %stack_index)
declare void @llvm.tvm.print.value(i64 %value)
declare void @llvm.tvm.logflush()
