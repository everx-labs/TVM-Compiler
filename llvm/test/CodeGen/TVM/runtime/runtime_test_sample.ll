; RUN: llc < %s -O0 -march=tvm | tvm-testrun --no-trace --entry main | FileCheck %s

target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define dso_local i257 @f()  {
  ret i257 1234
}

define dso_local void @main()  {
; CHECK-NOT: custom error
  %1 = call i257 @f()
  %2 = sub i257 %1, 1234
  call void @llvm.tvm.throwif(i257 %2, i257 13)
  ret void
}

declare void @llvm.tvm.throwif(i257 %cond, i257 %exception)
