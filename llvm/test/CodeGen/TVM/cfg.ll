; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: brcond
define i257 @brcond(i1 %par) nounwind {
entry:
; CHECK:      PUSHCONT {
; CHECK-NEXT:   PUSH2 s2, s1
; CHECK-NEXT:   IFELSE
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:   PUSHINT 42
; CHECK-NEXT:   RET
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:   PUSHINT 77
; CHECK-NEXT:   RET
; CHECK-NEXT: }
; CHECK-NEXT: BLKSWAP 1, 3
; CHECK-NEXT: PUSH s3
; CHECK-NEXT: EXECUTE
; CHECK-NEXT: BLKSWAP 3, 1
; CHECK-NEXT: BLKDROP 3
  br i1 %par, label %exit1, label %exit2
exit1:
  ret i257 42
exit2:
  ret i257 77
}

declare void @foo()
declare void @bar()
declare void @bazz()

; CHECK-LABEL: diamond
define void @diamond(i1 %par) nounwind {
entry:
; CHECK:      PUSHCONT {
; CHECK-NEXT:   PUSH2 s3, s2
; CHECK-NEXT:   IFELSE
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:   CALL  $foo$
; CHECK-NEXT:   PUSH  s0
; CHECK-NEXT:   JMPX
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:   CALL  $bar$
; CHECK-NEXT:   PUSH  s0
; CHECK-NEXT:   JMPX
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:   RET
; CHECK-NEXT: }
; CHECK-NEXT: BLKSWAP 1, 4
; CHECK-NEXT: PUSH s4
; CHECK-NEXT: EXECUTE
; CHECK-NEXT: BLKDROP 4
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
define void @diamondswitch(i257 %par) nounwind {
entry:
  switch i257 %par, label %bb3 [ i257 0, label %bb1
                                i257 1, label %bb2 ]
bb1:
; CHECK-DAG: CALL $foo$
  call void @foo()
  br label %exit
bb2:
; CHECK-DAG: CALL $bar$
  call void @bar()
  br label %exit
bb3:
; CHECK-DAG: CALL $bazz$
  call void @bazz()
  br label %exit
exit:
  ret void
}

; CHECK-LABEL: trivial_phi
define i257 @trivial_phi(i1 %par, i257 %val) nounwind {
entry:
; CHECK:      PUSHCONT {
; CHECK-NEXT:   SWAP
; CHECK-NEXT:   PUSH2 s4, s3
; CHECK-NEXT:   IFELSE
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:   INC
; CHECK-NEXT:   PUSH  s1
; CHECK-NEXT:   JMPX
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:   DEC
; CHECK-NEXT:   PUSH  s1
; CHECK-NEXT:   JMPX
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:   RET
; CHECK-NEXT: }
; CHECK-NEXT: BLKSWAP 2, 4
; CHECK-NEXT: PUSH s5
; CHECK-NEXT: EXECUTE
; CHECK-NEXT: BLKSWAP 4, 1
; CHECK-NEXT: BLKDROP 4
  br i1 %par, label %bb1, label %bb2
bb1:

  %0 = add i257 %val, 1
  br label %exit
bb2:
  %1 = sub i257 %val, 1
  br label %exit
exit:
  %2 = phi i257 [%0, %bb1], [%1, %bb2]
  ret i257 %2
}

; CHECK-LABEL: cfg1
define i257 @cfg0(i1 %cond, i1 %cond2, i257 %par) nounwind {
entry:
  br i1 %cond, label %bb1, label %bb4
bb1:
  %0 = add i257 %par, 1
  br i1 %cond2, label %bb2, label %bb3
bb2:
  %1 = add i257 %0, 2
  ret i257 %1
bb3:
  %2 = add i257 %0, 3
  ret i257 %2
bb4:
  %3 = add i257 %par, 4
  ret i257 %3
}

; CHECK-LABEL: cfg1
define i257 @cfg1(i1 %cond1, i1 %cond2, i257 %ret1) nounwind {
entry:
  br i1 %cond1, label %bb1, label %bb2
bb1:
  br label %exit1
bb2:
  br i1 %cond2, label %exit1, label %exit2
exit1:
  ret i257 0
exit2:
  ret i257 %ret1
}
