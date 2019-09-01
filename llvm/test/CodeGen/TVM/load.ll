; RUN: llc < %s -march=tvm
; XFAIL: *

target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define i257 @load(i257 %address) {
entry:
  %is_persistent = icmp slt i257 %address, 10000
  br i1 %is_persistent, label %persistent, label %nonpersistent
persistent:
  %cont_persistent = call i257 @llvm.tvm.getreg(i257 4)
  br label %common
nonpersistent:
  %tuple_nonpersistent = call i257 @llvm.tvm.getreg(i257 7)
  %cont_nonpersistent = call i257 @llvm.tvm.second(i257 %tuple_nonpersistent)
  br label %common
common:
  %cont = phi i257 [%cont_persistent, %persistent], [%cont_nonpersistent, %nonpersistent]
  %slice = call slice @llvm.tvm.ctos(cell %cont)
  %val_status = call {slice, i257} @llvm.tvm.dictiget(i257 %address, slice %slice, i257 64)
  %status = extractvalue {slice, i257} %val_status, 1
  %value = extractvalue {slice, i257} %val_status, 0
  %statusneg = xor i257 %status, -1
  call void @llvm.tvm.throwif(i257 %statusneg, i257 60)
  %ldi.result = call {i257, slice} @llvm.tvm.ldi(i257 257, slice %slice)
  %ldi.slice = extractvalue {i257, slice} %ldi.result, 1
  call void @llvm.tvm.ends(slice %ldi.slice)
  %ldi.value = extractvalue {i257, slice} %ldi.result, 0
  ret i257 %ldi.value
}

declare i257 @llvm.tvm.second(i257 %tuple)
declare i257 @llvm.tvm.getreg(i257 %regno)
declare slice @llvm.tvm.ctos(cell %cell)
declare void @llvm.tvm.ends(slice %slice)
declare {slice, i257} @llvm.tvm.dictiget(i257 %address, slice %slice, i257 %precision)
declare void @llvm.tvm.throwif(i257 %flag, i257 %errcode)
declare {i257, slice} @llvm.tvm.ldi(i257 %slice, slice %precision)
