; RUN: llc < %s -O0 -march=tvm | tvm-testrun --no-trace --entry main | FileCheck %s

target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define dso_local i64 @f()  {
  ret i64 1234
}

define dso_local void @main()  {
; CHECK-NOT: custom error
  %1 = call i64 @f()
  %2 = sub i64 %1, 1234
  call void @llvm.tvm.throwif(i64 %2, i64 13)
  ret void
}

declare void @llvm.tvm.throwif(i64 %cond, i64 %exception)
