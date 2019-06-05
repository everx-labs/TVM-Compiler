; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: nounwind uwtable
define i64 @func(i64, i64, i64) nounwind {
  %4 = icmp sgt i64 %0, 0
  br i1 %4, label %5, label %8

; <label>:5:                                      ; preds = %3, %23
  %6 = phi i64 [ %24, %23 ], [ 0, %3 ]
  %7 = phi i64 [ %17, %23 ], [ 0, %3 ]
  br label %10

; <label>:8:                                      ; preds = %23, %3
  %9 = phi i64 [ 0, %3 ], [ %17, %23 ]
  ret i64 %9

; <label>:10:                                     ; preds = %5, %20
  %11 = phi i64 [ %17, %20 ], [ %7, %5 ]
  %12 = phi i64 [ %21, %20 ], [ 0, %5 ]
  br label %13

; <label>:13:                                     ; preds = %13, %10
  %14 = phi i64 [ %11, %10 ], [ %17, %13 ]
  %15 = phi i64 [ 0, %10 ], [ %18, %13 ]
  %16 = tail call i64 @foo(i64 %6, i64 %12, i64 %15) #2
  %17 = add i64 %16, %14
  %18 = add i64 %15, 1
  %19 = icmp slt i64 %18, %2
  br i1 %19, label %13, label %20

; <label>:20:                                     ; preds = %13
  %21 = add i64 %12, 1
  %22 = icmp slt i64 %21, %1
  br i1 %22, label %10, label %23

; <label>:23:                                     ; preds = %20
  %24 = add i64 %6, 1
  %25 = icmp eq i64 %24, %0
  br i1 %25, label %8, label %5
}

declare i64 @foo(i64, i64, i64) nounwind
