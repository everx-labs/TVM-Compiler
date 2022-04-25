; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define i1 @approve(i257 %owner, i257 %spender, i257 %amount) nounwind {
entry:
  %root = call i257 @llvm.tvm.get.persistent.data();
  %allowance = call {i257, i257} @llvm.tvm.dictuget(i257 1, i257 %root, i257 256)
  %al_dict_ref = extractvalue {i257, i257} %allowance, 0
  %al_dict_st = extractvalue {i257, i257} %allowance, 1
  %cond = icmp ne i257 %al_dict_st, 0
  br i1 %cond, label %unpack_adict, label %add_adict
add_adict: ; create allowance dict
  %al_dict_new = call i257 @llvm.tvm.newdict()
  br label %a_dict
unpack_adict: ; unpack existing allowance dict
  %ref_ctos = call {i257, i257} @llvm.tvm.ldrefctos(i257 %al_dict_ref)
  %dict_unpack = extractvalue {i257, i257} %ref_ctos, 0
  br label %a_dict
a_dict: ; find owner in alowance subdict
  %al_dict = phi i257 [%al_dict_new, %add_adict], [%dict_unpack, %unpack_adict]
  %0 = call {i257, i257} @llvm.tvm.dictuget(i257 %owner, i257 %al_dict, i257 256)
  %al_sdict_ref = extractvalue {i257, i257} %0, 0
  %al_sdict_st = extractvalue {i257, i257} %0, 1
  %cond1 = icmp ne i257 %al_sdict_st, 0
  br i1 %cond1, label %unpack_asdict, label %add_asdict
unpack_asdict: ; unpack owner's alowance subdict
  %ref_ctos1 = call {i257, i257} @llvm.tvm.ldrefctos(i257 %al_sdict_ref)
  %sdict_unpack = extractvalue {i257, i257} %ref_ctos1, 0
  br label %a_sdict
add_asdict: ; create owner's alowance subdict
  %al_sdict_new = call i257 @llvm.tvm.newdict()
  br label %a_sdict
a_sdict:
  %al_sdict = phi i257 [%al_sdict_new, %add_asdict], [%sdict_unpack, %unpack_asdict]
  %new_rec = call i257 @llvm.tvm.inttoslice(i257 %amount)
  %new_al_sdict = call i257 @llvm.tvm.dictuset(i257 %new_rec, i257 %spender, i257 %al_sdict, i257 256)
  %new_al_sdict_ref = call i257 @llvm.tvm.cellrefdict(i257 %new_al_sdict)
  %new_al_dict = call i257 @llvm.tvm.dictuset(i257 %new_al_sdict_ref, i257 %owner, i257 %al_dict, i257 256)
  %new_al_dict_ref = call i257 @llvm.tvm.cellrefdict(i257 %new_al_dict)
  %new_root = call i257 @llvm.tvm.dictuset(i257 %new_al_dict_ref, i257 1, i257 %root, i257 256)
  call void @llvm.tvm.set.persistent.data(i257 %new_root)

  ret i1 -1
}

declare i257 @llvm.tvm.newdict() nounwind
declare i257 @llvm.tvm.get.persistent.data() nounwind
declare {i257, i257} @llvm.tvm.dictuget(i257 %key, i257 %dict_id, i257 %keylen) nounwind
declare {i257, i257} @llvm.tvm.ldrefctos(i257 %slice) nounwind
declare i257 @llvm.tvm.inttoslice(i257 %val) nounwind
declare i257 @llvm.tvm.dictuset(i257 %value, i257 %ind, i257 %dict, i257 %ind_bit) nounwind
declare i257 @llvm.tvm.cellrefdict(i257 %dict) nounwind
declare void @llvm.tvm.set.persistent.data(i257 %index) nounwind
