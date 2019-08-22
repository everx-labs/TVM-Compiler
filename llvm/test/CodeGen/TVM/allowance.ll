; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: allowance
define i257 @allowance(i257 %tokenOwner, i257 %spender) nounwind {
entry:
  %persistent_root = call i257 @llvm.tvm.get.persistent.data();
  %result = call {i257, i257} @llvm.tvm.dictuget(i257 1, i257 %persistent_root, i257 256)
  %data = extractvalue {i257, i257} %result, 0
  %status = extractvalue {i257, i257} %result, 1
  %cond = icmp ne i257 %status, 0
  br i1 %cond, label %bb2, label %bb1
bb1:
  %dict_bb1 = call i257 @llvm.tvm.newdict()
  br label %bb3
bb2:
  %ref_rtos = call {i257, i257} @llvm.tvm.ldrefrtos(i257 %data)
  %dict_bb2 = extractvalue {i257, i257} %ref_rtos, 0
  br label %bb3
bb3:
  %dict = phi i257 [%dict_bb1, %bb1], [%dict_bb2, %bb2]
  %spender_amount_status = call {i257, i257} @llvm.tvm.dictuget(i257 %spender, i257 %dict, i257 256)
  %amount = extractvalue {i257, i257} %spender_amount_status, 0
  %status1 = extractvalue {i257, i257} %spender_amount_status, 1
  %cond1 = icmp ne i257 %status1, 0
  br i1 %cond1, label %bb5, label %bb4
bb4:
  %new_slice = call i257 @llvm.tvm.inttoslice(i257 0)
  br label %bb5
bb5:
  %result_slice = phi i257 [%amount, %bb3], [%new_slice, %bb4]
  %retres_comb = call {i257, i257} @llvm.tvm.ldu(i257 %result_slice)
  %retres = extractvalue {i257, i257} %retres_comb, 0
  ret i257 %retres
}

declare i257 @llvm.tvm.newdict() nounwind
declare i257 @llvm.tvm.get.persistent.data() nounwind
declare {i257, i257} @llvm.tvm.dictuget(i257 %key, i257 %dict_id, i257 %keylen) nounwind
declare {i257, i257} @llvm.tvm.ldrefrtos(i257 %slice) nounwind
declare i257 @llvm.tvm.inttoslice(i257 %val) nounwind
declare {i257, i257} @llvm.tvm.ldu(i257 %slice) nounwind
