; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: allowance
define i257 @allowance(i257 %tokenOwner, i257 %spender) nounwind {
entry:
  %persistent_root_cell = call cell @llvm.tvm.get.persistent.data();
  %persistent_root = call slice @llvm.tvm.ctos(cell %persistent_root_cell)
  %result = call {slice, i257} @llvm.tvm.dictuget(i257 1, slice %persistent_root, i257 256)
  %data = extractvalue {slice, i257} %result, 0
  %status = extractvalue {slice, i257} %result, 1
  %cond = icmp ne i257 %status, 0
  br i1 %cond, label %bb2, label %bb1
bb1:
  %dict_bb1 = call slice @llvm.tvm.newdict()
  br label %bb3
bb2:
  %ref_rtos = call {slice, slice} @llvm.tvm.ldrefrtos(slice %data)
  %dict_bb2 = extractvalue {slice, slice} %ref_rtos, 0
  br label %bb3
bb3:
  %dict = phi slice [%dict_bb1, %bb1], [%dict_bb2, %bb2]
  %spender_amount_status = call {slice, i257} @llvm.tvm.dictuget(i257 %spender, slice %dict, i257 256)
  %amount = extractvalue {slice, i257} %spender_amount_status, 0
  %status1 = extractvalue {slice, i257} %spender_amount_status, 1
  %cond1 = icmp ne i257 %status1, 0
  br i1 %cond1, label %bb5, label %bb4
bb4:
  %new_slice = call slice @llvm.tvm.inttoslice(i257 0)
  br label %bb5
bb5:
  %result_slice = phi slice [%amount, %bb3], [%new_slice, %bb4]
  %retres_comb = call {i257, slice} @llvm.tvm.ldu(slice %result_slice)
  %retres = extractvalue {i257, slice} %retres_comb, 0
  ret i257 %retres
}

declare slice @llvm.tvm.newdict() nounwind
declare cell @llvm.tvm.get.persistent.data() nounwind
declare slice @llvm.tvm.ctos(cell %cell) nounwind
declare {slice, i257} @llvm.tvm.dictuget(i257 %key, slice %dict_id, i257 %keylen) nounwind
declare {slice, slice} @llvm.tvm.ldrefrtos(slice %slice) nounwind
declare slice @llvm.tvm.inttoslice(i257 %val) nounwind
declare {i257, slice} @llvm.tvm.ldu(slice %slice) nounwind
