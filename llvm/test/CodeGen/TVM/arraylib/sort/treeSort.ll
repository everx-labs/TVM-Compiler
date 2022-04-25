; RUN: llc < %s -march=tvm 
; ModuleID = 'treeSort.c'
source_filename = "treeSort.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

%struct.node = type { i257, %struct.node*, %struct.node* }

@root = dso_local local_unnamed_addr global %struct.node* null, align 1

; Function Attrs: nounwind
define dso_local void @insert(i257 %data) local_unnamed_addr #0 {
entry:
  %temp = alloca %struct.node, align 1
  %0 = bitcast %struct.node* %temp to i8*
  call void @llvm.lifetime.start.p0i8(i64 3, i8* nonnull %0) #3
  %data1 = getelementptr inbounds %struct.node, %struct.node* %temp, i257 0, i32 0
  store i257 %data, i257* %data1, align 1, !tbaa !2
  %leftChild = getelementptr inbounds %struct.node, %struct.node* %temp, i257 0, i32 1
  store %struct.node* null, %struct.node** %leftChild, align 1, !tbaa !8
  %rightChild = getelementptr inbounds %struct.node, %struct.node* %temp, i257 0, i32 2
  store %struct.node* null, %struct.node** %rightChild, align 1, !tbaa !9
  %1 = load %struct.node*, %struct.node** @root, align 1, !tbaa !10
  %cmp = icmp eq %struct.node* %1, null
  br i1 %cmp, label %cleanup, label %while.cond

while.cond:                                       ; preds = %entry, %while.cond.backedge
  %current.0 = phi %struct.node* [ %current.0.be, %while.cond.backedge ], [ %1, %entry ]
  %data2 = getelementptr inbounds %struct.node, %struct.node* %current.0, i257 0, i32 0
  %2 = load i257, i257* %data2, align 1, !tbaa !2
  %cmp3 = icmp sgt i257 %2, %data
  br i1 %cmp3, label %if.then4, label %if.else9

if.then4:                                         ; preds = %while.cond
  %leftChild5 = getelementptr inbounds %struct.node, %struct.node* %current.0, i257 0, i32 1
  %3 = load %struct.node*, %struct.node** %leftChild5, align 1, !tbaa !8
  %cmp6 = icmp eq %struct.node* %3, null
  br i1 %cmp6, label %if.then7, label %while.cond.backedge

if.then7:                                         ; preds = %if.then4
  %leftChild5.le = getelementptr inbounds %struct.node, %struct.node* %current.0, i257 0, i32 1
  br label %cleanup

if.else9:                                         ; preds = %while.cond
  %rightChild10 = getelementptr inbounds %struct.node, %struct.node* %current.0, i257 0, i32 2
  %4 = load %struct.node*, %struct.node** %rightChild10, align 1, !tbaa !9
  %cmp11 = icmp eq %struct.node* %4, null
  br i1 %cmp11, label %if.then12, label %while.cond.backedge

while.cond.backedge:                              ; preds = %if.else9, %if.then4
  %current.0.be = phi %struct.node* [ %3, %if.then4 ], [ %4, %if.else9 ]
  br label %while.cond

if.then12:                                        ; preds = %if.else9
  %rightChild10.le = getelementptr inbounds %struct.node, %struct.node* %current.0, i257 0, i32 2
  br label %cleanup

cleanup:                                          ; preds = %entry, %if.then12, %if.then7
  %root.sink = phi %struct.node** [ %rightChild10.le, %if.then12 ], [ %leftChild5.le, %if.then7 ], [ @root, %entry ]
  store %struct.node* %temp, %struct.node** %root.sink, align 1, !tbaa !10
  call void @llvm.lifetime.end.p0i8(i64 3, i8* nonnull %0) #3
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #1

; Function Attrs: nounwind readonly
define dso_local void @inorder_traversal(%struct.node* nocapture %root) local_unnamed_addr #2 {
entry:
  ret void
}

; Function Attrs: nounwind
define dso_local void @treeSort(i257* nocapture readonly %array, i257 %len) local_unnamed_addr #0 {
entry:
  %temp.i = alloca %struct.node, align 1
  %cmp5 = icmp sgt i257 %len, 0
  br i1 %cmp5, label %for.body.lr.ph, label %for.end

