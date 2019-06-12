; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %n) nounwind {
entry:
  %cmp33 = icmp slt i64 %n, 1
  %rem2834 = urem i64 %n, 11
  %cmp135 = icmp eq i64 %rem2834, 0
  %or.cond36 = or i1 %cmp33, %cmp135
  %rem22937 = urem i64 %n, 7
  %cmp338 = icmp eq i64 %rem22937, 0
  %or.cond3239 = or i1 %cmp338, %or.cond36
  br i1 %or.cond3239, label %cleanup, label %if.else5

if.else5:                                         ; preds = %entry, %for.inc
  %iter.041 = phi i64 [ %dec, %for.inc ], [ %n, %entry ]
  %res.040 = phi i64 [ %res.1, %for.inc ], [ 1, %entry ]
  %rem630 = urem i64 %iter.041, 5
  %cmp7 = icmp eq i64 %rem630, 0
  br i1 %cmp7, label %for.inc, label %if.else9

if.else9:                                         ; preds = %if.else5
  %rem1031 = urem i64 %iter.041, 3
  %cmp11 = icmp eq i64 %rem1031, 0
  br i1 %cmp11, label %cleanup, label %if.end15

if.end15:                                         ; preds = %if.else9
  %mul = mul i64 %iter.041, %res.040
  br label %for.inc

for.inc:                                          ; preds = %if.else5, %if.end15
  %res.1 = phi i64 [ %res.040, %if.else5 ], [ %mul, %if.end15 ]
  %dec = add i64 %iter.041, -1
  %cmp = icmp slt i64 %iter.041, 2
  %rem28 = urem i64 %dec, 11
  %cmp1 = icmp eq i64 %rem28, 0
  %or.cond = or i1 %cmp, %cmp1
  %rem229 = urem i64 %dec, 7
  %cmp3 = icmp eq i64 %rem229, 0
  %or.cond32 = or i1 %cmp3, %or.cond
  br i1 %or.cond32, label %cleanup, label %if.else5

cleanup:                                          ; preds = %if.else9, %for.inc, %entry
  %0 = phi i64 [ 1, %entry ], [ %res.1, %for.inc ], [ -1, %if.else9 ]
  ret i64 %0
}