; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: nounwind uwtable
define i64 @func(i64, i64, i64) nounwind {
  %4 = icmp sgt i64 %1, 0
  %5 = icmp sgt i64 %2, 0
  br i1 %4, label %6, label %26

; <label>:6:                                      ; preds = %3, %9
  %7 = phi i64 [ %10, %9 ], [ 0, %3 ]
  %8 = phi i64 [ %11, %9 ], [ 0, %3 ]
  br i1 %5, label %13, label %9

; <label>:9:                                      ; preds = %16, %6
  %10 = phi i64 [ %7, %6 ], [ %23, %16 ]
  %11 = add i64 %8, 1
  %12 = icmp slt i64 %11, %0
  br i1 %12, label %6, label %26

; <label>:13:                                     ; preds = %6, %16
  %14 = phi i64 [ %17, %16 ], [ 0, %6 ]
  %15 = phi i64 [ %23, %16 ], [ %7, %6 ]
  br label %19

; <label>:16:                                     ; preds = %19
  %17 = add i64 %14, 1
  %18 = icmp eq i64 %17, %1
  br i1 %18, label %9, label %13

; <label>:19:                                     ; preds = %19, %13
  %20 = phi i64 [ 0, %13 ], [ %24, %19 ]
  %21 = phi i64 [ %15, %13 ], [ %23, %19 ]
  %22 = tail call i64 @foo(i64 %8, i64 %14, i64 %20) #2
  %23 = add i64 %22, %21
  %24 = add i64 %20, 1
  %25 = icmp eq i64 %24, %2
  br i1 %25, label %16, label %19

; <label>:26:                                     ; preds = %9, %3
  %27 = phi i64 [ 0, %3 ], [ %10, %9 ]
  ret i64 %27
}

declare i64 @foo(i64, i64, i64) nounwind 
