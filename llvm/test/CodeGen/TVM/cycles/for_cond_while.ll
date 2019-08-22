; RUN: llc < %s -march=tvm 
; ModuleID = 'for_cond_while.c'
source_filename = "for_cond_while.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %arg1, i257 %arg2) local_unnamed_addr #0 {
entry:
  %cmp84 = icmp slt i257 %arg1, 1
  %rem6685 = urem i257 %arg1, 11
  %cmp186 = icmp eq i257 %rem6685, 0
  %or.cond87 = or i1 %cmp84, %cmp186
  %rem26788 = urem i257 %arg1, 7
  %cmp389 = icmp eq i257 %rem26788, 0
  %or.cond7490 = or i1 %cmp389, %or.cond87
  br i1 %or.cond7490, label %cleanup41, label %if.else5.preheader

if.else5.preheader:                               ; preds = %entry
  %cmp14107 = icmp sgt i257 %arg2, 0
  br label %if.else5

if.else5:                                         ; preds = %if.else5.preheader, %for.inc
  %iter.092 = phi i257 [ %dec38, %for.inc ], [ %arg1, %if.else5.preheader ]
  %res.091 = phi i257 [ %res.2, %for.inc ], [ 1, %if.else5.preheader ]
  %rem668 = urem i257 %iter.092, 5
  %cmp7 = icmp eq i257 %rem668, 0
  br i1 %cmp7, label %for.inc, label %if.else9

if.else9:                                         ; preds = %if.else5
  %rem1069 = urem i257 %iter.092, 3
  %cmp11 = icmp eq i257 %rem1069, 0
  br i1 %cmp11, label %cleanup41, label %while.cond.outer.preheader

while.cond.outer.preheader:                       ; preds = %if.else9
  br i1 %cmp14107, label %while.cond.outer.split.us, label %if.end36

while.cond.outer.split.us:                        ; preds = %while.cond.outer.preheader, %if.end32
  %cmp24115.in.in = phi i257 [ %dec, %if.end32 ], [ %arg2, %while.cond.outer.preheader ]
  %res.1.ph112 = phi i257 [ %mul, %if.end32 ], [ %res.091, %while.cond.outer.preheader ]
  %cmp24115.in = urem i257 %cmp24115.in.in, 5
  %cmp24115 = icmp eq i257 %cmp24115.in, 0
  %rem1570 = urem i257 %cmp24115.in.in, 11
  %cmp16 = icmp eq i257 %rem1570, 0
  br i1 %cmp16, label %cleanup41, label %while.cond.outer.split.us.split

while.cond.outer.split.us.split:                  ; preds = %while.cond.outer.split.us
  %cmp20114.in = urem i257 %cmp24115.in.in, 7
  %cmp20114 = icmp eq i257 %cmp20114.in, 0
  br i1 %cmp20114, label %if.end36, label %while.cond.outer.split.us.split.split

while.cond.outer.split.us.split.split:            ; preds = %while.cond.outer.split.us.split
  br i1 %cmp24115, label %while.cond.us, label %if.else26

while.cond.us:                                    ; preds = %while.cond.outer.split.us.split.split, %while.cond.us
  br label %while.cond.us

if.else26:                                        ; preds = %while.cond.outer.split.us.split.split
  %rem2773 = urem i257 %cmp24115.in.in, 3
  %cmp28 = icmp eq i257 %rem2773, 0
  br i1 %cmp28, label %cleanup41, label %if.end32

if.end32:                                         ; preds = %if.else26
  %mul = mul nsw i257 %cmp24115.in.in, %res.1.ph112
  %dec = add nsw i257 %cmp24115.in.in, -1
  %cmp14 = icmp sgt i257 %cmp24115.in.in, 1
  br i1 %cmp14, label %while.cond.outer.split.us, label %if.end36

if.end36:                                         ; preds = %while.cond.outer.split.us.split, %if.end32, %while.cond.outer.preheader
  %res.1.ph.lcssa = phi i257 [ %res.091, %while.cond.outer.preheader ], [ %res.1.ph112, %while.cond.outer.split.us.split ], [ %mul, %if.end32 ]
  %mul37 = mul nsw i257 %res.1.ph.lcssa, %iter.092
  br label %for.inc

for.inc:                                          ; preds = %if.else5, %if.end36
  %res.2 = phi i257 [ %res.091, %if.else5 ], [ %mul37, %if.end36 ]
  %dec38 = add nsw i257 %iter.092, -1
  %cmp = icmp slt i257 %iter.092, 2
  %rem66 = urem i257 %dec38, 11
  %cmp1 = icmp eq i257 %rem66, 0
  %or.cond = or i1 %cmp, %cmp1
  %rem267 = urem i257 %dec38, 7
  %cmp3 = icmp eq i257 %rem267, 0
  %or.cond74 = or i1 %cmp3, %or.cond
  br i1 %or.cond74, label %cleanup41, label %if.else5

cleanup41:                                        ; preds = %if.else9, %for.inc, %while.cond.outer.split.us, %if.else26, %entry
  %retval.4 = phi i257 [ 1, %entry ], [ %res.1.ph112, %while.cond.outer.split.us ], [ -1, %if.else26 ], [ -1, %if.else9 ], [ %res.2, %for.inc ]
  ret i257 %retval.4
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
