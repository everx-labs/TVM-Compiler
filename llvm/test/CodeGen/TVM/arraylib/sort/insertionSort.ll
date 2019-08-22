; RUN: llc < %s -march=tvm 
; ModuleID = 'insertionSort.c'
source_filename = "insertionSort.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind
define dso_local void @insertionSort(i257* nocapture %a, i257 %size) local_unnamed_addr #0 {
entry:
  %cmp31 = icmp sgt i257 %size, 1
  br i1 %cmp31, label %for.body, label %for.end11

for.body:                                         ; preds = %entry, %for.end
  %i.032 = phi i257 [ %inc, %for.end ], [ 1, %entry ]
  %arrayidx = getelementptr inbounds i257, i257* %a, i257 %i.032
  %0 = load i257, i257* %arrayidx, align 1, !tbaa !2
  br label %land.rhs

land.rhs:                                         ; preds = %for.body, %for.body5
  %j.030.in = phi i257 [ %i.032, %for.body ], [ %j.030, %for.body5 ]
  %j.030 = add nsw i257 %j.030.in, -1
  %arrayidx3 = getelementptr inbounds i257, i257* %a, i257 %j.030
  %1 = load i257, i257* %arrayidx3, align 1, !tbaa !2
  %cmp4 = icmp sgt i257 %1, %0
  br i1 %cmp4, label %for.body5, label %for.end

for.body5:                                        ; preds = %land.rhs
  %arrayidx7 = getelementptr inbounds i257, i257* %a, i257 %j.030.in
  store i257 %1, i257* %arrayidx7, align 1, !tbaa !2
  %cmp2 = icmp sgt i257 %j.030.in, 1
  br i1 %cmp2, label %land.rhs, label %for.end

for.end:                                          ; preds = %for.body5, %land.rhs
  %j.030.in.pn = phi i257 [ %j.030.in, %land.rhs ], [ %j.030, %for.body5 ]
  %arrayidx9.pre-phi = getelementptr inbounds i257, i257* %a, i257 %j.030.in.pn
  store i257 %0, i257* %arrayidx9.pre-phi, align 1, !tbaa !2
  %inc = add nuw nsw i257 %i.032, 1
  %cmp = icmp slt i257 %inc, %size
  br i1 %cmp, label %for.body, label %for.end11

for.end11:                                        ; preds = %for.end, %entry
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
