; RUN: llc < %s -march=tvm 
; ModuleID = 'while_while_while.c'
source_filename = "while_while_while.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: nounwind
define dso_local i257 @func(i257 %n1, i257 %n2, i257 %n3) local_unnamed_addr #0 {
entry:
  %cmp29 = icmp sgt i257 %n1, 0
  br i1 %cmp29, label %while.cond1.preheader.lr.ph, label %while.end10

while.cond1.preheader.lr.ph:                      ; preds = %entry
  %cmp225 = icmp sgt i257 %n2, 0
  %cmp522 = icmp sgt i257 %n3, 0
  br i1 %cmp225, label %while.cond1.preheader.us, label %while.end10

while.cond1.preheader.us:                         ; preds = %while.cond1.preheader.lr.ph, %while.cond1.while.end8_crit_edge.us
  %v1.031.us = phi i257 [ %inc9.us, %while.cond1.while.end8_crit_edge.us ], [ 0, %while.cond1.preheader.lr.ph ]
  %res.030.us = phi i257 [ %split28.us, %while.cond1.while.end8_crit_edge.us ], [ 0, %while.cond1.preheader.lr.ph ]
  br i1 %cmp522, label %while.cond4.preheader.us.us, label %while.cond1.while.end8_crit_edge.us

while.cond1.while.end8_crit_edge.us:              ; preds = %while.cond4.while.end_crit_edge.us.us, %while.cond1.preheader.us
  %split28.us = phi i257 [ %res.030.us, %while.cond1.preheader.us ], [ %add.us.us, %while.cond4.while.end_crit_edge.us.us ]
  %inc9.us = add nuw nsw i257 %v1.031.us, 1
  %cmp.us = icmp slt i257 %inc9.us, %n1
  br i1 %cmp.us, label %while.cond1.preheader.us, label %while.end10

while.cond4.preheader.us.us:                      ; preds = %while.cond1.preheader.us, %while.cond4.while.end_crit_edge.us.us
  %v2.027.us.us = phi i257 [ %inc7.us.us, %while.cond4.while.end_crit_edge.us.us ], [ 0, %while.cond1.preheader.us ]
  %res.126.us.us = phi i257 [ %add.us.us, %while.cond4.while.end_crit_edge.us.us ], [ %res.030.us, %while.cond1.preheader.us ]
  br label %while.body6.us.us

while.cond4.while.end_crit_edge.us.us:            ; preds = %while.body6.us.us
  %inc7.us.us = add nuw nsw i257 %v2.027.us.us, 1
  %cmp2.us.us = icmp slt i257 %inc7.us.us, %n2
  br i1 %cmp2.us.us, label %while.cond4.preheader.us.us, label %while.cond1.while.end8_crit_edge.us

while.body6.us.us:                                ; preds = %while.body6.us.us, %while.cond4.preheader.us.us
  %v3.024.us.us = phi i257 [ 0, %while.cond4.preheader.us.us ], [ %inc.us.us, %while.body6.us.us ]
  %res.223.us.us = phi i257 [ %res.126.us.us, %while.cond4.preheader.us.us ], [ %add.us.us, %while.body6.us.us ]
  %call.us.us = tail call i257 @foo(i257 %v1.031.us, i257 %v2.027.us.us, i257 %v3.024.us.us) #2
  %add.us.us = add nsw i257 %call.us.us, %res.223.us.us
  %inc.us.us = add nuw nsw i257 %v3.024.us.us, 1
  %cmp5.us.us = icmp slt i257 %inc.us.us, %n3
  br i1 %cmp5.us.us, label %while.body6.us.us, label %while.cond4.while.end_crit_edge.us.us

while.end10:                                      ; preds = %while.cond1.while.end8_crit_edge.us, %while.cond1.preheader.lr.ph, %entry
  %res.0.lcssa = phi i257 [ 0, %entry ], [ 0, %while.cond1.preheader.lr.ph ], [ %split28.us, %while.cond1.while.end8_crit_edge.us ]
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
