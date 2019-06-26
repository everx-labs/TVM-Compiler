; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind sspstrong uwtable
define dso_local void @countSort(i64* nocapture, i64, i64) local_unnamed_addr #0 {
  %4 = bitcast i64* %0 to i8*
  %5 = alloca i64, i64 %1, align 16
  %6 = bitcast i64* %5 to i8*
  %7 = alloca i64, i64 %2, align 16
  %8 = icmp sgt i64 %2, 0
  br i1 %8, label %9, label %12

; <label>:9:                                      ; preds = %3
  %10 = bitcast i64* %7 to i8*
  %11 = shl i64 %2, 3
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 %10, i8 0, i64 %11, i1 false)
  br label %12

; <label>:12:                                     ; preds = %9, %3
  %13 = icmp sgt i64 %1, 0
  br i1 %13, label %14, label %34

; <label>:14:                                     ; preds = %12
  %15 = add i64 %1, -1
  %16 = and i64 %1, 3
  %17 = icmp ult i64 %15, 3
  br i1 %17, label %20, label %18

; <label>:18:                                     ; preds = %14
  %19 = sub i64 %1, %16
  br label %41

; <label>:20:                                     ; preds = %41, %14
  %21 = phi i64 [ 0, %14 ], [ %67, %41 ]
  %22 = icmp eq i64 %16, 0
  br i1 %22, label %34, label %23

; <label>:23:                                     ; preds = %20, %23
  %24 = phi i64 [ %31, %23 ], [ %21, %20 ]
  %25 = phi i64 [ %32, %23 ], [ %16, %20 ]
  %26 = getelementptr inbounds i64, i64* %0, i64 %24
  %27 = load i64, i64* %26, align 8, !tbaa !4
  %28 = getelementptr inbounds i64, i64* %7, i64 %27
  %29 = load i64, i64* %28, align 8, !tbaa !4
  %30 = add nsw i64 %29, 1
  store i64 %30, i64* %28, align 8, !tbaa !4
  %31 = add nuw nsw i64 %24, 1
  %32 = add i64 %25, -1
  %33 = icmp eq i64 %32, 0
  br i1 %33, label %34, label %23, !llvm.loop !8

; <label>:34:                                     ; preds = %20, %23, %12
  br i1 %8, label %35, label %84

; <label>:35:                                     ; preds = %34
  %36 = add i64 %2, -1
  %37 = and i64 %2, 3
  %38 = icmp ult i64 %36, 3
  br i1 %38, label %70, label %39

; <label>:39:                                     ; preds = %35
  %40 = sub i64 %2, %37
  br label %90

; <label>:41:                                     ; preds = %41, %18
  %42 = phi i64 [ 0, %18 ], [ %67, %41 ]
  %43 = phi i64 [ %19, %18 ], [ %68, %41 ]
  %44 = getelementptr inbounds i64, i64* %0, i64 %42
  %45 = load i64, i64* %44, align 8, !tbaa !4
  %46 = getelementptr inbounds i64, i64* %7, i64 %45
  %47 = load i64, i64* %46, align 8, !tbaa !4
  %48 = add nsw i64 %47, 1
  store i64 %48, i64* %46, align 8, !tbaa !4
  %49 = or i64 %42, 1
  %50 = getelementptr inbounds i64, i64* %0, i64 %49
  %51 = load i64, i64* %50, align 8, !tbaa !4
  %52 = getelementptr inbounds i64, i64* %7, i64 %51
  %53 = load i64, i64* %52, align 8, !tbaa !4
  %54 = add nsw i64 %53, 1
  store i64 %54, i64* %52, align 8, !tbaa !4
  %55 = or i64 %42, 2
  %56 = getelementptr inbounds i64, i64* %0, i64 %55
  %57 = load i64, i64* %56, align 8, !tbaa !4
  %58 = getelementptr inbounds i64, i64* %7, i64 %57
  %59 = load i64, i64* %58, align 8, !tbaa !4
  %60 = add nsw i64 %59, 1
  store i64 %60, i64* %58, align 8, !tbaa !4
  %61 = or i64 %42, 3
  %62 = getelementptr inbounds i64, i64* %0, i64 %61
  %63 = load i64, i64* %62, align 8, !tbaa !4
  %64 = getelementptr inbounds i64, i64* %7, i64 %63
  %65 = load i64, i64* %64, align 8, !tbaa !4
  %66 = add nsw i64 %65, 1
  store i64 %66, i64* %64, align 8, !tbaa !4
  %67 = add nuw nsw i64 %42, 4
  %68 = add i64 %43, -4
  %69 = icmp eq i64 %68, 0
  br i1 %69, label %20, label %41

; <label>:70:                                     ; preds = %90, %35
  %71 = phi i64 [ 0, %35 ], [ %109, %90 ]
  %72 = phi i64 [ 0, %35 ], [ %108, %90 ]
  %73 = icmp eq i64 %37, 0
  br i1 %73, label %84, label %74

