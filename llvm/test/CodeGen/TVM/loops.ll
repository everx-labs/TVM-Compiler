; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; CHECK-LABEL: for_loop
define i64 @for_loop(i64 %N) nounwind {
entry:
  %cmp6 = icmp sgt i64 %N, 0
  ; CHECK: PUSHCONT
  ; CHECK: IFELSE
  ; CHECK: PUSHCONT
  ; CHECK: JMPX
  br i1 %cmp6, label %for.body, label %for.cond.cleanup

for.cond.cleanup:
  %sum.0.lcssa = phi i64 [ 0, %entry ], [ %add, %for.body ]
  ret i64 %sum.0.lcssa

for.body:
  %i.08 = phi i64 [ %inc, %for.body ], [ 0, %entry ]
  %sum.07 = phi i64 [ %add, %for.body ], [ 0, %entry ]
  %add = add nsw i64 %i.08, %sum.07
  %inc = add nuw nsw i64 %i.08, 1
  %cmp = icmp slt i64 %inc, %N
  br i1 %cmp, label %for.body, label %for.cond.cleanup
}

; CHECK-LABEL: while_loop
define i64 @while_loop(i64 %N) nounwind {
entry:
  %cmp5 = icmp sgt i64 %N, 0
  ; CHECK: PUSHCONT
  ; CHECK: IFELSE
  ; CHECK: PUSHCONT
  ; CHECK: JMPX
  br i1 %cmp5, label %while.body, label %while.end

while.body:
  %i.07 = phi i64 [ %inc, %while.body ], [ 0, %entry ]
  %sum.06 = phi i64 [ %add, %while.body ], [ 0, %entry ]
  %inc = add nuw nsw i64 %i.07, 1
  %add = add nsw i64 %i.07, %sum.06
  %cmp = icmp slt i64 %inc, %N
  br i1 %cmp, label %while.body, label %while.end

while.end:
  %sum.0.lcssa = phi i64 [ 0, %entry ], [ %add, %while.body ]
  ret i64 %sum.0.lcssa
}
