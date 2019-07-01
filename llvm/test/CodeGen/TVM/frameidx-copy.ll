; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
; XFAIL:*

target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: noinline norecurse nounwind
define dso_local void @f(i64* nocapture %val) local_unnamed_addr noinline norecurse nounwind {
; CHECK-LABEL: f:
; CHECK:	PUSH	s1
; CHECK:	CALL	$:load$
; CHECK:	INC
; CHECK:	CALL	$:store$

entry:
  %0 = load i64, i64* %val, align 8
  %inc = add nsw i64 %0, 1
  store i64 %inc, i64* %val, align 8
  ret void
}

; Function Attrs: nounwind
define dso_local i64 @check_main() local_unnamed_addr nounwind {
; CHECK-LABEL: check_main:
; CHECK:	PUSHINT	8
; CHECK:	CALL	$:enter$
; CHECK:	PUSHINT	0
; CHECK:	CALL	$:frameidx$
; CHECK:	PUSHINT	0
; CHECK:	CALL	$:store$
; CHECK:	PUSHINT	0
; CHECK:	CALL	$:frameidx$
; CHECK:	PUSHINT	$f$
; CHECK:	CALL	1
; CHECK:	PUSHINT	0
; CHECK:	CALL	$:frameidx$
; CHECK:	CALL	$:load$
; CHECK:	PUSHINT	8
; CHECK:	CALL	$:leave$

entry:
  %var = alloca i64, align 8
  store i64 0, i64* %var, align 8
  call void @f(i64* nonnull %var)
  %0 = load i64, i64* %var, align 8
  ret i64 %0
}

