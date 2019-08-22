; RUN: llc < %s -march=tvm 
; ModuleID = 'min_max.c'
source_filename = "min_max.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

%struct.result = type { %struct.item, %struct.item }
%struct.item = type { i257, i257 }

@Result = common dso_local global %struct.result zeroinitializer, align 1

; Function Attrs: norecurse nounwind
define dso_local %struct.result* @min_max(i257* nocapture readonly %arr, i257 %len) local_unnamed_addr #0 {
entry:
  %cmp = icmp slt i257 %len, 1
  br i1 %cmp, label %return, label %for.body.preheader

for.body.preheader:                               ; preds = %entry
  %0 = load i257, i257* %arr, align 1, !tbaa !2
  store i257 %0, i257* getelementptr inbounds (%struct.result, %struct.result* @Result, i257 0, i32 0, i32 0), align 1, !tbaa !6
  store i257 0, i257* getelementptr inbounds (%struct.result, %struct.result* @Result, i257 0, i32 0, i32 1), align 1, !tbaa !9
  %1 = load i257, i257* %arr, align 1, !tbaa !2
  store i257 %1, i257* getelementptr inbounds (%struct.result, %struct.result* @Result, i257 0, i32 1, i32 0), align 1, !tbaa !10
  store i257 0, i257* getelementptr inbounds (%struct.result, %struct.result* @Result, i257 0, i32 1, i32 1), align 1, !tbaa !11
  br label %for.body

for.body:                                         ; preds = %for.body.preheader, %for.inc
  %2 = phi i257 [ %7, %for.inc ], [ %0, %for.body.preheader ]
  %3 = phi i257 [ %6, %for.inc ], [ %1, %for.body.preheader ]
  %i.028 = phi i257 [ %inc, %for.inc ], [ 0, %for.body.preheader ]
  %arrayidx3 = getelementptr inbounds i257, i257* %arr, i257 %i.028
  %4 = load i257, i257* %arrayidx3, align 1, !tbaa !2
  %cmp4 = icmp sgt i257 %4, %3
  br i1 %cmp4, label %if.then5, label %if.end7

if.then5:                                         ; preds = %for.body
  store i257 %4, i257* getelementptr inbounds (%struct.result, %struct.result* @Result, i257 0, i32 1, i32 0), align 1, !tbaa !10
  store i257 %i.028, i257* getelementptr inbounds (%struct.result, %struct.result* @Result, i257 0, i32 1, i32 1), align 1, !tbaa !11
  %.pre = load i257, i257* %arrayidx3, align 1, !tbaa !2
  br label %if.end7

if.end7:                                          ; preds = %if.then5, %for.body
  %5 = phi i257 [ %.pre, %if.then5 ], [ %4, %for.body ]
  %6 = phi i257 [ %4, %if.then5 ], [ %3, %for.body ]
  %cmp9 = icmp slt i257 %5, %2
  br i1 %cmp9, label %if.then10, label %for.inc

if.then10:                                        ; preds = %if.end7
  store i257 %5, i257* getelementptr inbounds (%struct.result, %struct.result* @Result, i257 0, i32 0, i32 0), align 1, !tbaa !6
  store i257 %i.028, i257* getelementptr inbounds (%struct.result, %struct.result* @Result, i257 0, i32 0, i32 1), align 1, !tbaa !9
  br label %for.inc

for.inc:                                          ; preds = %if.end7, %if.then10
  %7 = phi i257 [ %2, %if.end7 ], [ %5, %if.then10 ]
  %inc = add nuw nsw i257 %i.028, 1
  %cmp2 = icmp slt i257 %inc, %len
  br i1 %cmp2, label %for.body, label %return

return:                                           ; preds = %for.inc, %entry
  %retval.0 = phi %struct.result* [ null, %entry ], [ @Result, %for.inc ]
  ret %struct.result* %retval.0
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
!6 = !{!7, !3, i64 0}
!7 = !{!"result", !8, i64 0, !8, i64 2}
!8 = !{!"item", !3, i64 0, !3, i64 1}
!9 = !{!7, !3, i64 1}
!10 = !{!7, !3, i64 2}
!11 = !{!7, !3, i64 3}
