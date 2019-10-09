; RUN: llc -O3 -tvm-trace-calls < %s -march=tvm | FileCheck %s

target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

declare void @llvm.tvm.logstr(i257*) #1
declare void @llvm.tvm.printstr(i257*) #1

@.str = private unnamed_addr constant [22 x i8] c"123456789098765432123\00", align 1

; CHECK-LABEL: test_logstr
define void @test_logstr(i257 %x) {
; CHECK: LOGSTR test_logstr
; CHECK: LOGSTR 123456789098765
  %ptr = getelementptr inbounds [22 x i8], [22 x i8]* @.str, i257 0, i257 0
  %ptr257 = bitcast i8* %ptr to i257*
  tail call void @llvm.tvm.logstr(i257* %ptr257)
  ret void
}

; CHECK-LABEL: test_printstr
define void @test_printstr(i257 %x) {
; CHECK: LOGSTR test_printstr
; CHECK: PRINTSTR 123456789098765
  %ptr = getelementptr inbounds [22 x i8], [22 x i8]* @.str, i257 0, i257 0
  %ptr257 = bitcast i8* %ptr to i257*
  tail call void @llvm.tvm.printstr(i257* %ptr257)
  ret void
}

