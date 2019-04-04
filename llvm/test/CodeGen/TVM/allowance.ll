; XFAIL: *

; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; CHECK-LABEL: allowance
define i64 @allowance(i64 %tokenOwner, i64 %spender) nounwind {
entry:
  %persistent_root = call i64 @llvm.tvm.get.persistent.data(i64 1);
  %result = call {i64, i64} @llvm.tvm.dictuget(i64 1, i64 %persistent_root, i64 256)
  %data = extractvalue {i64, i64} %result, 0
  %status = extractvalue {i64, i64} %result, 1
  %cond = icmp eq i64 %status, 0
  br i1 %cond, label %bb1, label %bb2
bb1:
  %dict_bb1 = call i64 @llvm.tvm.newdict()
  br label %bb3
bb2:
  %ref_rtos = call {i64, i64} @llvm.tvm.ldrefrtos(i64 %data)
  %dict_bb2 = extractvalue {i64, i64} %ref_rtos, 0
  br label %bb3
bb3:
  %dict = phi i64 [%dict_bb1, %bb1], [%dict_bb2, %bb2]
  %spender_amount_status = call {i64, i64} @llvm.tvm.dictuget(i64 %spender, i64 %dict, i64 256)
  %amount = extractvalue {i64, i64} %spender_amount_status, 0
  %status1 = extractvalue {i64, i64} %spender_amount_status, 1
  %cond1 = icmp eq i64 %status1, 0
  br i1 %cond1, label %bb4, label %bb5
bb4:
  %new_slice = call i64 @llvm.tvm.inttoslice(i64 0)
  br label %bb5
bb5:
  %result_slice = phi i64 [%amount, %bb3], [%new_slice, %bb4]
  %retres_comb = call {i64, i64} @llvm.tvm.ldu(i64 %result_slice)
  %retres = extractvalue {i64, i64} %retres_comb, 0
  ret i64 %retres
}

declare i64 @llvm.tvm.newdict() nounwind
declare i64 @llvm.tvm.get.persistent.data(i64 %index) nounwind
declare {i64, i64} @llvm.tvm.dictuget(i64 %key, i64 %dict_id, i64 %keylen) nounwind
declare {i64, i64} @llvm.tvm.ldrefrtos(i64 %slice) nounwind
declare i64 @llvm.tvm.inttoslice(i64 %val) nounwind
declare {i64, i64} @llvm.tvm.ldu(i64 %slice) nounwind
