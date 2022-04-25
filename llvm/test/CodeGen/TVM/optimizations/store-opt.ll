; RUN: opt -tvm-store-combine -S < %s -march=tvm | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: test1
define builder @test1() {
  %1 = call builder @llvm.tvm.newc()
  %2 = call builder @llvm.tvm.stu(i257 7, builder %1, i257 5)
  %3 = call builder @llvm.tvm.stu(i257 42, builder %2, i257 7)
  ; CHECK: %[[VR1:[0-9]+]] = call builder @llvm.tvm.stu(i257 938, builder %{{[0-9]+}}, i257 12)
  %4 = call builder @llvm.tvm.stu(i257 42, builder %3, i257 250)
  %5 = call builder @llvm.tvm.stu(i257 2, builder %4, i257 6)
  ; CHECK: %[[VR2:[0-9]+]] = call builder @llvm.tvm.stu(i257 2690, builder %[[VR1]], i257 256)
  %6 = call builder @llvm.tvm.sti(i257 -1, builder %5, i257 1)
  %7 = call builder @llvm.tvm.stu(i257 3, builder %6, i257 2)
  %8 = call builder @llvm.tvm.sti(i257 -7, builder %7, i257 3)
  ; CHECK: %[[VR3:[0-9]+]] = call builder @llvm.tvm.stu(i257 121, builder %[[VR2]], i257 6)
  %9 = call builder @llvm.tvm.sti(i257 -7, builder %8, i257 251)
  ; CHECK: %{{[0-9]+}} = call builder @llvm.tvm.sti(i257 -7, builder %[[VR3]], i257 251)
  ret builder %9
}

; CHECK-LABEL: test2
define builder @test2() {
  %1 = call builder @llvm.tvm.newc()
  %2 = call builder @llvm.tvm.newc()
  %3 = call builder @llvm.tvm.stu(i257 7, builder %1, i257 5)
  %4 = call builder @llvm.tvm.stu(i257 7, builder %3, i257 5)
  ; CHECK: %{{[0-9]+}} = call builder @llvm.tvm.stu(i257 231, builder %1, i257 10)
  %5 = call builder @llvm.tvm.stu(i257 7, builder %2, i257 5)
  ; CHECK: %{{[0-9]+}} = call builder @llvm.tvm.stu(i257 7, builder %2, i257 5)
  ret builder %4
}

; CHECK-LABEL: test3
define builder @test3() {
  %1 = call builder @llvm.tvm.newc()
  %2 = call builder @llvm.tvm.stu(i257 1, builder %1, i257 1)
  %3 = call builder @llvm.tvm.stu(i257 1, builder %2, i257 1)
  ; CHECK: %[[VR:[0-9]+]] = call builder @llvm.tvm.stu(i257 3, builder %{{[0-9]+}}, i257 2)
  %4 = call builder @llvm.tvm.stu(i257 1, builder %3, i257 1)
  ; CHECK: %{{[0-9]+}} = call builder @llvm.tvm.stu(i257 1, builder %[[VR]], i257 1)
  %5 = call builder @llvm.tvm.stu(i257 1, builder %3, i257 1)
  ; CHECK: %{{[0-9]+}} = call builder @llvm.tvm.stu(i257 1, builder %[[VR]], i257 1)
  ret builder %5
}

declare builder @llvm.tvm.newc()
declare builder @llvm.tvm.stu(i257, builder, i257)
declare builder @llvm.tvm.sti(i257, builder, i257)
