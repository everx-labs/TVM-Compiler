; RUN: llc < %s -march=tvm 
; ModuleID = 'do_while.c'
source_filename = "do_while.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %arg) local_unnamed_addr #0 {
entry:
  %cmp = icmp slt i257 %arg, 1
  br i1 %cmp, label %return, label %do.body

do.body:                                          ; preds = %entry, %do.body
  %res.0 = phi i257 [ %mul, %do.body ], [ 1, %entry ]
  %iterator.0 = phi i257 [ %dec, %do.body ], [ %arg, %entry ]
  %mul = mul nsw i257 %iterator.0, %res.0
  %dec = add nsw i257 %iterator.0, -1
  %cmp1 = icmp sgt i257 %iterator.0, 1
  br i1 %cmp1, label %do.body, label %return

return:                                           ; preds = %do.body, %entry
  %retval.0 = phi i257 [ 1, %entry ], [ %mul, %do.body ]
  ret i257 %retval.0
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
