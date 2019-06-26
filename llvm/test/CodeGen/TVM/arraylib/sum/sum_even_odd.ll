; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readonly sspstrong uwtable
define dso_local i64 @sum_even(i64* nocapture readonly, i64) local_unnamed_addr #0 {
  %3 = icmp sgt i64 %1, 0
  br i1 %3, label %4, label %27

; <label>:4:                                      ; preds = %2
  %5 = add i64 %1, -1
  %6 = lshr i64 %5, 1
  %7 = add nuw i64 %6, 1
  %8 = and i64 %7, 7
  %9 = icmp ult i64 %5, 14
  br i1 %9, label %12, label %10

; <label>:10:                                     ; preds = %4
  %11 = sub i64 %7, %8
  br label %29

; <label>:12:                                     ; preds = %29, %4
  %13 = phi i64 [ undef, %4 ], [ %63, %29 ]
  %14 = phi i64 [ 0, %4 ], [ %64, %29 ]
  %15 = phi i64 [ 0, %4 ], [ %63, %29 ]
  %16 = icmp eq i64 %8, 0
  br i1 %16, label %27, label %17

; <label>:17:                                     ; preds = %12, %17
  %18 = phi i64 [ %24, %17 ], [ %14, %12 ]
  %19 = phi i64 [ %23, %17 ], [ %15, %12 ]
  %20 = phi i64 [ %25, %17 ], [ %8, %12 ]
  %21 = getelementptr inbounds i64, i64* %0, i64 %18
  %22 = load i64, i64* %21, align 8, !tbaa !4
  %23 = add nsw i64 %22, %19
  %24 = add nuw nsw i64 %18, 2
  %25 = add i64 %20, -1
  %26 = icmp eq i64 %25, 0
  br i1 %26, label %27, label %17, !llvm.loop !8

; <label>:27:                                     ; preds = %12, %17, %2
  %28 = phi i64 [ 0, %2 ], [ %13, %12 ], [ %23, %17 ]
  ret i64 %28

; <label>:29:                                     ; preds = %29, %10
  %30 = phi i64 [ 0, %10 ], [ %64, %29 ]
  %31 = phi i64 [ 0, %10 ], [ %63, %29 ]
  %32 = phi i64 [ %11, %10 ], [ %65, %29 ]
  %33 = getelementptr inbounds i64, i64* %0, i64 %30
  %34 = load i64, i64* %33, align 8, !tbaa !4
  %35 = add nsw i64 %34, %31
  %36 = or i64 %30, 2
  %37 = getelementptr inbounds i64, i64* %0, i64 %36
  %38 = load i64, i64* %37, align 8, !tbaa !4
  %39 = add nsw i64 %38, %35
  %40 = or i64 %30, 4
  %41 = getelementptr inbounds i64, i64* %0, i64 %40
  %42 = load i64, i64* %41, align 8, !tbaa !4
  %43 = add nsw i64 %42, %39
  %44 = or i64 %30, 6
  %45 = getelementptr inbounds i64, i64* %0, i64 %44
  %46 = load i64, i64* %45, align 8, !tbaa !4
  %47 = add nsw i64 %46, %43
  %48 = or i64 %30, 8
  %49 = getelementptr inbounds i64, i64* %0, i64 %48
  %50 = load i64, i64* %49, align 8, !tbaa !4
  %51 = add nsw i64 %50, %47
  %52 = or i64 %30, 10
  %53 = getelementptr inbounds i64, i64* %0, i64 %52
  %54 = load i64, i64* %53, align 8, !tbaa !4
  %55 = add nsw i64 %54, %51
  %56 = or i64 %30, 12
  %57 = getelementptr inbounds i64, i64* %0, i64 %56
  %58 = load i64, i64* %57, align 8, !tbaa !4
  %59 = add nsw i64 %58, %55
  %60 = or i64 %30, 14
  %61 = getelementptr inbounds i64, i64* %0, i64 %60
  %62 = load i64, i64* %61, align 8, !tbaa !4
  %63 = add nsw i64 %62, %59
  %64 = add nuw nsw i64 %30, 16
  %65 = add i64 %32, -8
  %66 = icmp eq i64 %65, 0
  br i1 %66, label %12, label %29
}

