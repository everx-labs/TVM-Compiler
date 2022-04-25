; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: nounwind
; CHECK-LABEL: make_internal_msg_cell
define cell @make_internal_msg_cell(i257 %to_addr, i257 %value) local_unnamed_addr #0 {
entry:
; CHECK: NEWC
  %0 = call builder @llvm.tvm.newc()
; CHECK: STU
  %1 = call builder @llvm.tvm.stu(i257 0, builder %0, i257 5)
; CHECK: STU
  %2 = call builder @llvm.tvm.stu(i257 -1, builder %1, i257 8)
; CHECK: STU
  %3 = call builder @llvm.tvm.stu(i257 %to_addr, builder %2, i257 256)
; CHECK: STU
  %4 = call builder @llvm.tvm.stu(i257 %value, builder %3, i257 32)
; CHECK: ENDC
  %5 = call cell @llvm.tvm.endc(builder %4)
  ret cell %5
}

; Function Attrs: nounwind
; CHECK-LABEL: send_raw_message
define void @send_raw_message(i257 %to_addr, i257 %value) local_unnamed_addr #0 {
entry:
  %call = call cell @make_internal_msg_cell(i257 %to_addr, i257 %value)
; CHECK: SENDRAWMSG
  call void @llvm.tvm.sendrawmsg(cell %call, i257 0)
  ret void
}

declare void @llvm.tvm.sendrawmsg(cell, i257) #1
declare builder @llvm.tvm.newc() #1
declare builder @llvm.tvm.stu(i257, builder, i257) #1
declare cell @llvm.tvm.endc(builder) #1
