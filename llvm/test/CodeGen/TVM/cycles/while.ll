; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64) nounwind {
  %2 = icmp sgt i64 %0, 0
  br i1 %2, label %3, label %9

; <label>:3:                                      ; preds = %1, %3
  %4 = phi i64 [ %7, %3 ], [ %0, %1 ]
  %5 = phi i64 [ %6, %3 ], [ 1, %1 ]
  %6 = mul i64 %4, %5
  %7 = add i64 %4, -1
  %8 = icmp sgt i64 %4, 1
  br i1 %8, label %3, label %9

; <label>:9:                                      ; preds = %3, %1
  %10 = phi i64 [ 1, %1 ], [ %6, %3 ]
  ret i64 %10
}