; Function Attrs: norecurse nounwind readonly sspstrong uwtable
define dso_local i64 @sum_odd(i64* nocapture readonly, i64) local_unnamed_addr #0 {
  %3 = icmp sgt i64 %1, 1
  br i1 %3, label %4, label %27

; <label>:4:                                      ; preds = %2
  %5 = add i64 %1, -2
  %6 = lshr i64 %5, 1
  %7 = add nuw i64 %6, 1
  %8 = and i64 %7, 7
  %9 = icmp ult i64 %5, 14
  br i1 %9, label %12, label %10

; <label>:10:                                     ; preds = %4
  %11 = sub i64 %7, %8
  br label %29

; <label>:12:                                     ; preds = %29, %4
  %13 = phi i64 [ undef, %4 ], [ %63, %29 ]
  %14 = phi i64 [ 1, %4 ], [ %64, %29 ]
  %15 = phi i64 [ 0, %4 ], [ %63, %29 ]
  %16 = icmp eq i64 %8, 0
  br i1 %16, label %27, label %17

; <label>:17:                                     ; preds = %12, %17
  %18 = phi i64 [ %24, %17 ], [ %14, %12 ]
  %19 = phi i64 [ %23, %17 ], [ %15, %12 ]
  %20 = phi i64 [ %25, %17 ], [ %8, %12 ]
  %21 = getelementptr inbounds i64, i64* %0, i64 %18
  %22 = load i64, i64* %21, align 8, !tbaa !4
  %23 = add nsw i64 %22, %19
  %24 = add nuw nsw i64 %18, 2
  %25 = add i64 %20, -1
  %26 = icmp eq i64 %25, 0
  br i1 %26, label %27, label %17, !llvm.loop !10

; <label>:27:                                     ; preds = %12, %17, %2
  %28 = phi i64 [ 0, %2 ], [ %13, %12 ], [ %23, %17 ]
  ret i64 %28

; <label>:29:                                     ; preds = %29, %10
  %30 = phi i64 [ 1, %10 ], [ %64, %29 ]
  %31 = phi i64 [ 0, %10 ], [ %63, %29 ]
  %32 = phi i64 [ %11, %10 ], [ %65, %29 ]
  %33 = getelementptr inbounds i64, i64* %0, i64 %30
  %34 = load i64, i64* %33, align 8, !tbaa !4
  %35 = add nsw i64 %34, %31
  %36 = add nuw nsw i64 %30, 2
  %37 = getelementptr inbounds i64, i64* %0, i64 %36
  %38 = load i64, i64* %37, align 8, !tbaa !4
  %39 = add nsw i64 %38, %35
  %40 = add nuw nsw i64 %30, 4
  %41 = getelementptr inbounds i64, i64* %0, i64 %40
  %42 = load i64, i64* %41, align 8, !tbaa !4
  %43 = add nsw i64 %42, %39
  %44 = add nuw nsw i64 %30, 6
  %45 = getelementptr inbounds i64, i64* %0, i64 %44
  %46 = load i64, i64* %45, align 8, !tbaa !4
  %47 = add nsw i64 %46, %43
  %48 = add nuw nsw i64 %30, 8
  %49 = getelementptr inbounds i64, i64* %0, i64 %48
  %50 = load i64, i64* %49, align 8, !tbaa !4
  %51 = add nsw i64 %50, %47
  %52 = add nuw nsw i64 %30, 10
  %53 = getelementptr inbounds i64, i64* %0, i64 %52
  %54 = load i64, i64* %53, align 8, !tbaa !4
  %55 = add nsw i64 %54, %51
  %56 = add nuw nsw i64 %30, 12
  %57 = getelementptr inbounds i64, i64* %0, i64 %56
  %58 = load i64, i64* %57, align 8, !tbaa !4
  %59 = add nsw i64 %58, %55
  %60 = add nuw nsw i64 %30, 14
  %61 = getelementptr inbounds i64, i64* %0, i64 %60
  %62 = load i64, i64* %61, align 8, !tbaa !4
  %63 = add nsw i64 %62, %59
  %64 = add nuw nsw i64 %30, 16
  %65 = add i64 %32, -8
  %66 = icmp eq i64 %65, 0
  br i1 %66, label %12, label %29
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
!8 = distinct !{!8, !9}
!9 = !{!"llvm.loop.unroll.disable"}
!10 = distinct !{!10, !9}
