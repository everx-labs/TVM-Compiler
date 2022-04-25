; XFAIL: *
; RUN: llc < %s -march=tvm 
; ModuleID = 'mergeSort.c'
source_filename = "mergeSort.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind
define dso_local void @merge(i257* nocapture %arr, i257 %l, i257 %m, i257 %r) local_unnamed_addr #0 {
entry:
  %sub = sub nsw i257 %m, %l
  %add = add nsw i257 %sub, 1
  %sub1 = sub nsw i257 %r, %m
  %vla = alloca i257, i257 %add, align 1
  %vla2 = alloca i257, i257 %sub1, align 1
  %cmp107 = icmp slt i257 %sub, 0
  br i1 %cmp107, label %for.cond6.preheader, label %for.body

for.cond6.preheader:                              ; preds = %for.body, %entry
  %cmp7105 = icmp sgt i257 %sub1, 0
  br i1 %cmp7105, label %for.body8, label %while.cond28.preheader

for.body:                                         ; preds = %entry, %for.body
  %i.0108 = phi i257 [ %inc, %for.body ], [ 0, %entry ]
  %add4 = add nsw i257 %i.0108, %l
  %arrayidx = getelementptr inbounds i257, i257* %arr, i257 %add4
  %0 = load i257, i257* %arrayidx, align 1, !tbaa !2
  %arrayidx5 = getelementptr inbounds i257, i257* %vla, i257 %i.0108
  store i257 %0, i257* %arrayidx5, align 1, !tbaa !2
  %inc = add nuw nsw i257 %i.0108, 1
  %cmp = icmp slt i257 %i.0108, %sub
  br i1 %cmp, label %for.body, label %for.cond6.preheader

while.cond.preheader:                             ; preds = %for.body8
  %cmp1696 = icmp sgt i257 %sub, -1
  %or.cond98 = and i1 %cmp7105, %cmp1696
  br i1 %or.cond98, label %while.body, label %while.cond28.preheader

for.body8:                                        ; preds = %for.cond6.preheader, %for.body8
  %j.0106 = phi i257 [ %add9, %for.body8 ], [ 0, %for.cond6.preheader ]
  %add9 = add nuw nsw i257 %j.0106, 1
  %add10 = add i257 %add9, %m
  %arrayidx11 = getelementptr inbounds i257, i257* %arr, i257 %add10
  %1 = load i257, i257* %arrayidx11, align 1, !tbaa !2
  %arrayidx12 = getelementptr inbounds i257, i257* %vla2, i257 %j.0106
  store i257 %1, i257* %arrayidx12, align 1, !tbaa !2
  %cmp7 = icmp slt i257 %add9, %sub1
  br i1 %cmp7, label %for.body8, label %while.cond.preheader

while.cond28.preheader:                           ; preds = %if.end, %for.cond6.preheader, %while.cond.preheader
  %k.0.lcssa = phi i257 [ %l, %while.cond.preheader ], [ %l, %for.cond6.preheader ], [ %inc27, %if.end ]
  %j.1.lcssa = phi i257 [ 0, %while.cond.preheader ], [ 0, %for.cond6.preheader ], [ %j.2, %if.end ]
  %i.1.lcssa = phi i257 [ 0, %while.cond.preheader ], [ 0, %for.cond6.preheader ], [ %i.2, %if.end ]
  %cmp2993 = icmp sgt i257 %i.1.lcssa, %sub
  br i1 %cmp2993, label %while.cond36.preheader, label %while.body30

while.body:                                       ; preds = %while.cond.preheader, %if.end
  %i.1101 = phi i257 [ %i.2, %if.end ], [ 0, %while.cond.preheader ]
  %j.1100 = phi i257 [ %j.2, %if.end ], [ 0, %while.cond.preheader ]
  %k.099 = phi i257 [ %inc27, %if.end ], [ %l, %while.cond.preheader ]
  %arrayidx18 = getelementptr inbounds i257, i257* %vla, i257 %i.1101
  %2 = load i257, i257* %arrayidx18, align 1, !tbaa !2
  %arrayidx19 = getelementptr inbounds i257, i257* %vla2, i257 %j.1100
  %3 = load i257, i257* %arrayidx19, align 1, !tbaa !2
  %cmp20 = icmp sgt i257 %2, %3
  %arrayidx25 = getelementptr inbounds i257, i257* %arr, i257 %k.099
  br i1 %cmp20, label %if.else, label %if.then

