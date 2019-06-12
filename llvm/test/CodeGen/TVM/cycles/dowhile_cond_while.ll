; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg1, i64 %arg2) nounwind {
entry:
  %cmp1387 = icmp sgt i64 %arg2, 0
  br label %do.body

do.body:                                          ; preds = %do.cond, %entry
  %res.0 = phi i64 [ 1, %entry ], [ %res.2, %do.cond ]
  %iter.0 = phi i64 [ %arg1, %entry ], [ %iter.1, %do.cond ]
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
  br i1 %cmp10, label %cleanup39, label %while.cond.outer.preheader

while.cond.outer.preheader:                       ; preds = %if.else8
  br i1 %cmp1387, label %while.cond.outer.split.us, label %if.end35

while.cond.outer.split.us:                        ; preds = %while.cond.outer.preheader, %if.end31
  %cmp2395.in.in = phi i64 [ %dec, %if.end31 ], [ %arg2, %while.cond.outer.preheader ]
  %res.1.ph92 = phi i64 [ %mul, %if.end31 ], [ %res.0, %while.cond.outer.preheader ]
  %cmp2395.in = urem i64 %cmp2395.in.in, 5
  %cmp2395 = icmp eq i64 %cmp2395.in, 0
  %rem1462 = urem i64 %cmp2395.in.in, 11
  %cmp15 = icmp eq i64 %rem1462, 0
  br i1 %cmp15, label %cleanup39, label %while.cond.outer.split.us.split

while.cond.outer.split.us.split:                  ; preds = %while.cond.outer.split.us
  %cmp1994.in = urem i64 %cmp2395.in.in, 7
  %cmp1994 = icmp eq i64 %cmp1994.in, 0
  br i1 %cmp1994, label %if.end35, label %while.cond.outer.split.us.split.split

while.cond.outer.split.us.split.split:            ; preds = %while.cond.outer.split.us.split
  br i1 %cmp2395, label %while.cond.us, label %if.else25

while.cond.us:                                    ; preds = %while.cond.outer.split.us.split.split, %while.cond.us
  br label %while.cond.us

if.else25:                                        ; preds = %while.cond.outer.split.us.split.split
  %rem2665 = urem i64 %cmp2395.in.in, 3
  %cmp27 = icmp eq i64 %rem2665, 0
  br i1 %cmp27, label %cleanup39, label %if.end31

if.end31:                                         ; preds = %if.else25
  %mul = mul i64 %cmp2395.in.in, %res.1.ph92
  %dec = add i64 %cmp2395.in.in, -1
  %cmp13 = icmp sgt i64 %cmp2395.in.in, 1
  br i1 %cmp13, label %while.cond.outer.split.us, label %if.end35

if.end35:                                         ; preds = %while.cond.outer.split.us.split, %if.end31, %while.cond.outer.preheader
  %res.1.ph.lcssa = phi i64 [ %res.0, %while.cond.outer.preheader ], [ %res.1.ph92, %while.cond.outer.split.us.split ], [ %mul, %if.end31 ]
  %mul36 = mul i64 %res.1.ph.lcssa, %iter.0
  %dec37 = add i64 %iter.0, -1
  br label %do.cond

do.cond:                                          ; preds = %if.else4, %if.end35
  %res.2 = phi i64 [ %res.0, %if.else4 ], [ %mul36, %if.end35 ]
  %iter.1 = phi i64 [ %iter.0, %if.else4 ], [ %dec37, %if.end35 ]
  %cmp38 = icmp sgt i64 %iter.1, 0
  br i1 %cmp38, label %do.body, label %cleanup39

cleanup39:                                        ; preds = %if.else8, %do.cond, %do.body, %while.cond.outer.split.us, %if.else25
  %retval.3 = phi i64 [ %res.1.ph92, %while.cond.outer.split.us ], [ -1, %if.else25 ], [ -1, %if.else8 ], [ %res.2, %do.cond ], [ %res.0, %do.body ]
  ret i64 %retval.3
}