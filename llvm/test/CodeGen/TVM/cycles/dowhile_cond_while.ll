; RUN: llc < %s -march=tvm 
; ModuleID = 'dowhile_cond_while.c'
source_filename = "dowhile_cond_while.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %arg1, i257 %arg2) local_unnamed_addr #0 {
entry:
  %cmp1387 = icmp sgt i257 %arg2, 0
  br label %do.body

do.body:                                          ; preds = %do.cond, %entry
  %res.0 = phi i257 [ 1, %entry ], [ %res.2, %do.cond ]
  %iter.0 = phi i257 [ %arg1, %entry ], [ %iter.1, %do.cond ]
  %rem = srem i257 %iter.0, 11
  %cmp = icmp eq i257 %rem, 0
  %rem1 = srem i257 %iter.0, 7
  %cmp2 = icmp eq i257 %rem1, 0
  %or.cond = or i1 %cmp, %cmp2
  br i1 %or.cond, label %cleanup39, label %if.else4

if.else4:                                         ; preds = %do.body
  %rem5 = srem i257 %iter.0, 5
  %cmp6 = icmp eq i257 %rem5, 0
  br i1 %cmp6, label %do.cond, label %if.else8

if.else8:                                         ; preds = %if.else4
  %rem9 = srem i257 %iter.0, 3
  %cmp10 = icmp eq i257 %rem9, 0
  br i1 %cmp10, label %cleanup39, label %while.cond.outer.preheader

while.cond.outer.preheader:                       ; preds = %if.else8
  br i1 %cmp1387, label %while.cond.outer.split.us, label %if.end35

while.cond.outer.split.us:                        ; preds = %while.cond.outer.preheader, %if.end31
  %cmp2395.in.in = phi i257 [ %dec, %if.end31 ], [ %arg2, %while.cond.outer.preheader ]
  %res.1.ph92 = phi i257 [ %mul, %if.end31 ], [ %res.0, %while.cond.outer.preheader ]
  %cmp2395.in = urem i257 %cmp2395.in.in, 5
  %cmp2395 = icmp eq i257 %cmp2395.in, 0
  %rem1462 = urem i257 %cmp2395.in.in, 11
  %cmp15 = icmp eq i257 %rem1462, 0
  br i1 %cmp15, label %cleanup39, label %while.cond.outer.split.us.split

while.cond.outer.split.us.split:                  ; preds = %while.cond.outer.split.us
  %cmp1994.in = urem i257 %cmp2395.in.in, 7
  %cmp1994 = icmp eq i257 %cmp1994.in, 0
  br i1 %cmp1994, label %if.end35, label %while.cond.outer.split.us.split.split

while.cond.outer.split.us.split.split:            ; preds = %while.cond.outer.split.us.split
  br i1 %cmp2395, label %while.cond.us, label %if.else25

while.cond.us:                                    ; preds = %while.cond.outer.split.us.split.split, %while.cond.us
  br label %while.cond.us

if.else25:                                        ; preds = %while.cond.outer.split.us.split.split
  %rem2665 = urem i257 %cmp2395.in.in, 3
  %cmp27 = icmp eq i257 %rem2665, 0
  br i1 %cmp27, label %cleanup39, label %if.end31

if.end31:                                         ; preds = %if.else25
  %mul = mul nsw i257 %cmp2395.in.in, %res.1.ph92
  %dec = add nsw i257 %cmp2395.in.in, -1
  %cmp13 = icmp sgt i257 %cmp2395.in.in, 1
  br i1 %cmp13, label %while.cond.outer.split.us, label %if.end35

if.end35:                                         ; preds = %while.cond.outer.split.us.split, %if.end31, %while.cond.outer.preheader
  %res.1.ph.lcssa = phi i257 [ %res.0, %while.cond.outer.preheader ], [ %res.1.ph92, %while.cond.outer.split.us.split ], [ %mul, %if.end31 ]
  %mul36 = mul nsw i257 %res.1.ph.lcssa, %iter.0
  %dec37 = add nsw i257 %iter.0, -1
  br label %do.cond

do.cond:                                          ; preds = %if.else4, %if.end35
  %res.2 = phi i257 [ %res.0, %if.else4 ], [ %mul36, %if.end35 ]
  %iter.1 = phi i257 [ %iter.0, %if.else4 ], [ %dec37, %if.end35 ]
  %cmp38 = icmp sgt i257 %iter.1, 0
  br i1 %cmp38, label %do.body, label %cleanup39

cleanup39:                                        ; preds = %if.else8, %do.cond, %do.body, %while.cond.outer.split.us, %if.else25
  %retval.3 = phi i257 [ %res.1.ph92, %while.cond.outer.split.us ], [ -1, %if.else25 ], [ -1, %if.else8 ], [ %res.2, %do.cond ], [ %res.0, %do.body ]
  ret i257 %retval.3
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
