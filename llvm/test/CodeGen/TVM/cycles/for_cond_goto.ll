; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg) nounwind {
entry:
  %cmp13 = icmp sgt i64 %arg, 0
  br i1 %cmp13, label %for.body, label %cleanup

for.body:                                         ; preds = %entry, %if.else
  %iterator.015 = phi i64 [ %dec2, %if.else ], [ %arg, %entry ]
  %res.014 = phi i64 [ %mul, %if.else ], [ 1, %entry ]
  %rem12 = urem i64 %iterator.015, 11
  %cmp1 = icmp eq i64 %rem12, 0
  br i1 %cmp1, label %cleanup, label %if.else

if.else:                                          ; preds = %for.body
  %mul = mul i64 %iterator.015, %res.014
  %dec2 = add i64 %iterator.015, -2
  %cmp = icmp sgt i64 %iterator.015, 2
  br i1 %cmp, label %for.body, label %cleanup

cleanup:                                          ; preds = %if.else, %for.body, %entry
  %0 = phi i64 [ 1, %entry ], [ -1, %for.body ], [ %mul, %if.else ]
  ret i64 %0
}