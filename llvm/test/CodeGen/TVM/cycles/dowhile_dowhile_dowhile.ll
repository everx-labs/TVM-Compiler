; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: nounwind uwtable
define i64 @func(i64, i64, i64) nounwind {
  br label %4

; <label>:4:                                      ; preds = %20, %3
  %5 = phi i64 [ 0, %3 ], [ %14, %20 ]
  %6 = phi i64 [ 0, %3 ], [ %21, %20 ]
  br label %7

; <label>:7:                                      ; preds = %17, %4
  %8 = phi i64 [ %5, %4 ], [ %14, %17 ]
  %9 = phi i64 [ 0, %4 ], [ %18, %17 ]
  br label %10

; <label>:10:                                     ; preds = %10, %7
  %11 = phi i64 [ %8, %7 ], [ %14, %10 ]
  %12 = phi i64 [ 0, %7 ], [ %15, %10 ]
  %13 = tail call i64 @foo(i64 %6, i64 %9, i64 %12) #2
  %14 = add i64 %13, %11
  %15 = add i64 %12, 1
  %16 = icmp slt i64 %15, %2
  br i1 %16, label %10, label %17

; <label>:17:                                     ; preds = %10
  %18 = add i64 %9, 1
  %19 = icmp slt i64 %18, %1
  br i1 %19, label %7, label %20

; <label>:20:                                     ; preds = %17
  %21 = add i64 %6, 1
  %22 = icmp slt i64 %21, %0
  br i1 %22, label %4, label %23

; <label>:23:                                     ; preds = %20
  ret i64 %14
}

declare i64 @foo(i64, i64, i64) nounwind
