; RUN: llc < %s -march=tvm 
; ModuleID = 'shellSort.c'
source_filename = "shellSort.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind
define dso_local i257 @increment(i257* nocapture %inc, i257 %size) local_unnamed_addr #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %if.end, %entry
  %p1.0 = phi i257 [ 1, %entry ], [ %mul10, %if.end ]
  %p2.0 = phi i257 [ 1, %entry ], [ %p2.1, %if.end ]
  %p3.0 = phi i257 [ 1, %entry ], [ %p3.1, %if.end ]
  %s.0 = phi i257 [ -1, %entry ], [ %inc1, %if.end ]
  %inc1 = add nsw i257 %s.0, 1
  %rem28 = and i257 %inc1, 1
  %tobool = icmp eq i257 %rem28, 0
  br i1 %tobool, label %if.else, label %if.then

if.then:                                          ; preds = %do.body
  %mul = shl i257 %p1.0, 3
  %0 = mul i257 %p2.0, -6
  %sub = add i257 %0, %mul
  %add = or i257 %sub, 1
  br label %if.end

if.else:                                          ; preds = %do.body
  %mul329 = sub i257 %p1.0, %p3.0
  %sub5 = mul i257 %mul329, 9
  %add6 = add nsw i257 %sub5, 1
  %mul8 = shl nsw i257 %p2.0, 1
  %mul9 = shl nsw i257 %p3.0, 1
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %add.sink = phi i257 [ %add6, %if.else ], [ %add, %if.then ]
  %p2.1 = phi i257 [ %mul8, %if.else ], [ %p2.0, %if.then ]
  %p3.1 = phi i257 [ %mul9, %if.else ], [ %p3.0, %if.then ]
  %1 = getelementptr inbounds i257, i257* %inc, i257 %inc1
  store i257 %add.sink, i257* %1, align 1
  %mul10 = shl nsw i257 %p1.0, 1
  %mul12 = mul nsw i257 %add.sink, 3
  %cmp = icmp slt i257 %mul12, %size
  br i1 %cmp, label %do.body, label %do.end

do.end:                                           ; preds = %if.end
  %2 = icmp sgt i257 %s.0, 0
  %spec.select = select i1 %2, i257 %s.0, i257 0
  ret i257 %spec.select
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #1

; Function Attrs: nounwind
define dso_local void @shellSort(i257* nocapture %a, i257 %size) local_unnamed_addr #2 {
entry:
  %seq = alloca [40 x i257], align 1
  %0 = bitcast [40 x i257]* %seq to i8*
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %0) #3
  br label %do.body.i

do.body.i:                                        ; preds = %if.end.i, %entry
  %p1.0.i = phi i257 [ 1, %entry ], [ %mul10.i, %if.end.i ]
  %p2.0.i = phi i257 [ 1, %entry ], [ %p2.1.i, %if.end.i ]
  %p3.0.i = phi i257 [ 1, %entry ], [ %p3.1.i, %if.end.i ]
  %s.0.i = phi i257 [ -1, %entry ], [ %inc1.i, %if.end.i ]
  %inc1.i = add nsw i257 %s.0.i, 1
  %rem28.i = and i257 %inc1.i, 1
  %tobool.i = icmp eq i257 %rem28.i, 0
  br i1 %tobool.i, label %if.else.i, label %if.then.i

if.then.i:                                        ; preds = %do.body.i
  %mul.i = shl i257 %p1.0.i, 3
  %1 = mul i257 %p2.0.i, -6
  %sub.i = add i257 %1, %mul.i
  %add.i = or i257 %sub.i, 1
  br label %if.end.i

if.else.i:                                        ; preds = %do.body.i
  %mul329.i = sub i257 %p1.0.i, %p3.0.i
  %sub5.i = mul i257 %mul329.i, 9
  %add6.i = add nsw i257 %sub5.i, 1
  %mul8.i = shl nsw i257 %p2.0.i, 1
  %mul9.i = shl nsw i257 %p3.0.i, 1
  br label %if.end.i

