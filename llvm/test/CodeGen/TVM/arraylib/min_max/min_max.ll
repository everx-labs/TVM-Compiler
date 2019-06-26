; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

%struct.result = type { %struct.item, %struct.item }
%struct.item = type { i64, i64 }

@Result = common dso_local global %struct.result zeroinitializer, align 8

; Function Attrs: norecurse nounwind sspstrong uwtable
define dso_local %struct.result* @min_max(i64* nocapture readonly, i64) local_unnamed_addr #0 {
  %3 = icmp slt i64 %1, 1
  br i1 %3, label %47, label %4

; <label>:4:                                      ; preds = %2
  %5 = load i64, i64* %0, align 8, !tbaa !4
  store i64 %5, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 0, i32 0), align 8, !tbaa !8
  store i64 0, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 0, i32 1), align 8, !tbaa !11
  %6 = load i64, i64* %0, align 8, !tbaa !4
  store i64 %6, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 1, i32 0), align 8, !tbaa !12
  store i64 0, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 1, i32 1), align 8, !tbaa !13
  %7 = and i64 %1, 1
  %8 = icmp eq i64 %1, 1
  br i1 %8, label %32, label %9

; <label>:9:                                      ; preds = %4
  %10 = sub i64 %1, %7
  br label %11

; <label>:11:                                     ; preds = %56, %9
  %12 = phi i64 [ %5, %9 ], [ %57, %56 ]
  %13 = phi i64 [ %6, %9 ], [ %53, %56 ]
  %14 = phi i64 [ 0, %9 ], [ %58, %56 ]
  %15 = phi i64 [ %10, %9 ], [ %59, %56 ]
  %16 = getelementptr inbounds i64, i64* %0, i64 %14
  %17 = load i64, i64* %16, align 8, !tbaa !4
  %18 = icmp sgt i64 %17, %13
  br i1 %18, label %19, label %21

; <label>:19:                                     ; preds = %11
  store i64 %17, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 1, i32 0), align 8, !tbaa !12
  store i64 %14, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 1, i32 1), align 8, !tbaa !13
  %20 = load i64, i64* %16, align 8, !tbaa !4
  br label %21

; <label>:21:                                     ; preds = %19, %11
  %22 = phi i64 [ %20, %19 ], [ %17, %11 ]
  %23 = phi i64 [ %17, %19 ], [ %13, %11 ]
  %24 = icmp slt i64 %22, %12
  br i1 %24, label %25, label %26

; <label>:25:                                     ; preds = %21
  store i64 %22, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 0, i32 0), align 8, !tbaa !8
  store i64 %14, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 0, i32 1), align 8, !tbaa !11
  br label %26

; <label>:26:                                     ; preds = %21, %25
  %27 = phi i64 [ %12, %21 ], [ %22, %25 ]
  %28 = or i64 %14, 1
  %29 = getelementptr inbounds i64, i64* %0, i64 %28
  %30 = load i64, i64* %29, align 8, !tbaa !4
  %31 = icmp sgt i64 %30, %23
  br i1 %31, label %49, label %51

; <label>:32:                                     ; preds = %56, %4
  %33 = phi i64 [ %5, %4 ], [ %57, %56 ]
  %34 = phi i64 [ %6, %4 ], [ %53, %56 ]
  %35 = phi i64 [ 0, %4 ], [ %58, %56 ]
  %36 = icmp eq i64 %7, 0
  br i1 %36, label %47, label %37

; <label>:37:                                     ; preds = %32
  %38 = getelementptr inbounds i64, i64* %0, i64 %35
  %39 = load i64, i64* %38, align 8, !tbaa !4
  %40 = icmp sgt i64 %39, %34
  br i1 %40, label %41, label %43

; <label>:41:                                     ; preds = %37
  store i64 %39, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 1, i32 0), align 8, !tbaa !12
  store i64 %35, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 1, i32 1), align 8, !tbaa !13
  %42 = load i64, i64* %38, align 8, !tbaa !4
  br label %43

; <label>:43:                                     ; preds = %41, %37
  %44 = phi i64 [ %42, %41 ], [ %39, %37 ]
  %45 = icmp slt i64 %44, %33
  br i1 %45, label %46, label %47

; <label>:46:                                     ; preds = %43
  store i64 %44, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 0, i32 0), align 8, !tbaa !8
  store i64 %35, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 0, i32 1), align 8, !tbaa !11
  br label %47

; <label>:47:                                     ; preds = %32, %43, %46, %2
  %48 = phi %struct.result* [ null, %2 ], [ @Result, %46 ], [ @Result, %43 ], [ @Result, %32 ]
  ret %struct.result* %48

; <label>:49:                                     ; preds = %26
  store i64 %30, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 1, i32 0), align 8, !tbaa !12
  store i64 %28, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 1, i32 1), align 8, !tbaa !13
  %50 = load i64, i64* %29, align 8, !tbaa !4
  br label %51

; <label>:51:                                     ; preds = %49, %26
  %52 = phi i64 [ %50, %49 ], [ %30, %26 ]
  %53 = phi i64 [ %30, %49 ], [ %23, %26 ]
  %54 = icmp slt i64 %52, %27
  br i1 %54, label %55, label %56

; <label>:55:                                     ; preds = %51
  store i64 %52, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 0, i32 0), align 8, !tbaa !8
  store i64 %28, i64* getelementptr inbounds (%struct.result, %struct.result* @Result, i64 0, i32 0, i32 1), align 8, !tbaa !11
  br label %56

; <label>:56:                                     ; preds = %55, %51
  %57 = phi i64 [ %27, %51 ], [ %52, %55 ]
  %58 = add nuw nsw i64 %14, 2
  %59 = add i64 %15, -2
  %60 = icmp eq i64 %59, 0
  br i1 %60, label %32, label %11
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
!8 = !{!9, !5, i64 0}
!9 = !{!"result", !10, i64 0, !10, i64 16}
!10 = !{!"item", !5, i64 0, !5, i64 8}
!11 = !{!9, !5, i64 8}
!12 = !{!9, !5, i64 16}
!13 = !{!9, !5, i64 24}
