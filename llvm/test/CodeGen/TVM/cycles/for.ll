; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define i64 @func(i64 %arg) nounwind {
entry:
  %cmp.entry = icmp slt i64 %arg, 2
  br i1 %cmp.entry, label %for.cond, label %for.body

for.cond:
  %res = phi i64 [ 1, %entry ], [ %mul, %for.body ]
  ret i64 %res

for.body:
  %iterator = phi i64 [ %inc, %for.body ], [ 2, %entry ]
  %res.body = phi i64 [ %mul, %for.body ], [ 1, %entry ]
  %mul = mul i64 %iterator, %res.body
  %inc = add i64 %iterator, 1
  %exitcond = icmp eq i64 %iterator, %arg
  br i1 %exitcond, label %for.cond, label %for.body
}
