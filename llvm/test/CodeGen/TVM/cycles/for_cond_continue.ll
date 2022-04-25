; RUN: llc < %s -march=tvm 
; ModuleID = 'for_cond_continue.c'
source_filename = "for_cond_continue.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %arg) local_unnamed_addr #0 {
entry:
  %cmp10 = icmp sgt i257 %arg, 0
  br i1 %cmp10, label %for.body, label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.body, %entry
  %res.0.lcssa = phi i257 [ 1, %entry ], [ %res.1, %for.body ]
  ret i257 %res.0.lcssa

for.body:                                         ; preds = %entry, %for.body
  %iterator.012 = phi i257 [ %dec, %for.body ], [ %arg, %entry ]
  %res.011 = phi i257 [ %res.1, %for.body ], [ 1, %entry ]
  %div9 = udiv i257 %iterator.012, 11
  %0 = mul i257 %div9, 11
  %1 = sub i257 %iterator.012, %0
  %cmp1 = icmp eq i257 %1, 0
  %mul = mul nsw i257 %iterator.012, %res.011
  %res.1 = select i1 %cmp1, i257 %res.011, i257 %mul
  %iterator.1 = select i1 %cmp1, i257 %div9, i257 %iterator.012
  %dec = add nsw i257 %iterator.1, -1
  %cmp = icmp sgt i257 %iterator.1, 1
  br i1 %cmp, label %for.body, label %for.cond.cleanup
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
