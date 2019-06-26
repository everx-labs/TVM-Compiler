; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind sspstrong uwtable
define dso_local i64 @increment(i64* nocapture, i64) local_unnamed_addr #0 {
  br label %3

; <label>:3:                                      ; preds = %22, %2
  %4 = phi i64 [ 1, %2 ], [ %27, %22 ]
  %5 = phi i64 [ 1, %2 ], [ %24, %22 ]
  %6 = phi i64 [ 1, %2 ], [ %25, %22 ]
  %7 = phi i64 [ -1, %2 ], [ %8, %22 ]
  %8 = add nsw i64 %7, 1
  %9 = and i64 %8, 1
  %10 = icmp eq i64 %9, 0
  br i1 %10, label %16, label %11

; <label>:11:                                     ; preds = %3
  %12 = shl i64 %4, 3
  %13 = mul i64 %5, -6
  %14 = add i64 %13, %12
  %15 = or i64 %14, 1
  br label %22

; <label>:16:                                     ; preds = %3
  %17 = sub i64 %4, %6
  %18 = mul i64 %17, 9
  %19 = add nsw i64 %18, 1
  %20 = shl nsw i64 %5, 1
  %21 = shl nsw i64 %6, 1
  br label %22

; <label>:22:                                     ; preds = %16, %11
  %23 = phi i64 [ %19, %16 ], [ %15, %11 ]
  %24 = phi i64 [ %20, %16 ], [ %5, %11 ]
  %25 = phi i64 [ %21, %16 ], [ %6, %11 ]
  %26 = getelementptr inbounds i64, i64* %0, i64 %8
  store i64 %23, i64* %26, align 8
  %27 = shl nsw i64 %4, 1
  %28 = mul nsw i64 %23, 3
  %29 = icmp slt i64 %28, %1
  br i1 %29, label %3, label %30

; <label>:30:                                     ; preds = %22
  %31 = icmp sgt i64 %7, 0
  %32 = select i1 %31, i64 %7, i64 0
  ret i64 %32
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #1

; Function Attrs: nounwind sspstrong uwtable
define dso_local void @shellSort(i64* nocapture, i64) local_unnamed_addr #2 {
  %3 = alloca [40 x i64], align 16
  %4 = bitcast [40 x i64]* %3 to i8*
  call void @llvm.lifetime.start.p0i8(i64 320, i8* nonnull %4) #3
  br label %5

; <label>:5:                                      ; preds = %24, %2
  %6 = phi i64 [ 1, %2 ], [ %29, %24 ]
  %7 = phi i64 [ 1, %2 ], [ %26, %24 ]
  %8 = phi i64 [ 1, %2 ], [ %27, %24 ]
  %9 = phi i64 [ -1, %2 ], [ %10, %24 ]
  %10 = add nsw i64 %9, 1
  %11 = and i64 %10, 1
  %12 = icmp eq i64 %11, 0
  br i1 %12, label %18, label %13

; <label>:13:                                     ; preds = %5
  %14 = shl i64 %6, 3
  %15 = mul i64 %7, -6
  %16 = add i64 %15, %14
  %17 = or i64 %16, 1
  br label %24

; <label>:18:                                     ; preds = %5
  %19 = sub i64 %6, %8
  %20 = mul i64 %19, 9
  %21 = add nsw i64 %20, 1
  %22 = shl nsw i64 %7, 1
  %23 = shl nsw i64 %8, 1
  br label %24

; <label>:24:                                     ; preds = %18, %13
  %25 = phi i64 [ %21, %18 ], [ %17, %13 ]
  %26 = phi i64 [ %22, %18 ], [ %7, %13 ]
  %27 = phi i64 [ %23, %18 ], [ %8, %13 ]
  %28 = getelementptr inbounds [40 x i64], [40 x i64]* %3, i64 0, i64 %10
  store i64 %25, i64* %28, align 8
  %29 = shl nsw i64 %6, 1
  %30 = mul nsw i64 %25, 3
  %31 = icmp slt i64 %30, %1
  br i1 %31, label %5, label %32

; <label>:32:                                     ; preds = %24
  %33 = icmp sgt i64 %9, 0
  %34 = select i1 %33, i64 %9, i64 0
  %35 = icmp sgt i64 %34, -1
  br i1 %35, label %38, label %63

; <label>:36:                                     ; preds = %58, %38
  %37 = icmp sgt i64 %39, 0
  br i1 %37, label %38, label %63

; <label>:38:                                     ; preds = %32, %36
  %39 = phi i64 [ %40, %36 ], [ %34, %32 ]
  %40 = add nsw i64 %39, -1
  %41 = getelementptr inbounds [40 x i64], [40 x i64]* %3, i64 0, i64 %39
  %42 = load i64, i64* %41, align 8, !tbaa !4
  %43 = icmp slt i64 %42, %1
  br i1 %43, label %44, label %36

; <label>:44:                                     ; preds = %38, %58
  %45 = phi i64 [ %61, %58 ], [ %42, %38 ]
  %46 = getelementptr inbounds i64, i64* %0, i64 %45
  %47 = load i64, i64* %46, align 8, !tbaa !4
  %48 = icmp slt i64 %45, %42
  br i1 %48, label %58, label %49

; <label>:49:                                     ; preds = %44, %55
  %50 = phi i64 [ %51, %55 ], [ %45, %44 ]
  %51 = sub nsw i64 %50, %42
  %52 = getelementptr inbounds i64, i64* %0, i64 %51
  %53 = load i64, i64* %52, align 8, !tbaa !4
  %54 = icmp slt i64 %47, %53
  br i1 %54, label %55, label %58

; <label>:55:                                     ; preds = %49
  %56 = getelementptr inbounds i64, i64* %0, i64 %50
  store i64 %53, i64* %56, align 8, !tbaa !4
  %57 = icmp slt i64 %51, %42
  br i1 %57, label %58, label %49

; <label>:58:                                     ; preds = %49, %55, %44
  %59 = phi i64 [ %45, %44 ], [ %51, %55 ], [ %50, %49 ]
  %60 = getelementptr inbounds i64, i64* %0, i64 %59
  store i64 %47, i64* %60, align 8, !tbaa !4
  %61 = add nsw i64 %45, 1
  %62 = icmp eq i64 %61, %1
  br i1 %62, label %36, label %44

; <label>:63:                                     ; preds = %36, %32
  call void @llvm.lifetime.end.p0i8(i64 320, i8* nonnull %4) #3
  ret void
}

attributes #0 = { norecurse nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

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
