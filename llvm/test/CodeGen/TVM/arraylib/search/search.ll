; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readonly sspstrong uwtable
define dso_local i64 @search(i64, i64* nocapture readonly, i64) local_unnamed_addr #0 {
  %4 = icmp sgt i64 %2, 0
  br i1 %4, label %5, label %13

; <label>:5:                                      ; preds = %3, %10
  %6 = phi i64 [ %11, %10 ], [ 0, %3 ]
  %7 = getelementptr inbounds i64, i64* %1, i64 %6
  %8 = load i64, i64* %7, align 8, !tbaa !4
  %9 = icmp eq i64 %8, %0
  br i1 %9, label %13, label %10

; <label>:10:                                     ; preds = %5
  %11 = add nuw nsw i64 %6, 1
  %12 = icmp slt i64 %11, %2
  br i1 %12, label %5, label %13

; <label>:13:                                     ; preds = %5, %10, %3
  %14 = phi i64 [ -1, %3 ], [ -1, %10 ], [ %6, %5 ]
  ret i64 %14
}

attributes #0 = { norecurse nounwind readonly sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
!4 = !{!5, !5, i64 0}
!5 = !{!"long", !6, i64 0}
!6 = !{!"omnipotent char", !7, i64 0}
!7 = !{!"Simple C/C++ TBAA"}
