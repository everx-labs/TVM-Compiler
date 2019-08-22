; RUN: llc < %s -march=tvm 
; ModuleID = 'dowhile_while_dowhile.c'
source_filename = "dowhile_while_dowhile.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: nounwind
define dso_local i257 @func(i257 %n1, i257 %n2, i257 %n3) local_unnamed_addr #0 {
entry:
  %cmp19 = icmp sgt i257 %n2, 0
  br i1 %cmp19, label %do.body.us, label %do.end7

do.body.us:                                       ; preds = %entry, %while.cond.while.end_crit_edge.us
  %res.0.us = phi i257 [ %add.us, %while.cond.while.end_crit_edge.us ], [ 0, %entry ]
  %v1.0.us = phi i257 [ %inc4.us, %while.cond.while.end_crit_edge.us ], [ 0, %entry ]
  br label %do.body1.preheader.us

do.body1.us:                                      ; preds = %do.body1.preheader.us, %do.body1.us
  %res.2.us = phi i257 [ %add.us, %do.body1.us ], [ %res.120.us, %do.body1.preheader.us ]
  %v3.0.us = phi i257 [ %inc.us, %do.body1.us ], [ 0, %do.body1.preheader.us ]
  %call.us = tail call i257 @foo(i257 %v1.0.us, i257 %v2.021.us, i257 %v3.0.us) #2
  %add.us = add nsw i257 %call.us, %res.2.us
  %inc.us = add nuw nsw i257 %v3.0.us, 1
  %cmp2.us = icmp slt i257 %inc.us, %n3
  br i1 %cmp2.us, label %do.body1.us, label %do.end.us

do.end.us:                                        ; preds = %do.body1.us
  %inc3.us = add nuw nsw i257 %v2.021.us, 1
  %cmp.us = icmp slt i257 %inc3.us, %n2
  br i1 %cmp.us, label %do.body1.preheader.us, label %while.cond.while.end_crit_edge.us

do.body1.preheader.us:                            ; preds = %do.body.us, %do.end.us
  %v2.021.us = phi i257 [ 0, %do.body.us ], [ %inc3.us, %do.end.us ]
  %res.120.us = phi i257 [ %res.0.us, %do.body.us ], [ %add.us, %do.end.us ]
  br label %do.body1.us

while.cond.while.end_crit_edge.us:                ; preds = %do.end.us
  %inc4.us = add nuw nsw i257 %v1.0.us, 1
  %cmp6.us = icmp slt i257 %inc4.us, %n1
  br i1 %cmp6.us, label %do.body.us, label %do.end7

do.end7:                                          ; preds = %while.cond.while.end_crit_edge.us, %entry
  %res.1.lcssa.lcssa = phi i257 [ 0, %entry ], [ %add.us, %while.cond.while.end_crit_edge.us ]
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
