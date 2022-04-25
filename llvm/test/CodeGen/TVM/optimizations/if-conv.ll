; RUN: llc -O3 < %s -march=tvm -filetype=asm | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: throwif
define i257 @throwif() {
entry:
  %call = tail call i257 @foo()
  %cmp = icmp eq i257 %call, 170
  br i1 %cmp, label %if.end, label %if.then

; CHECK: THROWIFNOT
if.then:
  tail call void @llvm.tvm.throw(i257 13)
  unreachable

if.end:
  ret i257 %call
}

declare void @llvm.tvm.throw(i257 %errcode) noreturn
declare i257 @foo()
