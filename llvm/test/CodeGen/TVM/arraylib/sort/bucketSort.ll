; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind sspstrong uwtable
define dso_local void @bucketSort(i64* nocapture, i64) local_unnamed_addr #0 {
  %3 = alloca i64, i64 %1, align 16
  %4 = icmp sgt i64 %1, 0
  br i1 %4, label %5, label %102

; <label>:5:                                      ; preds = %2
  %6 = bitcast i64* %3 to i8*
  %7 = shl i64 %1, 3
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 %6, i8 0, i64 %7, i1 false)
  %8 = add i64 %1, -1
  %9 = and i64 %1, 3
  %10 = icmp ult i64 %8, 3
  br i1 %10, label %13, label %11

; <label>:11:                                     ; preds = %5
  %12 = sub i64 %1, %9
  br label %28

; <label>:13:                                     ; preds = %28, %5
  %14 = phi i64 [ 0, %5 ], [ %54, %28 ]
  %15 = icmp eq i64 %9, 0
  br i1 %15, label %27, label %16

; <label>:16:                                     ; preds = %13, %16
  %17 = phi i64 [ %24, %16 ], [ %14, %13 ]
  %18 = phi i64 [ %25, %16 ], [ %9, %13 ]
  %19 = getelementptr inbounds i64, i64* %0, i64 %17
  %20 = load i64, i64* %19, align 8, !tbaa !4
  %21 = getelementptr inbounds i64, i64* %3, i64 %20
  %22 = load i64, i64* %21, align 8, !tbaa !4
  %23 = add nsw i64 %22, 1
  store i64 %23, i64* %21, align 8, !tbaa !4
  %24 = add nuw nsw i64 %17, 1
  %25 = add i64 %18, -1
  %26 = icmp eq i64 %25, 0
  br i1 %26, label %27, label %16, !llvm.loop !8

; <label>:27:                                     ; preds = %16, %13
  br i1 %4, label %57, label %102

; <label>:28:                                     ; preds = %28, %11
  %29 = phi i64 [ 0, %11 ], [ %54, %28 ]
  %30 = phi i64 [ %12, %11 ], [ %55, %28 ]
  %31 = getelementptr inbounds i64, i64* %0, i64 %29
  %32 = load i64, i64* %31, align 8, !tbaa !4
  %33 = getelementptr inbounds i64, i64* %3, i64 %32
  %34 = load i64, i64* %33, align 8, !tbaa !4
  %35 = add nsw i64 %34, 1
  store i64 %35, i64* %33, align 8, !tbaa !4
  %36 = or i64 %29, 1
  %37 = getelementptr inbounds i64, i64* %0, i64 %36
  %38 = load i64, i64* %37, align 8, !tbaa !4
  %39 = getelementptr inbounds i64, i64* %3, i64 %38
  %40 = load i64, i64* %39, align 8, !tbaa !4
  %41 = add nsw i64 %40, 1
  store i64 %41, i64* %39, align 8, !tbaa !4
  %42 = or i64 %29, 2
  %43 = getelementptr inbounds i64, i64* %0, i64 %42
  %44 = load i64, i64* %43, align 8, !tbaa !4
  %45 = getelementptr inbounds i64, i64* %3, i64 %44
  %46 = load i64, i64* %45, align 8, !tbaa !4
  %47 = add nsw i64 %46, 1
  store i64 %47, i64* %45, align 8, !tbaa !4
  %48 = or i64 %29, 3
  %49 = getelementptr inbounds i64, i64* %0, i64 %48
  %50 = load i64, i64* %49, align 8, !tbaa !4
  %51 = getelementptr inbounds i64, i64* %3, i64 %50
  %52 = load i64, i64* %51, align 8, !tbaa !4
  %53 = add nsw i64 %52, 1
  store i64 %53, i64* %51, align 8, !tbaa !4
  %54 = add nuw nsw i64 %29, 4
  %55 = add i64 %30, -4
  %56 = icmp eq i64 %55, 0
  br i1 %56, label %13, label %28

