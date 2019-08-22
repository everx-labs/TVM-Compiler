; RUN: llc < %s -march=tvm 
; ModuleID = 'search.c'
source_filename = "search.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readonly
define dso_local i257 @search(i257 %key, i257* nocapture readonly %arr, i257 %len) local_unnamed_addr #0 {
entry:
  %cmp5 = icmp sgt i257 %len, 0
  br i1 %cmp5, label %for.body, label %cleanup

for.body:                                         ; preds = %entry, %for.inc
  %i.06 = phi i257 [ %inc, %for.inc ], [ 0, %entry ]
  %arrayidx = getelementptr inbounds i257, i257* %arr, i257 %i.06
  %0 = load i257, i257* %arrayidx, align 1, !tbaa !2
  %cmp1 = icmp eq i257 %0, %key
  br i1 %cmp1, label %cleanup, label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nuw nsw i257 %i.06, 1
  %cmp = icmp slt i257 %inc, %len
  br i1 %cmp, label %for.body, label %cleanup

cleanup:                                          ; preds = %for.body, %for.inc, %entry
  %1 = phi i257 [ -1, %entry ], [ -1, %for.inc ], [ %i.06, %for.body ]
  ret i257 %1
}

attributes #0 = { norecurse nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
!2 = !{!3, !3, i64 0}
!3 = !{!"long", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
