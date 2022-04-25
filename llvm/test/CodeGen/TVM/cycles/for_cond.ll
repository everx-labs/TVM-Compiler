; RUN: llc < %s -march=tvm 
; ModuleID = 'for_cond.c'
source_filename = "for_cond.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %n) local_unnamed_addr #0 {
entry:
  %cmp33 = icmp slt i257 %n, 1
  %rem2834 = urem i257 %n, 11
  %cmp135 = icmp eq i257 %rem2834, 0
  %or.cond36 = or i1 %cmp33, %cmp135
  %rem22937 = urem i257 %n, 7
  %cmp338 = icmp eq i257 %rem22937, 0
  %or.cond3239 = or i1 %cmp338, %or.cond36
  br i1 %or.cond3239, label %cleanup, label %if.else5

if.else5:                                         ; preds = %entry, %for.inc
  %iter.041 = phi i257 [ %dec, %for.inc ], [ %n, %entry ]
  %res.040 = phi i257 [ %res.1, %for.inc ], [ 1, %entry ]
  %rem630 = urem i257 %iter.041, 5
  %cmp7 = icmp eq i257 %rem630, 0
  br i1 %cmp7, label %for.inc, label %if.else9

if.else9:                                         ; preds = %if.else5
  %rem1031 = urem i257 %iter.041, 3
  %cmp11 = icmp eq i257 %rem1031, 0
  br i1 %cmp11, label %cleanup, label %if.end15

if.end15:                                         ; preds = %if.else9
  %mul = mul nsw i257 %iter.041, %res.040
  br label %for.inc

for.inc:                                          ; preds = %if.else5, %if.end15
  %res.1 = phi i257 [ %res.040, %if.else5 ], [ %mul, %if.end15 ]
  %dec = add nsw i257 %iter.041, -1
  %cmp = icmp slt i257 %iter.041, 2
  %rem28 = urem i257 %dec, 11
  %cmp1 = icmp eq i257 %rem28, 0
  %or.cond = or i1 %cmp, %cmp1
  %rem229 = urem i257 %dec, 7
  %cmp3 = icmp eq i257 %rem229, 0
  %or.cond32 = or i1 %cmp3, %or.cond
  br i1 %or.cond32, label %cleanup, label %if.else5

cleanup:                                          ; preds = %if.else9, %for.inc, %entry
  %0 = phi i257 [ 1, %entry ], [ %res.1, %for.inc ], [ -1, %if.else9 ]
  ret i257 %0
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
