; RUN: llc < %s -march=tvm 
; ModuleID = 'dowhile_dowhile_while.c'
source_filename = "dowhile_dowhile_while.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: nounwind
define dso_local i257 @func(i257 %n1, i257 %n2, i257 %n3) local_unnamed_addr #0 {
entry:
  %cmp19 = icmp sgt i257 %n3, 0
  br i1 %cmp19, label %do.body.us, label %do.end7

do.body.us:                                       ; preds = %entry, %do.end.us-lcssa.us.us
  %res.0.us = phi i257 [ %add.us.us, %do.end.us-lcssa.us.us ], [ 0, %entry ]
  %v1.0.us = phi i257 [ %inc4.us, %do.end.us-lcssa.us.us ], [ 0, %entry ]
  br label %do.body1.us.us

do.body1.us.us:                                   ; preds = %do.body.us, %while.cond.while.end_crit_edge.us.us
  %res.1.us.us = phi i257 [ %res.0.us, %do.body.us ], [ %add.us.us, %while.cond.while.end_crit_edge.us.us ]
  %v2.0.us.us = phi i257 [ 0, %do.body.us ], [ %inc2.us.us, %while.cond.while.end_crit_edge.us.us ]
  br label %while.body.us.us

while.cond.while.end_crit_edge.us.us:             ; preds = %while.body.us.us
  %inc2.us.us = add nuw nsw i257 %v2.0.us.us, 1
  %cmp3.us.us = icmp slt i257 %inc2.us.us, %n2
  br i1 %cmp3.us.us, label %do.body1.us.us, label %do.end.us-lcssa.us.us

while.body.us.us:                                 ; preds = %while.body.us.us, %do.body1.us.us
  %v3.021.us.us = phi i257 [ 0, %do.body1.us.us ], [ %inc.us.us, %while.body.us.us ]
  %res.220.us.us = phi i257 [ %res.1.us.us, %do.body1.us.us ], [ %add.us.us, %while.body.us.us ]
  %call.us.us = tail call i257 @foo(i257 %v1.0.us, i257 %v2.0.us.us, i257 %v3.021.us.us) #2
  %add.us.us = add nsw i257 %call.us.us, %res.220.us.us
  %inc.us.us = add nuw nsw i257 %v3.021.us.us, 1
  %cmp.us.us = icmp slt i257 %inc.us.us, %n3
  br i1 %cmp.us.us, label %while.body.us.us, label %while.cond.while.end_crit_edge.us.us

do.end.us-lcssa.us.us:                            ; preds = %while.cond.while.end_crit_edge.us.us
  %inc4.us = add nuw nsw i257 %v1.0.us, 1
  %cmp6.us = icmp slt i257 %inc4.us, %n1
  br i1 %cmp6.us, label %do.body.us, label %do.end7

do.end7:                                          ; preds = %do.end.us-lcssa.us.us, %entry
  %res.2.lcssa.lcssa.lcssa = phi i257 [ 0, %entry ], [ %add.us.us, %do.end.us-lcssa.us.us ]
  ret i257 %res.2.lcssa.lcssa.lcssa
}

declare dso_local i257 @foo(i257, i257, i257) local_unnamed_addr #1

attributes #0 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
