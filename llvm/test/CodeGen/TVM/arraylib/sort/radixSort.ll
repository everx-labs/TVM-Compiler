; RUN: llc < %s -march=tvm 
; ModuleID = 'radixSort.c'
source_filename = "radixSort.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readonly
define dso_local i257 @largest(i257* nocapture readonly %a, i257 %n) local_unnamed_addr #0 {
entry:
  %0 = load i257, i257* %a, align 1, !tbaa !2
  %cmp11 = icmp sgt i257 %n, 1
  br i1 %cmp11, label %for.body, label %for.end

for.body:                                         ; preds = %entry, %for.body
  %i.013 = phi i257 [ %inc, %for.body ], [ 1, %entry ]
  %large.012 = phi i257 [ %spec.select, %for.body ], [ %0, %entry ]
  %arrayidx1 = getelementptr inbounds i257, i257* %a, i257 %i.013
  %1 = load i257, i257* %arrayidx1, align 1, !tbaa !2
  %cmp2 = icmp slt i257 %large.012, %1
  %spec.select = select i1 %cmp2, i257 %1, i257 %large.012
  %inc = add nuw nsw i257 %i.013, 1
  %cmp = icmp slt i257 %inc, %n
  br i1 %cmp, label %for.body, label %for.end

for.end:                                          ; preds = %for.body, %entry
  %large.0.lcssa = phi i257 [ %0, %entry ], [ %spec.select, %for.body ]
  ret i257 %large.0.lcssa
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #1

; Function Attrs: nounwind
define dso_local void @RadixSort(i257* nocapture %a, i257 %n) local_unnamed_addr #2 {
entry:
  %bucket = alloca [10 x [10 x i257]], align 1
  %bucket_count = alloca [10 x i257], align 1
  %0 = bitcast [10 x [10 x i257]]* %bucket to i8*
  call void @llvm.lifetime.start.p0i8(i64 100, i8* nonnull %0) #3
  %1 = bitcast [10 x i257]* %bucket_count to i8*
  call void @llvm.lifetime.start.p0i8(i64 10, i8* nonnull %1) #3
  %2 = load i257, i257* %a, align 1, !tbaa !2
  %cmp11.i = icmp sgt i257 %n, 1
  br i1 %cmp11.i, label %for.body.i, label %largest.exit

for.body.i:                                       ; preds = %entry, %for.body.i
  %i.013.i = phi i257 [ %inc.i, %for.body.i ], [ 1, %entry ]
  %large.012.i = phi i257 [ %spec.select.i, %for.body.i ], [ %2, %entry ]
  %arrayidx1.i = getelementptr inbounds i257, i257* %a, i257 %i.013.i
  %3 = load i257, i257* %arrayidx1.i, align 1, !tbaa !2
  %cmp2.i = icmp slt i257 %large.012.i, %3
  %spec.select.i = select i1 %cmp2.i, i257 %3, i257 %large.012.i
  %inc.i = add nuw nsw i257 %i.013.i, 1
  %cmp.i = icmp slt i257 %inc.i, %n
  br i1 %cmp.i, label %for.body.i, label %largest.exit

largest.exit:                                     ; preds = %for.body.i, %entry
  %large.0.lcssa.i = phi i257 [ %2, %entry ], [ %spec.select.i, %for.body.i ]
  %cmp82 = icmp sgt i257 %large.0.lcssa.i, 0
  br i1 %cmp82, label %while.body, label %for.end38

for.cond2.preheader.lr.ph:                        ; preds = %while.body
  %cmp771 = icmp sgt i257 %n, 0
  br i1 %cmp771, label %for.cond2.preheader.us.preheader, label %for.cond2.preheader.preheader

for.cond2.preheader.preheader:                    ; preds = %for.cond2.preheader.lr.ph
  %4 = bitcast [10 x i257]* %bucket_count to i257*
  br label %for.inc33.9

