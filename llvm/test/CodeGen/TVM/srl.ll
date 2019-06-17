; RUN: llc < %s -march=tvm | FileCheck %s

target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i64 @test_srl(i64 %val) local_unnamed_addr nounwind {
; CHECK-LABEL: test_srl:
entry:
; CHECK: PUSHINT 18446744073709551615
; CHECK: AND
; CHECK: RSHIFT	16
  %shr = lshr i64 %val, 16
  ret i64 %shr
}

