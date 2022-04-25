; RUN: llc < %s -march=tvm 
; ModuleID = 'shakerSort.c'
source_filename = "shakerSort.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind
define dso_local void @shaker_sort(i257* nocapture %array, i257 %size) local_unnamed_addr #0 {
entry:
  %cmp77 = icmp sgt i257 %size, 1
  br i1 %cmp77, label %for.cond1.preheader.preheader, label %for.cond.cleanup

for.cond1.preheader.preheader:                    ; preds = %entry
  %sub = add nsw i257 %size, -1
  br label %for.body4.preheader

for.body4.preheader:                              ; preds = %for.cond.cleanup16, %for.cond1.preheader.preheader
  %left_idx.079 = phi i257 [ %inc34, %for.cond.cleanup16 ], [ 0, %for.cond1.preheader.preheader ]
  %right_idx.078 = phi i257 [ %dec, %for.cond.cleanup16 ], [ %sub, %for.cond1.preheader.preheader ]
  %arrayidx5.phi.trans.insert = getelementptr inbounds i257, i257* %array, i257 %left_idx.079
  %.pre = load i257, i257* %arrayidx5.phi.trans.insert, align 1, !tbaa !2
  br label %for.body4

for.cond.cleanup:                                 ; preds = %for.cond.cleanup16, %entry
  ret void

for.cond.cleanup3:                                ; preds = %for.inc
  %dec = add nsw i257 %right_idx.078, -1
  %cmp1575 = icmp sgt i257 %dec, %left_idx.079
  br i1 %cmp1575, label %for.body17.preheader, label %for.cond.cleanup16

for.body17.preheader:                             ; preds = %for.cond.cleanup3
  %arrayidx20.phi.trans.insert = getelementptr inbounds i257, i257* %array, i257 %dec
  %.pre80 = load i257, i257* %arrayidx20.phi.trans.insert, align 1, !tbaa !2
  br label %for.body17

for.body4:                                        ; preds = %for.body4.preheader, %for.inc
  %0 = phi i257 [ %2, %for.inc ], [ %.pre, %for.body4.preheader ]
  %idx.074 = phi i257 [ %add, %for.inc ], [ %left_idx.079, %for.body4.preheader ]
  %add = add nuw nsw i257 %idx.074, 1
  %arrayidx = getelementptr inbounds i257, i257* %array, i257 %add
  %1 = load i257, i257* %arrayidx, align 1, !tbaa !2
  %cmp6 = icmp slt i257 %1, %0
  br i1 %cmp6, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body4
  %arrayidx5 = getelementptr inbounds i257, i257* %array, i257 %idx.074
  store i257 %1, i257* %arrayidx5, align 1, !tbaa !2
  store i257 %0, i257* %arrayidx, align 1, !tbaa !2
  br label %for.inc

for.inc:                                          ; preds = %for.body4, %if.then
  %2 = phi i257 [ %1, %for.body4 ], [ %0, %if.then ]
  %cmp2 = icmp slt i257 %add, %right_idx.078
  br i1 %cmp2, label %for.body4, label %for.cond.cleanup3

for.cond.cleanup16:                               ; preds = %for.inc31, %for.cond.cleanup3
  %inc34 = add nuw nsw i257 %left_idx.079, 1
  %cmp = icmp slt i257 %inc34, %dec
  br i1 %cmp, label %for.body4.preheader, label %for.cond.cleanup

for.body17:                                       ; preds = %for.body17.preheader, %for.inc31
  %3 = phi i257 [ %5, %for.inc31 ], [ %.pre80, %for.body17.preheader ]
  %idx13.076 = phi i257 [ %sub18, %for.inc31 ], [ %dec, %for.body17.preheader ]
  %sub18 = add nsw i257 %idx13.076, -1
  %arrayidx19 = getelementptr inbounds i257, i257* %array, i257 %sub18
  %4 = load i257, i257* %arrayidx19, align 1, !tbaa !2
  %cmp21 = icmp sgt i257 %4, %3
  br i1 %cmp21, label %if.then22, label %for.inc31

if.then22:                                        ; preds = %for.body17
  %arrayidx20 = getelementptr inbounds i257, i257* %array, i257 %idx13.076
  store i257 %3, i257* %arrayidx19, align 1, !tbaa !2
  store i257 %4, i257* %arrayidx20, align 1, !tbaa !2
  br label %for.inc31

for.inc31:                                        ; preds = %for.body17, %if.then22
  %5 = phi i257 [ %4, %for.body17 ], [ %3, %if.then22 ]
  %cmp15 = icmp sgt i257 %sub18, %left_idx.079
  br i1 %cmp15, label %for.body17, label %for.cond.cleanup16
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
