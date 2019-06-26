; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind sspstrong uwtable
define dso_local void @shaker_sort(i64* nocapture, i64) local_unnamed_addr #0 {
  %3 = add nsw i64 %1, -1
  %4 = icmp sgt i64 %3, 0
  br i1 %4, label %5, label %29

; <label>:5:                                      ; preds = %2
  %6 = add i64 %1, -2
  %7 = add i64 %1, -3
  %8 = and i64 %1, 1
  %9 = icmp eq i64 %8, 0
  %10 = and i64 %6, 1
  %11 = icmp eq i64 %10, 0
  br label %12

; <label>:12:                                     ; preds = %5, %63
  %13 = phi i64 [ %64, %63 ], [ 0, %5 ]
  %14 = phi i64 [ %31, %63 ], [ %3, %5 ]
  %15 = getelementptr inbounds i64, i64* %0, i64 %13
  %16 = load i64, i64* %15, align 8, !tbaa !4
  br i1 %9, label %17, label %24

; <label>:17:                                     ; preds = %12
  %18 = add nuw nsw i64 %13, 1
  %19 = getelementptr inbounds i64, i64* %0, i64 %18
  %20 = load i64, i64* %19, align 8, !tbaa !4
  %21 = icmp slt i64 %20, %16
  br i1 %21, label %22, label %24

; <label>:22:                                     ; preds = %17
  %23 = getelementptr inbounds i64, i64* %0, i64 %13
  store i64 %20, i64* %23, align 8, !tbaa !4
  store i64 %16, i64* %19, align 8, !tbaa !4
  br label %24

; <label>:24:                                     ; preds = %12, %22, %17
  %25 = phi i64 [ %16, %12 ], [ %20, %17 ], [ %16, %22 ]
  %26 = phi i64 [ %13, %12 ], [ %18, %22 ], [ %18, %17 ]
  %27 = shl nuw i64 %13, 1
  %28 = icmp eq i64 %6, %27
  br i1 %28, label %30, label %48

; <label>:29:                                     ; preds = %63, %2
  ret void

; <label>:30:                                     ; preds = %83, %24
  %31 = add nsw i64 %14, -1
  %32 = icmp sgt i64 %31, %13
  br i1 %32, label %33, label %63

; <label>:33:                                     ; preds = %30
  %34 = getelementptr inbounds i64, i64* %0, i64 %31
  %35 = load i64, i64* %34, align 8, !tbaa !4
  br i1 %11, label %43, label %36

; <label>:36:                                     ; preds = %33
  %37 = add nsw i64 %14, -2
  %38 = getelementptr inbounds i64, i64* %0, i64 %37
  %39 = load i64, i64* %38, align 8, !tbaa !4
  %40 = icmp sgt i64 %39, %35
  br i1 %40, label %41, label %43

; <label>:41:                                     ; preds = %36
  %42 = getelementptr inbounds i64, i64* %0, i64 %31
  store i64 %35, i64* %38, align 8, !tbaa !4
  store i64 %39, i64* %42, align 8, !tbaa !4
  br label %43

; <label>:43:                                     ; preds = %33, %41, %36
  %44 = phi i64 [ %35, %33 ], [ %39, %36 ], [ %35, %41 ]
  %45 = phi i64 [ %31, %33 ], [ %37, %41 ], [ %37, %36 ]
  %46 = shl nuw i64 %13, 1
  %47 = icmp eq i64 %7, %46
  br i1 %47, label %63, label %66

; <label>:48:                                     ; preds = %24, %83
  %49 = phi i64 [ %84, %83 ], [ %25, %24 ]
  %50 = phi i64 [ %59, %83 ], [ %26, %24 ]
  %51 = add nuw nsw i64 %50, 1
  %52 = getelementptr inbounds i64, i64* %0, i64 %51
  %53 = load i64, i64* %52, align 8, !tbaa !4
  %54 = icmp slt i64 %53, %49
  br i1 %54, label %55, label %57

; <label>:55:                                     ; preds = %48
  %56 = getelementptr inbounds i64, i64* %0, i64 %50
  store i64 %53, i64* %56, align 8, !tbaa !4
  store i64 %49, i64* %52, align 8, !tbaa !4
  br label %57

; <label>:57:                                     ; preds = %48, %55
  %58 = phi i64 [ %53, %48 ], [ %49, %55 ]
  %59 = add nsw i64 %50, 2
  %60 = getelementptr inbounds i64, i64* %0, i64 %59
  %61 = load i64, i64* %60, align 8, !tbaa !4
  %62 = icmp slt i64 %61, %58
  br i1 %62, label %81, label %83

; <label>:63:                                     ; preds = %43, %88, %30
  %64 = add nuw nsw i64 %13, 1
  %65 = icmp slt i64 %64, %31
  br i1 %65, label %12, label %29

; <label>:66:                                     ; preds = %43, %88
  %67 = phi i64 [ %89, %88 ], [ %44, %43 ]
  %68 = phi i64 [ %77, %88 ], [ %45, %43 ]
  %69 = add nsw i64 %68, -1
  %70 = getelementptr inbounds i64, i64* %0, i64 %69
  %71 = load i64, i64* %70, align 8, !tbaa !4
  %72 = icmp sgt i64 %71, %67
  br i1 %72, label %73, label %75

; <label>:73:                                     ; preds = %66
  %74 = getelementptr inbounds i64, i64* %0, i64 %68
  store i64 %67, i64* %70, align 8, !tbaa !4
  store i64 %71, i64* %74, align 8, !tbaa !4
  br label %75

; <label>:75:                                     ; preds = %66, %73
  %76 = phi i64 [ %71, %66 ], [ %67, %73 ]
  %77 = add nsw i64 %68, -2
  %78 = getelementptr inbounds i64, i64* %0, i64 %77
  %79 = load i64, i64* %78, align 8, !tbaa !4
  %80 = icmp sgt i64 %79, %76
  br i1 %80, label %86, label %88

; <label>:81:                                     ; preds = %57
  %82 = getelementptr inbounds i64, i64* %0, i64 %51
  store i64 %61, i64* %82, align 8, !tbaa !4
  store i64 %58, i64* %60, align 8, !tbaa !4
  br label %83

; <label>:83:                                     ; preds = %81, %57
  %84 = phi i64 [ %61, %57 ], [ %58, %81 ]
  %85 = icmp eq i64 %59, %14
  br i1 %85, label %30, label %48

; <label>:86:                                     ; preds = %75
  %87 = getelementptr inbounds i64, i64* %0, i64 %69
  store i64 %76, i64* %78, align 8, !tbaa !4
  store i64 %79, i64* %87, align 8, !tbaa !4
  br label %88

; <label>:88:                                     ; preds = %86, %75
  %89 = phi i64 [ %79, %75 ], [ %76, %86 ]
  %90 = icmp sgt i64 %77, %13
  br i1 %90, label %66, label %63
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
