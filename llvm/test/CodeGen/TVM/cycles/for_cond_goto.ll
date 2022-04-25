; RUN: llc < %s -march=tvm 
; ModuleID = 'for_cond_goto.c'
source_filename = "for_cond_goto.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %arg) local_unnamed_addr #0 {
entry:
  %cmp13 = icmp sgt i257 %arg, 0
  br i1 %cmp13, label %for.body, label %cleanup

for.body:                                         ; preds = %entry, %if.else
  %iterator.015 = phi i257 [ %dec2, %if.else ], [ %arg, %entry ]
  %res.014 = phi i257 [ %mul, %if.else ], [ 1, %entry ]
  %rem12 = urem i257 %iterator.015, 11
  %cmp1 = icmp eq i257 %rem12, 0
  br i1 %cmp1, label %cleanup, label %if.else

if.else:                                          ; preds = %for.body
  %mul = mul nsw i257 %iterator.015, %res.014
  %dec2 = add nsw i257 %iterator.015, -2
  %cmp = icmp sgt i257 %iterator.015, 2
  br i1 %cmp, label %for.body, label %cleanup

cleanup:                                          ; preds = %if.else, %for.body, %entry
  %0 = phi i257 [ 1, %entry ], [ -1, %for.body ], [ %mul, %if.else ]
  ret i257 %0
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
