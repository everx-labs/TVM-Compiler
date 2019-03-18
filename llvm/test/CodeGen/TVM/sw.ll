; RUN: llc < %s -march=tvm | FileCheck %s
; XFAIL: *
; ModuleID = 'sw.cpp'
source_filename = "sw.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local i64 @_Z1axx(i64 %x, i64 %y) local_unnamed_addr #0 {
entry:
  switch i64 %x, label %sw.default [
    i64 0, label %return
    i64 1, label %sw.bb1
    i64 2, label %sw.bb2
    i64 3, label %sw.bb3
  ]

sw.bb1:                                           ; preds = %entry
  br label %return

sw.bb2:                                           ; preds = %entry
  %mul = mul nsw i64 %y, %y
  br label %return

sw.bb3:                                           ; preds = %entry
  %mul4 = mul nsw i64 %y, %y
  %mul5 = mul nsw i64 %mul4, %y
  br label %return

sw.default:                                       ; preds = %entry
  br label %return

return:                                           ; preds = %entry, %sw.default, %sw.bb3, %sw.bb2, %sw.bb1
  %retval.0 = phi i64 [ 0, %sw.default ], [ %mul5, %sw.bb3 ], [ %mul, %sw.bb2 ], [ %y, %sw.bb1 ], [ 1, %entry ]
  ret i64 %retval.0
}

attributes #0 = { norecurse nounwind readnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 7.0.0 (tags/RELEASE_700/final)"}
