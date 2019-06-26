; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind sspstrong uwtable
define dso_local void @insertionSort(i64* nocapture, i64) local_unnamed_addr #0 {
  %3 = icmp sgt i64 %1, 1
  br i1 %3, label %4, label %48

; <label>:4:                                      ; preds = %2
  %5 = add i64 %1, -1
  %6 = and i64 %5, 1
  %7 = icmp eq i64 %1, 2
  br i1 %7, label %30, label %8

; <label>:8:                                      ; preds = %4
  %9 = sub i64 %5, %6
  br label %10

; <label>:10:                                     ; preds = %58, %8
  %11 = phi i64 [ 1, %8 ], [ %61, %58 ]
  %12 = phi i64 [ %9, %8 ], [ %62, %58 ]
  %13 = getelementptr inbounds i64, i64* %0, i64 %11
  %14 = load i64, i64* %13, align 8, !tbaa !4
  br label %15

; <label>:15:                                     ; preds = %10, %21
  %16 = phi i64 [ %11, %10 ], [ %17, %21 ]
  %17 = add nsw i64 %16, -1
  %18 = getelementptr inbounds i64, i64* %0, i64 %17
  %19 = load i64, i64* %18, align 8, !tbaa !4
  %20 = icmp sgt i64 %19, %14
  br i1 %20, label %21, label %24

; <label>:21:                                     ; preds = %15
  %22 = getelementptr inbounds i64, i64* %0, i64 %16
  store i64 %19, i64* %22, align 8, !tbaa !4
  %23 = icmp sgt i64 %17, 0
  br i1 %23, label %15, label %24

; <label>:24:                                     ; preds = %21, %15
  %25 = phi i64 [ %17, %21 ], [ %16, %15 ]
  %26 = getelementptr inbounds i64, i64* %0, i64 %25
  store i64 %14, i64* %26, align 8, !tbaa !4
  %27 = add nuw nsw i64 %11, 1
  %28 = getelementptr inbounds i64, i64* %0, i64 %27
  %29 = load i64, i64* %28, align 8, !tbaa !4
  br label %49

; <label>:30:                                     ; preds = %58, %4
  %31 = phi i64 [ 1, %4 ], [ %61, %58 ]
  %32 = icmp eq i64 %6, 0
  br i1 %32, label %48, label %33

; <label>:33:                                     ; preds = %30
  %34 = getelementptr inbounds i64, i64* %0, i64 %31
  %35 = load i64, i64* %34, align 8, !tbaa !4
  br label %36

; <label>:36:                                     ; preds = %42, %33
  %37 = phi i64 [ %31, %33 ], [ %38, %42 ]
  %38 = add nsw i64 %37, -1
  %39 = getelementptr inbounds i64, i64* %0, i64 %38
  %40 = load i64, i64* %39, align 8, !tbaa !4
  %41 = icmp sgt i64 %40, %35
  br i1 %41, label %42, label %45

; <label>:42:                                     ; preds = %36
  %43 = getelementptr inbounds i64, i64* %0, i64 %37
  store i64 %40, i64* %43, align 8, !tbaa !4
  %44 = icmp sgt i64 %38, 0
  br i1 %44, label %36, label %45

; <label>:45:                                     ; preds = %42, %36
  %46 = phi i64 [ %38, %42 ], [ %37, %36 ]
  %47 = getelementptr inbounds i64, i64* %0, i64 %46
  store i64 %35, i64* %47, align 8, !tbaa !4
  br label %48

; <label>:48:                                     ; preds = %45, %30, %2
  ret void

; <label>:49:                                     ; preds = %55, %24
  %50 = phi i64 [ %27, %24 ], [ %51, %55 ]
  %51 = add nsw i64 %50, -1
  %52 = getelementptr inbounds i64, i64* %0, i64 %51
  %53 = load i64, i64* %52, align 8, !tbaa !4
  %54 = icmp sgt i64 %53, %29
  br i1 %54, label %55, label %58

; <label>:55:                                     ; preds = %49
  %56 = getelementptr inbounds i64, i64* %0, i64 %50
  store i64 %53, i64* %56, align 8, !tbaa !4
  %57 = icmp sgt i64 %51, 0
  br i1 %57, label %49, label %58

; <label>:58:                                     ; preds = %55, %49
  %59 = phi i64 [ %51, %55 ], [ %50, %49 ]
  %60 = getelementptr inbounds i64, i64* %0, i64 %59
  store i64 %29, i64* %60, align 8, !tbaa !4
  %61 = add nuw nsw i64 %11, 2
  %62 = add i64 %12, -2
  %63 = icmp eq i64 %62, 0
  br i1 %63, label %30, label %10
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
