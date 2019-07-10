; RUN: llc -O3 -tvm-trace-calls < %s -march=tvm | FileCheck %s

target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

declare void @llvm.tvm.logstr(i8*) #1

@.str = private unnamed_addr constant [22 x i8] c"123456789098765432123\00", align 1

; CHECK-LABEL: test_function
define void @test_function(i64 %x) {
; CHECK: LOGSTR test_function
; CHECK: LOGSTR 123456789098765
  tail call void @llvm.tvm.logstr(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str, i64 0, i64 0))
  ret void
}

