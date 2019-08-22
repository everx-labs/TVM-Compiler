; RUN: llc < %s -march=tvm 
; ModuleID = 'gnomeSort.c'
source_filename = "gnomeSort.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind
define dso_local void @gnome_sort(i257* nocapture %A, i257 %N) local_unnamed_addr #0 {
entry:
  %cmp29 = icmp sgt i257 %N, 1
  br i1 %cmp29, label %for.body, label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.inc, %entry
  ret void

for.body:                                         ; preds = %entry, %for.inc
  %add31 = phi i257 [ %add, %for.inc ], [ 1, %entry ]
  %i.030 = phi i257 [ %inc, %for.inc ], [ 0, %entry ]
  %arrayidx = getelementptr inbounds i257, i257* %A, i257 %i.030
  %0 = load i257, i257* %arrayidx, align 1, !tbaa !2
  %arrayidx2 = getelementptr inbounds i257, i257* %A, i257 %add31
  %1 = load i257, i257* %arrayidx2, align 1, !tbaa !2
  %cmp3 = icmp sgt i257 %0, %1
  br i1 %cmp3, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body
  store i257 %1, i257* %arrayidx, align 1, !tbaa !2
  store i257 %0, i257* %arrayidx2, align 1, !tbaa !2
  %cmp10 = icmp eq i257 %i.030, 0
  %sub = add nsw i257 %i.030, -2
  %spec.select = select i1 %cmp10, i257 0, i257 %sub
  br label %for.inc

for.inc:                                          ; preds = %for.body, %if.then
  %i.2 = phi i257 [ %spec.select, %if.then ], [ %i.030, %for.body ]
  %inc = add nsw i257 %i.2, 1
  %add = add nsw i257 %i.2, 2
  %cmp = icmp slt i257 %add, %N
  br i1 %cmp, label %for.body, label %for.cond.cleanup
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
