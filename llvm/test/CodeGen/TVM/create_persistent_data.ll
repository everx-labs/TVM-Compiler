; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define void @create_persistent_data() nounwind {
  %bal1 = call i257 @llvm.tvm.inttoslice(i257 100)
  %bal2 = call i257 @llvm.tvm.inttoslice(i257 200)
  %1 = call i257 @llvm.tvm.newdict()
  %2 = call i257 @llvm.tvm.dictuset(i257 %bal1, i257 0, i257 %1, i257 256)
  %dict_balance = call i257 @llvm.tvm.dictuset(i257 %bal2, i257 1, i257 %2, i257 256)
  %dict_balance_ref = call i257 @llvm.tvm.cellrefdict(i257 %dict_balance)

  %allowed01 = call i257 @llvm.tvm.inttoslice(i257 50)
  %3 = call i257 @llvm.tvm.newdict()
  %sub_dict_allowance = call i257 @llvm.tvm.dictuset(i257 %allowed01, i257 1, i257 %3, i257 256)
  %sub_dict_allowed_ref = call i257 @llvm.tvm.cellrefdict(i257 %sub_dict_allowance)
  %4 = call i257 @llvm.tvm.newdict()
  %dict_allowed = call i257 @llvm.tvm.dictuset(i257 %sub_dict_allowed_ref, i257 0, i257 %4, i257 256)
  %dict_allowed_ref = call i257 @llvm.tvm.cellrefdict(i257 %dict_allowed)

  %total_supply = call i257 @llvm.tvm.inttoslice(i257 1000000)

  %6 = call i257 @llvm.tvm.newdict()
  %7 = call i257 @llvm.tvm.dictuset(i257 %dict_balance_ref, i257 0, i257 %6, i257 256)
  %8 = call i257 @llvm.tvm.dictuset(i257 %dict_allowed_ref, i257 1, i257 %7, i257 256)
  %persistent_data = call i257 @llvm.tvm.dictuset(i257 %total_supply, i257 2, i257 %8, i257 256)
  call i257 @llvm.tvm.set.persistent.data(i257 %persistent_data)

  ret void
}

declare i257 @llvm.tvm.newdict() nounwind
declare i257 @llvm.tvm.inttoslice(i257 %val) nounwind
declare void @llvm.tvm.set.persistent.data(i257 %index) nounwind
declare i257 @llvm.tvm.dictuset(i257 %value, i257 %ind, i257 %dict, i257 %ind_bit) nounwind
declare i257 @llvm.tvm.cellrefdict(i257 %dict) nounwind
