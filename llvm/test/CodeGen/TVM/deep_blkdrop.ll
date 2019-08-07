; RUN: llc < %s -march=tvm | FileCheck %s 

target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define dso_local i64 @deep_blkdrop(i64 %v0, i64 %v1, i64 %v2, i64 %v3, i64 %v4, i64 %v5, i64 %v6, i64 %v7, i64 %a0, i64 %a1, i64 %a2, i64 %a3, i64 %a4, i64 %a5, i64 %a6, i64 %a7, i64 %b0, i64 %b1) local_unnamed_addr norecurse nounwind readnone {
; CHECK-LABEL: deep_blkdrop:
; CHECK:       PUSHINT	18
; CHECK-NEXT:  DROPX
entry:
  ret i64 0
}

define dso_local i64 @small_blkdrop(i64 %v0, i64 %v1, i64 %v2) local_unnamed_addr norecurse nounwind readnone {
entry:
; CHECK-LABEL: small_blkdrop:
; CHECK:       BLKDROP	3
  ret i64 0
}