for.body.lr.ph:                                   ; preds = %entry
  %0 = bitcast %struct.node* %temp.i to i8*
  %data1.i = getelementptr inbounds %struct.node, %struct.node* %temp.i, i257 0, i32 0
  %leftChild.i = getelementptr inbounds %struct.node, %struct.node* %temp.i, i257 0, i32 1
  %rightChild.i = getelementptr inbounds %struct.node, %struct.node* %temp.i, i257 0, i32 2
  %.pre = load %struct.node*, %struct.node** @root, align 1, !tbaa !10
  br label %for.body

for.body:                                         ; preds = %for.body.lr.ph, %insert.exit
  %1 = phi %struct.node* [ %.pre, %for.body.lr.ph ], [ %6, %insert.exit ]
  %i.06 = phi i257 [ 0, %for.body.lr.ph ], [ %inc, %insert.exit ]
  %arrayidx = getelementptr inbounds i257, i257* %array, i257 %i.06
  %2 = load i257, i257* %arrayidx, align 1, !tbaa !11
  call void @llvm.lifetime.start.p0i8(i64 3, i8* nonnull %0) #3
  store i257 %2, i257* %data1.i, align 1, !tbaa !2
  store %struct.node* null, %struct.node** %leftChild.i, align 1, !tbaa !8
  store %struct.node* null, %struct.node** %rightChild.i, align 1, !tbaa !9
  %cmp.i = icmp eq %struct.node* %1, null
  br i1 %cmp.i, label %insert.exit, label %while.cond.i

while.cond.i:                                     ; preds = %for.body, %while.cond.i.backedge
  %current.0.i = phi %struct.node* [ %current.0.i.be, %while.cond.i.backedge ], [ %1, %for.body ]
  %data2.i = getelementptr inbounds %struct.node, %struct.node* %current.0.i, i257 0, i32 0
  %3 = load i257, i257* %data2.i, align 1, !tbaa !2
  %cmp3.i = icmp sgt i257 %3, %2
  br i1 %cmp3.i, label %if.then4.i, label %if.else9.i

if.then4.i:                                       ; preds = %while.cond.i
  %leftChild5.i = getelementptr inbounds %struct.node, %struct.node* %current.0.i, i257 0, i32 1
  %4 = load %struct.node*, %struct.node** %leftChild5.i, align 1, !tbaa !8
  %cmp6.i = icmp eq %struct.node* %4, null
  br i1 %cmp6.i, label %if.then7.i, label %while.cond.i.backedge

if.then7.i:                                       ; preds = %if.then4.i
  %leftChild5.i.le = getelementptr inbounds %struct.node, %struct.node* %current.0.i, i257 0, i32 1
  br label %insert.exit

if.else9.i:                                       ; preds = %while.cond.i
  %rightChild10.i = getelementptr inbounds %struct.node, %struct.node* %current.0.i, i257 0, i32 2
  %5 = load %struct.node*, %struct.node** %rightChild10.i, align 1, !tbaa !9
  %cmp11.i = icmp eq %struct.node* %5, null
  br i1 %cmp11.i, label %if.then12.i, label %while.cond.i.backedge

while.cond.i.backedge:                            ; preds = %if.else9.i, %if.then4.i
  %current.0.i.be = phi %struct.node* [ %4, %if.then4.i ], [ %5, %if.else9.i ]
  br label %while.cond.i

if.then12.i:                                      ; preds = %if.else9.i
  %rightChild10.i.le = getelementptr inbounds %struct.node, %struct.node* %current.0.i, i257 0, i32 2
  br label %insert.exit

insert.exit:                                      ; preds = %for.body, %if.then7.i, %if.then12.i
  %root.sink = phi %struct.node** [ %leftChild5.i.le, %if.then7.i ], [ %rightChild10.i.le, %if.then12.i ], [ @root, %for.body ]
  %6 = phi %struct.node* [ %1, %if.then7.i ], [ %1, %if.then12.i ], [ %temp.i, %for.body ]
  store %struct.node* %temp.i, %struct.node** %root.sink, align 1, !tbaa !10
  call void @llvm.lifetime.end.p0i8(i64 3, i8* nonnull %0) #3
  %inc = add nuw nsw i257 %i.06, 1
  %cmp = icmp slt i257 %inc, %len
  br i1 %cmp, label %for.body, label %for.end

for.end:                                          ; preds = %insert.exit, %entry
  ret void
}

attributes #0 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
!2 = !{!3, !4, i64 0}
!3 = !{!"node", !4, i64 0, !7, i64 1, !7, i64 2}
!4 = !{!"long", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = !{!"any pointer", !5, i64 0}
!8 = !{!3, !7, i64 1}
!9 = !{!3, !7, i64 2}
!10 = !{!7, !7, i64 0}
!11 = !{!4, !4, i64 0}
