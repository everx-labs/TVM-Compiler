; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: nounwind readonly sspstrong uwtable
define dso_local i64 @bSearchR(i64* readonly, i64, i64, i64) local_unnamed_addr #0 {
  %5 = icmp sgt i64 %1, %2
  br i1 %5, label %24, label %6

; <label>:6:                                      ; preds = %4, %21
  %7 = phi i64 [ %10, %21 ], [ %2, %4 ]
  %8 = phi i64 [ %22, %21 ], [ %1, %4 ]
  br label %9

; <label>:9:                                      ; preds = %16, %6
  %10 = phi i64 [ %7, %6 ], [ %17, %16 ]
  %11 = add nsw i64 %10, %8
  %12 = sdiv i64 %11, 2
  %13 = getelementptr inbounds i64, i64* %0, i64 %12
  %14 = load i64, i64* %13, align 8, !tbaa !4
  %15 = icmp sgt i64 %14, %3
  br i1 %15, label %16, label %19

; <label>:16:                                     ; preds = %9
  %17 = add nsw i64 %12, -1
  %18 = icmp slt i64 %8, %12
  br i1 %18, label %9, label %24

; <label>:19:                                     ; preds = %9
  %20 = icmp slt i64 %14, %3
  br i1 %20, label %21, label %24

; <label>:21:                                     ; preds = %19
  %22 = add nsw i64 %12, 1
  %23 = icmp slt i64 %12, %10
  br i1 %23, label %6, label %24

; <label>:24:                                     ; preds = %21, %19, %16, %4
  %25 = phi i64 [ -1, %4 ], [ -1, %16 ], [ -1, %21 ], [ %12, %19 ]
  ret i64 %25
}

attributes #0 = { nounwind readonly sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }

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
