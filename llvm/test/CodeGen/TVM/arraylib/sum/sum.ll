; RUN: llc < %s -march=tvm 
; ModuleID = 'sum.c'
source_filename = "sum.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readonly
define dso_local i257 @sum(i257* nocapture readonly %arr, i257 %len) local_unnamed_addr #0 {
entry:
  %cmp6 = icmp sgt i257 %len, 0
  br i1 %cmp6, label %for.body, label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.body, %entry
  %res.0.lcssa = phi i257 [ 0, %entry ], [ %add, %for.body ]
  ret i257 %res.0.lcssa

for.body:                                         ; preds = %entry, %for.body
  %i.08 = phi i257 [ %inc, %for.body ], [ 0, %entry ]
  %res.07 = phi i257 [ %add, %for.body ], [ 0, %entry ]
  %arrayidx = getelementptr inbounds i257, i257* %arr, i257 %i.08
  %0 = load i257, i257* %arrayidx, align 1, !tbaa !2
  %add = add nsw i257 %0, %res.07
  %inc = add nuw nsw i257 %i.08, 1
  %cmp = icmp slt i257 %inc, %len
  br i1 %cmp, label %for.body, label %for.cond.cleanup
}

; Function Attrs: nounwind readonly
define dso_local i257 @sumR(i257* nocapture %arr, i257 %len) local_unnamed_addr #1 {
entry:
  %cmp5 = icmp slt i257 %len, 1
  br i1 %cmp5, label %return, label %if.end

if.end:                                           ; preds = %entry, %if.end
  %len.tr7 = phi i257 [ %sub, %if.end ], [ %len, %entry ]
  %accumulator.tr6 = phi i257 [ %add, %if.end ], [ 0, %entry ]
  %sub = add nsw i257 %len.tr7, -1
  %arrayidx = getelementptr inbounds i257, i257* %arr, i257 %sub
  %0 = load i257, i257* %arrayidx, align 1, !tbaa !2
  %add = add nsw i257 %0, %accumulator.tr6
  %cmp = icmp slt i257 %len.tr7, 2
  br i1 %cmp, label %return, label %if.end

return:                                           ; preds = %if.end, %entry
  %accumulator.tr.lcssa = phi i257 [ 0, %entry ], [ %add, %if.end ]
  ret i257 %accumulator.tr.lcssa
}

attributes #0 = { norecurse nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
!2 = !{!3, !3, i64 0}
!3 = !{!"long", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
