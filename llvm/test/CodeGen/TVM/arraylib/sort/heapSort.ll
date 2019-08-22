; RUN: llc < %s -march=tvm 
; ModuleID = 'heapSort.c'
source_filename = "heapSort.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind
define dso_local void @downHeap(i257* nocapture %a, i257 %k, i257 %n) local_unnamed_addr #0 {
entry:
  %arrayidx = getelementptr inbounds i257, i257* %a, i257 %k
  %0 = load i257, i257* %arrayidx, align 1, !tbaa !2
  %div = sdiv i257 %n, 2
  %cmp32 = icmp slt i257 %div, %k
  br i1 %cmp32, label %while.end, label %while.body

while.body:                                       ; preds = %entry, %if.end8
  %k.addr.033 = phi i257 [ %child.0, %if.end8 ], [ %k, %entry ]
  %mul = shl nsw i257 %k.addr.033, 1
  %cmp1 = icmp slt i257 %mul, %n
  br i1 %cmp1, label %land.lhs.true, label %if.end

land.lhs.true:                                    ; preds = %while.body
  %arrayidx2 = getelementptr inbounds i257, i257* %a, i257 %mul
  %1 = load i257, i257* %arrayidx2, align 1, !tbaa !2
  %add = or i257 %mul, 1
  %arrayidx3 = getelementptr inbounds i257, i257* %a, i257 %add
  %2 = load i257, i257* %arrayidx3, align 1, !tbaa !2
  %cmp4 = icmp slt i257 %1, %2
  %spec.select = select i1 %cmp4, i257 %add, i257 %mul
  br label %if.end

if.end:                                           ; preds = %land.lhs.true, %while.body
  %child.0 = phi i257 [ %mul, %while.body ], [ %spec.select, %land.lhs.true ]
  %arrayidx5 = getelementptr inbounds i257, i257* %a, i257 %child.0
  %3 = load i257, i257* %arrayidx5, align 1, !tbaa !2
  %cmp6 = icmp slt i257 %0, %3
  %arrayidx10 = getelementptr inbounds i257, i257* %a, i257 %k.addr.033
  br i1 %cmp6, label %if.end8, label %while.end

if.end8:                                          ; preds = %if.end
  store i257 %3, i257* %arrayidx10, align 1, !tbaa !2
  %cmp = icmp sgt i257 %child.0, %div
  br i1 %cmp, label %while.end.loopexit, label %while.body

while.end.loopexit:                               ; preds = %if.end8
  %arrayidx5.le = getelementptr inbounds i257, i257* %a, i257 %child.0
  br label %while.end

while.end:                                        ; preds = %if.end, %while.end.loopexit, %entry
  %arrayidx11.pre-phi = phi i257* [ %arrayidx, %entry ], [ %arrayidx5.le, %while.end.loopexit ], [ %arrayidx10, %if.end ]
  store i257 %0, i257* %arrayidx11.pre-phi, align 1, !tbaa !2
  ret void
}

; Function Attrs: norecurse nounwind
define dso_local void @heapSort(i257* nocapture %a, i257 %size) local_unnamed_addr #0 {
entry:
  %cmp63 = icmp sgt i257 %size, 1
  br i1 %cmp63, label %for.body.lr.ph, label %for.end12

for.body.lr.ph:                                   ; preds = %entry
  %div69 = lshr i257 %size, 1
  %sub1 = add nsw i257 %size, -1
  %div.i = sdiv i257 %sub1, 2
  br label %for.body

for.cond3.preheader:                              ; preds = %downHeap.exit
  br i1 %cmp63, label %for.body5, label %for.end12

for.body:                                         ; preds = %for.body.lr.ph, %downHeap.exit
  %i.064.in = phi i257 [ %div69, %for.body.lr.ph ], [ %i.064, %downHeap.exit ]
  %i.064 = add nsw i257 %i.064.in, -1
  %arrayidx.i = getelementptr inbounds i257, i257* %a, i257 %i.064
  %0 = load i257, i257* %arrayidx.i, align 1, !tbaa !2
  %cmp32.i = icmp slt i257 %div.i, %i.064
  br i1 %cmp32.i, label %downHeap.exit, label %while.body.i

while.body.i:                                     ; preds = %for.body, %if.end8.i
  %k.addr.033.i = phi i257 [ %child.0.i, %if.end8.i ], [ %i.064, %for.body ]
  %mul.i = shl nsw i257 %k.addr.033.i, 1
  %cmp1.i = icmp slt i257 %mul.i, %sub1
  br i1 %cmp1.i, label %land.lhs.true.i, label %if.end.i

land.lhs.true.i:                                  ; preds = %while.body.i
  %arrayidx2.i = getelementptr inbounds i257, i257* %a, i257 %mul.i
  %1 = load i257, i257* %arrayidx2.i, align 1, !tbaa !2
  %add.i = or i257 %mul.i, 1
  %arrayidx3.i = getelementptr inbounds i257, i257* %a, i257 %add.i
  %2 = load i257, i257* %arrayidx3.i, align 1, !tbaa !2
  %cmp4.i = icmp slt i257 %1, %2
  %spec.select.i = select i1 %cmp4.i, i257 %add.i, i257 %mul.i
  br label %if.end.i

