; RUN: llc < %s -march=tvm | FileCheck %s

target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readonly
; CHECK-LABEL: sextload_pattern
define dso_local i64 @sextload_pattern(i8* nocapture readonly %c) local_unnamed_addr #0 {
entry:
  %add.ptr = getelementptr inbounds i8, i8* %c, i64 1
  %0 = load i8, i8* %add.ptr, align 1, !tbaa !2
;CHECK: CALL  $:load$
  %conv = sext i8 %0 to i64
  ret i64 %conv
}

attributes #0 = { norecurse nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 8}
!1 = !{!"clang version 7.0.0 (tags/RELEASE_700/final)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"omnipotent char", !4, i64 0}
!4 = !{!"Simple C/C++ TBAA"}
