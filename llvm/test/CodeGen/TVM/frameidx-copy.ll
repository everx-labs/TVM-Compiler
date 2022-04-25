; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s

target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: noinline norecurse nounwind
define dso_local void @f(i257* nocapture %val) local_unnamed_addr noinline norecurse nounwind {
; CHECK-LABEL: f:
; CHECK:	GETGLOB 13 CALLX
; CHECK:	INC
; CHECK:	GETGLOB 14 CALLX

entry:
  %0 = load i257, i257* %val, align 1
  %inc = add nsw i257 %0, 1
  store i257 %inc, i257* %val, align 1
  ret void
}

; Function Attrs: nounwind
define dso_local i257 @check_main() local_unnamed_addr nounwind {
; CHECK-LABEL: check_main:
; CHECK:	GETGLOB 5
; CHECK-NEXT: ADDCONST -1
; CHECK-NEXT: SETGLOB 5
; CHECK:	GETGLOB 5
; CHECK:	ZERO
; CHECK:	GETGLOB 14 CALLX
; CHECK:	GETGLOB 5
; CHECK:	CALL	$f$
; CHECK:	GETGLOB 5
; CHECK:	GETGLOB 13 CALLX
; CHECK:	GETGLOB 5
; CHECK-NEXT: ADDCONST 1
; CHECK-NEXT: SETGLOB 5

entry:
  %var = alloca i257, align 1
  store i257 0, i257* %var, align 1
  call void @f(i257* nonnull %var)
  %rv = load i257, i257* %var, align 1
  %not_eq = sub i257 %rv, 1
  %bad_flag = trunc i257 %not_eq to i1
  br i1 %bad_flag, label %do_throw, label %ok
ok:
  ret i257 %rv
do_throw:
  call void @llvm.tvm.throw(i257 13)
  unreachable
}

declare void @llvm.tvm.throw(i257 %exception) noreturn