if.then:                                          ; preds = %while.body
  store i257 %2, i257* %arrayidx25, align 1, !tbaa !2
  %inc23 = add nsw i257 %i.1101, 1
  br label %if.end

if.else:                                          ; preds = %while.body
  store i257 %3, i257* %arrayidx25, align 1, !tbaa !2
  %inc26 = add nsw i257 %j.1100, 1
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %j.2 = phi i257 [ %j.1100, %if.then ], [ %inc26, %if.else ]
  %i.2 = phi i257 [ %inc23, %if.then ], [ %i.1101, %if.else ]
  %inc27 = add nsw i257 %k.099, 1
  %cmp16 = icmp sle i257 %i.2, %sub
  %cmp17 = icmp slt i257 %j.2, %sub1
  %or.cond = and i1 %cmp17, %cmp16
  br i1 %or.cond, label %while.body, label %while.cond28.preheader

while.cond36.preheader:                           ; preds = %while.body30, %while.cond28.preheader
  %k.1.lcssa = phi i257 [ %k.0.lcssa, %while.cond28.preheader ], [ %inc34, %while.body30 ]
  %cmp3790 = icmp slt i257 %j.1.lcssa, %sub1
  br i1 %cmp3790, label %while.body38, label %while.end43

while.body30:                                     ; preds = %while.cond28.preheader, %while.body30
  %i.395 = phi i257 [ %inc33, %while.body30 ], [ %i.1.lcssa, %while.cond28.preheader ]
  %k.194 = phi i257 [ %inc34, %while.body30 ], [ %k.0.lcssa, %while.cond28.preheader ]
  %arrayidx31 = getelementptr inbounds i257, i257* %vla, i257 %i.395
  %4 = load i257, i257* %arrayidx31, align 1, !tbaa !2
  %arrayidx32 = getelementptr inbounds i257, i257* %arr, i257 %k.194
  store i257 %4, i257* %arrayidx32, align 1, !tbaa !2
  %inc33 = add nsw i257 %i.395, 1
  %inc34 = add nsw i257 %k.194, 1
  %cmp29 = icmp slt i257 %i.395, %sub
  br i1 %cmp29, label %while.body30, label %while.cond36.preheader

while.body38:                                     ; preds = %while.cond36.preheader, %while.body38
  %j.392 = phi i257 [ %inc41, %while.body38 ], [ %j.1.lcssa, %while.cond36.preheader ]
  %k.291 = phi i257 [ %inc42, %while.body38 ], [ %k.1.lcssa, %while.cond36.preheader ]
  %arrayidx39 = getelementptr inbounds i257, i257* %vla2, i257 %j.392
  %5 = load i257, i257* %arrayidx39, align 1, !tbaa !2
  %arrayidx40 = getelementptr inbounds i257, i257* %arr, i257 %k.291
  store i257 %5, i257* %arrayidx40, align 1, !tbaa !2
  %inc41 = add nsw i257 %j.392, 1
  %inc42 = add nsw i257 %k.291, 1
  %cmp37 = icmp slt i257 %inc41, %sub1
  br i1 %cmp37, label %while.body38, label %while.end43

while.end43:                                      ; preds = %while.body38, %while.cond36.preheader
  ret void
}

; Function Attrs: nounwind
define dso_local void @mergeSort(i257* %arr, i257 %l, i257 %r) local_unnamed_addr #1 {
entry:
  %cmp = icmp sgt i257 %r, %l
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %sub = sub nsw i257 %r, %l
  %div = sdiv i257 %sub, 2
  %add = add nsw i257 %div, %l
  tail call void @mergeSort(i257* %arr, i257 %l, i257 %add)
  %add1 = add nsw i257 %add, 1
  tail call void @mergeSort(i257* %arr, i257 %add1, i257 %r)
  tail call void @merge(i257* %arr, i257 %l, i257 %add, i257 %r)
  ret void

if.end:                                           ; preds = %entry
  ret void
}

attributes #0 = { norecurse nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
!2 = !{!3, !3, i64 0}
!3 = !{!"long", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