; <label>:57:                                     ; preds = %27, %98
  %58 = phi i64 [ %100, %98 ], [ 0, %27 ]
  %59 = phi i64 [ %99, %98 ], [ 0, %27 ]
  %60 = getelementptr inbounds i64, i64* %3, i64 %58
  %61 = load i64, i64* %60, align 8, !tbaa !4
  %62 = icmp sgt i64 %61, 0
  br i1 %62, label %63, label %98

; <label>:63:                                     ; preds = %57
  %64 = add i64 %59, %61
  %65 = add i64 %61, -1
  %66 = and i64 %61, 7
  %67 = icmp eq i64 %66, 0
  br i1 %67, label %75, label %68

; <label>:68:                                     ; preds = %63, %68
  %69 = phi i64 [ %71, %68 ], [ %59, %63 ]
  %70 = phi i64 [ %73, %68 ], [ %66, %63 ]
  %71 = add nsw i64 %69, 1
  %72 = getelementptr inbounds i64, i64* %0, i64 %69
  store i64 %58, i64* %72, align 8, !tbaa !4
  %73 = add i64 %70, -1
  %74 = icmp eq i64 %73, 0
  br i1 %74, label %75, label %68, !llvm.loop !10

; <label>:75:                                     ; preds = %68, %63
  %76 = phi i64 [ %59, %63 ], [ %71, %68 ]
  %77 = icmp ult i64 %65, 7
  br i1 %77, label %97, label %78

; <label>:78:                                     ; preds = %75, %78
  %79 = phi i64 [ %94, %78 ], [ %76, %75 ]
  %80 = add nsw i64 %79, 1
  %81 = getelementptr inbounds i64, i64* %0, i64 %79
  store i64 %58, i64* %81, align 8, !tbaa !4
  %82 = add nsw i64 %79, 2
  %83 = getelementptr inbounds i64, i64* %0, i64 %80
  store i64 %58, i64* %83, align 8, !tbaa !4
  %84 = add nsw i64 %79, 3
  %85 = getelementptr inbounds i64, i64* %0, i64 %82
  store i64 %58, i64* %85, align 8, !tbaa !4
  %86 = add nsw i64 %79, 4
  %87 = getelementptr inbounds i64, i64* %0, i64 %84
  store i64 %58, i64* %87, align 8, !tbaa !4
  %88 = add nsw i64 %79, 5
  %89 = getelementptr inbounds i64, i64* %0, i64 %86
  store i64 %58, i64* %89, align 8, !tbaa !4
  %90 = add nsw i64 %79, 6
  %91 = getelementptr inbounds i64, i64* %0, i64 %88
  store i64 %58, i64* %91, align 8, !tbaa !4
  %92 = add nsw i64 %79, 7
  %93 = getelementptr inbounds i64, i64* %0, i64 %90
  store i64 %58, i64* %93, align 8, !tbaa !4
  %94 = add nsw i64 %79, 8
  %95 = getelementptr inbounds i64, i64* %0, i64 %92
  store i64 %58, i64* %95, align 8, !tbaa !4
  %96 = icmp eq i64 %94, %64
  br i1 %96, label %97, label %78

; <label>:97:                                     ; preds = %78, %75
  store i64 0, i64* %60, align 8, !tbaa !4
  br label %98

; <label>:98:                                     ; preds = %97, %57
  %99 = phi i64 [ %64, %97 ], [ %59, %57 ]
  %100 = add nuw nsw i64 %58, 1
  %101 = icmp eq i64 %100, %1
  br i1 %101, label %102, label %57

; <label>:102:                                    ; preds = %98, %2, %27
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1) #1

attributes #0 = { norecurse nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }

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
!8 = distinct !{!8, !9}
!9 = !{!"llvm.loop.unroll.disable"}
!10 = distinct !{!10, !9}
