; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg1, i64 %arg2) nounwind {
entry:
  %cmp84 = icmp slt i64 %arg1, 1
  %rem7285 = urem i64 %arg1, 11
  %cmp186 = icmp eq i64 %rem7285, 0
  %or.cond87 = or i1 %cmp84, %cmp186
  %rem27388 = urem i64 %arg1, 7
  %cmp389 = icmp eq i64 %rem27388, 0
  %or.cond7690 = or i1 %cmp389, %or.cond87
  br i1 %or.cond7690, label %cleanup46, label %if.else5.lr.ph

if.else5.lr.ph:                                   ; preds = %entry
  %cmp1580 = icmp slt i64 %arg2, 0
  br i1 %cmp1580, label %if.else5.us.preheader, label %if.else5

if.else5.us.preheader:                            ; preds = %if.else5.lr.ph
  %rem18.us109 = srem i64 %arg2, 11
  %cmp19.us110 = icmp eq i64 %rem18.us109, 0
  br label %if.else5.us

if.else5.us:                                      ; preds = %if.else5.us.preheader, %for.inc41.us
  %iter.092.us = phi i64 [ %dec42.us, %for.inc41.us ], [ %arg1, %if.else5.us.preheader ]
  %res.091.us = phi i64 [ %res.3.us, %for.inc41.us ], [ 1, %if.else5.us.preheader ]
  %rem674.us = urem i64 %iter.092.us, 5
  %cmp7.us = icmp eq i64 %rem674.us, 0
  br i1 %cmp7.us, label %for.inc41.us, label %if.else9.us

if.else9.us:                                      ; preds = %if.else5.us
  %rem1075.us = urem i64 %iter.092.us, 3
  %cmp11.us = icmp eq i64 %rem1075.us, 0
  %brmerge = or i1 %cmp11.us, %cmp19.us110
  %.mux = select i1 %cmp11.us, i64 -1, i64 %res.091.us
  br i1 %brmerge, label %cleanup46, label %if.else21.us

if.else21.us:                                     ; preds = %if.else9.us, %for.inc.us
  %res.181.us112 = phi i64 [ %res.2.us, %for.inc.us ], [ %res.091.us, %if.else9.us ]
  %iter1.082.us111 = phi i64 [ %dec.us, %for.inc.us ], [ %arg2, %if.else9.us ]
  %rem22.us = srem i64 %iter1.082.us111, 7
  %cmp23.us = icmp eq i64 %rem22.us, 0
  br i1 %cmp23.us, label %if.end39.us, label %if.else25.us

if.else25.us:                                     ; preds = %if.else21.us
  %rem26.us = srem i64 %iter1.082.us111, 5
  %cmp27.us = icmp eq i64 %rem26.us, 0
  br i1 %cmp27.us, label %for.inc.us, label %if.else29.us

if.else29.us:                                     ; preds = %if.else25.us
  %rem30.us = srem i64 %iter1.082.us111, 3
  %cmp31.us = icmp eq i64 %rem30.us, 0
  br i1 %cmp31.us, label %cleanup46, label %if.end35.us

if.end35.us:                                      ; preds = %if.else29.us
  %mul.us = mul i64 %iter1.082.us111, %res.181.us112
  br label %for.inc.us

for.inc.us:                                       ; preds = %if.end35.us, %if.else25.us
  %res.2.us = phi i64 [ %res.181.us112, %if.else25.us ], [ %mul.us, %if.end35.us ]
  %dec.us = add i64 %iter1.082.us111, -1
  %rem18.us = srem i64 %dec.us, 11
  %cmp19.us = icmp eq i64 %rem18.us, 0
  br i1 %cmp19.us, label %cleanup46, label %if.else21.us

if.end39.us:                                      ; preds = %if.else21.us
  %mul40.us = mul i64 %res.181.us112, %iter.092.us
  br label %for.inc41.us

for.inc41.us:                                     ; preds = %if.end39.us, %if.else5.us
  %res.3.us = phi i64 [ %res.091.us, %if.else5.us ], [ %mul40.us, %if.end39.us ]
  %dec42.us = add i64 %iter.092.us, -1
  %cmp.us = icmp slt i64 %iter.092.us, 2
  %rem72.us = urem i64 %dec42.us, 11
  %cmp1.us = icmp eq i64 %rem72.us, 0
  %or.cond.us = or i1 %cmp.us, %cmp1.us
  %rem273.us = urem i64 %dec42.us, 7
  %cmp3.us = icmp eq i64 %rem273.us, 0
  %or.cond76.us = or i1 %cmp3.us, %or.cond.us
  br i1 %or.cond76.us, label %cleanup46, label %if.else5.us

if.else5:                                         ; preds = %if.else5.lr.ph, %for.inc41
  %iter.092 = phi i64 [ %dec42, %for.inc41 ], [ %arg1, %if.else5.lr.ph ]
  %res.091 = phi i64 [ %res.3, %for.inc41 ], [ 1, %if.else5.lr.ph ]
  %rem674 = urem i64 %iter.092, 5
  %cmp7 = icmp eq i64 %rem674, 0
  br i1 %cmp7, label %for.inc41, label %if.else9

if.else9:                                         ; preds = %if.else5
  %rem1075 = urem i64 %iter.092, 3
  %cmp11 = icmp eq i64 %rem1075, 0
  br i1 %cmp11, label %cleanup46, label %if.end39

if.end39:                                         ; preds = %if.else9
  %mul40 = mul i64 %res.091, %iter.092
  br label %for.inc41

for.inc41:                                        ; preds = %if.else5, %if.end39
  %res.3 = phi i64 [ %res.091, %if.else5 ], [ %mul40, %if.end39 ]
  %dec42 = add i64 %iter.092, -1
  %cmp = icmp slt i64 %iter.092, 2
  %rem72 = urem i64 %dec42, 11
  %cmp1 = icmp eq i64 %rem72, 0
  %or.cond = or i1 %cmp, %cmp1
  %rem273 = urem i64 %dec42, 7
  %cmp3 = icmp eq i64 %rem273, 0
  %or.cond76 = or i1 %cmp3, %or.cond
  br i1 %or.cond76, label %cleanup46, label %if.else5

cleanup46:                                        ; preds = %if.else9, %for.inc41, %for.inc41.us, %if.else9.us, %for.inc.us, %if.else29.us, %entry
  %retval.4 = phi i64 [ 1, %entry ], [ -1, %if.else29.us ], [ %res.2.us, %for.inc.us ], [ %.mux, %if.else9.us ], [ %res.3.us, %for.inc41.us ], [ -1, %if.else9 ], [ %res.3, %for.inc41 ]
  ret i64 %retval.4
}