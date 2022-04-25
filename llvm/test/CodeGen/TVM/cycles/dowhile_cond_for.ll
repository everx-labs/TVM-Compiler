; RUN: llc < %s -march=tvm 
; ModuleID = 'dowhile_cond_for.c'
source_filename = "dowhile_cond_for.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %arg1, i257 %arg2) local_unnamed_addr #0 {
entry:
  %cmp1365 = icmp slt i257 %arg2, 0
  br i1 %cmp1365, label %do.body.us.preheader, label %do.body

do.body.us.preheader:                             ; preds = %entry
  %rem14.us82 = srem i257 %arg2, 11
  %cmp15.us83 = icmp eq i257 %rem14.us82, 0
  br label %do.body.us

do.body.us:                                       ; preds = %do.body.us.preheader, %do.cond.us
  %res.0.us = phi i257 [ %res.3.us, %do.cond.us ], [ 1, %do.body.us.preheader ]
  %iter.0.us = phi i257 [ %iter.1.us, %do.cond.us ], [ %arg1, %do.body.us.preheader ]
  %rem.us = srem i257 %iter.0.us, 11
  %cmp.us = icmp eq i257 %rem.us, 0
  %rem1.us = srem i257 %iter.0.us, 7
  %cmp2.us = icmp eq i257 %rem1.us, 0
  %or.cond.us = or i1 %cmp.us, %cmp2.us
  br i1 %or.cond.us, label %cleanup39, label %if.else4.us

if.else4.us:                                      ; preds = %do.body.us
  %rem5.us = srem i257 %iter.0.us, 5
  %cmp6.us = icmp eq i257 %rem5.us, 0
  br i1 %cmp6.us, label %do.cond.us, label %if.else8.us

if.else8.us:                                      ; preds = %if.else4.us
  %rem9.us = srem i257 %iter.0.us, 3
  %cmp10.us = icmp eq i257 %rem9.us, 0
  %brmerge = or i1 %cmp10.us, %cmp15.us83
  %.mux = select i1 %cmp10.us, i257 -1, i257 %res.0.us
  br i1 %brmerge, label %cleanup39, label %if.else17.us

if.else17.us:                                     ; preds = %if.else8.us, %for.inc.us
  %res.166.us85 = phi i257 [ %res.2.us, %for.inc.us ], [ %res.0.us, %if.else8.us ]
  %iter1.067.us84 = phi i257 [ %dec.us, %for.inc.us ], [ %arg2, %if.else8.us ]
  %rem18.us = srem i257 %iter1.067.us84, 7
  %cmp19.us = icmp eq i257 %rem18.us, 0
  br i1 %cmp19.us, label %if.end35.us, label %if.else21.us

if.else21.us:                                     ; preds = %if.else17.us
  %rem22.us = srem i257 %iter1.067.us84, 5
  %cmp23.us = icmp eq i257 %rem22.us, 0
  br i1 %cmp23.us, label %for.inc.us, label %if.else25.us

if.else25.us:                                     ; preds = %if.else21.us
  %rem26.us = srem i257 %iter1.067.us84, 3
  %cmp27.us = icmp eq i257 %rem26.us, 0
  br i1 %cmp27.us, label %cleanup39, label %if.end31.us

if.end31.us:                                      ; preds = %if.else25.us
  %mul.us = mul nsw i257 %iter1.067.us84, %res.166.us85
  br label %for.inc.us

for.inc.us:                                       ; preds = %if.end31.us, %if.else21.us
  %res.2.us = phi i257 [ %res.166.us85, %if.else21.us ], [ %mul.us, %if.end31.us ]
  %dec.us = add nsw i257 %iter1.067.us84, -1
  %rem14.us = srem i257 %dec.us, 11
  %cmp15.us = icmp eq i257 %rem14.us, 0
  br i1 %cmp15.us, label %cleanup39, label %if.else17.us

if.end35.us:                                      ; preds = %if.else17.us
  %mul36.us = mul nsw i257 %res.166.us85, %iter.0.us
  %dec37.us = add nsw i257 %iter.0.us, -1
  br label %do.cond.us

do.cond.us:                                       ; preds = %if.end35.us, %if.else4.us
  %res.3.us = phi i257 [ %res.0.us, %if.else4.us ], [ %mul36.us, %if.end35.us ]
  %iter.1.us = phi i257 [ %iter.0.us, %if.else4.us ], [ %dec37.us, %if.end35.us ]
  %cmp38.us = icmp sgt i257 %iter.1.us, 0
  br i1 %cmp38.us, label %do.body.us, label %cleanup39

do.body:                                          ; preds = %entry, %do.cond
  %res.0 = phi i257 [ %res.3, %do.cond ], [ 1, %entry ]
  %iter.0 = phi i257 [ %iter.1, %do.cond ], [ %arg1, %entry ]
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
  br i1 %cmp10, label %cleanup39, label %if.end35

if.end35:                                         ; preds = %if.else8
  %mul36 = mul nsw i257 %res.0, %iter.0
  %dec37 = add nsw i257 %iter.0, -1
  br label %do.cond

do.cond:                                          ; preds = %if.else4, %if.end35
  %res.3 = phi i257 [ %res.0, %if.else4 ], [ %mul36, %if.end35 ]
  %iter.1 = phi i257 [ %iter.0, %if.else4 ], [ %dec37, %if.end35 ]
  %cmp38 = icmp sgt i257 %iter.1, 0
  br i1 %cmp38, label %do.body, label %cleanup39

cleanup39:                                        ; preds = %if.else8, %do.cond, %do.body, %do.cond.us, %do.body.us, %if.else8.us, %for.inc.us, %if.else25.us
  %retval.3 = phi i257 [ -1, %if.else25.us ], [ %res.2.us, %for.inc.us ], [ %res.0.us, %do.body.us ], [ %res.3.us, %do.cond.us ], [ %.mux, %if.else8.us ], [ %res.0, %do.body ], [ %res.3, %do.cond ], [ -1, %if.else8 ]
  ret i257 %retval.3
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
