; RUN: llc < %s -march=tvm | FileCheck %s

target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @sum(i257 %a0, i257 %a1, i257 %a2, i257 %a3, i257 %a4, i257 %a5, i257 %a6, i257 %a7, i257 %a8, i257 %a9, i257 %a10, i257 %a11, i257 %a12, i257 %a13, i257 %a14, i257 %a15, i257 %a16) local_unnamed_addr #0 {
; CHECK-LABEL: sum:
; CHECK XCHG	s0, s16
; CHECK XCHG	s0, s1
; CHECK XCHG	s0, s16
entry:
  %add = add nsw i257 %a16, %a0
  ret i257 %add
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 8}
!1 = !{!"clang version 7.0.0 (tags/RELEASE_700/final)"}
