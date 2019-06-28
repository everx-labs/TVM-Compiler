; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %n) nounwind {
entry:
  %cmp32 = icmp slt i64 %n, 1
  %rem33 = srem i64 %n, 11
  %cmp134 = icmp eq i64 %rem33, 0
  %or.cond35 = or i1 %cmp32, %cmp134
  %rem236 = srem i64 %n, 7
  %cmp337 = icmp eq i64 %rem236, 0
  %or.cond2738 = or i1 %cmp337, %or.cond35
  br i1 %or.cond2738, label %cleanup, label %while.cond.outer.split

while.cond.outer.split:                           ; preds = %entry, %if.end15
  %iter.0.ph40 = phi i64 [ %dec, %if.end15 ], [ %n, %entry ]
  %res.0.ph39 = phi i64 [ %mul, %if.end15 ], [ 1, %entry ]
  %rem628 = urem i64 %iter.0.ph40, 5
  %cmp7 = icmp eq i64 %rem628, 0
  br i1 %cmp7, label %while.cond, label %if.else9

while.cond:                                       ; preds = %while.cond.outer.split, %while.cond
  br label %while.cond

if.else9:                                         ; preds = %while.cond.outer.split
  %rem1029 = urem i64 %iter.0.ph40, 3
  %cmp11 = icmp eq i64 %rem1029, 0
  br i1 %cmp11, label %cleanup, label %if.end15

if.end15:                                         ; preds = %if.else9
  %mul = mul i64 %iter.0.ph40, %res.0.ph39
  %dec = add i64 %iter.0.ph40, -1
  %cmp = icmp slt i64 %iter.0.ph40, 2
  %rem = srem i64 %dec, 11
  %cmp1 = icmp eq i64 %rem, 0
  %or.cond = or i1 %cmp, %cmp1
  %rem2 = srem i64 %dec, 7
  %cmp3 = icmp eq i64 %rem2, 0
  %or.cond27 = or i1 %cmp3, %or.cond
  br i1 %or.cond27, label %cleanup, label %while.cond.outer.split

cleanup:                                          ; preds = %if.end15, %if.else9, %entry
  %retval.0 = phi i64 [ 1, %entry ], [ %mul, %if.end15 ], [ -1, %if.else9 ]
  ret i64 %retval.0
}
