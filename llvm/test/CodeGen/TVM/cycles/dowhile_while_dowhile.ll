; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: nounwind uwtable
define i64 @func(i64, i64, i64) nounwind 
 {
  %4 = icmp sgt i64 %1, 0
  br i1 %4, label %5, label %24

; <label>:5:                                      ; preds = %3, %21
  %6 = phi i64 [ %12, %21 ], [ 0, %3 ]
  %7 = phi i64 [ %22, %21 ], [ 0, %3 ]
  br label %18

; <label>:8:                                      ; preds = %18, %8
  %9 = phi i64 [ %12, %8 ], [ %20, %18 ]
  %10 = phi i64 [ %13, %8 ], [ 0, %18 ]
  %11 = tail call i64 @foo(i64 %7, i64 %19, i64 %10) #2
  %12 = add i64 %11, %9
  %13 = add i64 %10, 1
  %14 = icmp slt i64 %13, %2
  br i1 %14, label %8, label %15

; <label>:15:                                     ; preds = %8
  %16 = add i64 %19, 1
  %17 = icmp eq i64 %16, %1
  br i1 %17, label %21, label %18

; <label>:18:                                     ; preds = %15, %5
  %19 = phi i64 [ 0, %5 ], [ %16, %15 ]
  %20 = phi i64 [ %6, %5 ], [ %12, %15 ]
  br label %8

; <label>:21:                                     ; preds = %15
  %22 = add i64 %7, 1
  %23 = icmp slt i64 %22, %0
  br i1 %23, label %5, label %24

; <label>:24:                                     ; preds = %21, %3
  %25 = phi i64 [ 0, %3 ], [ %12, %21 ]
  ret i64 %25
}

declare i64 @foo(i64, i64, i64) nounwind 