for.cond2.preheader.us.preheader:                 ; preds = %for.cond2.preheader.lr.ph
  %arrayidx.us = getelementptr inbounds [10 x i257], [10 x i257]* %bucket_count, i257 0, i257 0
  %arrayidx.us.1 = getelementptr inbounds [10 x i257], [10 x i257]* %bucket_count, i257 0, i257 1
  %arrayidx.us.2 = getelementptr inbounds [10 x i257], [10 x i257]* %bucket_count, i257 0, i257 2
  %arrayidx.us.3 = getelementptr inbounds [10 x i257], [10 x i257]* %bucket_count, i257 0, i257 3
  %arrayidx.us.4 = getelementptr inbounds [10 x i257], [10 x i257]* %bucket_count, i257 0, i257 4
  %arrayidx.us.5 = getelementptr inbounds [10 x i257], [10 x i257]* %bucket_count, i257 0, i257 5
  %arrayidx.us.6 = getelementptr inbounds [10 x i257], [10 x i257]* %bucket_count, i257 0, i257 6
  %arrayidx.us.7 = getelementptr inbounds [10 x i257], [10 x i257]* %bucket_count, i257 0, i257 7
  %arrayidx.us.8 = getelementptr inbounds [10 x i257], [10 x i257]* %bucket_count, i257 0, i257 8
  %arrayidx.us.9 = getelementptr inbounds [10 x i257], [10 x i257]* %bucket_count, i257 0, i257 9
  %5 = bitcast [10 x i257]* %bucket_count to i257*
  br label %for.cond2.preheader.us

for.cond2.preheader.us:                           ; preds = %for.cond2.preheader.us.preheader, %for.inc33.us.9
  %pass.081.us = phi i257 [ %inc37.us, %for.inc33.us.9 ], [ 0, %for.cond2.preheader.us.preheader ]
  %divisor.080.us = phi i257 [ %mul.us, %for.inc33.us.9 ], [ 1, %for.cond2.preheader.us.preheader ]
  call void @llvm.memset.p0i257.i64(i257* nonnull align 1 %5, i257 0, i64 10, i1 false)
  br label %for.body8.us

for.inc33.us:                                     ; preds = %for.body25.us, %for.cond22.preheader.us.preheader
  %i.3.lcssa.us = phi i257 [ 0, %for.cond22.preheader.us.preheader ], [ %inc29.us, %for.body25.us ]
  %6 = load i257, i257* %arrayidx.us.1, align 1, !tbaa !2
  %cmp2473.us.1 = icmp sgt i257 %6, 0
  br i1 %cmp2473.us.1, label %for.body25.us.1, label %for.inc33.us.1

for.body25.us:                                    ; preds = %for.cond22.preheader.us.preheader, %for.body25.us
  %i.375.us = phi i257 [ %inc29.us, %for.body25.us ], [ 0, %for.cond22.preheader.us.preheader ]
  %j.074.us = phi i257 [ %inc31.us, %for.body25.us ], [ 0, %for.cond22.preheader.us.preheader ]
  %arrayidx27.us = getelementptr inbounds [10 x [10 x i257]], [10 x [10 x i257]]* %bucket, i257 0, i257 0, i257 %j.074.us
  %7 = load i257, i257* %arrayidx27.us, align 1, !tbaa !2
  %arrayidx28.us = getelementptr inbounds i257, i257* %a, i257 %i.375.us
  store i257 %7, i257* %arrayidx28.us, align 1, !tbaa !2
  %inc29.us = add nuw nsw i257 %i.375.us, 1
  %inc31.us = add nuw nsw i257 %j.074.us, 1
  %cmp24.us = icmp slt i257 %inc31.us, %10
  br i1 %cmp24.us, label %for.body25.us, label %for.inc33.us

for.body8.us:                                     ; preds = %for.cond2.preheader.us, %for.body8.us
  %i.172.us = phi i257 [ %inc17.us, %for.body8.us ], [ 0, %for.cond2.preheader.us ]
  %arrayidx9.us = getelementptr inbounds i257, i257* %a, i257 %i.172.us
  %8 = load i257, i257* %arrayidx9.us, align 1, !tbaa !2
  %div10.us = sdiv i257 %8, %divisor.080.us
  %rem.us = srem i257 %div10.us, 10
  %arrayidx13.us = getelementptr inbounds [10 x i257], [10 x i257]* %bucket_count, i257 0, i257 %rem.us
  %9 = load i257, i257* %arrayidx13.us, align 1, !tbaa !2
  %arrayidx14.us = getelementptr inbounds [10 x [10 x i257]], [10 x [10 x i257]]* %bucket, i257 0, i257 %rem.us, i257 %9
  store i257 %8, i257* %arrayidx14.us, align 1, !tbaa !2
  %add.us = add nsw i257 %9, 1
  store i257 %add.us, i257* %arrayidx13.us, align 1, !tbaa !2
  %inc17.us = add nuw nsw i257 %i.172.us, 1
  %cmp7.us = icmp slt i257 %inc17.us, %n
  br i1 %cmp7.us, label %for.body8.us, label %for.cond22.preheader.us.preheader

