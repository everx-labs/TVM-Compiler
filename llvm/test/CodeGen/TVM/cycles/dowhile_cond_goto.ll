; RUN: llc < %s -march=tvm 
; ModuleID = 'dowhile_cond_goto.c'
source_filename = "dowhile_cond_goto.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %arg) local_unnamed_addr #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %if.else, %entry
  %res.0 = phi i257 [ %arg, %entry ], [ %mul, %if.else ]
  %iterator.0 = phi i257 [ %arg, %entry ], [ %dec, %if.else ]
  %rem = srem i257 %iterator.0, 11
  %cmp = icmp eq i257 %rem, 0
  br i1 %cmp, label %cleanup, label %if.else

if.else:                                          ; preds = %do.body
  %dec = add nsw i257 %iterator.0, -1
  %mul = mul nsw i257 %dec, %res.0
  %cmp1 = icmp sgt i257 %iterator.0, 1
  br i1 %cmp1, label %do.body, label %cleanup

cleanup:                                          ; preds = %do.body, %if.else
  %retval.0 = phi i257 [ %mul, %if.else ], [ -1, %do.body ]
  ret i257 %retval.0
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
