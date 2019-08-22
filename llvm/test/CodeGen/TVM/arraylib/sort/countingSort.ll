; XFAIL: *
; RUN: llc < %s -march=tvm 
; ModuleID = 'countingSort.c'
source_filename = "countingSort.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind
define dso_local void @countSort(i257* nocapture %arr, i257 %n, i257 %k) local_unnamed_addr #0 {
entry:
  %vla = alloca i257, i257 %n, align 1
  %vla1 = alloca i257, i257 %k, align 1
  %cmp89 = icmp sgt i257 %k, 0
  br i1 %cmp89, label %for.body, label %for.cond4.preheader

for.cond4.preheader:                              ; preds = %for.body, %entry
  %cmp587 = icmp sgt i257 %n, 0
  br i1 %cmp587, label %for.body7, label %for.cond15.preheader

for.body:                                         ; preds = %entry, %for.body
  %i.090 = phi i257 [ %inc, %for.body ], [ 0, %entry ]
  %arrayidx = getelementptr inbounds i257, i257* %vla1, i257 %i.090
  store i257 0, i257* %arrayidx, align 1, !tbaa !2
  %inc = add nuw nsw i257 %i.090, 1
  %cmp = icmp slt i257 %inc, %k
  br i1 %cmp, label %for.body, label %for.cond4.preheader

for.cond15.preheader:                             ; preds = %for.body7, %for.cond4.preheader
  br i1 %cmp89, label %for.body18, label %for.cond25.preheader

for.body7:                                        ; preds = %for.cond4.preheader, %for.body7
  %i3.088 = phi i257 [ %inc12, %for.body7 ], [ 0, %for.cond4.preheader ]
  %arrayidx8 = getelementptr inbounds i257, i257* %arr, i257 %i3.088
  %0 = load i257, i257* %arrayidx8, align 1, !tbaa !2
  %arrayidx9 = getelementptr inbounds i257, i257* %vla1, i257 %0
  %1 = load i257, i257* %arrayidx9, align 1, !tbaa !2
  %inc10 = add nsw i257 %1, 1
  store i257 %inc10, i257* %arrayidx9, align 1, !tbaa !2
  %inc12 = add nuw nsw i257 %i3.088, 1
  %cmp5 = icmp slt i257 %inc12, %n
  br i1 %cmp5, label %for.body7, label %for.cond15.preheader

for.cond25.preheader:                             ; preds = %for.body18, %for.cond15.preheader
  br i1 %cmp587, label %for.body28, label %for.cond.cleanup42

for.body18:                                       ; preds = %for.cond15.preheader, %for.body18
  %i14.086 = phi i257 [ %inc22, %for.body18 ], [ 0, %for.cond15.preheader ]
  %total.085 = phi i257 [ %add, %for.body18 ], [ 0, %for.cond15.preheader ]
  %arrayidx19 = getelementptr inbounds i257, i257* %vla1, i257 %i14.086
  %2 = load i257, i257* %arrayidx19, align 1, !tbaa !2
  store i257 %total.085, i257* %arrayidx19, align 1, !tbaa !2
  %add = add nsw i257 %2, %total.085
  %inc22 = add nuw nsw i257 %i14.086, 1
  %cmp16 = icmp slt i257 %inc22, %k
  br i1 %cmp16, label %for.body18, label %for.cond25.preheader

for.cond40.preheader:                             ; preds = %for.body28
  br i1 %cmp587, label %for.body43, label %for.cond.cleanup42

for.body28:                                       ; preds = %for.cond25.preheader, %for.body28
  %i24.083 = phi i257 [ %inc37, %for.body28 ], [ 0, %for.cond25.preheader ]
  %arrayidx29 = getelementptr inbounds i257, i257* %arr, i257 %i24.083
  %3 = load i257, i257* %arrayidx29, align 1, !tbaa !2
  %arrayidx31 = getelementptr inbounds i257, i257* %vla1, i257 %3
  %4 = load i257, i257* %arrayidx31, align 1, !tbaa !2
  %arrayidx32 = getelementptr inbounds i257, i257* %vla, i257 %4
  store i257 %3, i257* %arrayidx32, align 1, !tbaa !2
  %inc35 = add nsw i257 %4, 1
  store i257 %inc35, i257* %arrayidx31, align 1, !tbaa !2
  %inc37 = add nuw nsw i257 %i24.083, 1
  %cmp26 = icmp slt i257 %inc37, %n
  br i1 %cmp26, label %for.body28, label %for.cond40.preheader

for.cond.cleanup42:                               ; preds = %for.body43, %for.cond25.preheader, %for.cond40.preheader
  ret void

for.body43:                                       ; preds = %for.cond40.preheader, %for.body43
  %i39.081 = phi i257 [ %inc47, %for.body43 ], [ 0, %for.cond40.preheader ]
  %arrayidx44 = getelementptr inbounds i257, i257* %vla, i257 %i39.081
  %5 = load i257, i257* %arrayidx44, align 1, !tbaa !2
  %arrayidx45 = getelementptr inbounds i257, i257* %arr, i257 %i39.081
  store i257 %5, i257* %arrayidx45, align 1, !tbaa !2
  %inc47 = add nuw nsw i257 %i39.081, 1
  %cmp41 = icmp slt i257 %inc47, %n
  br i1 %cmp41, label %for.body43, label %for.cond.cleanup42
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