for.cond22.preheader.us.preheader:                ; preds = %for.body8.us
  %10 = load i257, i257* %arrayidx.us, align 1, !tbaa !2
  %cmp2473.us = icmp sgt i257 %10, 0
  br i1 %cmp2473.us, label %for.body25.us, label %for.inc33.us

while.body:                                       ; preds = %largest.exit, %while.body
  %large.084 = phi i257 [ %div69, %while.body ], [ %large.0.lcssa.i, %largest.exit ]
  %NOP.083 = phi i257 [ %inc, %while.body ], [ 0, %largest.exit ]
  %inc = add nuw nsw i257 %NOP.083, 1
  %div69 = udiv i257 %large.084, 10
  %11 = icmp ugt i257 %large.084, 9
  br i1 %11, label %while.body, label %for.cond2.preheader.lr.ph

for.end38:                                        ; preds = %for.inc33.9, %for.inc33.us.9, %largest.exit
  call void @llvm.lifetime.end.p0i8(i64 10, i8* nonnull %1) #3
  call void @llvm.lifetime.end.p0i8(i64 100, i8* nonnull %0) #3
  ret void

for.body25.us.1:                                  ; preds = %for.inc33.us, %for.body25.us.1
  %i.375.us.1 = phi i257 [ %inc29.us.1, %for.body25.us.1 ], [ %i.3.lcssa.us, %for.inc33.us ]
  %j.074.us.1 = phi i257 [ %inc31.us.1, %for.body25.us.1 ], [ 0, %for.inc33.us ]
  %arrayidx27.us.1 = getelementptr inbounds [10 x [10 x i257]], [10 x [10 x i257]]* %bucket, i257 0, i257 1, i257 %j.074.us.1
  %12 = load i257, i257* %arrayidx27.us.1, align 1, !tbaa !2
  %arrayidx28.us.1 = getelementptr inbounds i257, i257* %a, i257 %i.375.us.1
  store i257 %12, i257* %arrayidx28.us.1, align 1, !tbaa !2
  %inc29.us.1 = add nsw i257 %i.375.us.1, 1
  %inc31.us.1 = add nuw nsw i257 %j.074.us.1, 1
  %cmp24.us.1 = icmp slt i257 %inc31.us.1, %6
  br i1 %cmp24.us.1, label %for.body25.us.1, label %for.inc33.us.1

for.inc33.us.1:                                   ; preds = %for.body25.us.1, %for.inc33.us
  %i.3.lcssa.us.1 = phi i257 [ %i.3.lcssa.us, %for.inc33.us ], [ %inc29.us.1, %for.body25.us.1 ]
  %13 = load i257, i257* %arrayidx.us.2, align 1, !tbaa !2
  %cmp2473.us.2 = icmp sgt i257 %13, 0
  br i1 %cmp2473.us.2, label %for.body25.us.2, label %for.inc33.us.2

for.body25.us.2:                                  ; preds = %for.inc33.us.1, %for.body25.us.2
  %i.375.us.2 = phi i257 [ %inc29.us.2, %for.body25.us.2 ], [ %i.3.lcssa.us.1, %for.inc33.us.1 ]
  %j.074.us.2 = phi i257 [ %inc31.us.2, %for.body25.us.2 ], [ 0, %for.inc33.us.1 ]
  %arrayidx27.us.2 = getelementptr inbounds [10 x [10 x i257]], [10 x [10 x i257]]* %bucket, i257 0, i257 2, i257 %j.074.us.2
  %14 = load i257, i257* %arrayidx27.us.2, align 1, !tbaa !2
  %arrayidx28.us.2 = getelementptr inbounds i257, i257* %a, i257 %i.375.us.2
  store i257 %14, i257* %arrayidx28.us.2, align 1, !tbaa !2
  %inc29.us.2 = add nsw i257 %i.375.us.2, 1
  %inc31.us.2 = add nuw nsw i257 %j.074.us.2, 1
  %cmp24.us.2 = icmp slt i257 %inc31.us.2, %13
  br i1 %cmp24.us.2, label %for.body25.us.2, label %for.inc33.us.2

