; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
source_filename = "sum-global-array.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm-unknown-unknown"

@array = dso_local global [9 x i257] [i257 1, i257 2, i257 3, i257 4, i257 5, i257 6, i257 7, i257 8, i257 9], align 1

; Function Attrs: noinline nounwind optnone
define dso_local i257 @sum(i257* %array, i257 %idx) #0 {
entry:
  %array.addr = alloca i257*, align 1
  %idx.addr = alloca i257, align 1
  store i257* %array, i257** %array.addr, align 1
  store i257 %idx, i257* %idx.addr, align 1
  ret i257 0
}

; Function Attrs: noinline nounwind optnone
; CHECK-LABEL: main
define dso_local i257 @main() #0 {
; CHECK: ZERO
; CHECK: CALL $sum$
  %retval = alloca i257, align 1
  %x = alloca i257*, align 1
  store i257 0, i257* %retval, align 1
  store i257* getelementptr inbounds ([9 x i257], [9 x i257]* @array, i32 0, i32 0), i257** %x, align 1
  %call = call i257 @sum(i257* getelementptr inbounds ([9 x i257], [9 x i257]* @array, i32 0, i32 0), i257 0)
  %1 = load i257*, i257** %x, align 1
  %call1 = call i257 @sum(i257* %1, i257 0)
  %add = add nsw i257 %call, %call1
  ret i257 %add
}
