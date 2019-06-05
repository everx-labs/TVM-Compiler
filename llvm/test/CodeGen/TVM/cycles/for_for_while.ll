; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: nounwind uwtable
define i64 @func(i64, i64, i64) nounwind {
  %4 = icmp sgt i64 %0, 0
  br i1 %4, label %5, label %28

; <label>:5:                                      ; preds = %3
  %6 = icmp sgt i64 %1, 0
  %7 = icmp sgt i64 %2, 0
  br i1 %6, label %8, label %28

; <label>:8:                                      ; preds = %5, %11
  %9 = phi i64 [ %13, %11 ], [ 0, %5 ]
  %10 = phi i64 [ %12, %11 ], [ 0, %5 ]
  br i1 %7, label %15, label %11

; <label>:11:                                     ; preds = %18, %8
  %12 = phi i64 [ %10, %8 ], [ %25, %18 ]
  %13 = add i64 %9, 1
  %14 = icmp eq i64 %13, %0
  br i1 %14, label %28, label %8

; <label>:15:                                     ; preds = %8, %18
  %16 = phi i64 [ %19, %18 ], [ 0, %8 ]
  %17 = phi i64 [ %25, %18 ], [ %10, %8 ]
  br label %21

; <label>:18:                                     ; preds = %21
  %19 = add i64 %16, 1
  %20 = icmp eq i64 %19, %1
  br i1 %20, label %11, label %15

; <label>:21:                                     ; preds = %21, %15
  %22 = phi i64 [ 0, %15 ], [ %26, %21 ]
  %23 = phi i64 [ %17, %15 ], [ %25, %21 ]
  %24 = tail call i64 @foo(i64 %9, i64 %16, i64 %22) #2
  %25 = add i64 %24, %23
  %26 = add i64 %22, 1
  %27 = icmp eq i64 %26, %2
  br i1 %27, label %18, label %21

; <label>:28:                                     ; preds = %11, %5, %3
  %29 = phi i64 [ 0, %3 ], [ 0, %5 ], [ %12, %11 ]
  ret i64 %29
}

declare i64 @foo(i64, i64, i64) nounwind 
