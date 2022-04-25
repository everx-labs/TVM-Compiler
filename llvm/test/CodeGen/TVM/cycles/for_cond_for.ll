; RUN: llc < %s -march=tvm 
; ModuleID = 'for_cond_for.c'
source_filename = "for_cond_for.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %arg1, i257 %arg2) local_unnamed_addr #0 {
entry:
  %cmp83 = icmp slt i257 %arg1, 1
  %rem7184 = urem i257 %arg1, 11
  %cmp185 = icmp eq i257 %rem7184, 0
  %or.cond86 = or i1 %cmp83, %cmp185
  %rem27287 = urem i257 %arg1, 7
  %cmp388 = icmp eq i257 %rem27287, 0
  %or.cond7589 = or i1 %cmp388, %or.cond86
  br i1 %or.cond7589, label %cleanup46, label %if.else5.lr.ph

if.else5.lr.ph:                                   ; preds = %entry
  %cmp1579 = icmp slt i257 %arg2, 0
  br i1 %cmp1579, label %if.else5.us.preheader, label %if.else5

if.else5.us.preheader:                            ; preds = %if.else5.lr.ph
  %rem18.us108 = srem i257 %arg2, 11
  %cmp19.us109 = icmp eq i257 %rem18.us108, 0
  br label %if.else5.us

if.else5.us:                                      ; preds = %if.else5.us.preheader, %for.inc41.us
  %iter.091.us = phi i257 [ %dec42.us, %for.inc41.us ], [ %arg1, %if.else5.us.preheader ]
  %res.090.us = phi i257 [ %res.3.us, %for.inc41.us ], [ 1, %if.else5.us.preheader ]
  %rem673.us = urem i257 %iter.091.us, 5
  %cmp7.us = icmp eq i257 %rem673.us, 0
  br i1 %cmp7.us, label %for.inc41.us, label %if.else9.us

if.else9.us:                                      ; preds = %if.else5.us
  %rem1074.us = urem i257 %iter.091.us, 3
  %cmp11.us = icmp eq i257 %rem1074.us, 0
  %brmerge = or i1 %cmp11.us, %cmp19.us109
  %.mux = select i1 %cmp11.us, i257 -1, i257 %res.090.us
  br i1 %brmerge, label %cleanup46, label %if.else21.us

if.else21.us:                                     ; preds = %if.else9.us, %for.inc.us
  %res.180.us111 = phi i257 [ %res.2.us, %for.inc.us ], [ %res.090.us, %if.else9.us ]
  %iter1.081.us110 = phi i257 [ %dec.us, %for.inc.us ], [ %arg2, %if.else9.us ]
  %rem22.us = srem i257 %iter1.081.us110, 7
  %cmp23.us = icmp eq i257 %rem22.us, 0
  br i1 %cmp23.us, label %if.end39.us, label %if.else25.us

if.else25.us:                                     ; preds = %if.else21.us
  %rem26.us = srem i257 %iter1.081.us110, 5
  %cmp27.us = icmp eq i257 %rem26.us, 0
  br i1 %cmp27.us, label %for.inc.us, label %if.else29.us

if.else29.us:                                     ; preds = %if.else25.us
  %rem30.us = srem i257 %iter1.081.us110, 3
  %cmp31.us = icmp eq i257 %rem30.us, 0
  br i1 %cmp31.us, label %cleanup46, label %if.end35.us

if.end35.us:                                      ; preds = %if.else29.us
  %mul.us = mul nsw i257 %iter1.081.us110, %res.180.us111
  br label %for.inc.us

for.inc.us:                                       ; preds = %if.end35.us, %if.else25.us
  %res.2.us = phi i257 [ %res.180.us111, %if.else25.us ], [ %mul.us, %if.end35.us ]
  %dec.us = add nsw i257 %iter1.081.us110, -1
  %rem18.us = srem i257 %dec.us, 11
  %cmp19.us = icmp eq i257 %rem18.us, 0
  br i1 %cmp19.us, label %cleanup46, label %if.else21.us

if.end39.us:                                      ; preds = %if.else21.us
  %mul40.us = mul nsw i257 %res.180.us111, %iter.091.us
  br label %for.inc41.us

for.inc41.us:                                     ; preds = %if.end39.us, %if.else5.us
  %res.3.us = phi i257 [ %res.090.us, %if.else5.us ], [ %mul40.us, %if.end39.us ]
  %dec42.us = add nsw i257 %iter.091.us, -1
  %cmp.us = icmp slt i257 %iter.091.us, 2
  %rem71.us = urem i257 %dec42.us, 11
  %cmp1.us = icmp eq i257 %rem71.us, 0
  %or.cond.us = or i1 %cmp.us, %cmp1.us
  %rem272.us = urem i257 %dec42.us, 7
  %cmp3.us = icmp eq i257 %rem272.us, 0
  %or.cond75.us = or i1 %cmp3.us, %or.cond.us
  br i1 %or.cond75.us, label %cleanup46, label %if.else5.us

if.else5:                                         ; preds = %if.else5.lr.ph, %for.inc41
  %iter.091 = phi i257 [ %dec42, %for.inc41 ], [ %arg1, %if.else5.lr.ph ]
  %res.090 = phi i257 [ %res.3, %for.inc41 ], [ 1, %if.else5.lr.ph ]
  %rem673 = urem i257 %iter.091, 5
  %cmp7 = icmp eq i257 %rem673, 0
  br i1 %cmp7, label %for.inc41, label %if.else9

if.else9:                                         ; preds = %if.else5
  %rem1074 = urem i257 %iter.091, 3
  %cmp11 = icmp eq i257 %rem1074, 0
  br i1 %cmp11, label %cleanup46, label %if.end39

if.end39:                                         ; preds = %if.else9
  %mul40 = mul nsw i257 %res.090, %iter.091
  br label %for.inc41

for.inc41:                                        ; preds = %if.else5, %if.end39
  %res.3 = phi i257 [ %res.090, %if.else5 ], [ %mul40, %if.end39 ]
  %dec42 = add nsw i257 %iter.091, -1
  %cmp = icmp slt i257 %iter.091, 2
  %rem71 = urem i257 %dec42, 11
  %cmp1 = icmp eq i257 %rem71, 0
  %or.cond = or i1 %cmp, %cmp1
  %rem272 = urem i257 %dec42, 7
  %cmp3 = icmp eq i257 %rem272, 0
  %or.cond75 = or i1 %cmp3, %or.cond
  br i1 %or.cond75, label %cleanup46, label %if.else5

cleanup46:                                        ; preds = %if.else9, %for.inc41, %for.inc41.us, %if.else9.us, %for.inc.us, %if.else29.us, %entry
  %retval.4 = phi i257 [ 1, %entry ], [ -1, %if.else29.us ], [ %res.2.us, %for.inc.us ], [ %.mux, %if.else9.us ], [ %res.3.us, %for.inc41.us ], [ -1, %if.else9 ], [ %res.3, %for.inc41 ]
  ret i257 %retval.4
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