for.inc33.us.2:                                   ; preds = %for.body25.us.2, %for.inc33.us.1
  %i.3.lcssa.us.2 = phi i257 [ %i.3.lcssa.us.1, %for.inc33.us.1 ], [ %inc29.us.2, %for.body25.us.2 ]
  %15 = load i257, i257* %arrayidx.us.3, align 1, !tbaa !2
  %cmp2473.us.3 = icmp sgt i257 %15, 0
  br i1 %cmp2473.us.3, label %for.body25.us.3, label %for.inc33.us.3

for.body25.us.3:                                  ; preds = %for.inc33.us.2, %for.body25.us.3
  %i.375.us.3 = phi i257 [ %inc29.us.3, %for.body25.us.3 ], [ %i.3.lcssa.us.2, %for.inc33.us.2 ]
  %j.074.us.3 = phi i257 [ %inc31.us.3, %for.body25.us.3 ], [ 0, %for.inc33.us.2 ]
  %arrayidx27.us.3 = getelementptr inbounds [10 x [10 x i257]], [10 x [10 x i257]]* %bucket, i257 0, i257 3, i257 %j.074.us.3
  %16 = load i257, i257* %arrayidx27.us.3, align 1, !tbaa !2
  %arrayidx28.us.3 = getelementptr inbounds i257, i257* %a, i257 %i.375.us.3
  store i257 %16, i257* %arrayidx28.us.3, align 1, !tbaa !2
  %inc29.us.3 = add nsw i257 %i.375.us.3, 1
  %inc31.us.3 = add nuw nsw i257 %j.074.us.3, 1
  %cmp24.us.3 = icmp slt i257 %inc31.us.3, %15
  br i1 %cmp24.us.3, label %for.body25.us.3, label %for.inc33.us.3

for.inc33.us.3:                                   ; preds = %for.body25.us.3, %for.inc33.us.2
  %i.3.lcssa.us.3 = phi i257 [ %i.3.lcssa.us.2, %for.inc33.us.2 ], [ %inc29.us.3, %for.body25.us.3 ]
  %17 = load i257, i257* %arrayidx.us.4, align 1, !tbaa !2
  %cmp2473.us.4 = icmp sgt i257 %17, 0
  br i1 %cmp2473.us.4, label %for.body25.us.4, label %for.inc33.us.4

for.body25.us.4:                                  ; preds = %for.inc33.us.3, %for.body25.us.4
  %i.375.us.4 = phi i257 [ %inc29.us.4, %for.body25.us.4 ], [ %i.3.lcssa.us.3, %for.inc33.us.3 ]
  %j.074.us.4 = phi i257 [ %inc31.us.4, %for.body25.us.4 ], [ 0, %for.inc33.us.3 ]
  %arrayidx27.us.4 = getelementptr inbounds [10 x [10 x i257]], [10 x [10 x i257]]* %bucket, i257 0, i257 4, i257 %j.074.us.4
  %18 = load i257, i257* %arrayidx27.us.4, align 1, !tbaa !2
  %arrayidx28.us.4 = getelementptr inbounds i257, i257* %a, i257 %i.375.us.4
  store i257 %18, i257* %arrayidx28.us.4, align 1, !tbaa !2
  %inc29.us.4 = add nsw i257 %i.375.us.4, 1
  %inc31.us.4 = add nuw nsw i257 %j.074.us.4, 1
  %cmp24.us.4 = icmp slt i257 %inc31.us.4, %17
  br i1 %cmp24.us.4, label %for.body25.us.4, label %for.inc33.us.4

for.inc33.us.4:                                   ; preds = %for.body25.us.4, %for.inc33.us.3
  %i.3.lcssa.us.4 = phi i257 [ %i.3.lcssa.us.3, %for.inc33.us.3 ], [ %inc29.us.4, %for.body25.us.4 ]
  %19 = load i257, i257* %arrayidx.us.5, align 1, !tbaa !2
  %cmp2473.us.5 = icmp sgt i257 %19, 0
  br i1 %cmp2473.us.5, label %for.body25.us.5, label %for.inc33.us.5

