; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg) nounwind {
entry:
  %cmp11 = icmp sgt i64 %arg, 0
  br i1 %cmp11, label %while.body, label %cleanup

while.cond:                                       ; preds = %while.body
  %dec = add i64 %iterator.013, -1
  %cmp = icmp sgt i64 %iterator.013, 1
  br i1 %cmp, label %while.body, label %cleanup

while.body:                                       ; preds = %entry, %while.cond
  %iterator.013 = phi i64 [ %dec, %while.cond ], [ %arg, %entry ]
  %res.012 = phi i64 [ %mul, %while.cond ], [ 1, %entry ]
  %mul = mul i64 %iterator.013, %res.012
  %rem10 = urem i64 %iterator.013, 11
  %cmp1 = icmp eq i64 %rem10, 0
  br i1 %cmp1, label %cleanup, label %while.cond

cleanup:                                          ; preds = %while.body, %while.cond, %entry
  %retval.0 = phi i64 [ 1, %entry ], [ %mul, %while.cond ], [ %mul, %while.body ]
  ret i64 %retval.0
}
