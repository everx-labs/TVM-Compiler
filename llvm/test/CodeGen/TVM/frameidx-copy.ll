; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s

target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: noinline norecurse nounwind
define dso_local void @f(i257* nocapture %val) local_unnamed_addr noinline norecurse nounwind {
; CHECK-LABEL: f:
; CHECK:	CALL	$:load$
; CHECK:	INC
; CHECK:	CALL	$:store$

entry:
  %0 = load i257, i257* %val, align 1
  %inc = add nsw i257 %0, 1
  store i257 %inc, i257* %val, align 1
  ret void
}

; Function Attrs: nounwind
define dso_local i257 @check_main() local_unnamed_addr nounwind {
; CHECK-LABEL: check_main:
; CHECK:	PUSHINT	1
; CHECK:	CALL	$:enter$
; CHECK:	PUSHINT	0
; CHECK:	CALL	$:frameidx$
; CHECK:	PUSHINT	0
; CHECK:	CALL	$:store$
; CHECK:	PUSHINT	0
; CHECK:	CALL	$:frameidx$
; CHECK:	CALL	$f$
; CHECK:	PUSHINT	0
; CHECK:	CALL	$:frameidx$
; CHECK:	CALL	$:load$
; CHECK:	PUSHINT	1
; CHECK:	CALL	$:leave$

entry:
  %var = alloca i257, align 1
  store i257 0, i257* %var, align 1
  call void @f(i257* nonnull %var)
  %rv = load i257, i257* %var, align 1
  %not_eq = sub i257 %rv, 1
  call void @llvm.tvm.throwif(i257 %not_eq, i257 13)
  ret i257 %rv
}

declare void @llvm.tvm.throwif(i257 %cond, i257 %exception)

