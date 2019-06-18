; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg) nounwind {
entry:
  br label %do.body

do.body:                                          ; preds = %if.else, %entry
  %res.0 = phi i64 [ %arg, %entry ], [ %mul, %if.else ]
  %iterator.0 = phi i64 [ %arg, %entry ], [ %dec, %if.else ]
  %rem = srem i64 %iterator.0, 11
  %cmp = icmp eq i64 %rem, 0
  br i1 %cmp, label %cleanup, label %if.else

if.else:                                          ; preds = %do.body
  %dec = add i64 %iterator.0, -1
  %mul = mul i64 %dec, %res.0
  %cmp1 = icmp sgt i64 %iterator.0, 1
  br i1 %cmp1, label %do.body, label %cleanup

cleanup:                                          ; preds = %do.body, %if.else
  %retval.0 = phi i64 [ %mul, %if.else ], [ -1, %do.body ]
  ret i64 %retval.0
}
