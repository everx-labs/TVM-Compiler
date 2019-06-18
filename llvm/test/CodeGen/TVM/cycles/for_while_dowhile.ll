; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: nounwind uwtable
define i64 @func(i64, i64, i64) nounwind {
  %4 = icmp sgt i64 %0, 0
  %5 = icmp sgt i64 %1, 0
  %6 = and i1 %4, %5
  br i1 %6, label %7, label %26

; <label>:7:                                      ; preds = %3, %23
  %8 = phi i64 [ %24, %23 ], [ 0, %3 ]
  %9 = phi i64 [ %14, %23 ], [ 0, %3 ]
  br label %20

; <label>:10:                                     ; preds = %20, %10
  %11 = phi i64 [ %14, %10 ], [ %22, %20 ]
  %12 = phi i64 [ %15, %10 ], [ 0, %20 ]
  %13 = tail call i64 @foo(i64 %8, i64 %21, i64 %12) #2
  %14 = add i64 %13, %11
  %15 = add i64 %12, 1
  %16 = icmp slt i64 %15, %2
  br i1 %16, label %10, label %17

; <label>:17:                                     ; preds = %10
  %18 = add i64 %21, 1
  %19 = icmp eq i64 %18, %1
  br i1 %19, label %23, label %20

; <label>:20:                                     ; preds = %17, %7
  %21 = phi i64 [ 0, %7 ], [ %18, %17 ]
  %22 = phi i64 [ %9, %7 ], [ %14, %17 ]
  br label %10

; <label>:23:                                     ; preds = %17
  %24 = add i64 %8, 1
  %25 = icmp eq i64 %24, %0
  br i1 %25, label %26, label %7

; <label>:26:                                     ; preds = %23, %3
  %27 = phi i64 [ 0, %3 ], [ %14, %23 ]
  ret i64 %27
}

declare i64 @foo(i64, i64, i64) nounwind 
