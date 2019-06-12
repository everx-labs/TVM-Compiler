; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg) nounwind {
entry:
  %cmp10 = icmp sgt i64 %arg, 0
  br i1 %cmp10, label %for.body, label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.body, %entry
  %res.0.lcssa = phi i64 [ 1, %entry ], [ %res.1, %for.body ]
  ret i64 %res.0.lcssa

for.body:                                         ; preds = %entry, %for.body
  %iterator.012 = phi i64 [ %dec, %for.body ], [ %arg, %entry ]
  %res.011 = phi i64 [ %res.1, %for.body ], [ 1, %entry ]
  %rem8 = urem i64 %iterator.012, 11
  %div9 = udiv i64 %iterator.012, 11
  %cmp1 = icmp eq i64 %rem8, 0
  %mul = mul i64 %iterator.012, %res.011
  %res.1 = select i1 %cmp1, i64 %res.011, i64 %mul
  %iterator.1 = select i1 %cmp1, i64 %div9, i64 %iterator.012
  %dec = add i64 %iterator.1, -1
  %cmp = icmp sgt i64 %iterator.1, 1
  br i1 %cmp, label %for.body, label %for.cond.cleanup
}