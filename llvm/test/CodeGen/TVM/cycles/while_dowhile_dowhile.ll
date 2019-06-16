; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: nounwind uwtable
define i64 @func(i64, i64, i64) nounwind {
  %4 = icmp sgt i64 %0, 0
  br i1 %4, label %5, label %24

; <label>:5:                                      ; preds = %3, %21
  %6 = phi i64 [ %22, %21 ], [ 0, %3 ]
  %7 = phi i64 [ %15, %21 ], [ 0, %3 ]
  br label %8

; <label>:8:                                      ; preds = %5, %18
  %9 = phi i64 [ %15, %18 ], [ %7, %5 ]
  %10 = phi i64 [ %19, %18 ], [ 0, %5 ]
  br label %11

; <label>:11:                                     ; preds = %11, %8
  %12 = phi i64 [ %9, %8 ], [ %15, %11 ]
  %13 = phi i64 [ 0, %8 ], [ %16, %11 ]
  %14 = tail call i64 @foo(i64 %6, i64 %10, i64 %13) #2
  %15 = add i64 %14, %12
  %16 = add i64 %13, 1
  %17 = icmp slt i64 %16, %2
  br i1 %17, label %11, label %18

; <label>:18:                                     ; preds = %11
  %19 = add i64 %10, 1
  %20 = icmp slt i64 %19, %1
  br i1 %20, label %8, label %21

; <label>:21:                                     ; preds = %18
  %22 = add i64 %6, 1
  %23 = icmp eq i64 %22, %0
  br i1 %23, label %24, label %5

; <label>:24:                                     ; preds = %21, %3
  %25 = phi i64 [ 0, %3 ], [ %15, %21 ]
  ret i64 %25
}

declare i64 @foo(i64, i64, i64) nounwind
