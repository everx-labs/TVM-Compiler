; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s

target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @ult_test(i257 %a, i257 %b) local_unnamed_addr norecurse nounwind readnone {
; CHECK-LABEL: ult_test:
; CHECK:	DUP2
; CHECK:	LESS
; CHECK:	CONDSEL
entry:
  %cmp = icmp ult i257 %a, %b
  %cond = select i1 %cmp, i257 %a, i257 %b
  ret i257 %cond
}

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @ule_test(i257 %a, i257 %b) local_unnamed_addr norecurse nounwind readnone {
; CHECK-LABEL: ule_test:
; CHECK:	DUP2
; CHECK:	LEQ
; CHECK:	CONDSEL
entry:
  %cmp = icmp ule i257 %a, %b
  %cond = select i1 %cmp, i257 %a, i257 %b
  ret i257 %cond
}

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @ugt_test(i257 %a, i257 %b) local_unnamed_addr norecurse nounwind readnone {
; CHECK-LABEL: ugt_test:
; CHECK:	DUP2
; CHECK:	GREATER
; CHECK:	CONDSEL
entry:
  %cmp = icmp ugt i257 %a, %b
  %cond = select i1 %cmp, i257 %a, i257 %b
  ret i257 %cond
}

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @uge_test(i257 %a, i257 %b) local_unnamed_addr norecurse nounwind readnone {
; CHECK-LABEL: uge_test:
; CHECK:	DUP2
; CHECK:	GEQ
; CHECK:	CONDSEL
entry:
  %cmp = icmp uge i257 %a, %b
  %cond = select i1 %cmp, i257 %a, i257 %b
  ret i257 %cond
}

