; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: balanceof
define i257 @balanceof(i257 %owner) nounwind {
entry:
   %root_cell = call cell @llvm.tvm.get.persistent.data()
   %bal_raw = call {slice, i257} @llvm.tvm.dictuget(i257 0, cell %root_cell, i257 256)
   %bal_ref = extractvalue {slice, i257} %bal_raw, 0
   %bal_ref_stat = extractvalue {slice, i257} %bal_raw, 1
   %cond = icmp ne i257 %bal_ref_stat, 0
   br i1 %cond, label %get_owner_dict, label %retres
get_owner_dict:
   %bal_dict_ext = call {cell, slice} @llvm.tvm.ldref(slice %bal_ref) nounwind
   %bal_dict = extractvalue {cell, slice} %bal_dict_ext, 0
   %bal_owner_raw = call {slice, i257} @llvm.tvm.dictuget(i257 %owner, cell %bal_dict, i257 256)
   %bal_owner_ref = extractvalue {slice, i257} %bal_owner_raw, 0
   %bal_owner_ref_stat = extractvalue {slice, i257} %bal_owner_raw, 1
   %cond1 = icmp ne i257 %bal_owner_ref_stat, 0
   br i1 %cond1, label %get_bal, label %retres
 get_bal:
   %bal_owner_struct = call {i257, slice} @llvm.tvm.ldu(slice %bal_owner_ref, i257 256)
   %bal_owner = extractvalue {i257, slice} %bal_owner_struct, 0
   br label %retres
retres:
   %retval = phi i257 [0, %entry], [0, %get_owner_dict], [%bal_owner, %get_bal]
   ret i257 %retval
}

declare cell @llvm.tvm.get.persistent.data() nounwind
declare slice @llvm.tvm.ctos(cell %cell) nounwind
declare {slice, i257} @llvm.tvm.dictuget(i257 %key, cell %dict_id, i257 %keylen)
declare {cell, slice} @llvm.tvm.ldref(slice %slice) nounwind
declare {i257, slice} @llvm.tvm.ldu(slice %slice, i257 %precision) nounwind
