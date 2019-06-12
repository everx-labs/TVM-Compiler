; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg) nounwind {
entry:
  %cmp9 = icmp sgt i64 %arg, 0
  br i1 %cmp9, label %while.body, label %while.end

while.body:                                       ; preds = %entry, %while.body
  %iterator.011 = phi i64 [ %dec, %while.body ], [ %arg, %entry ]
  %res.010 = phi i64 [ %res.1, %while.body ], [ 1, %entry ]
  %rem8 = urem i64 %iterator.011, 11
  %cmp1 = icmp eq i64 %rem8, 0
  %mul = mul i64 %iterator.011, %res.010
  %add = add i64 %iterator.011, %res.010
  %res.1 = select i1 %cmp1, i64 %mul, i64 %add
  %dec = add i64 %iterator.011, -1
  %cmp = icmp sgt i64 %iterator.011, 1
  br i1 %cmp, label %while.body, label %while.end

while.end:                                        ; preds = %while.body, %entry
  %res.0.lcssa = phi i64 [ 1, %entry ], [ %res.1, %while.body ]
  ret i64 %res.0.lcssa
}