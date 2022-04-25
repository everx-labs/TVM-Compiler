; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: quiet_load1
define i257 @quiet_load1(slice %sl) nounwind {
entry:
  ; CHECK: LDUQ 256 NULLROTRIFNOT
  %rv = call {i257, slice, i257} @llvm.tvm.lduq(slice %sl, i257 256)
  %succ = extractvalue {i257, slice, i257} %rv, 2
  %cond = icmp ne i257 %succ, 0
  br i1 %cond, label %if_success, label %if_error
if_success:
  %val = extractvalue {i257, slice, i257} %rv, 0
  ret i257 %val
if_error:
  ret i257 0
}

; CHECK-LABEL: quiet_load2
define i257 @quiet_load2(slice %sl) nounwind {
entry:
  ; CHECK: LDUXQ NULLROTRIFNOT
  %rv = call {i257, slice, i257} @llvm.tvm.lduq(slice %sl, i257 257)
  %succ = extractvalue {i257, slice, i257} %rv, 2
  %cond = icmp ne i257 %succ, 0
  br i1 %cond, label %if_success, label %if_error
if_success:
  %val = extractvalue {i257, slice, i257} %rv, 0
  ret i257 %val
if_error:
  ret i257 0
}

declare {i257, slice, i257} @llvm.tvm.lduq(slice %slice, i257 %precision) nounwind

