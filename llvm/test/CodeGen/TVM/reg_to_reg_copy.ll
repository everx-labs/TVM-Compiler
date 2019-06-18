; RUN: llc < %s -O0 -march=tvm | FileCheck %s

target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"
    
define dso_local i64 @reg_to_reg_copy(i64 %c) {
; CHECK-LABEL: reg_to_reg_copy:
; CHECK: GREATER
; CHECK: LESS
; CHECK: UNTIL
entry:
  %cmp4 = icmp sgt i64 %c, 0
  br i1 %cmp4, label %for.body, label %for.end

for.body:                                         ; preds = %entry, %for.body
  %i.05 = phi i64 [ %inc, %for.body ], [ 0, %entry ]
  %inc = add nuw nsw i64 %i.05, 1
  %cmp = icmp slt i64 %inc, %c
  br i1 %cmp, label %for.body, label %for.end

for.end:                                          ; preds = %for.body, %entry
  %i.0.lcssa = phi i64 [ 0, %entry ], [ %c, %for.body ]
  ret i64 %i.0.lcssa
}

declare dso_local i64 @g(i64) local_unnamed_addr #1
