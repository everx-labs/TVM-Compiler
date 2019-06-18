; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; CHECK-LABEL: balanceof
define i64 @balanceof(i64 %owner) nounwind {
entry:
   %root = call i64 @llvm.tvm.get.persistent.data();
   %bal_raw = call {i64, i64} @llvm.tvm.dictuget(i64 0, i64 %root, i64 256)
   %bal_ref = extractvalue {i64, i64} %bal_raw, 0
   %bal_ref_stat = extractvalue {i64, i64} %bal_raw, 1
   %cond = icmp ne i64 %bal_ref_stat, 0
   br i1 %cond, label %get_owner_dict, label %retres
get_owner_dict:
   %bal_dict_ext = call {i64, i64} @llvm.tvm.ldrefrtos(i64 %bal_ref) nounwind
   %bal_dict = extractvalue {i64, i64} %bal_dict_ext, 0
   %bal_owner_raw = call {i64, i64} @llvm.tvm.dictuget(i64 %owner, i64 %bal_dict, i64 256)
   %bal_owner_ref = extractvalue {i64, i64} %bal_owner_raw, 0
   %bal_owner_ref_stat = extractvalue {i64, i64} %bal_owner_raw, 1
   %cond1 = icmp ne i64 %bal_owner_ref_stat, 0
   br i1 %cond1, label %get_bal, label %retres
 get_bal:
   %bal_owner_struct = call {i64, i64} @llvm.tvm.ldu(i64 %bal_owner_ref)
   %bal_owner = extractvalue {i64, i64} %bal_owner_struct, 0
   br label %retres
retres:
   %retval = phi i64 [0, %entry], [0, %get_owner_dict], [%bal_owner, %get_bal]
   ret i64 %retval
}
declare i64 @llvm.tvm.get.persistent.data() nounwind
declare {i64, i64} @llvm.tvm.dictuget(i64 %key, i64 %dict_id, i64 %keylen) nounwind
declare {i64, i64} @llvm.tvm.ldrefrtos(i64 %slice) nounwind
declare {i64, i64} @llvm.tvm.ldu(i64 %slice) nounwind
