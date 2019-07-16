; RUN: llc < %s -O0 -march=tvm | tvm-testrun --no-trace --entry main | FileCheck %s

target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define dso_local i64 @test_func(i64 %v0, i64 %v1, i64 %v2, i64 %v3, i64 %v4, i64 %v5, i64 %v6, i64 %v7, i64 %a0, i64 %a1, i64 %a2, i64 %a3, i64 %a4, i64 %a5, i64 %a6, i64 %a7, i64 %a8, i64 %a9) local_unnamed_addr noinline norecurse nounwind readnone {
entry:
  %add = add i64 %v0, 2
  %add2 = add i64 %add, %v1
  %add3 = add i64 %add2, %v2
  %add4 = add i64 %add3, %v3
  %add5 = add i64 %add4, %v4
  %add6 = add i64 %add5, %v5
  %add7 = add i64 %add6, %v6
  %add8 = add i64 %add7, %v7
  %add9 = add i64 %add8, %a0
  %add10 = add i64 %add9, %a1
  %add11 = add i64 %add10, %a2
  %add12 = add i64 %add11, %a3
  %add13 = add i64 %add12, %a4
  %add14 = add i64 %add13, %a5
  %add15 = add i64 %add14, %a6
  %add16 = add i64 %add15, %a7
  %add17 = add i64 %add16, %a8
  %add18 = add i64 %add17, %a9
  ret i64 %add18
}

define dso_local i64 @main() local_unnamed_addr norecurse nounwind readnone {
entry:
; CHECK-NOT: custom error
  %call = call i64 @test_func(i64 0, i64 1, i64 2, i64 3, i64 4, i64 5, i64 6, i64 7, i64 8, i64 9, i64 10, i64 11, i64 12, i64 13, i64 14, i64 15, i64 16, i64 17)
  %not_eq = icmp ne i64 %call, 155
  %not_eq64 = select i1 %not_eq, i64 1, i64 0
  call void @llvm.tvm.throwif(i64 %not_eq64, i64 13)
  ret i64 %call
}

declare void @llvm.tvm.throwif(i64, i64) nounwind

