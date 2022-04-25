; RUN: llc < %s -march=tvm 
; ModuleID = 'while.c'
source_filename = "while.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %n) local_unnamed_addr #0 {
entry:
  %cmp6 = icmp sgt i257 %n, 0
  br i1 %cmp6, label %while.body, label %while.end

while.body:                                       ; preds = %entry, %while.body
  %iter.08 = phi i257 [ %dec, %while.body ], [ %n, %entry ]
  %res.07 = phi i257 [ %mul, %while.body ], [ 1, %entry ]
  %mul = mul nsw i257 %iter.08, %res.07
  %dec = add nsw i257 %iter.08, -1
  %cmp = icmp sgt i257 %iter.08, 1
  br i1 %cmp, label %while.body, label %while.end

while.end:                                        ; preds = %while.body, %entry
  %res.0.lcssa = phi i257 [ 1, %entry ], [ %mul, %while.body ]
  ret i257 %res.0.lcssa
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
