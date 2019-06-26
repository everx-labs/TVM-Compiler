; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind sspstrong uwtable
define dso_local void @bubbleSort(i64* nocapture, i64) local_unnamed_addr #0 {
  %3 = add i64 %1, -1
  %4 = icmp sgt i64 %3, 0
  br i1 %4, label %5, label %42

; <label>:5:                                      ; preds = %2
  %6 = and i64 %3, 1
  %7 = icmp eq i64 %1, 2
  %8 = sub i64 %3, %6
  %9 = icmp eq i64 %6, 0
  br label %10

; <label>:10:                                     ; preds = %5, %39
  %11 = phi i64 [ %40, %39 ], [ 0, %5 ]
  %12 = load i64, i64* %0, align 8, !tbaa !4
  br i1 %7, label %29, label %13

; <label>:13:                                     ; preds = %10, %45
  %14 = phi i64 [ %46, %45 ], [ %12, %10 ]
  %15 = phi i64 [ %25, %45 ], [ 0, %10 ]
  %16 = phi i64 [ %47, %45 ], [ %8, %10 ]
  %17 = or i64 %15, 1
  %18 = getelementptr inbounds i64, i64* %0, i64 %17
  %19 = load i64, i64* %18, align 8, !tbaa !4
  %20 = icmp slt i64 %19, %14
  br i1 %20, label %21, label %23

; <label>:21:                                     ; preds = %13
  %22 = getelementptr inbounds i64, i64* %0, i64 %15
  store i64 %14, i64* %18, align 8, !tbaa !4
  store i64 %19, i64* %22, align 8, !tbaa !4
  br label %23

; <label>:23:                                     ; preds = %21, %13
  %24 = phi i64 [ %14, %21 ], [ %19, %13 ]
  %25 = add nuw nsw i64 %15, 2
  %26 = getelementptr inbounds i64, i64* %0, i64 %25
  %27 = load i64, i64* %26, align 8, !tbaa !4
  %28 = icmp slt i64 %27, %24
  br i1 %28, label %43, label %45

; <label>:29:                                     ; preds = %45, %10
  %30 = phi i64 [ %12, %10 ], [ %46, %45 ]
  %31 = phi i64 [ 0, %10 ], [ %25, %45 ]
  br i1 %9, label %39, label %32

; <label>:32:                                     ; preds = %29
  %33 = add nuw nsw i64 %31, 1
  %34 = getelementptr inbounds i64, i64* %0, i64 %33
  %35 = load i64, i64* %34, align 8, !tbaa !4
  %36 = icmp slt i64 %35, %30
  br i1 %36, label %37, label %39

; <label>:37:                                     ; preds = %32
  %38 = getelementptr inbounds i64, i64* %0, i64 %31
  store i64 %30, i64* %34, align 8, !tbaa !4
  store i64 %35, i64* %38, align 8, !tbaa !4
  br label %39

; <label>:39:                                     ; preds = %37, %32, %29
  %40 = add nuw nsw i64 %11, 1
  %41 = icmp eq i64 %40, %3
  br i1 %41, label %42, label %10

; <label>:42:                                     ; preds = %39, %2
  ret void

; <label>:43:                                     ; preds = %23
  %44 = getelementptr inbounds i64, i64* %0, i64 %17
  store i64 %24, i64* %26, align 8, !tbaa !4
  store i64 %27, i64* %44, align 8, !tbaa !4
  br label %45

; <label>:45:                                     ; preds = %43, %23
  %46 = phi i64 [ %24, %43 ], [ %27, %23 ]
  %47 = add i64 %16, -2
  %48 = icmp eq i64 %47, 0
  br i1 %48, label %29, label %13
}

attributes #0 = { norecurse nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }

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
