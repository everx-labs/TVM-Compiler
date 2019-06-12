; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg) nounwind {
entry:
  %cmp9 = icmp sgt i64 %arg, 0
  br i1 %cmp9, label %while.body, label %while.end

while.cond:                                       ; preds = %while.body
  %dec = add i64 %iterator.011, -1
  %cmp = icmp sgt i64 %iterator.011, 1
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %entry, %while.cond
  %iterator.011 = phi i64 [ %dec, %while.cond ], [ %arg, %entry ]
  %res.010 = phi i64 [ %mul, %while.cond ], [ 1, %entry ]
  %mul = mul i64 %iterator.011, %res.010
  %rem8 = urem i64 %iterator.011, 11
  %cmp1 = icmp eq i64 %rem8, 0
  br i1 %cmp1, label %while.end, label %while.cond

while.end:                                        ; preds = %while.cond, %while.body, %entry
  %res.1 = phi i64 [ 1, %entry ], [ %mul, %while.body ], [ %mul, %while.cond ]
  ret i64 %res.1
}