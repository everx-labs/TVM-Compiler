; RUN: llc < %s -march=tvm
; XFAIL: *

target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define void @store(i64 %address, i64 %value) {
entry:
  %builder = call i64 @llvm.tvm.newc()
  %builder.1 = call i64 @llvm.tvm.sti(i64 %builder, i64 %value, i64 257)
  %cell = call i64 @llvm.tvm.endc(i64 %builder.1)
  %slice = call i64 @llvm.tvm.ctos(i64 %cell)
  %is_persistent = icmp slt i64 %address, 100000
  br i1 %is_persistent, label %persistent, label %non_persistent
persistent:
  %c4 = call i64 @llvm.tvm.getreg(i64 4)
  %c4.slice = call i64 @llvm.tvm.ctos(i64 %c4)
  %c4.dict = call i64 @llvm.tvm.plddict(i64 %c4.slice)
  %c4.dict.1 = call i64 @llvm.tvm.dictiset(i64 %slice, i64 %address, i64 %c4.dict, i64 64)
  %builder.2 = call i64 @llvm.tvm.newc()
  %builder.3 = call i64 @llvm.tvm.stdict(i64 %c4.dict.1, i64 %builder.2)
  %cell.1 = call i64 @llvm.tvm.endc(i64 %builder.3)
  call void @llvm.tvm.setreg(i64 4, i64 %cell.1)
  ret void
non_persistent:
  %c7 = call i64 @llvm.tvm.getreg(i64 7)
  %c7.cell = call i64 @llvm.tvm.second(i64 %c7)
  %c7.slice = call i64 @llvm.tvm.ctos(i64 %c7.cell)
  %c7.dict = call i64 @llvm.tvm.plddict(i64 %c7.slice)
  %c7.dict.1 = call i64 @llvm.tvm.dictiset(i64 %slice, i64 %address, i64 %c7.dict, i64 64)
  %builder.4 = call i64 @llvm.tvm.newc()
  %builder.5 = call i64 @llvm.tvm.stdict(i64 %c7.dict.1, i64 %builder.4)
  %cell.2 = call i64 @llvm.tvm.endc(i64 %builder.5)
  %c7.new = call i64 @llvm.tvm.setsecond(i64 %c7, i64 %cell.2)
  call void @llvm.tvm.setreg(i64 7, i64 %c7.new)
  ret void
}

declare i64 @llvm.tvm.getreg(i64 %regno)
declare void @llvm.tvm.setreg(i64 %regno, i64 %value)
declare i64 @llvm.tvm.ctos(i64 %cell)
declare i64 @llvm.tvm.plddict(i64 %slice)
declare i64 @llvm.tvm.stdict(i64 %dict, i64 %builder)
declare i64 @llvm.tvm.dictiset(i64 %value, i64 %key, i64 %dict, i64 %precision)
declare i64 @llvm.tvm.sti(i64 %builder, i64 %value, i64 %precision)
declare i64 @llvm.tvm.newc()
declare i64 @llvm.tvm.endc(i64 %builder)
declare i64 @llvm.tvm.second(i64 %tuple)
declare i64 @llvm.tvm.setsecond(i64 %tuple, i64 %value)
