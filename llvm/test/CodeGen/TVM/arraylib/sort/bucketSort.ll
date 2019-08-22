; XFAIL: *
; RUN: llc < %s -march=tvm 
; ModuleID = 'bucketSort.c'
source_filename = "bucketSort.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind
define dso_local void @bucketSort(i257* nocapture %array, i257 %n) local_unnamed_addr #0 {
entry:
  %vla = alloca i257, i257 %n, align 1
  %cmp47 = icmp sgt i257 %n, 0
  br i1 %cmp47, label %for.body, label %for.end24

for.cond1.preheader:                              ; preds = %for.body
  br i1 %cmp47, label %for.body3, label %for.end24

for.body:                                         ; preds = %entry, %for.body
  %i.048 = phi i257 [ %inc, %for.body ], [ 0, %entry ]
  %arrayidx = getelementptr inbounds i257, i257* %vla, i257 %i.048
  store i257 0, i257* %arrayidx, align 1, !tbaa !2
  %inc = add nuw nsw i257 %i.048, 1
  %cmp = icmp slt i257 %inc, %n
  br i1 %cmp, label %for.body, label %for.cond1.preheader

for.cond10.preheader:                             ; preds = %for.body3
  br i1 %cmp47, label %for.cond13.preheader, label %for.end24

for.body3:                                        ; preds = %for.cond1.preheader, %for.body3
  %i.146 = phi i257 [ %inc8, %for.body3 ], [ 0, %for.cond1.preheader ]
  %arrayidx4 = getelementptr inbounds i257, i257* %array, i257 %i.146
  %0 = load i257, i257* %arrayidx4, align 1, !tbaa !2
  %arrayidx5 = getelementptr inbounds i257, i257* %vla, i257 %0
  %1 = load i257, i257* %arrayidx5, align 1, !tbaa !2
  %inc6 = add nsw i257 %1, 1
  store i257 %inc6, i257* %arrayidx5, align 1, !tbaa !2
  %inc8 = add nuw nsw i257 %i.146, 1
  %cmp2 = icmp slt i257 %inc8, %n
  br i1 %cmp2, label %for.body3, label %for.cond10.preheader

for.cond13.preheader:                             ; preds = %for.cond10.preheader, %for.inc22
  %i.244 = phi i257 [ %inc23, %for.inc22 ], [ 0, %for.cond10.preheader ]
  %j.043 = phi i257 [ %j.1.lcssa, %for.inc22 ], [ 0, %for.cond10.preheader ]
  %arrayidx14 = getelementptr inbounds i257, i257* %vla, i257 %i.244
  %2 = load i257, i257* %arrayidx14, align 1, !tbaa !2
  %cmp1540 = icmp sgt i257 %2, 0
  br i1 %cmp1540, label %for.body16, label %for.inc22

for.body16:                                       ; preds = %for.cond13.preheader, %for.body16
  %3 = phi i257 [ %dec, %for.body16 ], [ %2, %for.cond13.preheader ]
  %j.141 = phi i257 [ %inc17, %for.body16 ], [ %j.043, %for.cond13.preheader ]
  %inc17 = add nsw i257 %j.141, 1
  %arrayidx18 = getelementptr inbounds i257, i257* %array, i257 %j.141
  store i257 %i.244, i257* %arrayidx18, align 1, !tbaa !2
  %dec = add nsw i257 %3, -1
  %cmp15 = icmp sgt i257 %3, 1
  br i1 %cmp15, label %for.body16, label %for.cond13.for.inc22_crit_edge

for.cond13.for.inc22_crit_edge:                   ; preds = %for.body16
  store i257 %dec, i257* %arrayidx14, align 1, !tbaa !2
  br label %for.inc22

for.inc22:                                        ; preds = %for.cond13.for.inc22_crit_edge, %for.cond13.preheader
  %j.1.lcssa = phi i257 [ %inc17, %for.cond13.for.inc22_crit_edge ], [ %j.043, %for.cond13.preheader ]
  %inc23 = add nuw nsw i257 %i.244, 1
  %cmp11 = icmp slt i257 %inc23, %n
  br i1 %cmp11, label %for.cond13.preheader, label %for.end24

for.end24:                                        ; preds = %for.inc22, %entry, %for.cond1.preheader, %for.cond10.preheader
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
