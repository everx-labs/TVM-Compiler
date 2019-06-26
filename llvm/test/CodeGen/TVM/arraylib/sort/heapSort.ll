; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind sspstrong uwtable
define dso_local void @downHeap(i64* nocapture, i64, i64) local_unnamed_addr #0 {
  %4 = getelementptr inbounds i64, i64* %0, i64 %1
  %5 = load i64, i64* %4, align 8, !tbaa !4
  %6 = sdiv i64 %2, 2
  %7 = icmp slt i64 %6, %1
  br i1 %7, label %28, label %8

; <label>:8:                                      ; preds = %3, %25
  %9 = phi i64 [ %21, %25 ], [ %1, %3 ]
  %10 = shl nsw i64 %9, 1
  %11 = icmp slt i64 %10, %2
  br i1 %11, label %12, label %20

; <label>:12:                                     ; preds = %8
  %13 = getelementptr inbounds i64, i64* %0, i64 %10
  %14 = load i64, i64* %13, align 8, !tbaa !4
  %15 = or i64 %10, 1
  %16 = getelementptr inbounds i64, i64* %0, i64 %15
  %17 = load i64, i64* %16, align 8, !tbaa !4
  %18 = icmp slt i64 %14, %17
  %19 = select i1 %18, i64 %15, i64 %10
  br label %20

; <label>:20:                                     ; preds = %12, %8
  %21 = phi i64 [ %10, %8 ], [ %19, %12 ]
  %22 = getelementptr inbounds i64, i64* %0, i64 %21
  %23 = load i64, i64* %22, align 8, !tbaa !4
  %24 = icmp slt i64 %5, %23
  br i1 %24, label %25, label %28

; <label>:25:                                     ; preds = %20
  %26 = getelementptr inbounds i64, i64* %0, i64 %9
  store i64 %23, i64* %26, align 8, !tbaa !4
  %27 = icmp sgt i64 %21, %6
  br i1 %27, label %28, label %8

; <label>:28:                                     ; preds = %25, %20, %3
  %29 = phi i64 [ %1, %3 ], [ %9, %20 ], [ %21, %25 ]
  %30 = getelementptr inbounds i64, i64* %0, i64 %29
  store i64 %5, i64* %30, align 8, !tbaa !4
  ret void
}

; Function Attrs: norecurse nounwind sspstrong uwtable
define dso_local void @heapSort(i64* nocapture, i64) local_unnamed_addr #0 {
  %3 = icmp sgt i64 %1, 1
  br i1 %3, label %6, label %4

; <label>:4:                                      ; preds = %2
  %5 = add nsw i64 %1, -1
  br label %10

; <label>:6:                                      ; preds = %2
  %7 = lshr i64 %1, 1
  %8 = add nsw i64 %1, -1
  %9 = sdiv i64 %8, 2
  br label %13

; <label>:10:                                     ; preds = %39, %4
  %11 = phi i64 [ %5, %4 ], [ %8, %39 ]
  %12 = icmp sgt i64 %11, 0
  br i1 %12, label %43, label %76

; <label>:13:                                     ; preds = %6, %39
  %14 = phi i64 [ %7, %6 ], [ %15, %39 ]
  %15 = add nsw i64 %14, -1
  %16 = getelementptr inbounds i64, i64* %0, i64 %15
  %17 = load i64, i64* %16, align 8, !tbaa !4
  %18 = icmp slt i64 %9, %15
  br i1 %18, label %39, label %19

; <label>:19:                                     ; preds = %13, %36
  %20 = phi i64 [ %32, %36 ], [ %15, %13 ]
  %21 = shl nsw i64 %20, 1
  %22 = icmp slt i64 %21, %8
  br i1 %22, label %23, label %31

