; RUN: llc < %s -march=tvm 
; ModuleID = 'bSearch.c'
source_filename = "bSearch.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readonly
define dso_local i257 @bSearch(i257* nocapture readonly %arr, i257 %size, i257 %key) local_unnamed_addr #0 {
entry:
  %cmp19 = icmp slt i257 %size, 0
  br i1 %cmp19, label %cleanup, label %while.body

while.body:                                       ; preds = %entry, %if.end
  %end.021 = phi i257 [ %end.1, %if.end ], [ %size, %entry ]
  %start.020 = phi i257 [ %start.1, %if.end ], [ 0, %entry ]
  %add = add nsw i257 %end.021, %start.020
  %div = sdiv i257 %add, 2
  %arrayidx = getelementptr inbounds i257, i257* %arr, i257 %div
  %0 = load i257, i257* %arrayidx, align 1, !tbaa !2
  %cmp1 = icmp eq i257 %0, %key
  br i1 %cmp1, label %cleanup, label %if.end

if.end:                                           ; preds = %while.body
  %cmp3 = icmp sgt i257 %0, %key
  %sub = add nsw i257 %div, -1
  %add5 = add nsw i257 %div, 1
  %start.1 = select i1 %cmp3, i257 %start.020, i257 %add5
  %end.1 = select i1 %cmp3, i257 %sub, i257 %end.021
  %cmp = icmp sgt i257 %start.1, %end.1
  br i1 %cmp, label %cleanup, label %while.body

cleanup:                                          ; preds = %while.body, %if.end, %entry
  %retval.0 = phi i257 [ -1, %entry ], [ -1, %if.end ], [ %div, %while.body ]
  ret i257 %retval.0
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
