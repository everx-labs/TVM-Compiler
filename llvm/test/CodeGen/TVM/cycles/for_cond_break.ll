; RUN: llc < %s -march=tvm 
; ModuleID = 'for_cond_break.c'
source_filename = "for_cond_break.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %arg) local_unnamed_addr #0 {
entry:
  %cmp12 = icmp sgt i257 %arg, 0
  br i1 %cmp12, label %for.body, label %cleanup

for.cond:                                         ; preds = %for.body
  %dec2 = add nsw i257 %iterator.014, -2
  %cmp = icmp sgt i257 %iterator.014, 2
  br i1 %cmp, label %for.body, label %cleanup

for.body:                                         ; preds = %entry, %for.cond
  %iterator.014 = phi i257 [ %dec2, %for.cond ], [ %arg, %entry ]
  %res.013 = phi i257 [ %mul, %for.cond ], [ 1, %entry ]
  %mul = mul nsw i257 %iterator.014, %res.013
  %rem11 = urem i257 %iterator.014, 11
  %cmp1 = icmp eq i257 %rem11, 0
  br i1 %cmp1, label %cleanup, label %for.cond

cleanup:                                          ; preds = %for.cond, %for.body, %entry
  %res.1 = phi i257 [ 1, %entry ], [ %mul, %for.body ], [ %mul, %for.cond ]
  ret i257 %res.1
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
