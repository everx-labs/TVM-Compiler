; RUN: llc < %s -march=tvm | FileCheck %s 

target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define dso_local i257 @deep_blkdrop(i257 %v0, i257 %v1, i257 %v2, i257 %v3, i257 %v4, i257 %v5, i257 %v6, i257 %v7, i257 %a0, i257 %a1, i257 %a2, i257 %a3, i257 %a4, i257 %a5, i257 %a6, i257 %a7, i257 %b0, i257 %b1) local_unnamed_addr norecurse nounwind readnone {
; CHECK-LABEL: deep_blkdrop:
; CHECK:       PUSHINT	18
; CHECK-NEXT:  DROPX
entry:
  ret i257 0
}

define dso_local i257 @small_blkdrop(i257 %v0, i257 %v1, i257 %v2) local_unnamed_addr norecurse nounwind readnone {
entry:
; CHECK-LABEL: small_blkdrop:
; CHECK:       BLKDROP	3
  ret i257 0
}

