; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: nounwind uwtable
define i64 @func(i64, i64, i64) nounwind
 {
  %4 = icmp sgt i64 %2, 0
  br i1 %4, label %5, label %24

; <label>:5:                                      ; preds = %3, %21
  %6 = phi i64 [ %18, %21 ], [ 0, %3 ]
  %7 = phi i64 [ %22, %21 ], [ 0, %3 ]
  br label %8

; <label>:8:                                      ; preds = %5, %11
  %9 = phi i64 [ %6, %5 ], [ %18, %11 ]
  %10 = phi i64 [ 0, %5 ], [ %12, %11 ]
  br label %14

; <label>:11:                                     ; preds = %14
  %12 = add i64 %10, 1
  %13 = icmp slt i64 %12, %1
  br i1 %13, label %8, label %21

; <label>:14:                                     ; preds = %14, %8
  %15 = phi i64 [ 0, %8 ], [ %19, %14 ]
  %16 = phi i64 [ %9, %8 ], [ %18, %14 ]
  %17 = tail call i64 @foo(i64 %7, i64 %10, i64 %15) #2
  %18 = add i64 %17, %16
  %19 = add i64 %15, 1
  %20 = icmp eq i64 %19, %2
  br i1 %20, label %11, label %14

; <label>:21:                                     ; preds = %11
  %22 = add i64 %7, 1
  %23 = icmp slt i64 %22, %0
  br i1 %23, label %5, label %24

; <label>:24:                                     ; preds = %21, %3
  %25 = phi i64 [ 0, %3 ], [ %18, %21 ]
  ret i64 %25
}

declare i64 @foo(i64, i64, i64) nounwind
