; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: nounwind sspstrong uwtable
define dso_local void @qSortI(i64* nocapture, i64) local_unnamed_addr #0 {
  %3 = alloca [50 x i64], align 16
  %4 = alloca [50 x i64], align 16
  %5 = bitcast [50 x i64]* %3 to i8*
  call void @llvm.lifetime.start.p0i8(i64 400, i8* nonnull %5) #2
  %6 = bitcast [50 x i64]* %4 to i8*
  call void @llvm.lifetime.start.p0i8(i64 400, i8* nonnull %6) #2
  %7 = getelementptr inbounds [50 x i64], [50 x i64]* %3, i64 0, i64 1
  store i64 0, i64* %7, align 8, !tbaa !4
  %8 = add nsw i64 %1, -1
  %9 = getelementptr inbounds [50 x i64], [50 x i64]* %4, i64 0, i64 1
  store i64 %8, i64* %9, align 8, !tbaa !4
  br label %10

; <label>:10:                                     ; preds = %69, %2
  %11 = phi i64 [ %8, %2 ], [ %73, %69 ]
  %12 = phi i64 [ 0, %2 ], [ %71, %69 ]
  %13 = phi i64 [ 1, %2 ], [ %64, %69 ]
  %14 = add nsw i64 %13, -1
  br label %15

; <label>:15:                                     ; preds = %62, %10
  %16 = phi i64 [ %11, %10 ], [ %63, %62 ]
  %17 = phi i64 [ %14, %10 ], [ %64, %62 ]
  %18 = phi i64 [ %12, %10 ], [ %65, %62 ]
  %19 = add nsw i64 %18, %16
  %20 = ashr i64 %19, 1
  %21 = getelementptr inbounds i64, i64* %0, i64 %20
  %22 = load i64, i64* %21, align 8, !tbaa !4
  br label %23

; <label>:23:                                     ; preds = %44, %15
  %24 = phi i64 [ %16, %15 ], [ %45, %44 ]
  %25 = phi i64 [ %18, %15 ], [ %46, %44 ]
  br label %26

; <label>:26:                                     ; preds = %26, %23
  %27 = phi i64 [ %25, %23 ], [ %31, %26 ]
  %28 = getelementptr inbounds i64, i64* %0, i64 %27
  %29 = load i64, i64* %28, align 8, !tbaa !4
  %30 = icmp slt i64 %29, %22
  %31 = add nsw i64 %27, 1
  br i1 %30, label %26, label %32

; <label>:32:                                     ; preds = %26
  %33 = getelementptr inbounds i64, i64* %0, i64 %27
  br label %34

; <label>:34:                                     ; preds = %34, %32
  %35 = phi i64 [ %39, %34 ], [ %24, %32 ]
  %36 = getelementptr inbounds i64, i64* %0, i64 %35
  %37 = load i64, i64* %36, align 8, !tbaa !4
  %38 = icmp slt i64 %22, %37
  %39 = add nsw i64 %35, -1
  br i1 %38, label %34, label %40

; <label>:40:                                     ; preds = %34
  %41 = icmp sgt i64 %27, %35
  br i1 %41, label %44, label %42

; <label>:42:                                     ; preds = %40
  %43 = getelementptr inbounds i64, i64* %0, i64 %35
  store i64 %37, i64* %33, align 8, !tbaa !4
  store i64 %29, i64* %43, align 8, !tbaa !4
  br label %44

; <label>:44:                                     ; preds = %40, %42
  %45 = phi i64 [ %39, %42 ], [ %35, %40 ]
  %46 = phi i64 [ %31, %42 ], [ %27, %40 ]
  %47 = icmp sgt i64 %46, %45
  br i1 %47, label %48, label %23

; <label>:48:                                     ; preds = %44
  %49 = icmp slt i64 %46, %20
  br i1 %49, label %50, label %56

; <label>:50:                                     ; preds = %48
  %51 = icmp slt i64 %46, %16
  br i1 %51, label %52, label %62

; <label>:52:                                     ; preds = %50
  %53 = add nsw i64 %17, 1
  %54 = getelementptr inbounds [50 x i64], [50 x i64]* %3, i64 0, i64 %53
  store i64 %46, i64* %54, align 8, !tbaa !4
  %55 = getelementptr inbounds [50 x i64], [50 x i64]* %4, i64 0, i64 %53
  store i64 %16, i64* %55, align 8, !tbaa !4
  br label %62

; <label>:56:                                     ; preds = %48
  %57 = icmp sgt i64 %45, %18
  br i1 %57, label %58, label %62

; <label>:58:                                     ; preds = %56
  %59 = add nsw i64 %17, 1
  %60 = getelementptr inbounds [50 x i64], [50 x i64]* %3, i64 0, i64 %59
  store i64 %18, i64* %60, align 8, !tbaa !4
  %61 = getelementptr inbounds [50 x i64], [50 x i64]* %4, i64 0, i64 %59
  store i64 %45, i64* %61, align 8, !tbaa !4
  br label %62

; <label>:62:                                     ; preds = %56, %58, %50, %52
  %63 = phi i64 [ %45, %52 ], [ %45, %50 ], [ %16, %58 ], [ %16, %56 ]
  %64 = phi i64 [ %53, %52 ], [ %17, %50 ], [ %59, %58 ], [ %17, %56 ]
  %65 = phi i64 [ %18, %52 ], [ %18, %50 ], [ %46, %58 ], [ %46, %56 ]
  %66 = icmp slt i64 %65, %63
  br i1 %66, label %15, label %67

; <label>:67:                                     ; preds = %62
  %68 = icmp eq i64 %64, 0
  br i1 %68, label %74, label %69

; <label>:69:                                     ; preds = %67
  %70 = getelementptr inbounds [50 x i64], [50 x i64]* %3, i64 0, i64 %64
  %71 = load i64, i64* %70, align 8, !tbaa !4
  %72 = getelementptr inbounds [50 x i64], [50 x i64]* %4, i64 0, i64 %64
  %73 = load i64, i64* %72, align 8, !tbaa !4
  br label %10

; <label>:74:                                     ; preds = %67
  call void @llvm.lifetime.end.p0i8(i64 400, i8* nonnull %6) #2
  call void @llvm.lifetime.end.p0i8(i64 400, i8* nonnull %5) #2
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #1

attributes #0 = { nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nounwind }

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
