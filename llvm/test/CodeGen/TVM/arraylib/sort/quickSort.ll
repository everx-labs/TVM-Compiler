; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: nounwind sspstrong uwtable
define dso_local void @quickSortR(i64*, i64) local_unnamed_addr #0 {
  br label %3

; <label>:3:                                      ; preds = %39, %2
  %4 = phi i64* [ %0, %2 ], [ %40, %39 ]
  %5 = phi i64 [ %1, %2 ], [ %41, %39 ]
  %6 = ashr i64 %5, 1
  %7 = getelementptr inbounds i64, i64* %4, i64 %6
  %8 = load i64, i64* %7, align 8, !tbaa !4
  br label %9

; <label>:9:                                      ; preds = %30, %3
  %10 = phi i64 [ %5, %3 ], [ %31, %30 ]
  %11 = phi i64 [ 0, %3 ], [ %32, %30 ]
  br label %12

; <label>:12:                                     ; preds = %12, %9
  %13 = phi i64 [ %11, %9 ], [ %17, %12 ]
  %14 = getelementptr inbounds i64, i64* %4, i64 %13
  %15 = load i64, i64* %14, align 8, !tbaa !4
  %16 = icmp slt i64 %15, %8
  %17 = add nsw i64 %13, 1
  br i1 %16, label %12, label %18

; <label>:18:                                     ; preds = %12
  %19 = getelementptr inbounds i64, i64* %4, i64 %13
  br label %20

; <label>:20:                                     ; preds = %20, %18
  %21 = phi i64 [ %25, %20 ], [ %10, %18 ]
  %22 = getelementptr inbounds i64, i64* %4, i64 %21
  %23 = load i64, i64* %22, align 8, !tbaa !4
  %24 = icmp sgt i64 %23, %8
  %25 = add nsw i64 %21, -1
  br i1 %24, label %20, label %26

; <label>:26:                                     ; preds = %20
  %27 = icmp sgt i64 %13, %21
  br i1 %27, label %30, label %28

; <label>:28:                                     ; preds = %26
  %29 = getelementptr inbounds i64, i64* %4, i64 %21
  store i64 %23, i64* %19, align 8, !tbaa !4
  store i64 %15, i64* %29, align 8, !tbaa !4
  br label %30

; <label>:30:                                     ; preds = %26, %28
  %31 = phi i64 [ %25, %28 ], [ %21, %26 ]
  %32 = phi i64 [ %17, %28 ], [ %13, %26 ]
  %33 = icmp sgt i64 %32, %31
  br i1 %33, label %34, label %9

; <label>:34:                                     ; preds = %30
  %35 = icmp sgt i64 %31, 0
  br i1 %35, label %36, label %37

; <label>:36:                                     ; preds = %34
  tail call void @quickSortR(i64* nonnull %4, i64 %31)
  br label %37

; <label>:37:                                     ; preds = %36, %34
  %38 = icmp sgt i64 %5, %32
  br i1 %38, label %39, label %42

; <label>:39:                                     ; preds = %37
  %40 = getelementptr inbounds i64, i64* %4, i64 %32
  %41 = sub nsw i64 %5, %32
  br label %3

; <label>:42:                                     ; preds = %37
  ret void
}

attributes #0 = { nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }

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
