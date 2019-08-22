; RUN: llc < %s -march=tvm 
; ModuleID = 'for_for_while.c'
source_filename = "for_for_while.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: nounwind
define dso_local i257 @func(i257 %n1, i257 %n2, i257 %n3) local_unnamed_addr #0 {
entry:
  %cmp28 = icmp sgt i257 %n1, 0
  br i1 %cmp28, label %for.cond1.preheader.lr.ph, label %for.cond.cleanup

for.cond1.preheader.lr.ph:                        ; preds = %entry
  %cmp224 = icmp sgt i257 %n2, 0
  %cmp521 = icmp sgt i257 %n3, 0
  br i1 %cmp224, label %for.cond1.preheader.us, label %for.cond.cleanup

for.cond1.preheader.us:                           ; preds = %for.cond1.preheader.lr.ph, %for.cond1.for.cond.cleanup3_crit_edge.us
  %v1.030.us = phi i257 [ %inc8.us, %for.cond1.for.cond.cleanup3_crit_edge.us ], [ 0, %for.cond1.preheader.lr.ph ]
  %res.029.us = phi i257 [ %split27.us, %for.cond1.for.cond.cleanup3_crit_edge.us ], [ 0, %for.cond1.preheader.lr.ph ]
  br i1 %cmp521, label %while.cond.preheader.us.us, label %for.cond1.for.cond.cleanup3_crit_edge.us

for.cond1.for.cond.cleanup3_crit_edge.us:         ; preds = %while.cond.while.end_crit_edge.us.us, %for.cond1.preheader.us
  %split27.us = phi i257 [ %res.029.us, %for.cond1.preheader.us ], [ %add.us.us, %while.cond.while.end_crit_edge.us.us ]
  %inc8.us = add nuw nsw i257 %v1.030.us, 1
  %cmp.us = icmp slt i257 %inc8.us, %n1
  br i1 %cmp.us, label %for.cond1.preheader.us, label %for.cond.cleanup

while.cond.preheader.us.us:                       ; preds = %for.cond1.preheader.us, %while.cond.while.end_crit_edge.us.us
  %v2.026.us.us = phi i257 [ %inc6.us.us, %while.cond.while.end_crit_edge.us.us ], [ 0, %for.cond1.preheader.us ]
  %res.125.us.us = phi i257 [ %add.us.us, %while.cond.while.end_crit_edge.us.us ], [ %res.029.us, %for.cond1.preheader.us ]
  br label %while.body.us.us

while.cond.while.end_crit_edge.us.us:             ; preds = %while.body.us.us
  %inc6.us.us = add nuw nsw i257 %v2.026.us.us, 1
  %cmp2.us.us = icmp slt i257 %inc6.us.us, %n2
  br i1 %cmp2.us.us, label %while.cond.preheader.us.us, label %for.cond1.for.cond.cleanup3_crit_edge.us

while.body.us.us:                                 ; preds = %while.body.us.us, %while.cond.preheader.us.us
  %v3.023.us.us = phi i257 [ 0, %while.cond.preheader.us.us ], [ %inc.us.us, %while.body.us.us ]
  %res.222.us.us = phi i257 [ %res.125.us.us, %while.cond.preheader.us.us ], [ %add.us.us, %while.body.us.us ]
  %call.us.us = tail call i257 @foo(i257 %v1.030.us, i257 %v2.026.us.us, i257 %v3.023.us.us) #2
  %add.us.us = add nsw i257 %call.us.us, %res.222.us.us
  %inc.us.us = add nuw nsw i257 %v3.023.us.us, 1
  %cmp5.us.us = icmp slt i257 %inc.us.us, %n3
  br i1 %cmp5.us.us, label %while.body.us.us, label %while.cond.while.end_crit_edge.us.us

for.cond.cleanup:                                 ; preds = %for.cond1.for.cond.cleanup3_crit_edge.us, %for.cond1.preheader.lr.ph, %entry
  %res.0.lcssa = phi i257 [ 0, %entry ], [ 0, %for.cond1.preheader.lr.ph ], [ %split27.us, %for.cond1.for.cond.cleanup3_crit_edge.us ]
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
