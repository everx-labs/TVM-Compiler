; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg) nounwind {
entry:
  %cmp10 = icmp sgt i64 %arg, 0
  br i1 %cmp10, label %while.body, label %cleanup

while.body:                                       ; preds = %entry, %if.else
  %iterator.012 = phi i64 [ %dec, %if.else ], [ %arg, %entry ]
  %res.011 = phi i64 [ %mul, %if.else ], [ 1, %entry ]
  %rem9 = urem i64 %iterator.012, 11
  %cmp1 = icmp eq i64 %rem9, 0
  br i1 %cmp1, label %cleanup, label %if.else

if.else:                                          ; preds = %while.body
  %mul = mul i64 %iterator.012, %res.011
  %dec = add i64 %iterator.012, -1
  %cmp = icmp sgt i64 %iterator.012, 1
  br i1 %cmp, label %while.body, label %cleanup

cleanup:                                          ; preds = %if.else, %while.body, %entry
  %retval.0 = phi i64 [ 1, %entry ], [ -1, %while.body ], [ %mul, %if.else ]
  ret i64 %retval.0
}