; <label>:23:                                     ; preds = %19
  %24 = getelementptr inbounds i64, i64* %0, i64 %21
  %25 = load i64, i64* %24, align 8, !tbaa !4
  %26 = or i64 %21, 1
  %27 = getelementptr inbounds i64, i64* %0, i64 %26
  %28 = load i64, i64* %27, align 8, !tbaa !4
  %29 = icmp slt i64 %25, %28
  %30 = select i1 %29, i64 %26, i64 %21
  br label %31

; <label>:31:                                     ; preds = %23, %19
  %32 = phi i64 [ %21, %19 ], [ %30, %23 ]
  %33 = getelementptr inbounds i64, i64* %0, i64 %32
  %34 = load i64, i64* %33, align 8, !tbaa !4
  %35 = icmp slt i64 %17, %34
  br i1 %35, label %36, label %39

; <label>:36:                                     ; preds = %31
  %37 = getelementptr inbounds i64, i64* %0, i64 %20
  store i64 %34, i64* %37, align 8, !tbaa !4
  %38 = icmp sgt i64 %32, %9
  br i1 %38, label %39, label %19

; <label>:39:                                     ; preds = %31, %36, %13
  %40 = phi i64 [ %15, %13 ], [ %32, %36 ], [ %20, %31 ]
  %41 = getelementptr inbounds i64, i64* %0, i64 %40
  store i64 %17, i64* %41, align 8, !tbaa !4
  %42 = icmp sgt i64 %15, 0
  br i1 %42, label %13, label %10

; <label>:43:                                     ; preds = %10, %71
  %44 = phi i64 [ %74, %71 ], [ %11, %10 ]
  %45 = phi i64 [ %44, %71 ], [ %1, %10 ]
  %46 = getelementptr inbounds i64, i64* %0, i64 %44
  %47 = load i64, i64* %46, align 8, !tbaa !4
  %48 = load i64, i64* %0, align 8, !tbaa !4
  store i64 %48, i64* %46, align 8, !tbaa !4
  store i64 %47, i64* %0, align 8, !tbaa !4
  %49 = add nsw i64 %45, -2
  %50 = sdiv i64 %49, 2
  br label %51

; <label>:51:                                     ; preds = %43, %68
  %52 = phi i64 [ %64, %68 ], [ 0, %43 ]
  %53 = shl nsw i64 %52, 1
  %54 = icmp slt i64 %53, %49
  br i1 %54, label %55, label %63

; <label>:55:                                     ; preds = %51
  %56 = getelementptr inbounds i64, i64* %0, i64 %53
  %57 = load i64, i64* %56, align 8, !tbaa !4
  %58 = or i64 %53, 1
  %59 = getelementptr inbounds i64, i64* %0, i64 %58
  %60 = load i64, i64* %59, align 8, !tbaa !4
  %61 = icmp slt i64 %57, %60
  %62 = select i1 %61, i64 %58, i64 %53
  br label %63

; <label>:63:                                     ; preds = %55, %51
  %64 = phi i64 [ %53, %51 ], [ %62, %55 ]
  %65 = getelementptr inbounds i64, i64* %0, i64 %64
  %66 = load i64, i64* %65, align 8, !tbaa !4
  %67 = icmp slt i64 %47, %66
  br i1 %67, label %68, label %71

; <label>:68:                                     ; preds = %63
  %69 = getelementptr inbounds i64, i64* %0, i64 %52
  store i64 %66, i64* %69, align 8, !tbaa !4
  %70 = icmp sgt i64 %64, %50
  br i1 %70, label %71, label %51

; <label>:71:                                     ; preds = %63, %68
  %72 = phi i64 [ %52, %63 ], [ %64, %68 ]
  %73 = getelementptr inbounds i64, i64* %0, i64 %72
  store i64 %47, i64* %73, align 8, !tbaa !4
  %74 = add nsw i64 %44, -1
  %75 = icmp sgt i64 %74, 0
  br i1 %75, label %43, label %76

; <label>:76:                                     ; preds = %71, %10
  ret void
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
