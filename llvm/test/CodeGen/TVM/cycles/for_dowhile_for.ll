; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: nounwind uwtable
define i64 @func(i64, i64, i64) nounwind {
entry:
  %3 = icmp sgt i64 %0, 0
  %4 = icmp sgt i64 %2, 0
  %5 = and i1 %3, %4
  br i1 %5, label %BB1, label %BB6

BB1:
  %6 = phi i64 [ %18, %BB5 ], [ 0, %entry ]
  %7 = phi i64 [ %15, %BB5 ], [ 0, %entry ]
  br label %BB2

BB2:
  %8 = phi i64 [ %14, %BB3 ], [ %7, %BB1 ]
  %9 = phi i64 [ %10, %BB3 ], [ 0, %BB1 ]
  br label %BB4

BB3:
  %10 = add i64 %9, 1
  %11 = icmp slt i64 %10, %1
  br i1 %11, label %BB2, label %BB5

BB4:
  %12 = phi i64 [ 0, %BB2 ], [ %16, %BB4 ]
  %13 = phi i64 [ %8, %BB2 ], [ %15, %BB4 ]
  %14 = tail call i64 @foo(i64 %6, i64 %9, i64 %12) #2
  %15 = add i64 %13, %14
  %16 = add i64 %12, 1
  %17 = icmp eq i64 %16, %2
  br i1 %17, label %BB3, label %BB4

BB5:
  %18 = add i64 %6, 1
  %19 = icmp eq i64 %18, %0
  br i1 %19, label %BB6, label %BB1

BB6:
  %20 = phi i64 [ 0, %entry ], [ %15, %BB5 ]
  ret i64 %20
}

declare i64 @foo(i64, i64, i64) nounwind
