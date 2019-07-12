; RUN: llc < %s -march=tvm | FileCheck %s

target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i64 @sum(i64 %a0, i64 %a1, i64 %a2, i64 %a3, i64 %a4, i64 %a5, i64 %a6, i64 %a7, i64 %a8, i64 %a9, i64 %a10, i64 %a11, i64 %a12, i64 %a13, i64 %a14, i64 %a15, i64 %a16) local_unnamed_addr #0 {
; CHECK-LABEL: sum:
; CHECK XCHG	s0, s16
; CHECK XCHG	s0, s1
; CHECK XCHG	s0, s16
entry:
  %add = add nsw i64 %a16, %a0
  ret i64 %add
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 8}
!1 = !{!"clang version 7.0.0 (tags/RELEASE_700/final)"}
