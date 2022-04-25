; RUN: llc < %s -march=tvm 
; ModuleID = 'while_cond_return.c'
source_filename = "while_cond_return.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %arg) local_unnamed_addr #0 {
entry:
  %cmp11 = icmp sgt i257 %arg, 0
  br i1 %cmp11, label %while.body, label %cleanup

while.cond:                                       ; preds = %while.body
  %dec = add nsw i257 %iterator.013, -1
  %cmp = icmp sgt i257 %iterator.013, 1
  br i1 %cmp, label %while.body, label %cleanup

while.body:                                       ; preds = %entry, %while.cond
  %iterator.013 = phi i257 [ %dec, %while.cond ], [ %arg, %entry ]
  %res.012 = phi i257 [ %mul, %while.cond ], [ 1, %entry ]
  %mul = mul nsw i257 %iterator.013, %res.012
  %rem10 = urem i257 %iterator.013, 11
  %cmp1 = icmp eq i257 %rem10, 0
  br i1 %cmp1, label %cleanup, label %while.cond

cleanup:                                          ; preds = %while.body, %while.cond, %entry
  %retval.0 = phi i257 [ 1, %entry ], [ %mul, %while.cond ], [ %mul, %while.body ]
  ret i257 %retval.0
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
