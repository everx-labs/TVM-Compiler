; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: nounwind uwtable
define i64 @func(i64, i64, i64) nounwind 
 {
  %4 = icmp sgt i64 %0, 0
  %5 = icmp sgt i64 %2, 0
  %6 = and i1 %4, %5
  br i1 %6, label %7, label %26

; <label>:7:                                      ; preds = %3, %23
  %8 = phi i64 [ %24, %23 ], [ 0, %3 ]
  %9 = phi i64 [ %20, %23 ], [ 0, %3 ]
  br label %10

; <label>:10:                                     ; preds = %7, %13
  %11 = phi i64 [ %20, %13 ], [ %9, %7 ]
  %12 = phi i64 [ %14, %13 ], [ 0, %7 ]
  br label %16

; <label>:13:                                     ; preds = %16
  %14 = add i64 %12, 1
  %15 = icmp slt i64 %14, %1
  br i1 %15, label %10, label %23

; <label>:16:                                     ; preds = %16, %10
  %17 = phi i64 [ 0, %10 ], [ %21, %16 ]
  %18 = phi i64 [ %11, %10 ], [ %20, %16 ]
  %19 = tail call i64 @foo(i64 %8, i64 %12, i64 %17) #2
  %20 = add i64 %19, %18
  %21 = add i64 %17, 1
  %22 = icmp eq i64 %21, %2
  br i1 %22, label %13, label %16

; <label>:23:                                     ; preds = %13
  %24 = add i64 %8, 1
  %25 = icmp eq i64 %24, %0
  br i1 %25, label %26, label %7

; <label>:26:                                     ; preds = %23, %3
  %27 = phi i64 [ 0, %3 ], [ %20, %23 ]
  ret i64 %27
}

declare i64 @foo(i64, i64, i64) nounwind 
