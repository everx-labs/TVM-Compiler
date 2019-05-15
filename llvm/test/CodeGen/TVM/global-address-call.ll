; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
source_filename = "sum-global-array.c"
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm-unknown-unknown"

@array = dso_local global [9 x i64] [i64 1, i64 2, i64 3, i64 4, i64 5, i64 6, i64 7, i64 8, i64 9], align 16

; Function Attrs: noinline nounwind optnone
define dso_local i64 @sum(i64* %array, i64 %idx) #0 {
entry:
  %array.addr = alloca i64*, align 8
  %idx.addr = alloca i64, align 8
  store i64* %array, i64** %array.addr, align 8
  store i64 %idx, i64* %idx.addr, align 8
  ret i64 0
}

; Function Attrs: noinline nounwind optnone
; CHECK-LABEL: main
define dso_local i64 @main() #0 {
; CHECK: PUSHINT $sum$
; CHECK: PUSHINT 0
; CHECK: PUSH s1
; CHECK: XCHG s0, s1
; CHECK: XCHG s1, s3
; CHECK: CALL 1
  %retval = alloca i64, align 8
  %x = alloca i64*, align 8
  store i64 0, i64* %retval, align 8
  store i64* getelementptr inbounds ([9 x i64], [9 x i64]* @array, i32 0, i32 0), i64** %x, align 8
  %call = call i64 @sum(i64* getelementptr inbounds ([9 x i64], [9 x i64]* @array, i32 0, i32 0), i64 0)
  %1 = load i64*, i64** %x, align 8
  %call1 = call i64 @sum(i64* %1, i64 0)
  %add = add nsw i64 %call, %call1
  ret i64 %add
}
