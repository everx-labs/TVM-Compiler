; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg) nounwind {
entry:
  br label %do.body

do.body:                                          ; preds = %do.cond, %entry
  %res.0 = phi i64 [ %arg, %entry ], [ %res.1, %do.cond ]
  %iterator.0 = phi i64 [ %arg, %entry ], [ %dec, %do.cond ]
  %dec = add i64 %iterator.0, -1
  %rem = srem i64 %dec, 11
  %cmp = icmp eq i64 %rem, 0
  %rem1 = srem i64 %dec, 7
  %cmp2 = icmp eq i64 %rem1, 0
  %or.cond = or i1 %cmp, %cmp2
  br i1 %or.cond, label %cleanup, label %if.else4

if.else4:                                         ; preds = %do.body
  %rem5 = srem i64 %dec, 5
  %cmp6 = icmp eq i64 %rem5, 0
  br i1 %cmp6, label %do.cond, label %if.else8

if.else8:                                         ; preds = %if.else4
  %rem9 = srem i64 %dec, 3
  %cmp10 = icmp eq i64 %rem9, 0
  br i1 %cmp10, label %cleanup, label %if.end14

if.end14:                                         ; preds = %if.else8
  %mul = mul i64 %dec, %res.0
  br label %do.cond

do.cond:                                          ; preds = %if.else4, %if.end14
  %res.1 = phi i64 [ %res.0, %if.else4 ], [ %mul, %if.end14 ]
  %cmp15 = icmp sgt i64 %iterator.0, 1
  br i1 %cmp15, label %do.body, label %cleanup

cleanup:                                          ; preds = %if.else8, %do.cond, %do.body
  %retval.0 = phi i64 [ %res.0, %do.body ], [ %res.1, %do.cond ], [ -1, %if.else8 ]
  ret i64 %retval.0
}
