; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg1, i64 %arg2) nounwind {
entry:
  %cmp107 = icmp slt i64 %arg1, 1
  %rem108 = srem i64 %arg1, 11
  %cmp1109 = icmp eq i64 %rem108, 0
  %or.cond110 = or i1 %cmp107, %cmp1109
  %rem2111 = srem i64 %arg1, 7
  %cmp3112 = icmp eq i64 %rem2111, 0
  %or.cond63113 = or i1 %cmp3112, %or.cond110
  br i1 %or.cond63113, label %cleanup42, label %while.cond.outer.split.preheader

while.cond.outer.split.preheader:                 ; preds = %entry
  %cmp1597 = icmp sgt i64 %arg2, 0
  br label %while.cond.outer.split

while.cond.outer.split:                           ; preds = %while.cond.outer.split.preheader, %if.end38
  %iter.0.ph115 = phi i64 [ %dec40, %if.end38 ], [ %arg1, %while.cond.outer.split.preheader ]
  %res.0.ph114 = phi i64 [ %mul39, %if.end38 ], [ 1, %while.cond.outer.split.preheader ]
  %rem666 = urem i64 %iter.0.ph115, 5
  %cmp7 = icmp eq i64 %rem666, 0
  br i1 %cmp7, label %while.cond, label %if.else9

while.cond:                                       ; preds = %while.cond.outer.split, %while.cond
  br label %while.cond

if.else9:                                         ; preds = %while.cond.outer.split
  %rem1067 = urem i64 %iter.0.ph115, 3
  %cmp11 = icmp eq i64 %rem1067, 0
  br i1 %cmp11, label %cleanup42, label %while.cond14.outer.preheader

while.cond14.outer.preheader:                     ; preds = %if.else9
  br i1 %cmp1597, label %while.cond14.outer.split.us, label %if.end38

while.cond14.outer.split.us:                      ; preds = %while.cond14.outer.preheader, %if.end34
  %cmp26105.in.in = phi i64 [ %dec, %if.end34 ], [ %arg2, %while.cond14.outer.preheader ]
  %res.1.ph102 = phi i64 [ %mul, %if.end34 ], [ %res.0.ph114, %while.cond14.outer.preheader ]
  %cmp26105.in = urem i64 %cmp26105.in.in, 5
  %cmp26105 = icmp eq i64 %cmp26105.in, 0
  %rem1768 = urem i64 %cmp26105.in.in, 11
  %cmp18 = icmp eq i64 %rem1768, 0
  br i1 %cmp18, label %cleanup42, label %while.cond14.outer.split.us.split

while.cond14.outer.split.us.split:                ; preds = %while.cond14.outer.split.us
  %cmp22104.in = urem i64 %cmp26105.in.in, 7
  %cmp22104 = icmp eq i64 %cmp22104.in, 0
  br i1 %cmp22104, label %if.end38, label %while.cond14.outer.split.us.split.split

while.cond14.outer.split.us.split.split:          ; preds = %while.cond14.outer.split.us.split
  br i1 %cmp26105, label %while.cond14.us, label %if.else28

while.cond14.us:                                  ; preds = %while.cond14.outer.split.us.split.split, %while.cond14.us
  br label %while.cond14.us

if.else28:                                        ; preds = %while.cond14.outer.split.us.split.split
  %rem2971 = urem i64 %cmp26105.in.in, 3
  %cmp30 = icmp eq i64 %rem2971, 0
  br i1 %cmp30, label %cleanup42, label %if.end34

if.end34:                                         ; preds = %if.else28
  %mul = mul i64 %cmp26105.in.in, %res.1.ph102
  %dec = add i64 %cmp26105.in.in, -1
  %cmp15 = icmp sgt i64 %cmp26105.in.in, 1
  br i1 %cmp15, label %while.cond14.outer.split.us, label %if.end38

if.end38:                                         ; preds = %while.cond14.outer.split.us.split, %if.end34, %while.cond14.outer.preheader
  %res.1.ph.lcssa = phi i64 [ %res.0.ph114, %while.cond14.outer.preheader ], [ %res.1.ph102, %while.cond14.outer.split.us.split ], [ %mul, %if.end34 ]
  %mul39 = mul i64 %res.1.ph.lcssa, %iter.0.ph115
  %dec40 = add i64 %iter.0.ph115, -1
  %cmp = icmp slt i64 %iter.0.ph115, 2
  %rem = srem i64 %dec40, 11
  %cmp1 = icmp eq i64 %rem, 0
  %or.cond = or i1 %cmp, %cmp1
  %rem2 = srem i64 %dec40, 7
  %cmp3 = icmp eq i64 %rem2, 0
  %or.cond63 = or i1 %cmp3, %or.cond
  br i1 %or.cond63, label %cleanup42, label %while.cond.outer.split

cleanup42:                                        ; preds = %if.else9, %if.end38, %while.cond14.outer.split.us, %if.else28, %entry
  %retval.2 = phi i64 [ 1, %entry ], [ %res.1.ph102, %while.cond14.outer.split.us ], [ -1, %if.else28 ], [ %mul39, %if.end38 ], [ -1, %if.else9 ]
  ret i64 %retval.2
}