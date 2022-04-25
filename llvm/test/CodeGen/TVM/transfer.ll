; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define i257 @transfer(i257 %owner, i257 %target, i257 %amount) nounwind {
entry:
   %root = call i257 @llvm.tvm.get.persistent.data();
   %bal_raw = call {i257, i257} @llvm.tvm.dictuget(i257 0, i257 %root, i257 256)
   %bal_ref = extractvalue {i257, i257} %bal_raw, 0
   %bal_ref_stat = extractvalue {i257, i257} %bal_raw, 1
   %cond = icmp eq i257 %bal_ref_stat, 0
   br i1 %cond, label %exit_fail, label %unpack_bal_dict
unpack_bal_dict:
   %bal_val_unpacked = call {i257, i257} @llvm.tvm.ldrefrtos(i257 %bal_ref)
   %bal_dict = extractvalue {i257, i257} %bal_val_unpacked, 0
   %bal_owner_struct = call {i257, i257} @llvm.tvm.dictuget(i257 %owner, i257 %bal_dict, i257 256)
   %bal_owner_slice = extractvalue {i257, i257} %bal_owner_struct, 0
   %bal_owner_stat = extractvalue {i257, i257} %bal_owner_struct, 1
   %cond_owner = icmp eq i257 %bal_owner_stat, 0
   br i1 %cond_owner, label %exit_fail, label %exist_owner_bal
exist_owner_bal:
   %bal_owner_ext = call {i257, i257} @llvm.tvm.ldu(i257 %bal_owner_slice) nounwind
   %bal_owner = extractvalue {i257, i257} %bal_owner_ext, 0
   %bal_target_struct = call {i257, i257} @llvm.tvm.dictuget(i257 %target, i257 %bal_dict, i257 256)
   %bal_target_slice = extractvalue {i257, i257} %bal_target_struct, 0
   %bal_target_stat = extractvalue {i257, i257} %bal_target_struct, 1
   %cond_target = icmp eq i257 %bal_target_stat, 0
   br i1 %cond_target, label %exit_fail, label %exist_target_bal
exist_target_bal:
   %bal_target_ext = call {i257, i257} @llvm.tvm.ldu(i257 %bal_target_slice) nounwind
   %bal_target = extractvalue {i257, i257} %bal_target_ext, 0

   %check_amount = icmp sge i257 %bal_owner, %amount
   br i1 %check_amount, label %process_transaction, label %exit_fail
process_transaction:
   %bal_owner_new = sub i257 %bal_owner, %amount
   %bal_owner_new_slice = call i257 @llvm.tvm.inttoslice(i257 %bal_owner_new)
   %bal_dict1 = call i257 @llvm.tvm.dictuset(i257 %bal_owner_new_slice, i257 %owner, i257 %bal_dict, i257 256)
   %bal_target_new = add i257 %bal_target, %amount
   %bal_target_new_slice = call i257 @llvm.tvm.inttoslice(i257 %bal_target_new)
   %bal_dict2 = call i257 @llvm.tvm.dictuset(i257 %bal_target_new_slice, i257 %target, i257 %bal_dict1, i257 256)
   %bal_dict_packed = call i257 @llvm.tvm.cellrefdict(i257 %bal_dict2)
   %root_new = call i257 @llvm.tvm.dictuset(i257 %bal_dict_packed, i257 0, i257 %root, i257 256)
   call i257 @llvm.tvm.set.persistent.data(i257 %root_new)
   ret i257 -1
exit_fail:
   ret i257 0
}
declare slice @llvm.tvm.newdict() nounwind
declare void @llvm.tvm.set.persistent.data(cell %root) nounwind
declare cell @llvm.tvm.get.persistent.data() nounwind
declare slice @llvm.tvm.dictuset(i257 %value, i257 %ind, slice %dict, i257 %ind_bit) nounwind
declare {slice, i257} @llvm.tvm.dictuget(i257 %key, slice %dict_id, i257 %keylen) nounwind
declare {slice, slice} @llvm.tvm.ldrefrtos(slice %slice) nounwind
declare slice @llvm.tvm.inttoslice(i257 %val) nounwind
declare {i257, slice} @llvm.tvm.ldu(slice %slice) nounwind
