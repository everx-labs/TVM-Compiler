; RUN: llc < %s -march=tvm 
; ModuleID = 'dowhile_dowhile_dowhile.c'
source_filename = "dowhile_dowhile_dowhile.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: nounwind
define dso_local i257 @func(i257 %n1, i257 %n2, i257 %n3) local_unnamed_addr #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %do.end6, %entry
  %res.0 = phi i257 [ 0, %entry ], [ %add, %do.end6 ]
  %v1.0 = phi i257 [ 0, %entry ], [ %inc7, %do.end6 ]
  br label %do.body1

do.body1:                                         ; preds = %do.end, %do.body
  %res.1 = phi i257 [ %res.0, %do.body ], [ %add, %do.end ]
  %v2.0 = phi i257 [ 0, %do.body ], [ %inc3, %do.end ]
  br label %do.body2

do.body2:                                         ; preds = %do.body2, %do.body1
  %res.2 = phi i257 [ %res.1, %do.body1 ], [ %add, %do.body2 ]
  %v3.0 = phi i257 [ 0, %do.body1 ], [ %inc, %do.body2 ]
  %call = tail call i257 @foo(i257 %v1.0, i257 %v2.0, i257 %v3.0) #2
  %add = add nsw i257 %call, %res.2
  %inc = add nuw nsw i257 %v3.0, 1
  %cmp = icmp slt i257 %inc, %n3
  br i1 %cmp, label %do.body2, label %do.end

do.end:                                           ; preds = %do.body2
  %inc3 = add nuw nsw i257 %v2.0, 1
  %cmp5 = icmp slt i257 %inc3, %n2
  br i1 %cmp5, label %do.body1, label %do.end6

do.end6:                                          ; preds = %do.end
  %inc7 = add nuw nsw i257 %v1.0, 1
  %cmp9 = icmp slt i257 %inc7, %n1
  br i1 %cmp9, label %do.body, label %do.end10

do.end10:                                         ; preds = %do.end6
  ret i257 %add
}

declare dso_local i257 @foo(i257, i257, i257) local_unnamed_addr #1

attributes #0 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
