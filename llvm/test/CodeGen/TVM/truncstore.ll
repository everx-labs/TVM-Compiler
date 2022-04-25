; RUN: llc < %s -march=tvm | FileCheck %s

target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind
; CHECK-LABEL: trucate
define dso_local void @trucate(i257 %i, i256* nocapture %c) local_unnamed_addr #0 {
entry:
; CHECK: GETGLOB 14 CALLX
  %conv = trunc i257 %i to i256
  store i256 %conv, i256* %c, align 1, !tbaa !2
  ret void
}

attributes #0 = { norecurse nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 8}
!1 = !{!"clang version 7.0.0 (tags/RELEASE_700/final)"}
!2 = !{!3, !3, i257 0}
!3 = !{!"omnipotent char", !4, i257 0}
!4 = !{!"Simple C/C++ TBAA"}
