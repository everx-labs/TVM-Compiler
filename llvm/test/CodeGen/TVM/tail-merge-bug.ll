; RUN: llc -march=tvm < %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define void @foo(i257 %v) {
entry:
  %cmp = icmp eq i257 %v, 0
  br i1 %cmp, label %then, label %else
then:
  ret void
else:
  ret void
}
