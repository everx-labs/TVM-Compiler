; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: nounwind
; CHECK-LABEL: make_internal_msg_cell
define dso_local i64 @make_internal_msg_cell(i64 %to_addr, i64 %value) local_unnamed_addr #0 {
entry:
; CHECK: NEWC
  %0 = tail call i64 @llvm.tvm.newc()
; CHECK: STU
  %1 = tail call i64 @llvm.tvm.stu(i64 %0, i64 0, i64 5)
; CHECK: STU
  %2 = tail call i64 @llvm.tvm.stu(i64 %1, i64 -1, i64 8)
; CHECK: STU
  %3 = tail call i64 @llvm.tvm.stu(i64 %2, i64 %to_addr, i64 256)
; CHECK: STU
  %4 = tail call i64 @llvm.tvm.stu(i64 %3, i64 %value, i64 32)
; CHECK: ENDC
  %5 = tail call i64 @llvm.tvm.endc(i64 %4)
  ret i64 %5
}

declare i64 @llvm.tvm.newc() #1
declare i64 @llvm.tvm.stu(i64, i64, i64) #1
declare i64 @llvm.tvm.endc(i64) #1
