; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readonly sspstrong uwtable
define dso_local i64 @sum(i64* nocapture readonly, i64) local_unnamed_addr #0 {
  %3 = icmp sgt i64 %1, 0
  br i1 %3, label %4, label %25

; <label>:4:                                      ; preds = %2
  %5 = add i64 %1, -1
  %6 = and i64 %1, 7
  %7 = icmp ult i64 %5, 7
  br i1 %7, label %10, label %8

; <label>:8:                                      ; preds = %4
  %9 = sub i64 %1, %6
  br label %27

; <label>:10:                                     ; preds = %27, %4
  %11 = phi i64 [ undef, %4 ], [ %61, %27 ]
  %12 = phi i64 [ 0, %4 ], [ %62, %27 ]
  %13 = phi i64 [ 0, %4 ], [ %61, %27 ]
  %14 = icmp eq i64 %6, 0
  br i1 %14, label %25, label %15

; <label>:15:                                     ; preds = %10, %15
  %16 = phi i64 [ %22, %15 ], [ %12, %10 ]
  %17 = phi i64 [ %21, %15 ], [ %13, %10 ]
  %18 = phi i64 [ %23, %15 ], [ %6, %10 ]
  %19 = getelementptr inbounds i64, i64* %0, i64 %16
  %20 = load i64, i64* %19, align 8, !tbaa !4
  %21 = add nsw i64 %20, %17
  %22 = add nuw nsw i64 %16, 1
  %23 = add i64 %18, -1
  %24 = icmp eq i64 %23, 0
  br i1 %24, label %25, label %15, !llvm.loop !8

; <label>:25:                                     ; preds = %10, %15, %2
  %26 = phi i64 [ 0, %2 ], [ %11, %10 ], [ %21, %15 ]
  ret i64 %26

; <label>:27:                                     ; preds = %27, %8
  %28 = phi i64 [ 0, %8 ], [ %62, %27 ]
  %29 = phi i64 [ 0, %8 ], [ %61, %27 ]
  %30 = phi i64 [ %9, %8 ], [ %63, %27 ]
  %31 = getelementptr inbounds i64, i64* %0, i64 %28
  %32 = load i64, i64* %31, align 8, !tbaa !4
  %33 = add nsw i64 %32, %29
  %34 = or i64 %28, 1
  %35 = getelementptr inbounds i64, i64* %0, i64 %34
  %36 = load i64, i64* %35, align 8, !tbaa !4
  %37 = add nsw i64 %36, %33
  %38 = or i64 %28, 2
  %39 = getelementptr inbounds i64, i64* %0, i64 %38
  %40 = load i64, i64* %39, align 8, !tbaa !4
  %41 = add nsw i64 %40, %37
  %42 = or i64 %28, 3
  %43 = getelementptr inbounds i64, i64* %0, i64 %42
  %44 = load i64, i64* %43, align 8, !tbaa !4
  %45 = add nsw i64 %44, %41
  %46 = or i64 %28, 4
  %47 = getelementptr inbounds i64, i64* %0, i64 %46
  %48 = load i64, i64* %47, align 8, !tbaa !4
  %49 = add nsw i64 %48, %45
  %50 = or i64 %28, 5
  %51 = getelementptr inbounds i64, i64* %0, i64 %50
  %52 = load i64, i64* %51, align 8, !tbaa !4
  %53 = add nsw i64 %52, %49
  %54 = or i64 %28, 6
  %55 = getelementptr inbounds i64, i64* %0, i64 %54
  %56 = load i64, i64* %55, align 8, !tbaa !4
  %57 = add nsw i64 %56, %53
  %58 = or i64 %28, 7
  %59 = getelementptr inbounds i64, i64* %0, i64 %58
  %60 = load i64, i64* %59, align 8, !tbaa !4
  %61 = add nsw i64 %60, %57
  %62 = add nuw nsw i64 %28, 8
  %63 = add i64 %30, -8
  %64 = icmp eq i64 %63, 0
  br i1 %64, label %10, label %27
}

; Function Attrs: nounwind readonly sspstrong uwtable
define dso_local i64 @sumR(i64* nocapture, i64) local_unnamed_addr #1 {
  %3 = icmp slt i64 %1, 1
  br i1 %3, label %12, label %4

; <label>:4:                                      ; preds = %2, %4
  %5 = phi i64 [ %7, %4 ], [ %1, %2 ]
  %6 = phi i64 [ %10, %4 ], [ 0, %2 ]
  %7 = add nsw i64 %5, -1
  %8 = getelementptr inbounds i64, i64* %0, i64 %7
  %9 = load i64, i64* %8, align 8, !tbaa !4
  %10 = add nsw i64 %9, %6
  %11 = icmp slt i64 %5, 2
  br i1 %11, label %12, label %4

; <label>:12:                                     ; preds = %4, %2
  %13 = phi i64 [ 0, %2 ], [ %10, %4 ]
  ret i64 %13
}

attributes #0 = { norecurse nounwind readonly sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readonly sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }

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
