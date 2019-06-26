; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readonly sspstrong uwtable
define dso_local i64 @bSearch(i64* nocapture readonly, i64, i64) local_unnamed_addr #0 {
  %4 = icmp slt i64 %1, 0
  br i1 %4, label %20, label %5

; <label>:5:                                      ; preds = %3, %13
  %6 = phi i64 [ %18, %13 ], [ %1, %3 ]
  %7 = phi i64 [ %17, %13 ], [ 0, %3 ]
  %8 = add nsw i64 %6, %7
  %9 = sdiv i64 %8, 2
  %10 = getelementptr inbounds i64, i64* %0, i64 %9
  %11 = load i64, i64* %10, align 8, !tbaa !4
  %12 = icmp eq i64 %11, %2
  br i1 %12, label %20, label %13

; <label>:13:                                     ; preds = %5
  %14 = icmp sgt i64 %11, %2
  %15 = add nsw i64 %9, -1
  %16 = add nsw i64 %9, 1
  %17 = select i1 %14, i64 %7, i64 %16
  %18 = select i1 %14, i64 %15, i64 %6
  %19 = icmp sgt i64 %17, %18
  br i1 %19, label %20, label %5

; <label>:20:                                     ; preds = %5, %13, %3
  %21 = phi i64 [ -1, %3 ], [ -1, %13 ], [ %9, %5 ]
  ret i64 %21
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
