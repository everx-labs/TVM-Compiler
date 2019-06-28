; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s

target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i64 @ult_test(i64 %a, i64 %b) local_unnamed_addr norecurse nounwind readnone {
; CHECK-LABEL: ult_test:
; CHECK:	PUSHINT	18446744073709551615
; CHECK:	PUSH	s1
; CHECK:	SWAP
; CHECK:	AND
; CHECK:	PUSHINT	18446744073709551615
; CHECK:	PUSH	s3
; CHECK:	SWAP
; CHECK:	AND
; CHECK:	SWAP
; CHECK:	LESS
; CHECK:	CONDSEL
entry:
  %cmp = icmp ult i64 %a, %b
  %cond = select i1 %cmp, i64 %a, i64 %b
  ret i64 %cond
}

; Function Attrs: norecurse nounwind readnone
define dso_local i64 @ule_test(i64 %a, i64 %b) local_unnamed_addr norecurse nounwind readnone {
; CHECK-LABEL: ule_test:
; CHECK:	PUSHINT	18446744073709551615
; CHECK:	PUSH	s1
; CHECK:	SWAP
; CHECK:	AND
; CHECK:	PUSHINT	18446744073709551615
; CHECK:	PUSH	s3
; CHECK:	SWAP
; CHECK:	AND
; CHECK:	SWAP
; CHECK:	LEQ
; CHECK:	CONDSEL
entry:
  %cmp = icmp ule i64 %a, %b
  %cond = select i1 %cmp, i64 %a, i64 %b
  ret i64 %cond
}

; Function Attrs: norecurse nounwind readnone
define dso_local i64 @ugt_test(i64 %a, i64 %b) local_unnamed_addr norecurse nounwind readnone {
; CHECK-LABEL: ugt_test:
; CHECK:	PUSHINT	18446744073709551615
; CHECK:	PUSH	s1
; CHECK:	SWAP
; CHECK:	AND
; CHECK:	PUSHINT	18446744073709551615
; CHECK:	PUSH	s3
; CHECK:	SWAP
; CHECK:	AND
; CHECK:	SWAP
; CHECK:	GREATER
; CHECK:	CONDSEL
entry:
  %cmp = icmp ugt i64 %a, %b
  %cond = select i1 %cmp, i64 %a, i64 %b
  ret i64 %cond
}

; Function Attrs: norecurse nounwind readnone
define dso_local i64 @uge_test(i64 %a, i64 %b) local_unnamed_addr norecurse nounwind readnone {
; CHECK-LABEL: uge_test:
; CHECK:	PUSHINT	18446744073709551615
; CHECK:	PUSH	s1
; CHECK:	SWAP
; CHECK:	AND
; CHECK:	PUSHINT	18446744073709551615
; CHECK:	PUSH	s3
; CHECK:	SWAP
; CHECK:	AND
; CHECK:	SWAP
; CHECK:	GEQ
; CHECK:	CONDSEL
entry:
  %cmp = icmp uge i64 %a, %b
  %cond = select i1 %cmp, i64 %a, i64 %b
  ret i64 %cond
}

