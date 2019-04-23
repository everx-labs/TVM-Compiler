target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define i64 @tvm_pdload(i64 %index) {
  %Root = call i64 @llvm.tvm.get.persistent.data()
  %RefRtos = call {i64, i64} @llvm.tvm.ldrefrtos(i64 %Root)
  %Dict = extractvalue {i64, i64} %RefRtos, 0
  %CellStatus = call {i64, i64} @llvm.tvm.dictuget(i64 %index, i64 %Dict, i64 256)
  %Cell = extractvalue {i64, i64} %CellStatus, 1
  %Status.Neg = extractvalue {i64, i64} %CellStatus, 0
  %Status = xor i64 %Status.Neg, -1
  call void @llvm.tvm.throwif(i64 %Status, i64 1)
  %ValueStatus = call {i64, i64} @llvm.tvm.ldu(i64 %Cell)
  %Value = extractvalue {i64, i64} %ValueStatus, 0
  ret i64 %Value
}

define void @tvm_pdstore(i64 %value, i64 %index) {
  %Root = call i64 @llvm.tvm.get.persistent.data()
  %RefRtos = call {i64, i64} @llvm.tvm.ldrefrtos(i64 %Root)
  %Dict = extractvalue {i64, i64} %RefRtos, 0
  %DictSlice = call i64 @llvm.tvm.dictuset(i64 %value, i64 %index, i64 %Dict, i64 256)
  %DictCell = call i64 @llvm.tvm.stoc(i64 %DictSlice)
  %Builder = call i64 @llvm.tvm.newc()
  %Builder1 = call i64 @llvm.tvm.stref(i64 %DictCell, i64 %Builder)
  %RootRef = call {i64, i64} @llvm.tvm.ldref(i64 %Root)
  %RootWORef = extractvalue {i64, i64} %RootRef, 0
  %NewRoot = call i64 @llvm.tvm.stslice(i64 %RootWORef, i64 %Builder1)
  call void @llvm.tvm.set.persistent.data(i64 %NewRoot)
  ret void
}

declare i64 @llvm.tvm.get.persistent.data() nounwind
declare i64 @llvm.tvm.newc() nounwind
declare i64 @llvm.tvm.stoc(i64 %Cell) nounwind
declare {i64, i64} @llvm.tvm.dictuget(i64 %key, i64 %dict_id, i64 %keylen) nounwind
declare {i64, i64} @llvm.tvm.ldrefrtos(i64 %slice) nounwind
declare i64 @llvm.tvm.inttoslice(i64 %val) nounwind
declare i64 @llvm.tvm.dictuset(i64 %value, i64 %ind, i64 %dict, i64 %ind_bit) nounwind
declare void @llvm.tvm.set.persistent.data(i64 %index) nounwind
declare void @llvm.tvm.throwif(i64 %cond, i64 %errcode)
declare {i64, i64} @llvm.tvm.ldu(i64 %slice) nounwind
declare i64 @llvm.tvm.stref(i64 %Cell, i64 %Builder) nounwind
declare {i64, i64} @llvm.tvm.ldref(i64 %Cell) nounwind
declare i64 @llvm.tvm.stslice(i64 %slice, i64 %builder) nounwind
