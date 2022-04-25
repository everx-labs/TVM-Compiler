; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; =================================== A.4 =====================================
; CHECK-LABEL: pushref
define cell @pushref() nounwind {
; CHECK: PUSHREF
  %ref = call cell @llvm.tvm.pushref()
  ret cell %ref
}

; CHECK-LABEL: pushrefslice
define slice @pushrefslice() nounwind {
; CHECK: PUSHREFSLICE
  %ref = call cell @llvm.tvm.pushref()
  %slice = call slice @llvm.tvm.ctos(cell %ref)
  ret slice %slice
}

; =================================== A.6 =====================================
; CHECK-LABEL: sempty
define i257 @sempty(slice %slice) {
; CHECK: SEMPTY
  %empty = call i257 @llvm.tvm.sempty(slice %slice)
  ret i257 %empty
}

; CHECK-LABEL: sdempty
define i257 @sdempty(slice %slice) {
; CHECK: SDEMPTY
  %empty = call i257 @llvm.tvm.sdempty(slice %slice)
  ret i257 %empty
}

; CHECK-LABEL: srempty
define i257 @srempty(slice %slice) {
; CHECK: SREMPTY
  %refempty = call i257 @llvm.tvm.srempty(slice %slice)
  ret i257 %refempty
}

; =================================== A.7 =====================================
; CHECK-LABEL: ends
define void @ends(slice %cell) {
; CHECK: ENDS
  call void @llvm.tvm.ends(slice %cell)
  ret void
}

; CHECK-LABEL: sti
define builder @sti(i257 %value, builder %builder) {
; CHECK: ZERO
; CHECK: STIX
  %builder.1 = call builder @llvm.tvm.sti(i257 %value, builder %builder, i257 0)
; CHECK: STI 42
  %builder.2 = call builder @llvm.tvm.sti(i257 %value, builder %builder.1, i257 42)
; CHECK: STI 256
  %builder.3 = call builder @llvm.tvm.sti(i257 %value, builder %builder.2, i257 256)
; CHECK: PUSHINT 257
; CHECK: STIX
  %builder.4 = call builder @llvm.tvm.sti(i257 %value, builder %builder.3, i257 257)
  ret builder %builder.4
}

; CHECK-LABEL: stix
define builder @stix(i257 %value, builder %builder, i257 %size) {
; CHECK: STIX
  %newbuilder = call builder @llvm.tvm.sti(i257 %value, builder %builder, i257 %size)
  ret builder %newbuilder
}

; CHECK-LABEL: stux
define builder @stux(i257 %value, builder %builder, i257 %size) {
; CHECK: STUX
  %newbuilder = call builder @llvm.tvm.stu(i257 %value, builder %builder, i257 %size)
  ret builder %newbuilder
}

; CHECK-LABEL: ldi
define slice @ldi(slice %slice, i257 %size) {
; CHECK: LDIX
  %ldi.1 = call {i257, slice} @llvm.tvm.ldi(slice %slice, i257 %size)
  %slice.1 = extractvalue {i257, slice} %ldi.1, 1
; CHECK: ZERO
; CHECK: LDIX
  %ldi.2 = call {i257, slice} @llvm.tvm.ldi(slice %slice.1, i257 0)
  %slice.2 = extractvalue {i257, slice} %ldi.2, 1
; CHECK: LDI 42
  %ldi.3 = call {i257, slice} @llvm.tvm.ldi(slice %slice.2, i257 42)
  %slice.3 = extractvalue {i257, slice} %ldi.3, 1
; CHECK: LDI 256
  %ldi.4 = call {i257, slice} @llvm.tvm.ldi(slice %slice.3, i257 256)
  %slice.4 = extractvalue {i257, slice} %ldi.4, 1
; CHECK: PUSHINT 257
; CHECK: LDIX
  %ldi.5 = call {i257, slice} @llvm.tvm.ldi(slice %slice.4, i257 257)
  %slice.5 = extractvalue {i257, slice} %ldi.5, 1
  ret slice %slice.5
}

; =================================== A.X =====================================
; CHECK-LABEL: newdict
define cell @newdict() nounwind {
; CHECK: NEWDICT
  %1 = call cell @llvm.tvm.newdict()
  ret cell %1
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
  %1 = call builder @llvm.tvm.newc()
  %2 = call builder @llvm.tvm.stu(i257 %arg, builder %1, i257 256)
  %3 = call cell @llvm.tvm.endc(builder %2)
  %4 = call slice @llvm.tvm.ctos(cell %3)
  ret slice %4
}