for.body25.us.5:                                  ; preds = %for.inc33.us.4, %for.body25.us.5
  %i.375.us.5 = phi i257 [ %inc29.us.5, %for.body25.us.5 ], [ %i.3.lcssa.us.4, %for.inc33.us.4 ]
  %j.074.us.5 = phi i257 [ %inc31.us.5, %for.body25.us.5 ], [ 0, %for.inc33.us.4 ]
  %arrayidx27.us.5 = getelementptr inbounds [10 x [10 x i257]], [10 x [10 x i257]]* %bucket, i257 0, i257 5, i257 %j.074.us.5
  %20 = load i257, i257* %arrayidx27.us.5, align 1, !tbaa !2
  %arrayidx28.us.5 = getelementptr inbounds i257, i257* %a, i257 %i.375.us.5
  store i257 %20, i257* %arrayidx28.us.5, align 1, !tbaa !2
  %inc29.us.5 = add nsw i257 %i.375.us.5, 1
  %inc31.us.5 = add nuw nsw i257 %j.074.us.5, 1
  %cmp24.us.5 = icmp slt i257 %inc31.us.5, %19
  br i1 %cmp24.us.5, label %for.body25.us.5, label %for.inc33.us.5

for.inc33.us.5:                                   ; preds = %for.body25.us.5, %for.inc33.us.4
  %i.3.lcssa.us.5 = phi i257 [ %i.3.lcssa.us.4, %for.inc33.us.4 ], [ %inc29.us.5, %for.body25.us.5 ]
  %21 = load i257, i257* %arrayidx.us.6, align 1, !tbaa !2
  %cmp2473.us.6 = icmp sgt i257 %21, 0
  br i1 %cmp2473.us.6, label %for.body25.us.6, label %for.inc33.us.6

for.body25.us.6:                                  ; preds = %for.inc33.us.5, %for.body25.us.6
  %i.375.us.6 = phi i257 [ %inc29.us.6, %for.body25.us.6 ], [ %i.3.lcssa.us.5, %for.inc33.us.5 ]
  %j.074.us.6 = phi i257 [ %inc31.us.6, %for.body25.us.6 ], [ 0, %for.inc33.us.5 ]
  %arrayidx27.us.6 = getelementptr inbounds [10 x [10 x i257]], [10 x [10 x i257]]* %bucket, i257 0, i257 6, i257 %j.074.us.6
  %22 = load i257, i257* %arrayidx27.us.6, align 1, !tbaa !2
  %arrayidx28.us.6 = getelementptr inbounds i257, i257* %a, i257 %i.375.us.6
  store i257 %22, i257* %arrayidx28.us.6, align 1, !tbaa !2
  %inc29.us.6 = add nsw i257 %i.375.us.6, 1
  %inc31.us.6 = add nuw nsw i257 %j.074.us.6, 1
  %cmp24.us.6 = icmp slt i257 %inc31.us.6, %21
  br i1 %cmp24.us.6, label %for.body25.us.6, label %for.inc33.us.6

for.inc33.us.6:                                   ; preds = %for.body25.us.6, %for.inc33.us.5
  %i.3.lcssa.us.6 = phi i257 [ %i.3.lcssa.us.5, %for.inc33.us.5 ], [ %inc29.us.6, %for.body25.us.6 ]
  %23 = load i257, i257* %arrayidx.us.7, align 1, !tbaa !2
  %cmp2473.us.7 = icmp sgt i257 %23, 0
  br i1 %cmp2473.us.7, label %for.body25.us.7, label %for.inc33.us.7

for.body25.us.7:                                  ; preds = %for.inc33.us.6, %for.body25.us.7
  %i.375.us.7 = phi i257 [ %inc29.us.7, %for.body25.us.7 ], [ %i.3.lcssa.us.6, %for.inc33.us.6 ]
  %j.074.us.7 = phi i257 [ %inc31.us.7, %for.body25.us.7 ], [ 0, %for.inc33.us.6 ]
  %arrayidx27.us.7 = getelementptr inbounds [10 x [10 x i257]], [10 x [10 x i257]]* %bucket, i257 0, i257 7, i257 %j.074.us.7
  %24 = load i257, i257* %arrayidx27.us.7, align 1, !tbaa !2
  %arrayidx28.us.7 = getelementptr inbounds i257, i257* %a, i257 %i.375.us.7
  store i257 %24, i257* %arrayidx28.us.7, align 1, !tbaa !2
  %inc29.us.7 = add nsw i257 %i.375.us.7, 1
  %inc31.us.7 = add nuw nsw i257 %j.074.us.7, 1
  %cmp24.us.7 = icmp slt i257 %inc31.us.7, %23
  br i1 %cmp24.us.7, label %for.body25.us.7, label %for.inc33.us.7

