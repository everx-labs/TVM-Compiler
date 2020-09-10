; RUN: llc < %s -O0 -march=tvm | FileCheck %s

target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"
    
define dso_local i257 @reg_to_reg_copy(i257 %c) {
; CHECK-LABEL: reg_to_reg_copy:
; CHECK: IFELSE
entry:
  %cmp4 = icmp sgt i257 %c, 0
  br i1 %cmp4, label %for.body, label %for.end

for.body:                                         ; preds = %entry, %for.body
  %i.05 = phi i257 [ %inc, %for.body ], [ 0, %entry ]
  %inc = add nuw nsw i257 %i.05, 1
  %cmp = icmp slt i257 %inc, %c
  br i1 %cmp, label %for.body, label %for.end

for.end:                                          ; preds = %for.body, %entry
  %i.0.lcssa = phi i257 [ 0, %entry ], [ %c, %for.body ]
  ret i257 %i.0.lcssa
}

declare dso_local i257 @g(i257) local_unnamed_addr #1
