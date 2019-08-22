; RUN: llc < %s -march=tvm 
; ModuleID = 'quickSortI.c'
source_filename = "quickSortI.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: nounwind
define dso_local void @qSortI(i257* nocapture %a, i257 %size) local_unnamed_addr #0 {
entry:
  %lbstack = alloca [50 x i257], align 1
  %ubstack = alloca [50 x i257], align 1
  %0 = bitcast [50 x i257]* %lbstack to i8*
  call void @llvm.lifetime.start.p0i8(i64 50, i8* nonnull %0) #2
  %1 = bitcast [50 x i257]* %ubstack to i8*
  call void @llvm.lifetime.start.p0i8(i64 50, i8* nonnull %1) #2
  %arrayidx = getelementptr inbounds [50 x i257], [50 x i257]* %lbstack, i257 0, i257 1
  store i257 0, i257* %arrayidx, align 1, !tbaa !2
  %sub = add nsw i257 %size, -1
  %arrayidx1 = getelementptr inbounds [50 x i257], [50 x i257]* %ubstack, i257 0, i257 1
  store i257 %sub, i257* %arrayidx1, align 1, !tbaa !2
  br label %do.body

do.body:                                          ; preds = %do.cond40.do.body_crit_edge, %entry
  %2 = phi i257 [ %sub, %entry ], [ %.pre99, %do.cond40.do.body_crit_edge ]
  %3 = phi i257 [ 0, %entry ], [ %.pre, %do.cond40.do.body_crit_edge ]
  %stackpos.0 = phi i257 [ 1, %entry ], [ %stackpos.4, %do.cond40.do.body_crit_edge ]
  %dec = add nsw i257 %stackpos.0, -1
  br label %do.body4

do.body4:                                         ; preds = %do.cond37, %do.body
  %ub.0 = phi i257 [ %2, %do.body ], [ %ub.1, %do.cond37 ]
  %stackpos.1 = phi i257 [ %dec, %do.body ], [ %stackpos.4, %do.cond37 ]
  %lb.0 = phi i257 [ %3, %do.body ], [ %lb.1, %do.cond37 ]
  %add = add nsw i257 %lb.0, %ub.0
  %shr = ashr i257 %add, 1
  %arrayidx5 = getelementptr inbounds i257, i257* %a, i257 %shr
  %4 = load i257, i257* %arrayidx5, align 1, !tbaa !2
  br label %do.body6

do.body6:                                         ; preds = %do.cond, %do.body4
  %j.0 = phi i257 [ %ub.0, %do.body4 ], [ %j.2, %do.cond ]
  %i.0 = phi i257 [ %lb.0, %do.body4 ], [ %i.2, %do.cond ]
  br label %while.cond

while.cond:                                       ; preds = %while.cond, %do.body6
  %i.1 = phi i257 [ %i.0, %do.body6 ], [ %inc, %while.cond ]
  %arrayidx7 = getelementptr inbounds i257, i257* %a, i257 %i.1
  %5 = load i257, i257* %arrayidx7, align 1, !tbaa !2
  %cmp = icmp slt i257 %5, %4
  %inc = add nsw i257 %i.1, 1
  br i1 %cmp, label %while.cond, label %while.cond8.preheader

while.cond8.preheader:                            ; preds = %while.cond
  %arrayidx7.le = getelementptr inbounds i257, i257* %a, i257 %i.1
  br label %while.cond8

while.cond8:                                      ; preds = %while.cond8, %while.cond8.preheader
  %j.1 = phi i257 [ %dec12, %while.cond8 ], [ %j.0, %while.cond8.preheader ]
  %arrayidx9 = getelementptr inbounds i257, i257* %a, i257 %j.1
  %6 = load i257, i257* %arrayidx9, align 1, !tbaa !2
  %cmp10 = icmp slt i257 %4, %6
  %dec12 = add nsw i257 %j.1, -1
  br i1 %cmp10, label %while.cond8, label %while.end13

while.end13:                                      ; preds = %while.cond8
  %cmp14 = icmp sgt i257 %i.1, %j.1
  br i1 %cmp14, label %do.cond, label %if.then

