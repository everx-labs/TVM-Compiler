; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; CHECK-LABEL: brcond
define i64 @brcond(i1 %par) nounwind {
entry:
; CHECK: PUSHCONT .LBB{{[0-9]+}}_2
; CHECK-NEXT: IFJMP
; CHECK-NEXT: PUSHCONT .LBB{{[0-9]+}}_1
; CHECK-NEXT: JMPX
  br i1 %par, label %exit1, label %exit2
exit1:
; CHECK: PUSHINT 42
; CHECK-NEXT: RETARGS 1
  ret i64 42
exit2:
; CHECK: PUSHINT 77
; CHECK-NOT: RETARGS
  ret i64 77
}

declare void @foo()
declare void @bar()
declare void @bazz()

; CHECK-LABEL: diamond
define void @diamond(i1 %par) nounwind {
entry:
; CHECK: PUSHCONT .LBB{{[0-9]+}}_2
; CHECK-NEXT: IFJMP
; CHECK-NEXT: PUSHCONT .LBB{{[0-9]+}}_1
; CHECK-NEXT: JMPX
  br i1 %par, label %bb1, label %bb2
bb1:
  call void @foo()
; CHECK: PUSHCONT .LBB1_3
; CHECK-NEXT: JMPX
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
; CHECK-DAG: PUSHCONT foo
  call void @foo()
  br label %exit
bb2:
; CHECK-DAG: PUSHCONT bar
  call void @bar()
  br label %exit
bb3:
; CHECK-DAG: PUSHCONT bazz
  call void @bazz()
  br label %exit
exit:
  ret void
}

; CHECK-LABEL: trivial_phi
define i64 @trivial_phi(i1 %par, i64 %val) nounwind {
entry:
; CHECK: PUSHCONT
; CHECK-NEXT: IFJMP
; CHECK-NEXT: PUSHCONT
; CHECK-NEXT: JMPX
  br i1 %par, label %bb1, label %bb2
bb1:
; CHECK: INC
  %0 = add i64 %val, 1
; CHECK: PUSHCONT
; CHECK-NEXT: JMPX
  br label %exit
bb2:
; CHECK: DEC
  %1 = sub i64 %val, 1
  br label %exit
exit:
  %2 = phi i64 [%0, %bb1], [%1, %bb2]
  ret i64 %2
}
