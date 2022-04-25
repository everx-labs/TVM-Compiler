; RUN: llc < %s -march=tvm 
; ModuleID = 'quickSort.c'
source_filename = "quickSort.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: nounwind
define dso_local void @quickSortR(i257* %a, i257 %N) local_unnamed_addr #0 {
entry:
  br label %tailrecurse

tailrecurse:                                      ; preds = %if.then19, %entry
  %a.tr = phi i257* [ %a, %entry ], [ %add.ptr, %if.then19 ]
  %N.tr = phi i257 [ %N, %entry ], [ %sub, %if.then19 ]
  %shr = ashr i257 %N.tr, 1
  %arrayidx = getelementptr inbounds i257, i257* %a.tr, i257 %shr
  %0 = load i257, i257* %arrayidx, align 1, !tbaa !2
  br label %do.body

do.body:                                          ; preds = %do.cond, %tailrecurse
  %j.0 = phi i257 [ %N.tr, %tailrecurse ], [ %j.2, %do.cond ]
  %i.0 = phi i257 [ 0, %tailrecurse ], [ %i.2, %do.cond ]
  br label %while.cond

while.cond:                                       ; preds = %while.cond, %do.body
  %i.1 = phi i257 [ %i.0, %do.body ], [ %inc, %while.cond ]
  %arrayidx1 = getelementptr inbounds i257, i257* %a.tr, i257 %i.1
  %1 = load i257, i257* %arrayidx1, align 1, !tbaa !2
  %cmp = icmp slt i257 %1, %0
  %inc = add nsw i257 %i.1, 1
  br i1 %cmp, label %while.cond, label %while.cond2.preheader

while.cond2.preheader:                            ; preds = %while.cond
  %arrayidx1.le = getelementptr inbounds i257, i257* %a.tr, i257 %i.1
  br label %while.cond2

while.cond2:                                      ; preds = %while.cond2, %while.cond2.preheader
  %j.1 = phi i257 [ %dec, %while.cond2 ], [ %j.0, %while.cond2.preheader ]
  %arrayidx3 = getelementptr inbounds i257, i257* %a.tr, i257 %j.1
  %2 = load i257, i257* %arrayidx3, align 1, !tbaa !2
  %cmp4 = icmp sgt i257 %2, %0
  %dec = add nsw i257 %j.1, -1
  br i1 %cmp4, label %while.cond2, label %while.end6

while.end6:                                       ; preds = %while.cond2
  %cmp7 = icmp sgt i257 %i.1, %j.1
  br i1 %cmp7, label %do.cond, label %if.then

if.then:                                          ; preds = %while.end6
  %arrayidx3.le = getelementptr inbounds i257, i257* %a.tr, i257 %j.1
  store i257 %2, i257* %arrayidx1.le, align 1, !tbaa !2
  store i257 %1, i257* %arrayidx3.le, align 1, !tbaa !2
  br label %do.cond

do.cond:                                          ; preds = %while.end6, %if.then
  %j.2 = phi i257 [ %dec, %if.then ], [ %j.1, %while.end6 ]
  %i.2 = phi i257 [ %inc, %if.then ], [ %i.1, %while.end6 ]
  %cmp14 = icmp sgt i257 %i.2, %j.2
  br i1 %cmp14, label %do.end, label %do.body

do.end:                                           ; preds = %do.cond
  %cmp15 = icmp sgt i257 %j.2, 0
  br i1 %cmp15, label %if.then16, label %if.end17

if.then16:                                        ; preds = %do.end
  tail call void @quickSortR(i257* nonnull %a.tr, i257 %j.2)
  br label %if.end17

if.end17:                                         ; preds = %if.then16, %do.end
  %cmp18 = icmp sgt i257 %N.tr, %i.2
  br i1 %cmp18, label %if.then19, label %if.end20

if.then19:                                        ; preds = %if.end17
  %add.ptr = getelementptr inbounds i257, i257* %a.tr, i257 %i.2
  %sub = sub nsw i257 %N.tr, %i.2
  br label %tailrecurse

if.end20:                                         ; preds = %if.end17
  ret void
}

attributes #0 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
!2 = !{!3, !3, i64 0}
!3 = !{!"long", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
