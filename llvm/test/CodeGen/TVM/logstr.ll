; RUN: llc -O3 -tvm-trace-calls < %s -march=tvm | FileCheck %s

target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

declare void @llvm.tvm.logstr(i257*) #1
declare void @llvm.tvm.printstr(i257*) #1

@.str = private unnamed_addr constant [22 x i257] [i257 49, i257 50, i257 51, i257 52, i257 53, i257 54, i257 55, i257 56, i257 57, i257 48, i257 57, i257 56, i257 55, i257 54, i257 53, i257 52, i257 51, i257 50, i257 49, i257 50, i257 51, i257 0], align 1

; CHECK-LABEL: test_logstr
define void @test_logstr(i257 %x) {
; CHECK: LOGSTR test_logstr
; CHECK: LOGSTR 123456789098765
  %ptr = getelementptr inbounds [22 x i257], [22 x i257]* @.str, i257 0, i257 0
  tail call void @llvm.tvm.logstr(i257* %ptr)
  ret void
}

; CHECK-LABEL: test_printstr
define void @test_printstr(i257 %x) {
; CHECK: LOGSTR test_printstr
; CHECK: PRINTSTR 123456789098765
  %ptr = getelementptr inbounds [22 x i257], [22 x i257]* @.str, i257 0, i257 0
  tail call void @llvm.tvm.printstr(i257* %ptr)
  ret void
}
