; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind sspstrong uwtable
define dso_local void @merge(i64* nocapture, i64, i64, i64) local_unnamed_addr #0 {
  %5 = sub i64 %2, %1
  %6 = add i64 %5, 1
  %7 = sub i64 %3, %2
  %8 = alloca i64, i64 %6, align 16
  %9 = alloca i64, i64 %7, align 16
  %10 = bitcast i64* %9 to i8*
  %11 = icmp slt i64 %5, 0
  br i1 %11, label %19, label %12

; <label>:12:                                     ; preds = %4
  %13 = bitcast i64* %8 to i8*
  %14 = add i64 %2, 1
  %15 = sub i64 %14, %1
  %16 = getelementptr i64, i64* %0, i64 %1
  %17 = bitcast i64* %16 to i8*
  %18 = shl i64 %15, 3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 16 %13, i8* align 8 %17, i64 %18, i1 false)
  br label %19

; <label>:19:                                     ; preds = %12, %4
  %20 = icmp sgt i64 %7, 0
  br i1 %20, label %21, label %29

; <label>:21:                                     ; preds = %19
  %22 = add i64 %2, 1
  %23 = getelementptr i64, i64* %0, i64 %22
  %24 = bitcast i64* %23 to i8*
  %25 = sub i64 %3, %2
  %26 = shl i64 %25, 3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 16 %10, i8* align 8 %24, i64 %26, i1 false)
  %27 = icmp sgt i64 %5, -1
  %28 = and i1 %20, %27
  br i1 %28, label %41, label %29

; <label>:29:                                     ; preds = %55, %19, %21
  %30 = phi i64 [ %1, %21 ], [ %1, %19 ], [ %58, %55 ]
  %31 = phi i64 [ 0, %21 ], [ 0, %19 ], [ %56, %55 ]
  %32 = phi i64 [ 0, %21 ], [ 0, %19 ], [ %57, %55 ]
  %33 = icmp sgt i64 %32, %5
  br i1 %33, label %62, label %34

; <label>:34:                                     ; preds = %29
  %35 = getelementptr i64, i64* %0, i64 %30
  %36 = bitcast i64* %35 to i8*
  %37 = getelementptr i64, i64* %8, i64 %32
  %38 = bitcast i64* %37 to i8*
  %39 = sub i64 %6, %32
  %40 = shl i64 %39, 3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %36, i8* align 8 %38, i64 %40, i1 false)
  br label %73

; <label>:41:                                     ; preds = %21, %55
  %42 = phi i64 [ %57, %55 ], [ 0, %21 ]
  %43 = phi i64 [ %56, %55 ], [ 0, %21 ]
  %44 = phi i64 [ %58, %55 ], [ %1, %21 ]
  %45 = getelementptr inbounds i64, i64* %8, i64 %42
  %46 = load i64, i64* %45, align 8, !tbaa !4
  %47 = getelementptr inbounds i64, i64* %9, i64 %43
  %48 = load i64, i64* %47, align 8, !tbaa !4
  %49 = icmp sgt i64 %46, %48
  %50 = getelementptr inbounds i64, i64* %0, i64 %44
  br i1 %49, label %53, label %51

; <label>:51:                                     ; preds = %41
  store i64 %46, i64* %50, align 8, !tbaa !4
  %52 = add nsw i64 %42, 1
  br label %55

; <label>:53:                                     ; preds = %41
  store i64 %48, i64* %50, align 8, !tbaa !4
  %54 = add nsw i64 %43, 1
  br label %55

; <label>:55:                                     ; preds = %53, %51
  %56 = phi i64 [ %43, %51 ], [ %54, %53 ]
  %57 = phi i64 [ %52, %51 ], [ %42, %53 ]
  %58 = add nsw i64 %44, 1
  %59 = icmp sle i64 %57, %5
  %60 = icmp slt i64 %56, %7
  %61 = and i1 %60, %59
  br i1 %61, label %41, label %29

; <label>:62:                                     ; preds = %73, %29
  %63 = phi i64 [ %30, %29 ], [ %77, %73 ]
  %64 = icmp slt i64 %31, %7
  br i1 %64, label %65, label %79

; <label>:65:                                     ; preds = %62
  %66 = getelementptr i64, i64* %0, i64 %63
  %67 = bitcast i64* %66 to i8*
  %68 = getelementptr i64, i64* %9, i64 %31
  %69 = bitcast i64* %68 to i8*
  %70 = sub i64 %3, %31
  %71 = sub i64 %70, %2
  %72 = shl i64 %71, 3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %67, i8* align 8 %69, i64 %72, i1 false)
  br label %79

; <label>:73:                                     ; preds = %34, %73
  %74 = phi i64 [ %76, %73 ], [ %32, %34 ]
  %75 = phi i64 [ %77, %73 ], [ %30, %34 ]
  %76 = add nsw i64 %74, 1
  %77 = add nsw i64 %75, 1
  %78 = icmp slt i64 %74, %5
  br i1 %78, label %73, label %62

; <label>:79:                                     ; preds = %65, %62
  ret void
}

; Function Attrs: nounwind sspstrong uwtable
define dso_local void @mergeSort(i64*, i64, i64) local_unnamed_addr #1 {
  %4 = icmp sgt i64 %2, %1
  br i1 %4, label %5, label %10

; <label>:5:                                      ; preds = %3
  %6 = sub nsw i64 %2, %1
  %7 = sdiv i64 %6, 2
  %8 = add nsw i64 %7, %1
  tail call void @mergeSort(i64* %0, i64 %1, i64 %8)
  %9 = add nsw i64 %8, 1
  tail call void @mergeSort(i64* %0, i64 %9, i64 %2)
  tail call void @merge(i64* %0, i64 %1, i64 %8, i64 %2)
  ret void

; <label>:10:                                     ; preds = %3
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1) #2

attributes #0 = { norecurse nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { argmemonly nounwind }

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