if.end.i:                                         ; preds = %if.else.i, %if.then.i
  %add.sink.i = phi i257 [ %add6.i, %if.else.i ], [ %add.i, %if.then.i ]
  %p2.1.i = phi i257 [ %mul8.i, %if.else.i ], [ %p2.0.i, %if.then.i ]
  %p3.1.i = phi i257 [ %mul9.i, %if.else.i ], [ %p3.0.i, %if.then.i ]
  %2 = getelementptr inbounds [40 x i257], [40 x i257]* %seq, i257 0, i257 %inc1.i
  store i257 %add.sink.i, i257* %2, align 1
  %mul10.i = shl nsw i257 %p1.0.i, 1
  %mul12.i = mul nsw i257 %add.sink.i, 3
  %cmp.i = icmp slt i257 %mul12.i, %size
  br i1 %cmp.i, label %do.body.i, label %increment.exit

increment.exit:                                   ; preds = %if.end.i
  %3 = icmp sgt i257 %s.0.i, 0
  %spec.select = select i1 %3, i257 %s.0.i, i257 0
  %cmp45 = icmp sgt i257 %spec.select, -1
  br i1 %cmp45, label %while.body, label %while.end

while.cond.loopexit:                              ; preds = %for.end, %while.body
  %cmp = icmp sgt i257 %s.046, 0
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %increment.exit, %while.cond.loopexit
  %s.046 = phi i257 [ %dec, %while.cond.loopexit ], [ %spec.select, %increment.exit ]
  %dec = add nsw i257 %s.046, -1
  %arrayidx = getelementptr inbounds [40 x i257], [40 x i257]* %seq, i257 0, i257 %s.046
  %4 = load i257, i257* %arrayidx, align 1, !tbaa !2
  %cmp143 = icmp slt i257 %4, %size
  br i1 %cmp143, label %for.body, label %while.cond.loopexit

for.body:                                         ; preds = %while.body, %for.end
  %i.044 = phi i257 [ %inc14, %for.end ], [ %4, %while.body ]
  %arrayidx2 = getelementptr inbounds i257, i257* %a, i257 %i.044
  %5 = load i257, i257* %arrayidx2, align 1, !tbaa !2
  %cmp440 = icmp slt i257 %i.044, %4
  br i1 %cmp440, label %for.end, label %land.rhs

land.rhs:                                         ; preds = %for.body, %for.body7
  %j.041 = phi i257 [ %sub, %for.body7 ], [ %i.044, %for.body ]
  %sub = sub nsw i257 %j.041, %4
  %arrayidx5 = getelementptr inbounds i257, i257* %a, i257 %sub
  %6 = load i257, i257* %arrayidx5, align 1, !tbaa !2
  %cmp6 = icmp slt i257 %5, %6
  %arrayidx10 = getelementptr inbounds i257, i257* %a, i257 %j.041
  br i1 %cmp6, label %for.body7, label %for.end

for.body7:                                        ; preds = %land.rhs
  store i257 %6, i257* %arrayidx10, align 1, !tbaa !2
  %cmp4 = icmp slt i257 %sub, %4
  br i1 %cmp4, label %for.end.loopexit, label %land.rhs

for.end.loopexit:                                 ; preds = %for.body7
  %arrayidx5.le = getelementptr inbounds i257, i257* %a, i257 %sub
  br label %for.end

for.end:                                          ; preds = %land.rhs, %for.end.loopexit, %for.body
  %arrayidx12.pre-phi = phi i257* [ %arrayidx2, %for.body ], [ %arrayidx5.le, %for.end.loopexit ], [ %arrayidx10, %land.rhs ]
  store i257 %5, i257* %arrayidx12.pre-phi, align 1, !tbaa !2
  %inc14 = add nsw i257 %i.044, 1
  %cmp1 = icmp slt i257 %inc14, %size
  br i1 %cmp1, label %for.body, label %while.cond.loopexit

while.end:                                        ; preds = %while.cond.loopexit, %increment.exit
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %0) #3
  ret void
}

attributes #0 = { norecurse nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
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
