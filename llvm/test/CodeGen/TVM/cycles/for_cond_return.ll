; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg) nounwind {
entry:
  %cmp14 = icmp sgt i64 %arg, 0
  br i1 %cmp14, label %for.body, label %cleanup

for.cond:                                         ; preds = %for.body
  %dec2 = add i64 %iterator.016, -2
  %cmp = icmp sgt i64 %iterator.016, 2
  br i1 %cmp, label %for.body, label %cleanup

for.body:                                         ; preds = %entry, %for.cond
  %iterator.016 = phi i64 [ %dec2, %for.cond ], [ %arg, %entry ]
  %res.015 = phi i64 [ %mul, %for.cond ], [ 1, %entry ]
  %mul = mul i64 %iterator.016, %res.015
  %rem13 = urem i64 %iterator.016, 11
  %cmp1 = icmp eq i64 %rem13, 0
  br i1 %cmp1, label %cleanup, label %for.cond

cleanup:                                          ; preds = %for.body, %for.cond, %entry
  %0 = phi i64 [ 1, %entry ], [ %mul, %for.cond ], [ %mul, %for.body ]
  ret i64 %0
}