; CHECK-LABEL: stoc
define cell @stoc(slice %slice) nounwind {
; CHECK: NEWC
; CHECK-NEXT: STSLICE
; CHECK-NEXT: ENDC
  %1 = call builder @llvm.tvm.newc()
  %2 = call builder @llvm.tvm.stslice(slice %slice, builder %1)
  %3 = call cell @llvm.tvm.endc(builder %2)
  ret cell %3
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

; CHECK-LABEL: ldslice
define slice @ldslice(slice %slice, i257 %size) nounwind {
; CHECK: LDSLICEX
  %1 = call {slice, slice} @llvm.tvm.ldslice(slice %slice, i257 %size)
  %2 = extractvalue {slice, slice} %1, 0
  ret slice %2
}

; CHECK-LABEL: throws
define void @throws(i257 %cond) {
; CHECK: THROWIF 42
  %flag = trunc i257 %cond to i1
  br i1 %flag, label %do_throw, label %ok
ok:
  ret void
do_throw:
  call void @llvm.tvm.throw(i257 42)
  unreachable
}

; CHECK-LABEL: throws_neg
define void @throws_neg(i257 %cond) {
; CHECK: THROWIFNOT 42
  %flag = trunc i257 %cond to i1
  br i1 %flag, label %ok, label %do_throw
ok:
  ret void
do_throw:
  call void @llvm.tvm.throw(i257 42)
  unreachable
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
; CHECK: ONE
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
; CHECK: ONE
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

; =================================== A.10 ====================================
; CHECK-LABEL: plddict
define cell @plddict(slice %slice) readnone {
; CHECK PLDDICT
  %dict = call cell @llvm.tvm.plddict(slice %slice)
  ret cell %dict
}

; CHECK-LABEL: stdicts
define builder @stdicts(slice %slice, builder %builder) readnone {
; CHECK STDICTS
  %dict = call builder @llvm.tvm.stdicts(slice %slice, builder %builder)
  ret builder %dict
}

; CHECK-LABEL: stdict
define builder @stdict(cell %dict, builder %builder) readnone {
; CHECK STDICT
  %dictnew = call builder @llvm.tvm.stdict(cell %dict, builder %builder)
  ret builder %dictnew
}
; =================================== A.11 ====================================
; CHECK-LABEL: accept
define void @accept() {
; CHECK: ACCEPT
  call void @llvm.tvm.accept()
  ret void
}

; CHECK-LABEL: hashcu
define i257 @hashcu(cell %cell) {
; CHECK: HASHCU
  %result = call i257 @llvm.tvm.hashcu(cell %cell)
  ret i257 %result
}

; CHECK-LABEL: hashsu
define i257 @hashsu(slice %sl) {
; CHECK: HASHSU
  %result = call i257 @llvm.tvm.hashsu(slice %sl)
  ret i257 %result
}

; CHECK-LABEL: chksignu
define i257 @chksignu(i257 %hash, slice %signature, i257 %key) {
; CHECK: CHKSIGNU
  %result = call i257 @llvm.tvm.chksignu(i257 %hash, slice %signature, i257 %key)
  ret i257 %result
}

; CHECK-LABEL: cdatasize
define {i257, i257, i257} @cdatasize(cell %c, i257 %len) {
; CHECK: CDATASIZE
  %result = call {i257, i257, i257} @llvm.tvm.cdatasize(cell %c, i257 %len)
  ret {i257, i257, i257} %result
}
; =================================== A.13 ====================================
; CHECK-LABEL: setcp
define void @setcp() {
; CHECK: SETCP 42
  call void @llvm.tvm.setcp(i257 42)
  ret void
}

declare void @llvm.tvm.nop()
declare i257 @llvm.tvm.first(i257 %tuple)
declare i257 @llvm.tvm.second(i257 %tuple)
declare i257 @llvm.tvm.setfirst(i257 %tuple, i257 %value)
declare i257 @llvm.tvm.setsecond(i257 %tuple, i257 %value)
declare cell @llvm.tvm.pushref()
declare i257 @llvm.tvm.sempty(slice %slice)
declare i257 @llvm.tvm.sdempty(slice %slice)
declare i257 @llvm.tvm.srempty(slice %slice)
declare void @llvm.tvm.ends(slice %slice)
declare slice @llvm.tvm.ctos(cell %cell)
declare builder @llvm.tvm.sti(i257 %value, builder %builder, i257 %size)
declare builder @llvm.tvm.stu(i257 %value, builder %builder, i257 %size)
declare {i257, slice} @llvm.tvm.ldi(slice %slice, i257 %size)
declare cell @llvm.tvm.newdict() nounwind
declare builder @llvm.tvm.newc() nounwind
declare cell @llvm.tvm.endc(builder %b) nounwind
declare cell @llvm.tvm.get.persistent.data() nounwind
declare i257 @llvm.tvm.getreg(i257 %regno) nounwind
declare void @llvm.tvm.setreg(i257 %regno, i257 %value) nounwind
declare void @llvm.tvm.set.persistent.data(cell %root) nounwind
declare builder @llvm.tvm.stslice(slice %slice, builder %builder) nounwind
declare {slice, slice} @llvm.tvm.ldslice(slice %slice, i257 %size) nounwind
declare {cell, slice} @llvm.tvm.ldref(slice %slice) nounwind
declare cell @llvm.tvm.plddict(slice %dict)
declare builder @llvm.tvm.stdict(cell %dict, builder %builder)
declare builder @llvm.tvm.stdicts(slice %slice, builder %builder)
declare builder @llvm.tvm.stref(cell %cell, builder %builder) nounwind
declare void @llvm.tvm.throw(i257 %exception) noreturn
declare void @llvm.tvm.accept()
declare i257 @llvm.tvm.chksignu(i257 %hash, slice %signature, i257 %key)
declare i257 @llvm.tvm.hashcu(cell %cell)
declare i257 @llvm.tvm.hashsu(slice %sl)
declare void @llvm.tvm.setcp(i257 %codepage)
declare void @llvm.tvm.dump(i257 %slot)
declare void @llvm.tvm.dump.value(i257 %slot)
declare void @llvm.tvm.dumpstk()
declare void @llvm.tvm.dumpstktop(i257 %number)
declare void @llvm.tvm.logflush()
declare void @llvm.tvm.print(i257 %number)
declare void @llvm.tvm.print.value(i257 %number)
declare {i257, i257, i257} @llvm.tvm.cdatasize(cell %c, i257 %len)
