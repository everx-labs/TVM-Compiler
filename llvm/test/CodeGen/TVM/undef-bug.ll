; RUN: llc < %s -march=tvm | FileCheck %s

target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"
define void @bar() {
; CHECK-LABEL: bar
entry:
  %0 = call i257 @foo(i257 0)
  %1 = call i257 @foo(i257 1)
  %2 = call i257 @foo(i257 2)
  %3 = call i257 @foo(i257 3)
  %4 = call i257 @foo(i257 4)
; CHECK: THROWIF	61
  call void @llvm.tvm.throwif(i257 undef, i257 61)
  br label %exit
exit:
  ret void
}
declare void @llvm.tvm.throwif(i257, i257)
declare i257 @foo(i257)

define i257 @baz() {
; CHECK-LABEL: baz
entry:
  ret i257 undef
}
