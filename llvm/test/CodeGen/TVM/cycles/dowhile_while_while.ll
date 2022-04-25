; RUN: llc < %s -march=tvm 
; ModuleID = 'dowhile_while_while.c'
source_filename = "dowhile_while_while.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: nounwind
define dso_local i257 @func(i257 %n1, i257 %n2, i257 %n3) local_unnamed_addr #0 {
entry:
  %cmp22 = icmp sgt i257 %n2, 0
  %cmp219 = icmp sgt i257 %n3, 0
  br i1 %cmp22, label %do.body.us, label %do.end

do.body.us:                                       ; preds = %entry, %while.cond.while.end5_crit_edge.us
  %res.0.us = phi i257 [ %split25.us, %while.cond.while.end5_crit_edge.us ], [ 0, %entry ]
  %v1.0.us = phi i257 [ %inc6.us, %while.cond.while.end5_crit_edge.us ], [ 0, %entry ]
  br i1 %cmp219, label %while.cond1.preheader.us.us, label %while.cond.while.end5_crit_edge.us

while.cond.while.end5_crit_edge.us:               ; preds = %while.cond1.while.end_crit_edge.us.us, %do.body.us
  %split25.us = phi i257 [ %res.0.us, %do.body.us ], [ %add.us.us, %while.cond1.while.end_crit_edge.us.us ]
  %inc6.us = add nuw nsw i257 %v1.0.us, 1
  %cmp7.us = icmp slt i257 %inc6.us, %n1
  br i1 %cmp7.us, label %do.body.us, label %do.end

while.cond1.preheader.us.us:                      ; preds = %do.body.us, %while.cond1.while.end_crit_edge.us.us
  %v2.024.us.us = phi i257 [ %inc4.us.us, %while.cond1.while.end_crit_edge.us.us ], [ 0, %do.body.us ]
  %res.123.us.us = phi i257 [ %add.us.us, %while.cond1.while.end_crit_edge.us.us ], [ %res.0.us, %do.body.us ]
  br label %while.body3.us.us

while.cond1.while.end_crit_edge.us.us:            ; preds = %while.body3.us.us
  %inc4.us.us = add nuw nsw i257 %v2.024.us.us, 1
  %cmp.us.us = icmp slt i257 %inc4.us.us, %n2
  br i1 %cmp.us.us, label %while.cond1.preheader.us.us, label %while.cond.while.end5_crit_edge.us

while.body3.us.us:                                ; preds = %while.body3.us.us, %while.cond1.preheader.us.us
  %v3.021.us.us = phi i257 [ 0, %while.cond1.preheader.us.us ], [ %inc.us.us, %while.body3.us.us ]
  %res.220.us.us = phi i257 [ %res.123.us.us, %while.cond1.preheader.us.us ], [ %add.us.us, %while.body3.us.us ]
  %call.us.us = tail call i257 @foo(i257 %v1.0.us, i257 %v2.024.us.us, i257 %v3.021.us.us) #2
  %add.us.us = add nsw i257 %call.us.us, %res.220.us.us
  %inc.us.us = add nuw nsw i257 %v3.021.us.us, 1
  %cmp2.us.us = icmp slt i257 %inc.us.us, %n3
  br i1 %cmp2.us.us, label %while.body3.us.us, label %while.cond1.while.end_crit_edge.us.us

do.end:                                           ; preds = %while.cond.while.end5_crit_edge.us, %entry
  %res.1.lcssa.lcssa = phi i257 [ 0, %entry ], [ %split25.us, %while.cond.while.end5_crit_edge.us ]
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
