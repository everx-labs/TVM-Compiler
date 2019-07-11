; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; CHECK-LABEL: brcond
define i64 @brcond(i1 %par) nounwind {
entry:
; CHECK: PUSHCONT
; CHECK: {
; CHECK:   PUSHINT 77
; CHECK: }
; CHECK: PUSHCONT
; CHECK: {
; CHECK:   PUSHINT 42
; CHECK: }
; CHECK: IFELSE
  br i1 %par, label %exit1, label %exit2
exit1:
  ret i64 42
exit2:
  ret i64 77
}

declare void @foo()
declare void @bar()
declare void @bazz()

; CHECK-LABEL: diamond
define void @diamond(i1 %par) nounwind {
entry:
; CHECK: PUSHCONT
; CHECK: {
; CHECK:   PUSHINT $bar$
; CHECK:   CALL
; CHECK: }
; CHECK: PUSHCONT
; CHECK: {
; CHECK:   PUSHINT $foo$
; CHECK:   CALL
; CHECK: }
; CHECK: IFELSE
  br i1 %par, label %bb1, label %bb2
bb1:
  call void @foo()
  br label %exit
bb2:
  call void @bar()
  br label %exit
exit:
  ret void
}

; CHECK-LABEL: diamondswitch
define void @diamondswitch(i64 %par) nounwind {
entry:
  switch i64 %par, label %bb3 [ i64 0, label %bb1
                                i64 1, label %bb2 ]
bb1:
; CHECK-DAG: PUSHINT $foo$
  call void @foo()
  br label %exit
bb2:
; CHECK-DAG: PUSHINT $bar$
  call void @bar()
  br label %exit
bb3:
; CHECK-DAG: PUSHINT $bazz$
  call void @bazz()
  br label %exit
exit:
  ret void
}

; CHECK-LABEL: trivial_phi
define i64 @trivial_phi(i1 %par, i64 %val) nounwind {
entry:
; CHECK: PUSHCONT
; CHECK: DEC
; CHECK: PUSHCONT
; CHECK: INC
; CHECK: IFELSE
  br i1 %par, label %bb1, label %bb2
bb1:

  %0 = add i64 %val, 1
  br label %exit
bb2:
  %1 = sub i64 %val, 1
  br label %exit
exit:
  %2 = phi i64 [%0, %bb1], [%1, %bb2]
  ret i64 %2
}

; CHECK-LABEL: cfg1
define i64 @cfg0(i1 %cond, i1 %cond2, i64 %par) nounwind {
entry:
  br i1 %cond, label %bb1, label %bb4
bb1:
  %0 = add i64 %par, 1
  br i1 %cond2, label %bb2, label %bb3
bb2:
  %1 = add i64 %0, 2
  ret i64 %1
bb3:
  %2 = add i64 %0, 3
  ret i64 %2
bb4:
  %3 = add i64 %par, 4
  ret i64 %3
}

; CHECK-LABEL: cfg1
define i64 @cfg1(i1 %cond1, i1 %cond2, i64 %ret1) nounwind {
entry:
  br i1 %cond1, label %bb1, label %bb2
bb1:
  br label %exit1
bb2:
  br i1 %cond2, label %exit1, label %exit2
exit1:
  ret i64 0
exit2:
  ret i64 %ret1
}
