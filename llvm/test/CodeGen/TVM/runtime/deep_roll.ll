; RUN: llc < %s -O0 -march=tvm | tvm-testrun --no-trace --entry main | FileCheck %s

target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define dso_local i257 @test_func(i257 %v0, i257 %v1, i257 %v2, i257 %v3, i257 %v4, i257 %v5, i257 %v6, i257 %v7, i257 %a0, i257 %a1, i257 %a2, i257 %a3, i257 %a4, i257 %a5, i257 %a6, i257 %a7, i257 %a8, i257 %a9) local_unnamed_addr noinline norecurse nounwind readnone {
entry:
  %add = add i257 %v0, 2
  %add2 = add i257 %add, %v1
  %add3 = add i257 %add2, %v2
  %add4 = add i257 %add3, %v3
  %add5 = add i257 %add4, %v4
  %add6 = add i257 %add5, %v5
  %add7 = add i257 %add6, %v6
  %add8 = add i257 %add7, %v7
  %add9 = add i257 %add8, %a0
  %add10 = add i257 %add9, %a1
  %add11 = add i257 %add10, %a2
  %add12 = add i257 %add11, %a3
  %add13 = add i257 %add12, %a4
  %add14 = add i257 %add13, %a5
  %add15 = add i257 %add14, %a6
  %add16 = add i257 %add15, %a7
  %add17 = add i257 %add16, %a8
  %add18 = add i257 %add17, %a9
  ret i257 %add18
}

define dso_local i257 @main() local_unnamed_addr norecurse nounwind readnone {
entry:
; CHECK-NOT: custom error
  %call = call i257 @test_func(i257 0, i257 1, i257 2, i257 3, i257 4, i257 5, i257 6, i257 7, i257 8, i257 9, i257 10, i257 11, i257 12, i257 13, i257 14, i257 15, i257 16, i257 17)
  %not_eq = icmp ne i257 %call, 155
  %not_eq64 = select i1 %not_eq, i257 1, i257 0
  call void @llvm.tvm.throwif(i257 %not_eq64, i257 13)
  ret i257 %call
}

declare void @llvm.tvm.throwif(i257, i257) nounwind

