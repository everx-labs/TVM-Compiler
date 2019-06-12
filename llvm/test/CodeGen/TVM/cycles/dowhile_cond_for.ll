; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg1, i64 %arg2) nounwind {
entry:
  %cmp1365 = icmp slt i64 %arg2, 0
  br i1 %cmp1365, label %do.body.us.preheader, label %do.body

do.body.us.preheader:                             ; preds = %entry
  %rem14.us82 = srem i64 %arg2, 11
  %cmp15.us83 = icmp eq i64 %rem14.us82, 0
  br label %do.body.us

do.body.us:                                       ; preds = %do.body.us.preheader, %do.cond.us
  %res.0.us = phi i64 [ %res.3.us, %do.cond.us ], [ 1, %do.body.us.preheader ]
  %iter.0.us = phi i64 [ %iter.1.us, %do.cond.us ], [ %arg1, %do.body.us.preheader ]
  %rem.us = srem i64 %iter.0.us, 11
  %cmp.us = icmp eq i64 %rem.us, 0
  %rem1.us = srem i64 %iter.0.us, 7
  %cmp2.us = icmp eq i64 %rem1.us, 0
  %or.cond.us = or i1 %cmp.us, %cmp2.us
  br i1 %or.cond.us, label %cleanup39, label %if.else4.us

if.else4.us:                                      ; preds = %do.body.us
  %rem5.us = srem i64 %iter.0.us, 5
  %cmp6.us = icmp eq i64 %rem5.us, 0
  br i1 %cmp6.us, label %do.cond.us, label %if.else8.us

if.else8.us:                                      ; preds = %if.else4.us
  %rem9.us = srem i64 %iter.0.us, 3
  %cmp10.us = icmp eq i64 %rem9.us, 0
  %brmerge = or i1 %cmp10.us, %cmp15.us83
  %.mux = select i1 %cmp10.us, i64 -1, i64 %res.0.us
  br i1 %brmerge, label %cleanup39, label %if.else17.us

if.else17.us:                                     ; preds = %if.else8.us, %for.inc.us
  %res.166.us85 = phi i64 [ %res.2.us, %for.inc.us ], [ %res.0.us, %if.else8.us ]
  %iter1.067.us84 = phi i64 [ %dec.us, %for.inc.us ], [ %arg2, %if.else8.us ]
  %rem18.us = srem i64 %iter1.067.us84, 7
  %cmp19.us = icmp eq i64 %rem18.us, 0
  br i1 %cmp19.us, label %if.end35.us, label %if.else21.us

if.else21.us:                                     ; preds = %if.else17.us
  %rem22.us = srem i64 %iter1.067.us84, 5
  %cmp23.us = icmp eq i64 %rem22.us, 0
  br i1 %cmp23.us, label %for.inc.us, label %if.else25.us

if.else25.us:                                     ; preds = %if.else21.us
  %rem26.us = srem i64 %iter1.067.us84, 3
  %cmp27.us = icmp eq i64 %rem26.us, 0
  br i1 %cmp27.us, label %cleanup39, label %if.end31.us

if.end31.us:                                      ; preds = %if.else25.us
  %mul.us = mul i64 %iter1.067.us84, %res.166.us85
  br label %for.inc.us

for.inc.us:                                       ; preds = %if.end31.us, %if.else21.us
  %res.2.us = phi i64 [ %res.166.us85, %if.else21.us ], [ %mul.us, %if.end31.us ]
  %dec.us = add i64 %iter1.067.us84, -1
  %rem14.us = srem i64 %dec.us, 11
  %cmp15.us = icmp eq i64 %rem14.us, 0
  br i1 %cmp15.us, label %cleanup39, label %if.else17.us

if.end35.us:                                      ; preds = %if.else17.us
  %mul36.us = mul i64 %res.166.us85, %iter.0.us
  %dec37.us = add i64 %iter.0.us, -1
  br label %do.cond.us

do.cond.us:                                       ; preds = %if.end35.us, %if.else4.us
  %res.3.us = phi i64 [ %res.0.us, %if.else4.us ], [ %mul36.us, %if.end35.us ]
  %iter.1.us = phi i64 [ %iter.0.us, %if.else4.us ], [ %dec37.us, %if.end35.us ]
  %cmp38.us = icmp sgt i64 %iter.1.us, 0
  br i1 %cmp38.us, label %do.body.us, label %cleanup39

do.body:                                          ; preds = %entry, %do.cond
  %res.0 = phi i64 [ %res.3, %do.cond ], [ 1, %entry ]
  %iter.0 = phi i64 [ %iter.1, %do.cond ], [ %arg1, %entry ]
  %rem = srem i64 %iter.0, 11
  %cmp = icmp eq i64 %rem, 0
  %rem1 = srem i64 %iter.0, 7
  %cmp2 = icmp eq i64 %rem1, 0
  %or.cond = or i1 %cmp, %cmp2
  br i1 %or.cond, label %cleanup39, label %if.else4

if.else4:                                         ; preds = %do.body
  %rem5 = srem i64 %iter.0, 5
  %cmp6 = icmp eq i64 %rem5, 0
  br i1 %cmp6, label %do.cond, label %if.else8

if.else8:                                         ; preds = %if.else4
  %rem9 = srem i64 %iter.0, 3
  %cmp10 = icmp eq i64 %rem9, 0
  br i1 %cmp10, label %cleanup39, label %if.end35

if.end35:                                         ; preds = %if.else8
  %mul36 = mul i64 %res.0, %iter.0
  %dec37 = add i64 %iter.0, -1
  br label %do.cond

do.cond:                                          ; preds = %if.else4, %if.end35
  %res.3 = phi i64 [ %res.0, %if.else4 ], [ %mul36, %if.end35 ]
  %iter.1 = phi i64 [ %iter.0, %if.else4 ], [ %dec37, %if.end35 ]
  %cmp38 = icmp sgt i64 %iter.1, 0
  br i1 %cmp38, label %do.body, label %cleanup39

cleanup39:                                        ; preds = %if.else8, %do.cond, %do.body, %do.cond.us, %do.body.us, %if.else8.us, %for.inc.us, %if.else25.us
  %retval.3 = phi i64 [ -1, %if.else25.us ], [ %res.2.us, %for.inc.us ], [ %res.0.us, %do.body.us ], [ %res.3.us, %do.cond.us ], [ %.mux, %if.else8.us ], [ %res.0, %do.body ], [ %res.3, %do.cond ], [ -1, %if.else8 ]
  ret i64 %retval.3
}