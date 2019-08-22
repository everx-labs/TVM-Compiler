; RUN: llc < %s -march=tvm 
; ModuleID = 'while_while_for.c'
source_filename = "while_while_for.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: nounwind
define dso_local i257 @func(i257 %n1, i257 %n2, i257 %n3) local_unnamed_addr #0 {
entry:
  %cmp26 = icmp sgt i257 %n1, 0
  br i1 %cmp26, label %while.cond1.preheader.lr.ph, label %while.end7

while.cond1.preheader.lr.ph:                      ; preds = %entry
  %cmp222 = icmp sgt i257 %n2, 0
  %cmp419 = icmp sgt i257 %n3, 0
  br i1 %cmp222, label %while.cond1.preheader.us, label %while.end7

while.cond1.preheader.us:                         ; preds = %while.cond1.preheader.lr.ph, %while.cond1.while.end_crit_edge.us
  %v1.028.us = phi i257 [ %inc6.us, %while.cond1.while.end_crit_edge.us ], [ 0, %while.cond1.preheader.lr.ph ]
  %res.027.us = phi i257 [ %split25.us, %while.cond1.while.end_crit_edge.us ], [ 0, %while.cond1.preheader.lr.ph ]
  br i1 %cmp419, label %for.cond.preheader.us.us, label %while.cond1.while.end_crit_edge.us

while.cond1.while.end_crit_edge.us:               ; preds = %for.cond.for.cond.cleanup_crit_edge.us.us, %while.cond1.preheader.us
  %split25.us = phi i257 [ %res.027.us, %while.cond1.preheader.us ], [ %add.us.us, %for.cond.for.cond.cleanup_crit_edge.us.us ]
  %inc6.us = add nuw nsw i257 %v1.028.us, 1
  %cmp.us = icmp slt i257 %inc6.us, %n1
  br i1 %cmp.us, label %while.cond1.preheader.us, label %while.end7

for.cond.preheader.us.us:                         ; preds = %while.cond1.preheader.us, %for.cond.for.cond.cleanup_crit_edge.us.us
  %v2.024.us.us = phi i257 [ %inc5.us.us, %for.cond.for.cond.cleanup_crit_edge.us.us ], [ 0, %while.cond1.preheader.us ]
  %res.123.us.us = phi i257 [ %add.us.us, %for.cond.for.cond.cleanup_crit_edge.us.us ], [ %res.027.us, %while.cond1.preheader.us ]
  br label %for.body.us.us

for.cond.for.cond.cleanup_crit_edge.us.us:        ; preds = %for.body.us.us
  %inc5.us.us = add nuw nsw i257 %v2.024.us.us, 1
  %cmp2.us.us = icmp slt i257 %inc5.us.us, %n2
  br i1 %cmp2.us.us, label %for.cond.preheader.us.us, label %while.cond1.while.end_crit_edge.us

for.body.us.us:                                   ; preds = %for.body.us.us, %for.cond.preheader.us.us
  %v3.021.us.us = phi i257 [ 0, %for.cond.preheader.us.us ], [ %inc.us.us, %for.body.us.us ]
  %res.220.us.us = phi i257 [ %res.123.us.us, %for.cond.preheader.us.us ], [ %add.us.us, %for.body.us.us ]
  %call.us.us = tail call i257 @foo(i257 %v1.028.us, i257 %v2.024.us.us, i257 %v3.021.us.us) #2
  %add.us.us = add nsw i257 %call.us.us, %res.220.us.us
  %inc.us.us = add nuw nsw i257 %v3.021.us.us, 1
  %cmp4.us.us = icmp slt i257 %inc.us.us, %n3
  br i1 %cmp4.us.us, label %for.body.us.us, label %for.cond.for.cond.cleanup_crit_edge.us.us

while.end7:                                       ; preds = %while.cond1.while.end_crit_edge.us, %while.cond1.preheader.lr.ph, %entry
  %res.0.lcssa = phi i257 [ 0, %entry ], [ 0, %while.cond1.preheader.lr.ph ], [ %split25.us, %while.cond1.while.end_crit_edge.us ]
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
