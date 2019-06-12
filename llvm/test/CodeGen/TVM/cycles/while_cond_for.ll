; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg1, i64 %arg2) nounwind {
entry:
  %cmp1471 = icmp slt i64 %arg2, 0
  %cmp.us98 = icmp slt i64 %arg1, 1
  %rem.us99 = srem i64 %arg1, 11
  %cmp1.us100 = icmp eq i64 %rem.us99, 0
  %or.cond.us101 = or i1 %cmp.us98, %cmp1.us100
  %rem2.us102 = srem i64 %arg1, 7
  %cmp3.us103 = icmp eq i64 %rem2.us102, 0
  %or.cond60.us104 = or i1 %cmp3.us103, %or.cond.us101
  br i1 %cmp1471, label %while.cond.outer.us.preheader, label %while.cond.outer.preheader

while.cond.outer.preheader:                       ; preds = %entry
  br i1 %or.cond60.us104, label %cleanup39, label %while.cond.outer.split

while.cond.outer.us.preheader:                    ; preds = %entry
  br i1 %or.cond60.us104, label %cleanup39, label %while.cond.outer.while.cond.outer.split_crit_edge.us.preheader

while.cond.outer.while.cond.outer.split_crit_edge.us.preheader: ; preds = %while.cond.outer.us.preheader
  %rem15.us92 = srem i64 %arg2, 11
  %cmp16.us93 = icmp eq i64 %rem15.us92, 0
  br label %while.cond.outer.while.cond.outer.split_crit_edge.us

if.else18.us:                                     ; preds = %while.cond.outer.split.if.else9.us-lcssa.split_crit_edge.us, %for.inc.us
  %res.172.us95 = phi i64 [ %res.2.us, %for.inc.us ], [ %res.0.ph.us105, %while.cond.outer.split.if.else9.us-lcssa.split_crit_edge.us ]
  %iter1.073.us94 = phi i64 [ %dec.us, %for.inc.us ], [ %arg2, %while.cond.outer.split.if.else9.us-lcssa.split_crit_edge.us ]
  %rem19.us = srem i64 %iter1.073.us94, 7
  %cmp20.us = icmp eq i64 %rem19.us, 0
  br i1 %cmp20.us, label %if.end36.us, label %if.else22.us

if.else22.us:                                     ; preds = %if.else18.us
  %rem23.us = srem i64 %iter1.073.us94, 5
  %cmp24.us = icmp eq i64 %rem23.us, 0
  br i1 %cmp24.us, label %for.inc.us, label %if.else26.us

if.else26.us:                                     ; preds = %if.else22.us
  %rem27.us = srem i64 %iter1.073.us94, 3
  %cmp28.us = icmp eq i64 %rem27.us, 0
  br i1 %cmp28.us, label %cleanup39, label %if.end32.us

if.end32.us:                                      ; preds = %if.else26.us
  %mul.us = mul i64 %iter1.073.us94, %res.172.us95
  br label %for.inc.us

for.inc.us:                                       ; preds = %if.end32.us, %if.else22.us
  %res.2.us = phi i64 [ %res.172.us95, %if.else22.us ], [ %mul.us, %if.end32.us ]
  %dec.us = add i64 %iter1.073.us94, -1
  %rem15.us = srem i64 %dec.us, 11
  %cmp16.us = icmp eq i64 %rem15.us, 0
  br i1 %cmp16.us, label %cleanup39, label %if.else18.us

if.end36.us:                                      ; preds = %if.else18.us
  %mul37.us = mul i64 %res.172.us95, %iter.0.ph.us106
  %dec38.us = add i64 %iter.0.ph.us106, -1
  %cmp.us = icmp slt i64 %iter.0.ph.us106, 2
  %rem.us = srem i64 %dec38.us, 11
  %cmp1.us = icmp eq i64 %rem.us, 0
  %or.cond.us = or i1 %cmp.us, %cmp1.us
  %rem2.us = srem i64 %dec38.us, 7
  %cmp3.us = icmp eq i64 %rem2.us, 0
  %or.cond60.us = or i1 %cmp3.us, %or.cond.us
  br i1 %or.cond60.us, label %cleanup39, label %while.cond.outer.while.cond.outer.split_crit_edge.us

while.cond.us76:                                  ; preds = %while.cond.outer.while.cond.outer.split_crit_edge.us, %while.cond.us76
  br label %while.cond.us76

while.cond.outer.while.cond.outer.split_crit_edge.us: ; preds = %while.cond.outer.while.cond.outer.split_crit_edge.us.preheader, %if.end36.us
  %iter.0.ph.us106 = phi i64 [ %dec38.us, %if.end36.us ], [ %arg1, %while.cond.outer.while.cond.outer.split_crit_edge.us.preheader ]
  %res.0.ph.us105 = phi i64 [ %mul37.us, %if.end36.us ], [ 1, %while.cond.outer.while.cond.outer.split_crit_edge.us.preheader ]
  %rem663.us = urem i64 %iter.0.ph.us106, 5
  %cmp7.us = icmp eq i64 %rem663.us, 0
  br i1 %cmp7.us, label %while.cond.us76, label %while.cond.outer.split.if.else9.us-lcssa.split_crit_edge.us

while.cond.outer.split.if.else9.us-lcssa.split_crit_edge.us: ; preds = %while.cond.outer.while.cond.outer.split_crit_edge.us
  %rem1064.us = urem i64 %iter.0.ph.us106, 3
  %cmp11.us = icmp eq i64 %rem1064.us, 0
  %brmerge = or i1 %cmp11.us, %cmp16.us93
  %.mux = select i1 %cmp11.us, i64 -1, i64 %res.0.ph.us105
  br i1 %brmerge, label %cleanup39, label %if.else18.us

while.cond.outer.split:                           ; preds = %while.cond.outer.preheader, %if.end36
  %iter.0.ph117 = phi i64 [ %dec38, %if.end36 ], [ %arg1, %while.cond.outer.preheader ]
  %res.0.ph116 = phi i64 [ %mul37, %if.end36 ], [ 1, %while.cond.outer.preheader ]
  %rem663 = urem i64 %iter.0.ph117, 5
  %cmp7 = icmp eq i64 %rem663, 0
  br i1 %cmp7, label %while.cond, label %if.else9

while.cond:                                       ; preds = %while.cond.outer.split, %while.cond
  br label %while.cond

if.else9:                                         ; preds = %while.cond.outer.split
  %rem1064 = urem i64 %iter.0.ph117, 3
  %cmp11 = icmp eq i64 %rem1064, 0
  br i1 %cmp11, label %cleanup39, label %if.end36

if.end36:                                         ; preds = %if.else9
  %mul37 = mul i64 %res.0.ph116, %iter.0.ph117
  %dec38 = add i64 %iter.0.ph117, -1
  %cmp = icmp slt i64 %iter.0.ph117, 2
  %rem = srem i64 %dec38, 11
  %cmp1 = icmp eq i64 %rem, 0
  %or.cond = or i1 %cmp, %cmp1
  %rem2 = srem i64 %dec38, 7
  %cmp3 = icmp eq i64 %rem2, 0
  %or.cond60 = or i1 %cmp3, %or.cond
  br i1 %or.cond60, label %cleanup39, label %while.cond.outer.split

cleanup39:                                        ; preds = %if.else9, %if.end36, %if.end36.us, %while.cond.outer.split.if.else9.us-lcssa.split_crit_edge.us, %for.inc.us, %if.else26.us, %while.cond.outer.preheader, %while.cond.outer.us.preheader
  %retval.2 = phi i64 [ 1, %while.cond.outer.us.preheader ], [ 1, %while.cond.outer.preheader ], [ -1, %if.else26.us ], [ %res.2.us, %for.inc.us ], [ %mul37.us, %if.end36.us ], [ %.mux, %while.cond.outer.split.if.else9.us-lcssa.split_crit_edge.us ], [ %mul37, %if.end36 ], [ -1, %if.else9 ]
  ret i64 %retval.2
}