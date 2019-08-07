; RUN: llc < %s -O0 -march=tvm | tvm-testrun --no-trace --entry main | FileCheck %s

target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define dso_local i64 @deep_blkdrop(i64 %v0, i64 %v1, i64 %v2, i64 %v3, i64 %v4, i64 %v5, i64 %v6, i64 %v7, i64 %a0, i64 %a1, i64 %a2, i64 %a3, i64 %a4, i64 %a5, i64 %a6, i64 %a7, i64 %b0, i64 %b1) local_unnamed_addr norecurse nounwind readnone noinline {
entry:
  ret i64 0
}

define dso_local i64 @small_blkdrop(i64 %v0, i64 %v1, i64 %v2) local_unnamed_addr norecurse nounwind readnone noinline {
entry:
  ret i64 0
}

define dso_local i64 @main() local_unnamed_addr norecurse nounwind readnone {
entry:
; CHECK-NOT: error: compilation failed
  call i64 @deep_blkdrop(i64 0, i64 1, i64 2, i64 3, i64 4, i64 5, i64 6, i64 7, i64 8, i64 9, i64 10, i64 11, i64 12, i64 13, i64 14, i64 15, i64 16, i64 17)
  call i64 @small_blkdrop(i64 0, i64 1, i64 2)
  ret i64 0
}