if.then:                                          ; preds = %while.end13
  %arrayidx9.le = getelementptr inbounds i257, i257* %a, i257 %j.1
  store i257 %6, i257* %arrayidx7.le, align 1, !tbaa !2
  store i257 %5, i257* %arrayidx9.le, align 1, !tbaa !2
  br label %do.cond

do.cond:                                          ; preds = %while.end13, %if.then
  %j.2 = phi i257 [ %dec12, %if.then ], [ %j.1, %while.end13 ]
  %i.2 = phi i257 [ %inc, %if.then ], [ %i.1, %while.end13 ]
  %cmp21 = icmp sgt i257 %i.2, %j.2
  br i1 %cmp21, label %do.end, label %do.body6

do.end:                                           ; preds = %do.cond
  %cmp22 = icmp slt i257 %i.2, %shr
  br i1 %cmp22, label %if.then23, label %if.else

if.then23:                                        ; preds = %do.end
  %cmp24 = icmp slt i257 %i.2, %ub.0
  br i1 %cmp24, label %if.then25, label %do.cond37

if.then25:                                        ; preds = %if.then23
  %inc26 = add nsw i257 %stackpos.1, 1
  %arrayidx27 = getelementptr inbounds [50 x i257], [50 x i257]* %lbstack, i257 0, i257 %inc26
  store i257 %i.2, i257* %arrayidx27, align 1, !tbaa !2
  %arrayidx28 = getelementptr inbounds [50 x i257], [50 x i257]* %ubstack, i257 0, i257 %inc26
  store i257 %ub.0, i257* %arrayidx28, align 1, !tbaa !2
  br label %do.cond37

if.else:                                          ; preds = %do.end
  %cmp30 = icmp sgt i257 %j.2, %lb.0
  br i1 %cmp30, label %if.then31, label %do.cond37

if.then31:                                        ; preds = %if.else
  %inc32 = add nsw i257 %stackpos.1, 1
  %arrayidx33 = getelementptr inbounds [50 x i257], [50 x i257]* %lbstack, i257 0, i257 %inc32
  store i257 %lb.0, i257* %arrayidx33, align 1, !tbaa !2
  %arrayidx34 = getelementptr inbounds [50 x i257], [50 x i257]* %ubstack, i257 0, i257 %inc32
  store i257 %j.2, i257* %arrayidx34, align 1, !tbaa !2
  br label %do.cond37

do.cond37:                                        ; preds = %if.else, %if.then31, %if.then23, %if.then25
  %ub.1 = phi i257 [ %j.2, %if.then25 ], [ %j.2, %if.then23 ], [ %ub.0, %if.then31 ], [ %ub.0, %if.else ]
  %stackpos.4 = phi i257 [ %inc26, %if.then25 ], [ %stackpos.1, %if.then23 ], [ %inc32, %if.then31 ], [ %stackpos.1, %if.else ]
  %lb.1 = phi i257 [ %lb.0, %if.then25 ], [ %lb.0, %if.then23 ], [ %i.2, %if.then31 ], [ %i.2, %if.else ]
  %cmp38 = icmp slt i257 %lb.1, %ub.1
  br i1 %cmp38, label %do.body4, label %do.cond40

do.cond40:                                        ; preds = %do.cond37
  %cmp41 = icmp eq i257 %stackpos.4, 0
  br i1 %cmp41, label %do.end42, label %do.cond40.do.body_crit_edge

do.cond40.do.body_crit_edge:                      ; preds = %do.cond40
  %arrayidx2.phi.trans.insert = getelementptr inbounds [50 x i257], [50 x i257]* %lbstack, i257 0, i257 %stackpos.4
  %.pre = load i257, i257* %arrayidx2.phi.trans.insert, align 1, !tbaa !2
  %arrayidx3.phi.trans.insert = getelementptr inbounds [50 x i257], [50 x i257]* %ubstack, i257 0, i257 %stackpos.4
  %.pre99 = load i257, i257* %arrayidx3.phi.trans.insert, align 1, !tbaa !2
  br label %do.body

do.end42:                                         ; preds = %do.cond40
  call void @llvm.lifetime.end.p0i8(i64 50, i8* nonnull %1) #2
  call void @llvm.lifetime.end.p0i8(i64 50, i8* nonnull %0) #2
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #1

attributes #0 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
!2 = !{!3, !3, i64 0}
!3 = !{!"long", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
