; RUN: llc < %s -march=tvm 
; ModuleID = 'selectSort.c'
source_filename = "selectSort.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind
define dso_local void @selectSort(i257* nocapture %arr, i257 %size) local_unnamed_addr #0 {
entry:
  %cmp39 = icmp sgt i257 %size, 0
  br i1 %cmp39, label %for.body, label %for.end12

for.body:                                         ; preds = %entry, %for.end
  %i.040 = phi i257 [ %add, %for.end ], [ 0, %entry ]
  %arrayidx = getelementptr inbounds i257, i257* %arr, i257 %i.040
  %0 = load i257, i257* %arrayidx, align 1, !tbaa !2
  %add = add nuw nsw i257 %i.040, 1
  %cmp234 = icmp slt i257 %add, %size
  br i1 %cmp234, label %for.body3, label %for.end12

for.body3:                                        ; preds = %for.body, %for.body3
  %pos.037 = phi i257 [ %spec.select33, %for.body3 ], [ %i.040, %for.body ]
  %j.036 = phi i257 [ %inc, %for.body3 ], [ %add, %for.body ]
  %tmp.035 = phi i257 [ %spec.select, %for.body3 ], [ %0, %for.body ]
  %arrayidx4 = getelementptr inbounds i257, i257* %arr, i257 %j.036
  %1 = load i257, i257* %arrayidx4, align 1, !tbaa !2
  %cmp5 = icmp slt i257 %1, %tmp.035
  %spec.select = select i1 %cmp5, i257 %1, i257 %tmp.035
  %spec.select33 = select i1 %cmp5, i257 %j.036, i257 %pos.037
  %inc = add nuw nsw i257 %j.036, 1
  %cmp2 = icmp slt i257 %inc, %size
  br i1 %cmp2, label %for.body3, label %for.end

for.end:                                          ; preds = %for.body3
  %.pre = getelementptr inbounds i257, i257* %arr, i257 %spec.select33
  store i257 %0, i257* %.pre, align 1, !tbaa !2
  store i257 %spec.select, i257* %arrayidx, align 1, !tbaa !2
  br i1 %cmp234, label %for.body, label %for.end12

for.end12:                                        ; preds = %for.end, %for.body, %entry
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
