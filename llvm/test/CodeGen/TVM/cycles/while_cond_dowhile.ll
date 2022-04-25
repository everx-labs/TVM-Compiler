; RUN: llc < %s -march=tvm 
; ModuleID = 'while_cond_dowhile.c'
source_filename = "while_cond_dowhile.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %arg1, i257 %arg2) local_unnamed_addr #0 {
entry:
  %cmp77 = icmp slt i257 %arg1, 1
  %rem78 = srem i257 %arg1, 11
  %cmp179 = icmp eq i257 %rem78, 0
  %or.cond80 = or i1 %cmp77, %cmp179
  %rem281 = srem i257 %arg1, 7
  %cmp382 = icmp eq i257 %rem281, 0
  %or.cond6083 = or i1 %cmp382, %or.cond80
  br i1 %or.cond6083, label %cleanup39, label %while.cond.outer.split

while.cond.outer.split:                           ; preds = %entry, %if.end36
  %iter.0.ph85 = phi i257 [ %dec38, %if.end36 ], [ %arg1, %entry ]
  %res.0.ph84 = phi i257 [ %mul37, %if.end36 ], [ 1, %entry ]
  %rem664 = urem i257 %iter.0.ph85, 5
  %cmp7 = icmp eq i257 %rem664, 0
  br i1 %cmp7, label %while.cond, label %if.else9

while.cond:                                       ; preds = %while.cond.outer.split, %while.cond
  br label %while.cond

if.else9:                                         ; preds = %while.cond.outer.split
  %rem1065 = urem i257 %iter.0.ph85, 3
  %cmp11 = icmp eq i257 %rem1065, 0
  br i1 %cmp11, label %cleanup39, label %do.body

do.body:                                          ; preds = %if.else9, %do.cond
  %res.1 = phi i257 [ %res.2, %do.cond ], [ %res.0.ph84, %if.else9 ]
  %iter1.0 = phi i257 [ %iter1.1, %do.cond ], [ %arg2, %if.else9 ]
  %rem14 = srem i257 %iter1.0, 11
  %cmp15 = icmp eq i257 %rem14, 0
  br i1 %cmp15, label %cleanup39, label %if.else17

if.else17:                                        ; preds = %do.body
  %rem18 = srem i257 %iter1.0, 7
  %cmp19 = icmp eq i257 %rem18, 0
  br i1 %cmp19, label %if.end36, label %if.else21

if.else21:                                        ; preds = %if.else17
  %rem22 = srem i257 %iter1.0, 5
  %cmp23 = icmp eq i257 %rem22, 0
  br i1 %cmp23, label %do.cond, label %if.else25

if.else25:                                        ; preds = %if.else21
  %rem26 = srem i257 %iter1.0, 3
  %cmp27 = icmp eq i257 %rem26, 0
  br i1 %cmp27, label %cleanup39, label %if.end31

if.end31:                                         ; preds = %if.else25
  %mul = mul nsw i257 %iter1.0, %res.1
  %dec = add nsw i257 %iter1.0, -1
  br label %do.cond

do.cond:                                          ; preds = %if.else21, %if.end31
  %res.2 = phi i257 [ %res.1, %if.else21 ], [ %mul, %if.end31 ]
  %iter1.1 = phi i257 [ %iter1.0, %if.else21 ], [ %dec, %if.end31 ]
  %cmp32 = icmp sgt i257 %iter1.1, 0
  br i1 %cmp32, label %do.body, label %if.end36

if.end36:                                         ; preds = %if.else17, %do.cond
  %res.4.ph = phi i257 [ %res.2, %do.cond ], [ %res.1, %if.else17 ]
  %mul37 = mul nsw i257 %res.4.ph, %iter.0.ph85
  %dec38 = add nsw i257 %iter.0.ph85, -1
  %cmp = icmp slt i257 %iter.0.ph85, 2
  %rem = srem i257 %dec38, 11
  %cmp1 = icmp eq i257 %rem, 0
  %or.cond = or i1 %cmp, %cmp1
  %rem2 = srem i257 %dec38, 7
  %cmp3 = icmp eq i257 %rem2, 0
  %or.cond60 = or i1 %cmp3, %or.cond
  br i1 %or.cond60, label %cleanup39, label %while.cond.outer.split

cleanup39:                                        ; preds = %if.else9, %if.end36, %do.body, %if.else25, %entry
  %retval.2 = phi i257 [ 1, %entry ], [ %res.1, %do.body ], [ -1, %if.else25 ], [ %mul37, %if.end36 ], [ -1, %if.else9 ]
  ret i257 %retval.2
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
