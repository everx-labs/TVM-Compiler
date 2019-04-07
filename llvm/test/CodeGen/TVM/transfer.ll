; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define i64 @transfer(i64 %owner, i64 %target, i64 %amount) nounwind {
entry:
   %root = call i64 @llvm.tvm.get.persistent.data();
   %bal_raw = call {i64, i64} @llvm.tvm.dictuget(i64 0, i64 %root, i64 256)
   %bal_ref = extractvalue {i64, i64} %bal_raw, 0
   %bal_ref_stat = extractvalue {i64, i64} %bal_raw, 1
   %cond = icmp eq i64 %bal_ref_stat, 0
   br i1 %cond, label %exit_fail, label %unpack_bal_dict
unpack_bal_dict:
   %bal_val_unpacked = call {i64, i64} @llvm.tvm.ldrefrtos(i64 %bal_ref)
   %bal_dict = extractvalue {i64, i64} %bal_val_unpacked, 0
   %bal_owner_struct = call {i64, i64} @llvm.tvm.dictuget(i64 %owner, i64 %bal_dict, i64 256)
   %bal_owner_slice = extractvalue {i64, i64} %bal_owner_struct, 0
   %bal_owner_stat = extractvalue {i64, i64} %bal_owner_struct, 1
   %cond_owner = icmp eq i64 %bal_owner_stat, 0
   br i1 %cond_owner, label %exit_fail, label %exist_owner_bal
exist_owner_bal:
   %bal_owner_ext = call {i64, i64} @llvm.tvm.ldu(i64 %bal_owner_slice) nounwind
   %bal_owner = extractvalue {i64, i64} %bal_owner_ext, 0
   %bal_target_struct = call {i64, i64} @llvm.tvm.dictuget(i64 %target, i64 %bal_dict, i64 256)
   %bal_target_slice = extractvalue {i64, i64} %bal_target_struct, 0
   %bal_target_stat = extractvalue {i64, i64} %bal_target_struct, 1
   %cond_target = icmp eq i64 %bal_target_stat, 0
   br i1 %cond_target, label %exit_fail, label %exist_target_bal
exist_target_bal:
   %bal_target_ext = call {i64, i64} @llvm.tvm.ldu(i64 %bal_target_slice) nounwind
   %bal_target = extractvalue {i64, i64} %bal_target_ext, 0

   %check_amount = icmp sge i64 %bal_owner, %amount
   br i1 %check_amount, label %process_transaction, label %exit_fail
process_transaction:
   %bal_owner_new = sub i64 %bal_owner, %amount
   %bal_owner_new_slice = call i64 @llvm.tvm.inttoslice(i64 %bal_owner_new)
   %bal_dict1 = call i64 @llvm.tvm.dictuset(i64 %bal_owner_new_slice, i64 %owner, i64 %bal_dict, i64 256)
   %bal_target_new = add i64 %bal_target, %amount
   %bal_target_new_slice = call i64 @llvm.tvm.inttoslice(i64 %bal_target_new)
   %bal_dict2 = call i64 @llvm.tvm.dictuset(i64 %bal_target_new_slice, i64 %target, i64 %bal_dict1, i64 256)
   %bal_dict_packed = call i64 @llvm.tvm.cellrefdict(i64 %bal_dict2)
   %root_new = call i64 @llvm.tvm.dictuset(i64 %bal_dict_packed, i64 0, i64 %root, i64 256)
   %final = call i64 @llvm.tvm.set.persistent.data()
   ret i64 -1
exit_fail:
   ret i64 0
}
declare i64 @llvm.tvm.newdict() nounwind
declare i64 @llvm.tvm.set.persistent.data() nounwind
declare i64 @llvm.tvm.get.persistent.data() nounwind
declare i64 @llvm.tvm.dictuset(i64 %value, i64 %ind, i64 %dict, i64 %ind_bit) nounwind
declare {i64, i64} @llvm.tvm.dictuget(i64 %key, i64 %dict_id, i64 %keylen) nounwind
declare {i64, i64} @llvm.tvm.ldrefrtos(i64 %slice) nounwind
declare i64 @llvm.tvm.inttoslice(i64 %val) nounwind
declare i64 @llvm.tvm.cellrefdict(i64 %dict) nounwind
declare {i64, i64} @llvm.tvm.ldu(i64 %slice) nounwind
