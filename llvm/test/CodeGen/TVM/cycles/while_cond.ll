; RUN: llc < %s -march=tvm 
; ModuleID = 'while_cond.c'
source_filename = "while_cond.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %n) local_unnamed_addr #0 {
entry:
  %cmp32 = icmp slt i257 %n, 1
  %rem33 = srem i257 %n, 11
  %cmp134 = icmp eq i257 %rem33, 0
  %or.cond35 = or i1 %cmp32, %cmp134
  %rem236 = srem i257 %n, 7
  %cmp337 = icmp eq i257 %rem236, 0
  %or.cond2738 = or i1 %cmp337, %or.cond35
  br i1 %or.cond2738, label %cleanup, label %while.cond.outer.split

while.cond.outer.split:                           ; preds = %entry, %if.end15
  %iter.0.ph40 = phi i257 [ %dec, %if.end15 ], [ %n, %entry ]
  %res.0.ph39 = phi i257 [ %mul, %if.end15 ], [ 1, %entry ]
  %rem628 = urem i257 %iter.0.ph40, 5
  %cmp7 = icmp eq i257 %rem628, 0
  br i1 %cmp7, label %while.cond, label %if.else9

while.cond:                                       ; preds = %while.cond.outer.split, %while.cond
  br label %while.cond

if.else9:                                         ; preds = %while.cond.outer.split
  %rem1029 = urem i257 %iter.0.ph40, 3
  %cmp11 = icmp eq i257 %rem1029, 0
  br i1 %cmp11, label %cleanup, label %if.end15

if.end15:                                         ; preds = %if.else9
  %mul = mul nsw i257 %iter.0.ph40, %res.0.ph39
  %dec = add nsw i257 %iter.0.ph40, -1
  %cmp = icmp slt i257 %iter.0.ph40, 2
  %rem = srem i257 %dec, 11
  %cmp1 = icmp eq i257 %rem, 0
  %or.cond = or i1 %cmp, %cmp1
  %rem2 = srem i257 %dec, 7
  %cmp3 = icmp eq i257 %rem2, 0
  %or.cond27 = or i1 %cmp3, %or.cond
  br i1 %or.cond27, label %cleanup, label %while.cond.outer.split

cleanup:                                          ; preds = %if.end15, %if.else9, %entry
  %retval.0 = phi i257 [ 1, %entry ], [ %mul, %if.end15 ], [ -1, %if.else9 ]
  ret i257 %retval.0
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
