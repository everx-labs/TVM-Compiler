; RUN: llc < %s -march=tvm 
; ModuleID = 'for_cond_dowhile.c'
source_filename = "for_cond_dowhile.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %arg1, i257 %arg2) local_unnamed_addr #0 {
entry:
  %cmp74 = icmp slt i257 %arg1, 1
  %rem6775 = urem i257 %arg1, 11
  %cmp176 = icmp eq i257 %rem6775, 0
  %or.cond77 = or i1 %cmp74, %cmp176
  %rem26878 = urem i257 %arg1, 7
  %cmp379 = icmp eq i257 %rem26878, 0
  %or.cond7180 = or i1 %cmp379, %or.cond77
  br i1 %or.cond7180, label %cleanup41, label %if.else5

if.else5:                                         ; preds = %entry, %for.inc
  %iter.082 = phi i257 [ %dec38, %for.inc ], [ %arg1, %entry ]
  %res.081 = phi i257 [ %res.5, %for.inc ], [ 1, %entry ]
  %rem669 = urem i257 %iter.082, 5
  %cmp7 = icmp eq i257 %rem669, 0
  br i1 %cmp7, label %for.inc, label %if.else9

if.else9:                                         ; preds = %if.else5
  %rem1070 = urem i257 %iter.082, 3
  %cmp11 = icmp eq i257 %rem1070, 0
  br i1 %cmp11, label %cleanup41, label %do.body

do.body:                                          ; preds = %if.else9, %do.cond
  %res.1 = phi i257 [ %res.2, %do.cond ], [ %res.081, %if.else9 ]
  %iter1.0 = phi i257 [ %iter1.1, %do.cond ], [ %arg2, %if.else9 ]
  %rem14 = srem i257 %iter1.0, 11
  %cmp15 = icmp eq i257 %rem14, 0
  br i1 %cmp15, label %cleanup41, label %if.else17

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
  br i1 %cmp27, label %cleanup41, label %if.end31

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
  %mul37 = mul nsw i257 %res.4.ph, %iter.082
  br label %for.inc

for.inc:                                          ; preds = %if.else5, %if.end36
  %res.5 = phi i257 [ %res.081, %if.else5 ], [ %mul37, %if.end36 ]
  %dec38 = add nsw i257 %iter.082, -1
  %cmp = icmp slt i257 %iter.082, 2
  %rem67 = urem i257 %dec38, 11
  %cmp1 = icmp eq i257 %rem67, 0
  %or.cond = or i1 %cmp, %cmp1
  %rem268 = urem i257 %dec38, 7
  %cmp3 = icmp eq i257 %rem268, 0
  %or.cond71 = or i1 %cmp3, %or.cond
  br i1 %or.cond71, label %cleanup41, label %if.else5

cleanup41:                                        ; preds = %if.else9, %for.inc, %do.body, %if.else25, %entry
  %retval.4 = phi i257 [ 1, %entry ], [ %res.1, %do.body ], [ -1, %if.else25 ], [ -1, %if.else9 ], [ %res.5, %for.inc ]
  ret i257 %retval.4
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
