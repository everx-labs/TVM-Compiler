; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

%struct.node = type { i64, %struct.node*, %struct.node* }

@root = dso_local local_unnamed_addr global %struct.node* null, align 8

; Function Attrs: nounwind sspstrong uwtable
define dso_local void @insert(i64) local_unnamed_addr #0 {
  %2 = alloca %struct.node, align 8
  %3 = bitcast %struct.node* %2 to i8*
  call void @llvm.lifetime.start.p0i8(i64 24, i8* nonnull %3) #3
  %4 = getelementptr inbounds %struct.node, %struct.node* %2, i64 0, i64 0
  store i64 %0, i64* %4, align 8, !tbaa !4
  %5 = getelementptr inbounds %struct.node, %struct.node* %2, i64 0, i64 1
  %6 = bitcast %struct.node** %5 to i8*
  call void @llvm.memset.p0i8.i64(i8* nonnull align 8 %6, i8 0, i64 16, i1 false)
  %7 = load %struct.node*, %struct.node** @root, align 8, !tbaa !10
  %8 = icmp eq %struct.node* %7, null
  br i1 %8, label %28, label %9

; <label>:9:                                      ; preds = %1, %24
  %10 = phi %struct.node* [ %25, %24 ], [ %7, %1 ]
  %11 = getelementptr inbounds %struct.node, %struct.node* %10, i64 0, i64 0
  %12 = load i64, i64* %11, align 8, !tbaa !4
  %13 = icmp sgt i64 %12, %0
  br i1 %13, label %14, label %20

; <label>:14:                                     ; preds = %9
  %15 = getelementptr inbounds %struct.node, %struct.node* %10, i64 0, i64 1
  %16 = load %struct.node*, %struct.node** %15, align 8, !tbaa !11
  %17 = icmp eq %struct.node* %16, null
  br i1 %17, label %18, label %24

; <label>:18:                                     ; preds = %14
  %19 = getelementptr inbounds %struct.node, %struct.node* %10, i64 0, i64 1
  br label %28

; <label>:20:                                     ; preds = %9
  %21 = getelementptr inbounds %struct.node, %struct.node* %10, i64 0, i64 2
  %22 = load %struct.node*, %struct.node** %21, align 8, !tbaa !12
  %23 = icmp eq %struct.node* %22, null
  br i1 %23, label %26, label %24

; <label>:24:                                     ; preds = %20, %14
  %25 = phi %struct.node* [ %16, %14 ], [ %22, %20 ]
  br label %9

; <label>:26:                                     ; preds = %20
  %27 = getelementptr inbounds %struct.node, %struct.node* %10, i64 0, i64 2
  br label %28

; <label>:28:                                     ; preds = %1, %26, %18
  %29 = phi %struct.node** [ %27, %26 ], [ %19, %18 ], [ @root, %1 ]
  store %struct.node* %2, %struct.node** %29, align 8, !tbaa !10
  call void @llvm.lifetime.end.p0i8(i64 24, i8* nonnull %3) #3
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #1

; Function Attrs: nounwind readonly sspstrong uwtable
define dso_local void @inorder_traversal(%struct.node* nocapture) local_unnamed_addr #2 {
  ret void
}

; Function Attrs: nounwind sspstrong uwtable
define dso_local void @treeSort(i64* nocapture readonly, i64) local_unnamed_addr #0 {
  %3 = alloca %struct.node, align 8
  %4 = icmp sgt i64 %1, 0
  br i1 %4, label %5, label %41

; <label>:5:                                      ; preds = %2
  %6 = bitcast %struct.node* %3 to i8*
  %7 = getelementptr inbounds %struct.node, %struct.node* %3, i64 0, i64 0
  %8 = getelementptr inbounds %struct.node, %struct.node* %3, i64 0, i64 1
  %9 = bitcast %struct.node** %8 to i8*
  %10 = load %struct.node*, %struct.node** @root, align 8, !tbaa !10
  br label %11

; <label>:11:                                     ; preds = %36, %5
  %12 = phi %struct.node* [ %10, %5 ], [ %38, %36 ]
  %13 = phi i64 [ 0, %5 ], [ %39, %36 ]
  %14 = getelementptr inbounds i64, i64* %0, i64 %13
  %15 = load i64, i64* %14, align 8, !tbaa !13
  call void @llvm.lifetime.start.p0i8(i64 24, i8* nonnull %6) #3
  store i64 %15, i64* %7, align 8, !tbaa !4
  call void @llvm.memset.p0i8.i64(i8* nonnull align 8 %9, i8 0, i64 16, i1 false) #3
  %16 = icmp eq %struct.node* %12, null
  br i1 %16, label %36, label %17

; <label>:17:                                     ; preds = %11, %32
  %18 = phi %struct.node* [ %33, %32 ], [ %12, %11 ]
  %19 = getelementptr inbounds %struct.node, %struct.node* %18, i64 0, i64 0
  %20 = load i64, i64* %19, align 8, !tbaa !4
  %21 = icmp sgt i64 %20, %15
  br i1 %21, label %22, label %28

; <label>:22:                                     ; preds = %17
  %23 = getelementptr inbounds %struct.node, %struct.node* %18, i64 0, i64 1
  %24 = load %struct.node*, %struct.node** %23, align 8, !tbaa !11
  %25 = icmp eq %struct.node* %24, null
  br i1 %25, label %26, label %32

; <label>:26:                                     ; preds = %22
  %27 = getelementptr inbounds %struct.node, %struct.node* %18, i64 0, i64 1
  br label %36

; <label>:28:                                     ; preds = %17
  %29 = getelementptr inbounds %struct.node, %struct.node* %18, i64 0, i64 2
  %30 = load %struct.node*, %struct.node** %29, align 8, !tbaa !12
  %31 = icmp eq %struct.node* %30, null
  br i1 %31, label %34, label %32

; <label>:32:                                     ; preds = %28, %22
  %33 = phi %struct.node* [ %24, %22 ], [ %30, %28 ]
  br label %17

; <label>:34:                                     ; preds = %28
  %35 = getelementptr inbounds %struct.node, %struct.node* %18, i64 0, i64 2
  br label %36

; <label>:36:                                     ; preds = %11, %26, %34
  %37 = phi %struct.node** [ %27, %26 ], [ %35, %34 ], [ @root, %11 ]
  %38 = phi %struct.node* [ %12, %26 ], [ %12, %34 ], [ %3, %11 ]
  store %struct.node* %3, %struct.node** %37, align 8, !tbaa !10
  call void @llvm.lifetime.end.p0i8(i64 24, i8* nonnull %6) #3
  %39 = add nuw nsw i64 %13, 1
  %40 = icmp eq i64 %39, %1
  br i1 %40, label %41, label %11

; <label>:41:                                     ; preds = %36, %2
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1) #1

attributes #0 = { nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nounwind readonly sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i64 1, !"wchar_size", i64 4}
!1 = !{i64 7, !"PIC Level", i64 2}
!2 = !{i64 7, !"PIE Level", i64 2}
!3 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
!4 = !{!5, !6, i64 0}
!5 = !{!"node", !6, i64 0, !9, i64 8, !9, i64 16}
!6 = !{!"long", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = !{!"any pointer", !7, i64 0}
!10 = !{!9, !9, i64 0}
!11 = !{!5, !9, i64 8}
!12 = !{!5, !9, i64 16}
!13 = !{!6, !6, i64 0}
