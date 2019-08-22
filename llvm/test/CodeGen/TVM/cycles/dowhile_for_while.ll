; RUN: llc < %s -march=tvm 
; ModuleID = 'dowhile_for_while.c'
source_filename = "dowhile_for_while.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: nounwind
define dso_local i257 @func(i257 %n1, i257 %n2, i257 %n3) local_unnamed_addr #0 {
entry:
  %cmp19 = icmp sgt i257 %n2, 0
  %cmp116 = icmp sgt i257 %n3, 0
  br i1 %cmp19, label %do.body.us, label %do.end

do.body.us:                                       ; preds = %entry, %for.cond.for.cond.cleanup_crit_edge.us
  %res.0.us = phi i257 [ %split22.us, %for.cond.for.cond.cleanup_crit_edge.us ], [ 0, %entry ]
  %v1.0.us = phi i257 [ %inc3.us, %for.cond.for.cond.cleanup_crit_edge.us ], [ 0, %entry ]
  br i1 %cmp116, label %while.cond.preheader.us.us, label %for.cond.for.cond.cleanup_crit_edge.us

for.cond.for.cond.cleanup_crit_edge.us:           ; preds = %while.cond.while.end_crit_edge.us.us, %do.body.us
  %split22.us = phi i257 [ %res.0.us, %do.body.us ], [ %add.us.us, %while.cond.while.end_crit_edge.us.us ]
  %inc3.us = add nuw nsw i257 %v1.0.us, 1
  %cmp4.us = icmp slt i257 %inc3.us, %n1
  br i1 %cmp4.us, label %do.body.us, label %do.end

while.cond.preheader.us.us:                       ; preds = %do.body.us, %while.cond.while.end_crit_edge.us.us
  %v2.021.us.us = phi i257 [ %inc2.us.us, %while.cond.while.end_crit_edge.us.us ], [ 0, %do.body.us ]
  %res.120.us.us = phi i257 [ %add.us.us, %while.cond.while.end_crit_edge.us.us ], [ %res.0.us, %do.body.us ]
  br label %while.body.us.us

while.cond.while.end_crit_edge.us.us:             ; preds = %while.body.us.us
  %inc2.us.us = add nuw nsw i257 %v2.021.us.us, 1
  %cmp.us.us = icmp slt i257 %inc2.us.us, %n2
  br i1 %cmp.us.us, label %while.cond.preheader.us.us, label %for.cond.for.cond.cleanup_crit_edge.us

while.body.us.us:                                 ; preds = %while.body.us.us, %while.cond.preheader.us.us
  %v3.018.us.us = phi i257 [ 0, %while.cond.preheader.us.us ], [ %inc.us.us, %while.body.us.us ]
  %res.217.us.us = phi i257 [ %res.120.us.us, %while.cond.preheader.us.us ], [ %add.us.us, %while.body.us.us ]
  %call.us.us = tail call i257 @foo(i257 %v1.0.us, i257 %v2.021.us.us, i257 %v3.018.us.us) #2
  %add.us.us = add nsw i257 %call.us.us, %res.217.us.us
  %inc.us.us = add nuw nsw i257 %v3.018.us.us, 1
  %cmp1.us.us = icmp slt i257 %inc.us.us, %n3
  br i1 %cmp1.us.us, label %while.body.us.us, label %while.cond.while.end_crit_edge.us.us

do.end:                                           ; preds = %for.cond.for.cond.cleanup_crit_edge.us, %entry
  %res.1.lcssa.lcssa = phi i257 [ 0, %entry ], [ %split22.us, %for.cond.for.cond.cleanup_crit_edge.us ]
  ret i257 %res.1.lcssa.lcssa
}

declare dso_local i257 @foo(i257, i257, i257) local_unnamed_addr #1

attributes #0 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
