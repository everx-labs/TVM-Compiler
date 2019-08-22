; RUN: llc < %s -march=tvm 
; ModuleID = 'bSearchR.c'
source_filename = "bSearchR.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: nounwind readonly
define dso_local i257 @bSearchR(i257* readonly %arr, i257 %start, i257 %end, i257 %key) local_unnamed_addr #0 {
entry:
  %cmp2729 = icmp sgt i257 %start, %end
  br i1 %cmp2729, label %cleanup, label %if.then.lr.ph

if.then.lr.ph:                                    ; preds = %entry, %if.then5
  %end.tr.ph31 = phi i257 [ %end.tr28, %if.then5 ], [ %end, %entry ]
  %start.tr.ph30 = phi i257 [ %add6, %if.then5 ], [ %start, %entry ]
  br label %if.then

if.then:                                          ; preds = %if.then2, %if.then.lr.ph
  %end.tr28 = phi i257 [ %end.tr.ph31, %if.then.lr.ph ], [ %sub, %if.then2 ]
  %add = add nsw i257 %end.tr28, %start.tr.ph30
  %div = sdiv i257 %add, 2
  %arrayidx = getelementptr inbounds i257, i257* %arr, i257 %div
  %0 = load i257, i257* %arrayidx, align 1, !tbaa !2
  %cmp1 = icmp sgt i257 %0, %key
  br i1 %cmp1, label %if.then2, label %if.end

if.then2:                                         ; preds = %if.then
  %sub = add nsw i257 %div, -1
  %cmp = icmp slt i257 %start.tr.ph30, %div
  br i1 %cmp, label %if.then, label %cleanup

if.end:                                           ; preds = %if.then
  %cmp4 = icmp slt i257 %0, %key
  br i1 %cmp4, label %if.then5, label %cleanup

if.then5:                                         ; preds = %if.end
  %add6 = add nsw i257 %div, 1
  %cmp27 = icmp slt i257 %div, %end.tr28
  br i1 %cmp27, label %if.then.lr.ph, label %cleanup

cleanup:                                          ; preds = %if.then5, %if.end, %if.then2, %entry
  %retval.0 = phi i257 [ -1, %entry ], [ -1, %if.then2 ], [ -1, %if.then5 ], [ %div, %if.end ]
  ret i257 %retval.0
}

attributes #0 = { nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
!2 = !{!3, !3, i64 0}
!3 = !{!"long", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
