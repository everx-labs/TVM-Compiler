; RUN: llc < %s -march=tvm | FileCheck %s

target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @test_srl(i257 %val) local_unnamed_addr nounwind {
; CHECK-LABEL: test_srl:
entry:
; CHECK: RSHIFT	16
  %shr = lshr i257 %val, 16
  ret i257 %shr
}

