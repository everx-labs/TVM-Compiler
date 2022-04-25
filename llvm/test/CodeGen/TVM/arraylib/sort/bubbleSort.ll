; RUN: llc < %s -march=tvm 
; ModuleID = 'bubbleSort.c'
source_filename = "bubbleSort.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind
define dso_local void @bubbleSort(i257* nocapture %arr, i257 %size) local_unnamed_addr #0 {
entry:
  %sub = add nsw i257 %size, -1
  %cmp35 = icmp sgt i257 %size, 1
  br i1 %cmp35, label %for.cond1.preheader.us, label %for.end15

for.cond1.preheader.us:                           ; preds = %entry, %for.cond1.for.inc13_crit_edge.us
  %i.036.us = phi i257 [ %inc14.us, %for.cond1.for.inc13_crit_edge.us ], [ 0, %entry ]
  %.pre = load i257, i257* %arr, align 1, !tbaa !2
  br label %for.body4.us

for.body4.us:                                     ; preds = %for.cond1.preheader.us, %for.inc.us
  %0 = phi i257 [ %.pre, %for.cond1.preheader.us ], [ %2, %for.inc.us ]
  %j.034.us = phi i257 [ 0, %for.cond1.preheader.us ], [ %add.us, %for.inc.us ]
  %add.us = add nuw nsw i257 %j.034.us, 1
  %arrayidx.us = getelementptr inbounds i257, i257* %arr, i257 %add.us
  %1 = load i257, i257* %arrayidx.us, align 1, !tbaa !2
  %cmp6.us = icmp slt i257 %1, %0
  br i1 %cmp6.us, label %if.then.us, label %for.inc.us

if.then.us:                                       ; preds = %for.body4.us
  %arrayidx5.us = getelementptr inbounds i257, i257* %arr, i257 %j.034.us
  store i257 %0, i257* %arrayidx.us, align 1, !tbaa !2
  store i257 %1, i257* %arrayidx5.us, align 1, !tbaa !2
  br label %for.inc.us

for.inc.us:                                       ; preds = %if.then.us, %for.body4.us
  %2 = phi i257 [ %0, %if.then.us ], [ %1, %for.body4.us ]
  %cmp3.us = icmp slt i257 %add.us, %sub
  br i1 %cmp3.us, label %for.body4.us, label %for.cond1.for.inc13_crit_edge.us

for.cond1.for.inc13_crit_edge.us:                 ; preds = %for.inc.us
  %inc14.us = add nuw nsw i257 %i.036.us, 1
  %cmp.us = icmp slt i257 %inc14.us, %sub
  br i1 %cmp.us, label %for.cond1.preheader.us, label %for.end15

for.end15:                                        ; preds = %for.cond1.for.inc13_crit_edge.us, %entry
  ret void
}

attributes #0 = { norecurse nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
!2 = !{!3, !3, i64 0}
!3 = !{!"long", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
