; RUN: llc < %s -march=tvm 
; ModuleID = 'dowhile_cond_dowhile.c'
source_filename = "dowhile_cond_dowhile.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %arg1, i257 %arg2) local_unnamed_addr #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %do.cond39, %entry
  %res.0 = phi i257 [ 1, %entry ], [ %res.5, %do.cond39 ]
  %iter.0 = phi i257 [ %arg1, %entry ], [ %iter.1, %do.cond39 ]
  %rem = srem i257 %iter.0, 11
  %cmp = icmp eq i257 %rem, 0
  %rem1 = srem i257 %iter.0, 7
  %cmp2 = icmp eq i257 %rem1, 0
  %or.cond = or i1 %cmp, %cmp2
  br i1 %or.cond, label %cleanup42, label %if.else4

if.else4:                                         ; preds = %do.body
  %rem5 = srem i257 %iter.0, 5
  %cmp6 = icmp eq i257 %rem5, 0
  br i1 %cmp6, label %do.cond39, label %if.else8

if.else8:                                         ; preds = %if.else4
  %rem9 = srem i257 %iter.0, 3
  %cmp10 = icmp eq i257 %rem9, 0
  br i1 %cmp10, label %cleanup42, label %do.body13

do.body13:                                        ; preds = %if.else8, %do.cond
  %res.1 = phi i257 [ %res.2, %do.cond ], [ %res.0, %if.else8 ]
  %iter1.0 = phi i257 [ %iter1.1, %do.cond ], [ %arg2, %if.else8 ]
  %rem14 = srem i257 %iter1.0, 11
  %cmp15 = icmp eq i257 %rem14, 0
  br i1 %cmp15, label %cleanup42, label %if.else17

if.else17:                                        ; preds = %do.body13
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
  br i1 %cmp27, label %cleanup42, label %if.end31

if.end31:                                         ; preds = %if.else25
  %mul = mul nsw i257 %iter1.0, %res.1
  %dec = add nsw i257 %iter1.0, -1
  br label %do.cond

do.cond:                                          ; preds = %if.else21, %if.end31
  %res.2 = phi i257 [ %res.1, %if.else21 ], [ %mul, %if.end31 ]
  %iter1.1 = phi i257 [ %iter1.0, %if.else21 ], [ %dec, %if.end31 ]
  %cmp32 = icmp sgt i257 %iter1.1, 0
  br i1 %cmp32, label %do.body13, label %if.end36

if.end36:                                         ; preds = %if.else17, %do.cond
  %res.4.ph = phi i257 [ %res.2, %do.cond ], [ %res.1, %if.else17 ]
  %mul37 = mul nsw i257 %res.4.ph, %iter.0
  %dec38 = add nsw i257 %iter.0, -1
  br label %do.cond39

do.cond39:                                        ; preds = %if.else4, %if.end36
  %res.5 = phi i257 [ %res.0, %if.else4 ], [ %mul37, %if.end36 ]
  %iter.1 = phi i257 [ %iter.0, %if.else4 ], [ %dec38, %if.end36 ]
  %cmp40 = icmp sgt i257 %iter.1, 0
  br i1 %cmp40, label %do.body, label %cleanup42

cleanup42:                                        ; preds = %if.else8, %do.cond39, %do.body, %do.body13, %if.else25
  %retval.3 = phi i257 [ %res.1, %do.body13 ], [ -1, %if.else25 ], [ -1, %if.else8 ], [ %res.5, %do.cond39 ], [ %res.0, %do.body ]
  ret i257 %retval.3
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