if.end.i:                                         ; preds = %land.lhs.true.i, %while.body.i
  %child.0.i = phi i257 [ %mul.i, %while.body.i ], [ %spec.select.i, %land.lhs.true.i ]
  %arrayidx5.i = getelementptr inbounds i257, i257* %a, i257 %child.0.i
  %3 = load i257, i257* %arrayidx5.i, align 1, !tbaa !2
  %cmp6.i = icmp slt i257 %0, %3
  %arrayidx10.i = getelementptr inbounds i257, i257* %a, i257 %k.addr.033.i
  br i1 %cmp6.i, label %if.end8.i, label %downHeap.exit

if.end8.i:                                        ; preds = %if.end.i
  store i257 %3, i257* %arrayidx10.i, align 1, !tbaa !2
  %cmp.i = icmp sgt i257 %child.0.i, %div.i
  br i1 %cmp.i, label %downHeap.exit.loopexit.split.loop.exit, label %while.body.i

downHeap.exit.loopexit.split.loop.exit:           ; preds = %if.end8.i
  %arrayidx5.i.le = getelementptr inbounds i257, i257* %a, i257 %child.0.i
  br label %downHeap.exit

downHeap.exit:                                    ; preds = %if.end.i, %downHeap.exit.loopexit.split.loop.exit, %for.body
  %arrayidx11.pre-phi.i = phi i257* [ %arrayidx.i, %for.body ], [ %arrayidx5.i.le, %downHeap.exit.loopexit.split.loop.exit ], [ %arrayidx10.i, %if.end.i ]
  store i257 %0, i257* %arrayidx11.pre-phi.i, align 1, !tbaa !2
  %cmp = icmp sgt i257 %i.064.in, 1
  br i1 %cmp, label %for.body, label %for.cond3.preheader

for.body5:                                        ; preds = %for.cond3.preheader, %downHeap.exit53
  %i.159.in = phi i257 [ %i.159, %downHeap.exit53 ], [ %size, %for.cond3.preheader ]
  %i.159 = add nsw i257 %i.159.in, -1
  %arrayidx = getelementptr inbounds i257, i257* %a, i257 %i.159
  %4 = load i257, i257* %arrayidx, align 1, !tbaa !2
  %5 = load i257, i257* %a, align 1, !tbaa !2
  store i257 %5, i257* %arrayidx, align 1, !tbaa !2
  store i257 %4, i257* %a, align 1, !tbaa !2
  %sub9 = add nsw i257 %i.159.in, -2
  %div.i29 = sdiv i257 %sub9, 2
  br label %while.body.i34

while.body.i34:                                   ; preds = %for.body5, %if.end8.i49
  %k.addr.033.i31 = phi i257 [ %child.0.i41, %if.end8.i49 ], [ 0, %for.body5 ]
  %mul.i32 = shl nsw i257 %k.addr.033.i31, 1
  %cmp1.i33 = icmp slt i257 %mul.i32, %sub9
  br i1 %cmp1.i33, label %land.lhs.true.i40, label %if.end.i44

land.lhs.true.i40:                                ; preds = %while.body.i34
  %arrayidx2.i35 = getelementptr inbounds i257, i257* %a, i257 %mul.i32
  %6 = load i257, i257* %arrayidx2.i35, align 1, !tbaa !2
  %add.i36 = or i257 %mul.i32, 1
  %arrayidx3.i37 = getelementptr inbounds i257, i257* %a, i257 %add.i36
  %7 = load i257, i257* %arrayidx3.i37, align 1, !tbaa !2
  %cmp4.i38 = icmp slt i257 %6, %7
  %spec.select.i39 = select i1 %cmp4.i38, i257 %add.i36, i257 %mul.i32
  br label %if.end.i44

if.end.i44:                                       ; preds = %land.lhs.true.i40, %while.body.i34
  %child.0.i41 = phi i257 [ %mul.i32, %while.body.i34 ], [ %spec.select.i39, %land.lhs.true.i40 ]
  %arrayidx5.i42 = getelementptr inbounds i257, i257* %a, i257 %child.0.i41
  %8 = load i257, i257* %arrayidx5.i42, align 1, !tbaa !2
  %cmp6.i43 = icmp slt i257 %4, %8
  %arrayidx10.i47 = getelementptr inbounds i257, i257* %a, i257 %k.addr.033.i31
  br i1 %cmp6.i43, label %if.end8.i49, label %downHeap.exit53

if.end8.i49:                                      ; preds = %if.end.i44
  store i257 %8, i257* %arrayidx10.i47, align 1, !tbaa !2
  %cmp.i48 = icmp sgt i257 %child.0.i41, %div.i29
  br i1 %cmp.i48, label %downHeap.exit53.split.loop.exit70, label %while.body.i34

downHeap.exit53.split.loop.exit70:                ; preds = %if.end8.i49
  %arrayidx5.i42.le = getelementptr inbounds i257, i257* %a, i257 %child.0.i41
  br label %downHeap.exit53

downHeap.exit53:                                  ; preds = %if.end.i44, %downHeap.exit53.split.loop.exit70
  %arrayidx11.pre-phi.i52 = phi i257* [ %arrayidx5.i42.le, %downHeap.exit53.split.loop.exit70 ], [ %arrayidx10.i47, %if.end.i44 ]
  store i257 %4, i257* %arrayidx11.pre-phi.i52, align 1, !tbaa !2
  %cmp4 = icmp sgt i257 %i.159.in, 2
  br i1 %cmp4, label %for.body5, label %for.end12

for.end12:                                        ; preds = %downHeap.exit53, %entry, %for.cond3.preheader
  ret void
}

attributes #0 = { norecurse nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
!2 = !{!3, !3, i64 0}
!3 = !{!"long", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
