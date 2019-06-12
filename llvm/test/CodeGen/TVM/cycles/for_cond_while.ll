; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg1, i64 %arg2) nounwind {
entry:
  %cmp85 = icmp slt i64 %arg1, 1
  %rem6786 = urem i64 %arg1, 11
  %cmp187 = icmp eq i64 %rem6786, 0
  %or.cond88 = or i1 %cmp85, %cmp187
  %rem26889 = urem i64 %arg1, 7
  %cmp390 = icmp eq i64 %rem26889, 0
  %or.cond7591 = or i1 %cmp390, %or.cond88
  br i1 %or.cond7591, label %cleanup41, label %if.else5.preheader

if.else5.preheader:                               ; preds = %entry
  %cmp14108 = icmp sgt i64 %arg2, 0
  br label %if.else5

if.else5:                                         ; preds = %if.else5.preheader, %for.inc
  %iter.093 = phi i64 [ %dec38, %for.inc ], [ %arg1, %if.else5.preheader ]
  %res.092 = phi i64 [ %res.2, %for.inc ], [ 1, %if.else5.preheader ]
  %rem669 = urem i64 %iter.093, 5
  %cmp7 = icmp eq i64 %rem669, 0
  br i1 %cmp7, label %for.inc, label %if.else9

if.else9:                                         ; preds = %if.else5
  %rem1070 = urem i64 %iter.093, 3
  %cmp11 = icmp eq i64 %rem1070, 0
  br i1 %cmp11, label %cleanup41, label %while.cond.outer.preheader

while.cond.outer.preheader:                       ; preds = %if.else9
  br i1 %cmp14108, label %while.cond.outer.split.us, label %if.end36

while.cond.outer.split.us:                        ; preds = %while.cond.outer.preheader, %if.end32
  %cmp24116.in.in = phi i64 [ %dec, %if.end32 ], [ %arg2, %while.cond.outer.preheader ]
  %res.1.ph113 = phi i64 [ %mul, %if.end32 ], [ %res.092, %while.cond.outer.preheader ]
  %cmp24116.in = urem i64 %cmp24116.in.in, 5
  %cmp24116 = icmp eq i64 %cmp24116.in, 0
  %rem1571 = urem i64 %cmp24116.in.in, 11
  %cmp16 = icmp eq i64 %rem1571, 0
  br i1 %cmp16, label %cleanup41, label %while.cond.outer.split.us.split

while.cond.outer.split.us.split:                  ; preds = %while.cond.outer.split.us
  %cmp20115.in = urem i64 %cmp24116.in.in, 7
  %cmp20115 = icmp eq i64 %cmp20115.in, 0
  br i1 %cmp20115, label %if.end36, label %while.cond.outer.split.us.split.split

while.cond.outer.split.us.split.split:            ; preds = %while.cond.outer.split.us.split
  br i1 %cmp24116, label %while.cond.us, label %if.else26

while.cond.us:                                    ; preds = %while.cond.outer.split.us.split.split, %while.cond.us
  br label %while.cond.us

if.else26:                                        ; preds = %while.cond.outer.split.us.split.split
  %rem2774 = urem i64 %cmp24116.in.in, 3
  %cmp28 = icmp eq i64 %rem2774, 0
  br i1 %cmp28, label %cleanup41, label %if.end32

if.end32:                                         ; preds = %if.else26
  %mul = mul i64 %cmp24116.in.in, %res.1.ph113
  %dec = add i64 %cmp24116.in.in, -1
  %cmp14 = icmp sgt i64 %cmp24116.in.in, 1
  br i1 %cmp14, label %while.cond.outer.split.us, label %if.end36

if.end36:                                         ; preds = %while.cond.outer.split.us.split, %if.end32, %while.cond.outer.preheader
  %res.1.ph.lcssa = phi i64 [ %res.092, %while.cond.outer.preheader ], [ %res.1.ph113, %while.cond.outer.split.us.split ], [ %mul, %if.end32 ]
  %mul37 = mul i64 %res.1.ph.lcssa, %iter.093
  br label %for.inc

for.inc:                                          ; preds = %if.else5, %if.end36
  %res.2 = phi i64 [ %res.092, %if.else5 ], [ %mul37, %if.end36 ]
  %dec38 = add i64 %iter.093, -1
  %cmp = icmp slt i64 %iter.093, 2
  %rem67 = urem i64 %dec38, 11
  %cmp1 = icmp eq i64 %rem67, 0
  %or.cond = or i1 %cmp, %cmp1
  %rem268 = urem i64 %dec38, 7
  %cmp3 = icmp eq i64 %rem268, 0
  %or.cond75 = or i1 %cmp3, %or.cond
  br i1 %or.cond75, label %cleanup41, label %if.else5

cleanup41:                                        ; preds = %if.else9, %for.inc, %while.cond.outer.split.us, %if.else26, %entry
  %retval.4 = phi i64 [ 1, %entry ], [ %res.1.ph113, %while.cond.outer.split.us ], [ -1, %if.else26 ], [ -1, %if.else9 ], [ %res.2, %for.inc ]
  ret i64 %retval.4
}