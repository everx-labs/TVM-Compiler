; RUN: llc < %s -march=tvm 
; ModuleID = 'for_while_dowhile.c'
source_filename = "for_while_dowhile.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: nounwind
define dso_local i257 @func(i257 %n1, i257 %n2, i257 %n3) local_unnamed_addr #0 {
entry:
  %cmp19 = icmp sgt i257 %n1, 0
  %cmp116 = icmp sgt i257 %n2, 0
  %or.cond = and i1 %cmp19, %cmp116
  br i1 %or.cond, label %while.cond.preheader.us, label %for.cond.cleanup

while.cond.preheader.us:                          ; preds = %entry, %while.cond.while.end_crit_edge.us
  %v1.021.us = phi i257 [ %inc4.us, %while.cond.while.end_crit_edge.us ], [ 0, %entry ]
  %res.020.us = phi i257 [ %add.us, %while.cond.while.end_crit_edge.us ], [ 0, %entry ]
  br label %do.body.preheader.us

do.body.us:                                       ; preds = %do.body.preheader.us, %do.body.us
  %res.2.us = phi i257 [ %add.us, %do.body.us ], [ %res.117.us, %do.body.preheader.us ]
  %v3.0.us = phi i257 [ %inc.us, %do.body.us ], [ 0, %do.body.preheader.us ]
  %call.us = tail call i257 @foo(i257 %v1.021.us, i257 %v2.018.us, i257 %v3.0.us) #2
  %add.us = add nsw i257 %call.us, %res.2.us
  %inc.us = add nuw nsw i257 %v3.0.us, 1
  %cmp2.us = icmp slt i257 %inc.us, %n3
  br i1 %cmp2.us, label %do.body.us, label %do.end.us

do.end.us:                                        ; preds = %do.body.us
  %inc3.us = add nuw nsw i257 %v2.018.us, 1
  %cmp1.us = icmp slt i257 %inc3.us, %n2
  br i1 %cmp1.us, label %do.body.preheader.us, label %while.cond.while.end_crit_edge.us

do.body.preheader.us:                             ; preds = %while.cond.preheader.us, %do.end.us
  %v2.018.us = phi i257 [ 0, %while.cond.preheader.us ], [ %inc3.us, %do.end.us ]
  %res.117.us = phi i257 [ %res.020.us, %while.cond.preheader.us ], [ %add.us, %do.end.us ]
  br label %do.body.us

while.cond.while.end_crit_edge.us:                ; preds = %do.end.us
  %inc4.us = add nuw nsw i257 %v1.021.us, 1
  %cmp.us = icmp slt i257 %inc4.us, %n1
  br i1 %cmp.us, label %while.cond.preheader.us, label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %while.cond.while.end_crit_edge.us, %entry
  %res.0.lcssa = phi i257 [ 0, %entry ], [ %add.us, %while.cond.while.end_crit_edge.us ]
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