for.inc33.us.7:                                   ; preds = %for.body25.us.7, %for.inc33.us.6
  %i.3.lcssa.us.7 = phi i257 [ %i.3.lcssa.us.6, %for.inc33.us.6 ], [ %inc29.us.7, %for.body25.us.7 ]
  %25 = load i257, i257* %arrayidx.us.8, align 1, !tbaa !2
  %cmp2473.us.8 = icmp sgt i257 %25, 0
  br i1 %cmp2473.us.8, label %for.body25.us.8, label %for.inc33.us.8

for.body25.us.8:                                  ; preds = %for.inc33.us.7, %for.body25.us.8
  %i.375.us.8 = phi i257 [ %inc29.us.8, %for.body25.us.8 ], [ %i.3.lcssa.us.7, %for.inc33.us.7 ]
  %j.074.us.8 = phi i257 [ %inc31.us.8, %for.body25.us.8 ], [ 0, %for.inc33.us.7 ]
  %arrayidx27.us.8 = getelementptr inbounds [10 x [10 x i257]], [10 x [10 x i257]]* %bucket, i257 0, i257 8, i257 %j.074.us.8
  %26 = load i257, i257* %arrayidx27.us.8, align 1, !tbaa !2
  %arrayidx28.us.8 = getelementptr inbounds i257, i257* %a, i257 %i.375.us.8
  store i257 %26, i257* %arrayidx28.us.8, align 1, !tbaa !2
  %inc29.us.8 = add nsw i257 %i.375.us.8, 1
  %inc31.us.8 = add nuw nsw i257 %j.074.us.8, 1
  %cmp24.us.8 = icmp slt i257 %inc31.us.8, %25
  br i1 %cmp24.us.8, label %for.body25.us.8, label %for.inc33.us.8

for.inc33.us.8:                                   ; preds = %for.body25.us.8, %for.inc33.us.7
  %i.3.lcssa.us.8 = phi i257 [ %i.3.lcssa.us.7, %for.inc33.us.7 ], [ %inc29.us.8, %for.body25.us.8 ]
  %27 = load i257, i257* %arrayidx.us.9, align 1, !tbaa !2
  %cmp2473.us.9 = icmp sgt i257 %27, 0
  br i1 %cmp2473.us.9, label %for.body25.us.9, label %for.inc33.us.9

for.body25.us.9:                                  ; preds = %for.inc33.us.8, %for.body25.us.9
  %i.375.us.9 = phi i257 [ %inc29.us.9, %for.body25.us.9 ], [ %i.3.lcssa.us.8, %for.inc33.us.8 ]
  %j.074.us.9 = phi i257 [ %inc31.us.9, %for.body25.us.9 ], [ 0, %for.inc33.us.8 ]
  %arrayidx27.us.9 = getelementptr inbounds [10 x [10 x i257]], [10 x [10 x i257]]* %bucket, i257 0, i257 9, i257 %j.074.us.9
  %28 = load i257, i257* %arrayidx27.us.9, align 1, !tbaa !2
  %arrayidx28.us.9 = getelementptr inbounds i257, i257* %a, i257 %i.375.us.9
  store i257 %28, i257* %arrayidx28.us.9, align 1, !tbaa !2
  %inc29.us.9 = add nsw i257 %i.375.us.9, 1
  %inc31.us.9 = add nuw nsw i257 %j.074.us.9, 1
  %cmp24.us.9 = icmp slt i257 %inc31.us.9, %27
  br i1 %cmp24.us.9, label %for.body25.us.9, label %for.inc33.us.9

for.inc33.us.9:                                   ; preds = %for.body25.us.9, %for.inc33.us.8
  %mul.us = mul nsw i257 %divisor.080.us, 10
  %inc37.us = add nuw nsw i257 %pass.081.us, 1
  %cmp1.us = icmp ult i257 %pass.081.us, %NOP.083
  br i1 %cmp1.us, label %for.cond2.preheader.us, label %for.end38

for.inc33.9:                                      ; preds = %for.cond2.preheader.preheader, %for.inc33.9
  %pass.081 = phi i257 [ %inc37, %for.inc33.9 ], [ 0, %for.cond2.preheader.preheader ]
  call void @llvm.memset.p0i257.i64(i257* nonnull align 1 %4, i257 0, i64 10, i1 false)
  %inc37 = add nuw nsw i257 %pass.081, 1
  %cmp1 = icmp ult i257 %pass.081, %NOP.083
  br i1 %cmp1, label %for.inc33.9, label %for.end38
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i257.i64(i257* nocapture writeonly, i257, i64, i1) #1

attributes #0 = { norecurse nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
!2 = !{!3, !3, i64 0}
!3 = !{!"long", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