; <label>:74:                                     ; preds = %70, %74
  %75 = phi i64 [ %81, %74 ], [ %71, %70 ]
  %76 = phi i64 [ %80, %74 ], [ %72, %70 ]
  %77 = phi i64 [ %82, %74 ], [ %37, %70 ]
  %78 = getelementptr inbounds i64, i64* %7, i64 %75
  %79 = load i64, i64* %78, align 8, !tbaa !4
  store i64 %76, i64* %78, align 8, !tbaa !4
  %80 = add nsw i64 %79, %76
  %81 = add nuw nsw i64 %75, 1
  %82 = add i64 %77, -1
  %83 = icmp eq i64 %82, 0
  br i1 %83, label %84, label %74, !llvm.loop !10

; <label>:84:                                     ; preds = %70, %74, %34
  br i1 %13, label %85, label %144

; <label>:85:                                     ; preds = %84
  %86 = and i64 %1, 1
  %87 = icmp eq i64 %1, 1
  br i1 %87, label %112, label %88

; <label>:88:                                     ; preds = %85
  %89 = sub i64 %1, %86
  br label %125

; <label>:90:                                     ; preds = %90, %39
  %91 = phi i64 [ 0, %39 ], [ %109, %90 ]
  %92 = phi i64 [ 0, %39 ], [ %108, %90 ]
  %93 = phi i64 [ %40, %39 ], [ %110, %90 ]
  %94 = getelementptr inbounds i64, i64* %7, i64 %91
  %95 = load i64, i64* %94, align 16, !tbaa !4
  store i64 %92, i64* %94, align 16, !tbaa !4
  %96 = add nsw i64 %95, %92
  %97 = or i64 %91, 1
  %98 = getelementptr inbounds i64, i64* %7, i64 %97
  %99 = load i64, i64* %98, align 8, !tbaa !4
  store i64 %96, i64* %98, align 8, !tbaa !4
  %100 = add nsw i64 %99, %96
  %101 = or i64 %91, 2
  %102 = getelementptr inbounds i64, i64* %7, i64 %101
  %103 = load i64, i64* %102, align 16, !tbaa !4
  store i64 %100, i64* %102, align 16, !tbaa !4
  %104 = add nsw i64 %103, %100
  %105 = or i64 %91, 3
  %106 = getelementptr inbounds i64, i64* %7, i64 %105
  %107 = load i64, i64* %106, align 8, !tbaa !4
  store i64 %104, i64* %106, align 8, !tbaa !4
  %108 = add nsw i64 %107, %104
  %109 = add nuw nsw i64 %91, 4
  %110 = add i64 %93, -4
  %111 = icmp eq i64 %110, 0
  br i1 %111, label %70, label %90

; <label>:112:                                    ; preds = %125, %85
  %113 = phi i64 [ 0, %85 ], [ %141, %125 ]
  %114 = icmp eq i64 %86, 0
  br i1 %114, label %122, label %115

; <label>:115:                                    ; preds = %112
  %116 = getelementptr inbounds i64, i64* %0, i64 %113
  %117 = load i64, i64* %116, align 8, !tbaa !4
  %118 = getelementptr inbounds i64, i64* %7, i64 %117
  %119 = load i64, i64* %118, align 8, !tbaa !4
  %120 = getelementptr inbounds i64, i64* %5, i64 %119
  store i64 %117, i64* %120, align 8, !tbaa !4
  %121 = add nsw i64 %119, 1
  store i64 %121, i64* %118, align 8, !tbaa !4
  br label %122

; <label>:122:                                    ; preds = %112, %115
  br i1 %13, label %123, label %144

; <label>:123:                                    ; preds = %122
  %124 = shl i64 %1, 3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 8 %4, i8* nonnull align 16 %6, i64 %124, i1 false)
  br label %144

; <label>:125:                                    ; preds = %125, %88
  %126 = phi i64 [ 0, %88 ], [ %141, %125 ]
  %127 = phi i64 [ %89, %88 ], [ %142, %125 ]
  %128 = getelementptr inbounds i64, i64* %0, i64 %126
  %129 = load i64, i64* %128, align 8, !tbaa !4
  %130 = getelementptr inbounds i64, i64* %7, i64 %129
  %131 = load i64, i64* %130, align 8, !tbaa !4
  %132 = getelementptr inbounds i64, i64* %5, i64 %131
  store i64 %129, i64* %132, align 8, !tbaa !4
  %133 = add nsw i64 %131, 1
  store i64 %133, i64* %130, align 8, !tbaa !4
  %134 = or i64 %126, 1
  %135 = getelementptr inbounds i64, i64* %0, i64 %134
  %136 = load i64, i64* %135, align 8, !tbaa !4
  %137 = getelementptr inbounds i64, i64* %7, i64 %136
  %138 = load i64, i64* %137, align 8, !tbaa !4
  %139 = getelementptr inbounds i64, i64* %5, i64 %138
  store i64 %136, i64* %139, align 8, !tbaa !4
  %140 = add nsw i64 %138, 1
  store i64 %140, i64* %137, align 8, !tbaa !4
  %141 = add nuw nsw i64 %126, 2
  %142 = add i64 %127, -2
  %143 = icmp eq i64 %142, 0
  br i1 %143, label %112, label %125

; <label>:144:                                    ; preds = %84, %123, %122
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1) #1

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
