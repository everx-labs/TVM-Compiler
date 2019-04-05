; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define i1 @approve(i64 %owner, i64 %spender, i64 %amount) nounwind {
entry:
  %root = call i64 @llvm.tvm.get.persistent.data();
  %allowance = call {i64, i64} @llvm.tvm.dictuget(i64 1, i64 %root, i64 256)
  %al_dict_ref = extractvalue {i64, i64} %allowance, 0
  %al_dict_st = extractvalue {i64, i64} %allowance, 1
  %cond = icmp eq i64 %al_dict_st, 0
  br i1 %cond, label %unpack_adict, label %add_adict
add_adict: ; create allowance dict
  %al_dict_new = call i64 @llvm.tvm.newdict()
  br label %a_dict
unpack_adict: ; unpack existing allowance dict
  %ref_ctos = call {i64, i64} @llvm.tvm.ldrefctos(i64 %al_dict_ref)
  %dict_unpack = extractvalue {i64, i64} %ref_ctos, 0
  br label %a_dict
a_dict: ; find owner in alowance subdict
  %al_dict = phi i64 [%al_dict_new, %add_adict], [%dict_unpack, %unpack_adict]
  %0 = call {i64, i64} @llvm.tvm.dictuget(i64 %owner, i64 %al_dict, i64 256)
  %al_sdict_ref = extractvalue {i64, i64} %0, 0
  %al_sdict_st = extractvalue {i64, i64} %0, 1
  %cond1 = icmp eq i64 %al_sdict_st, 0
  br i1 %cond1, label %unpack_asdict, label %add_asdict
unpack_asdict: ; unpack owner's alowance subdict
  %ref_ctos1 = call {i64, i64} @llvm.tvm.ldrefctos(i64 %al_sdict_ref)
  %sdict_unpack = extractvalue {i64, i64} %ref_ctos1, 0
  br label %a_sdict
add_asdict: ; create owner's alowance subdict
  %al_sdict_new = call i64 @llvm.tvm.newdict()
  br label %a_sdict
a_sdict:
  %al_sdict = phi i64 [%al_sdict_new, %add_asdict], [%sdict_unpack, %unpack_asdict]
  %new_rec = call i64 @llvm.tvm.inttoslice(i64 %amount)
  %new_al_sdict = call i64 @llvm.tvm.dictuset(i64 %new_rec, i64 %spender, i64 %al_sdict, i64 256)
  %new_al_sdict_ref = call i64 @llvm.tvm.cellrefdict(i64 %new_al_sdict)
  %new_al_dict = call i64 @llvm.tvm.dictuset(i64 %new_al_sdict_ref, i64 %owner, i64 %al_dict, i64 256)
  %new_al_dict_ref = call i64 @llvm.tvm.cellrefdict(i64 %new_al_dict)
  %new_root = call i64 @llvm.tvm.dictuset(i64 %new_al_dict_ref, i64 1, i64 %root, i64 256)
  %1 = call i64 @llvm.tvm.set.persistent.data(i64 %new_root)

  ret i1 -1
}

declare i64 @llvm.tvm.newdict() nounwind
declare i64 @llvm.tvm.get.persistent.data() nounwind
declare {i64, i64} @llvm.tvm.dictuget(i64 %key, i64 %dict_id, i64 %keylen) nounwind
declare {i64, i64} @llvm.tvm.ldrefctos(i64 %slice) nounwind
declare i64 @llvm.tvm.inttoslice(i64 %val) nounwind
declare i64 @llvm.tvm.dictuset(i64 %value, i64 %ind, i64 %dict, i64 %ind_bit) nounwind
declare i64 @llvm.tvm.cellrefdict(i64 %dict) nounwind
declare i64 @llvm.tvm.set.persistent.data(i64 %index) nounwind
