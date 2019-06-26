; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind sspstrong uwtable
define dso_local void @selectSort(i64* nocapture, i64) local_unnamed_addr #0 {
  %3 = icmp sgt i64 %1, 0
  br i1 %3, label %4, label %72

; <label>:4:                                      ; preds = %2
  %5 = add i64 %1, -2
  br label %6

; <label>:6:                                      ; preds = %4, %67
  %7 = phi i64 [ %11, %67 ], [ 0, %4 ]
  %8 = sub i64 %5, %7
  %9 = getelementptr inbounds i64, i64* %0, i64 %7
  %10 = load i64, i64* %9, align 8, !tbaa !4
  %11 = add nuw nsw i64 %7, 1
  %12 = icmp slt i64 %11, %1
  br i1 %12, label %13, label %67

; <label>:13:                                     ; preds = %6
  %14 = xor i64 %7, 3
  %15 = add i64 %14, %1
  %16 = and i64 %15, 3
  %17 = icmp eq i64 %16, 0
  br i1 %17, label %31, label %18

; <label>:18:                                     ; preds = %13, %18
  %19 = phi i64 [ %27, %18 ], [ %7, %13 ]
  %20 = phi i64 [ %28, %18 ], [ %11, %13 ]
  %21 = phi i64 [ %26, %18 ], [ %10, %13 ]
  %22 = phi i64 [ %29, %18 ], [ %16, %13 ]
  %23 = getelementptr inbounds i64, i64* %0, i64 %20
  %24 = load i64, i64* %23, align 8, !tbaa !4
  %25 = icmp slt i64 %24, %21
  %26 = select i1 %25, i64 %24, i64 %21
  %27 = select i1 %25, i64 %20, i64 %19
  %28 = add nuw nsw i64 %20, 1
  %29 = add i64 %22, -1
  %30 = icmp eq i64 %29, 0
  br i1 %30, label %31, label %18, !llvm.loop !8

; <label>:31:                                     ; preds = %18, %13
  %32 = phi i64 [ undef, %13 ], [ %26, %18 ]
  %33 = phi i64 [ undef, %13 ], [ %27, %18 ]
  %34 = phi i64 [ %7, %13 ], [ %27, %18 ]
  %35 = phi i64 [ %11, %13 ], [ %28, %18 ]
  %36 = phi i64 [ %10, %13 ], [ %26, %18 ]
  %37 = icmp ult i64 %8, 3
  br i1 %37, label %67, label %38

; <label>:38:                                     ; preds = %31, %38
  %39 = phi i64 [ %64, %38 ], [ %34, %31 ]
  %40 = phi i64 [ %65, %38 ], [ %35, %31 ]
  %41 = phi i64 [ %63, %38 ], [ %36, %31 ]
  %42 = getelementptr inbounds i64, i64* %0, i64 %40
  %43 = load i64, i64* %42, align 8, !tbaa !4
  %44 = icmp slt i64 %43, %41
  %45 = select i1 %44, i64 %43, i64 %41
  %46 = select i1 %44, i64 %40, i64 %39
  %47 = add nuw nsw i64 %40, 1
  %48 = getelementptr inbounds i64, i64* %0, i64 %47
  %49 = load i64, i64* %48, align 8, !tbaa !4
  %50 = icmp slt i64 %49, %45
  %51 = select i1 %50, i64 %49, i64 %45
  %52 = select i1 %50, i64 %47, i64 %46
  %53 = add nsw i64 %40, 2
  %54 = getelementptr inbounds i64, i64* %0, i64 %53
  %55 = load i64, i64* %54, align 8, !tbaa !4
  %56 = icmp slt i64 %55, %51
  %57 = select i1 %56, i64 %55, i64 %51
  %58 = select i1 %56, i64 %53, i64 %52
  %59 = add nsw i64 %40, 3
  %60 = getelementptr inbounds i64, i64* %0, i64 %59
  %61 = load i64, i64* %60, align 8, !tbaa !4
  %62 = icmp slt i64 %61, %57
  %63 = select i1 %62, i64 %61, i64 %57
  %64 = select i1 %62, i64 %59, i64 %58
  %65 = add nsw i64 %40, 4
  %66 = icmp eq i64 %65, %1
  br i1 %66, label %67, label %38

; <label>:67:                                     ; preds = %31, %38, %6
  %68 = phi i64 [ %10, %6 ], [ %32, %31 ], [ %63, %38 ]
  %69 = phi i64 [ %7, %6 ], [ %33, %31 ], [ %64, %38 ]
  %70 = getelementptr inbounds i64, i64* %0, i64 %69
  store i64 %10, i64* %70, align 8, !tbaa !4
  store i64 %68, i64* %9, align 8, !tbaa !4
  %71 = icmp eq i64 %11, %1
  br i1 %71, label %72, label %6

; <label>:72:                                     ; preds = %67, %2
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
!8 = distinct !{!8, !9}
!9 = !{!"llvm.loop.unroll.disable"}
