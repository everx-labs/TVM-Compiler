; RUN: llc < %s -march=tvm 
; ModuleID = 'for_dowhile_while.c'
source_filename = "for_dowhile_while.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: nounwind
define dso_local i257 @func(i257 %n1, i257 %n2, i257 %n3) local_unnamed_addr #0 {
entry:
  %cmp19 = icmp sgt i257 %n1, 0
  %cmp116 = icmp sgt i257 %n3, 0
  %or.cond = and i1 %cmp19, %cmp116
  br i1 %or.cond, label %do.body.preheader.us, label %for.cond.cleanup

do.body.preheader.us:                             ; preds = %entry, %do.end.us-lcssa.us.us
  %v1.021.us = phi i257 [ %inc4.us, %do.end.us-lcssa.us.us ], [ 0, %entry ]
  %res.020.us = phi i257 [ %add.us.us, %do.end.us-lcssa.us.us ], [ 0, %entry ]
  br label %do.body.us.us

do.body.us.us:                                    ; preds = %do.body.preheader.us, %while.cond.while.end_crit_edge.us.us
  %res.1.us.us = phi i257 [ %add.us.us, %while.cond.while.end_crit_edge.us.us ], [ %res.020.us, %do.body.preheader.us ]
  %v2.0.us.us = phi i257 [ %inc2.us.us, %while.cond.while.end_crit_edge.us.us ], [ 0, %do.body.preheader.us ]
  br label %while.body.us.us

while.cond.while.end_crit_edge.us.us:             ; preds = %while.body.us.us
  %inc2.us.us = add nuw nsw i257 %v2.0.us.us, 1
  %cmp3.us.us = icmp slt i257 %inc2.us.us, %n2
  br i1 %cmp3.us.us, label %do.body.us.us, label %do.end.us-lcssa.us.us

while.body.us.us:                                 ; preds = %while.body.us.us, %do.body.us.us
  %v3.018.us.us = phi i257 [ 0, %do.body.us.us ], [ %inc.us.us, %while.body.us.us ]
  %res.217.us.us = phi i257 [ %res.1.us.us, %do.body.us.us ], [ %add.us.us, %while.body.us.us ]
  %call.us.us = tail call i257 @foo(i257 %v1.021.us, i257 %v2.0.us.us, i257 %v3.018.us.us) #2
  %add.us.us = add nsw i257 %call.us.us, %res.217.us.us
  %inc.us.us = add nuw nsw i257 %v3.018.us.us, 1
  %cmp1.us.us = icmp slt i257 %inc.us.us, %n3
  br i1 %cmp1.us.us, label %while.body.us.us, label %while.cond.while.end_crit_edge.us.us

do.end.us-lcssa.us.us:                            ; preds = %while.cond.while.end_crit_edge.us.us
  %inc4.us = add nuw nsw i257 %v1.021.us, 1
  %cmp.us = icmp slt i257 %inc4.us, %n1
  br i1 %cmp.us, label %do.body.preheader.us, label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %do.end.us-lcssa.us.us, %entry
  %res.0.lcssa = phi i257 [ 0, %entry ], [ %add.us.us, %do.end.us-lcssa.us.us ]
  ret i257 %res.0.lcssa
}

declare dso_local i257 @foo(i257, i257, i257) local_unnamed_addr #1

attributes #0 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
