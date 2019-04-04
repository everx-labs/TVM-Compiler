; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define i64 @create_persistent_data() nounwind {
  %bal1 = call i64 @llvm.tvm.inttoslice(i64 100)
  %bal2 = call i64 @llvm.tvm.inttoslice(i64 200)
  %1 = call i64 @llvm.tvm.newdict()
  %2 = call i64 @llvm.tvm.dictuset(i64 %bal1, i64 0, i64 %1, i64 256)
  %dict_balance = call i64 @llvm.tvm.dictuset(i64 %bal2, i64 1, i64 %2, i64 256)
  %dict_balance_ref = call i64 @llvm.tvm.cellrefdict(i64 %dict_balance)

  %allowed01 = call i64 @llvm.tvm.inttoslice(i64 50)
  %3 = call i64 @llvm.tvm.newdict()
  %sub_dict_allowance = call i64 @llvm.tvm.dictuset(i64 %allowed01, i64 1, i64 %3, i64 256)
  %sub_dict_allowed_ref = call i64 @llvm.tvm.cellrefdict(i64 %sub_dict_allowance)
  %4 = call i64 @llvm.tvm.newdict()
  %dict_allowed = call i64 @llvm.tvm.dictuset(i64 %sub_dict_allowed_ref, i64 0, i64 %4, i64 256)
  %dict_allowed_ref = call i64 @llvm.tvm.cellrefdict(i64 %dict_allowed)

  %total_supply = call i64 @llvm.tvm.inttoslice(i64 1000000)

  %6 = call i64 @llvm.tvm.newdict()
  %7 = call i64 @llvm.tvm.dictuset(i64 %dict_balance_ref, i64 0, i64 %6, i64 256)
  %8 = call i64 @llvm.tvm.dictuset(i64 %dict_allowed_ref, i64 1, i64 %7, i64 256)
  %persistent_data = call i64 @llvm.tvm.dictuset(i64 %total_supply, i64 2, i64 %8, i64 256)
  %res = call i64 @llvm.tvm.set.persistent.data(i64 %persistent_data)

  ret i64 %res
}

declare i64 @llvm.tvm.newdict() nounwind
declare i64 @llvm.tvm.inttoslice(i64 %val) nounwind
declare i64 @llvm.tvm.set.persistent.data(i64 %index) nounwind
declare i64 @llvm.tvm.dictuset(i64 %value, i64 %ind, i64 %dict, i64 %ind_bit) nounwind
declare i64 @llvm.tvm.cellrefdict(i64 %dict) nounwind
