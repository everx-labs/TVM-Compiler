; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg) nounwind {
entry:
  br label %do.body

do.body:                                          ; preds = %do.body, %entry
  %res.0 = phi i64 [ %arg, %entry ], [ %res.1, %do.body ]
  %iterator.0 = phi i64 [ %arg, %entry ], [ %iterator.1, %do.body ]
  %dec = add i64 %iterator.0, -1
  %rem = srem i64 %dec, 11
  %div = sdiv i64 %dec, 11
  %cmp = icmp eq i64 %rem, 0
  %mul = mul i64 %dec, %res.0
  %res.1 = select i1 %cmp, i64 %res.0, i64 %mul
  %iterator.1 = select i1 %cmp, i64 %div, i64 %dec
  %cmp1 = icmp sgt i64 %iterator.1, 0
  br i1 %cmp1, label %do.body, label %do.end

do.end:                                           ; preds = %do.body
  ret i64 %res.1
}
