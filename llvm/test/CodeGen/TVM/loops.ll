; RUN: llc < %s -march=tvm -asm-verbose=0 | FileCheck %s
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: for_loop
; CHECK:      PUSHCONT {
; CHECK-NEXT:   PUSHINT 0
; CHECK-NEXT:   PUSHINT 0
; CHECK-NEXT:   PUSH  s2
; CHECK-NEXT:   SWAP
; CHECK-NEXT:   GREATER
; CHECK-NEXT:   ZERO
; CHECK-NEXT:   ROLLREV 2
; CHECK-NEXT:   PUSH2 s6, s4
; CHECK-NEXT:   IFELSE
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:   PUSHINT 0
; CHECK-NEXT:   PUSHINT 0
; CHECK-NEXT:   ROLL  2
; CHECK-NEXT:   DROP
; CHECK-NEXT:   ROLL  2
; CHECK-NEXT:   DROP
; CHECK-NEXT:   PUSH  s4
; CHECK-NEXT:   JMPX
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:   PUSH  s1
; CHECK-NEXT:   INC
; CHECK-NEXT:   PUSH2 s0, s3
; CHECK-NEXT:   LESS
; CHECK-NEXT:   XCHG2 s3, s2
; CHECK-NEXT:   ADD
; CHECK-NEXT:   XCHG  s0, s2
; CHECK-NEXT:   ROLLREV 2
; CHECK-NEXT:   PUSH2 s5, s4
; CHECK-NEXT:   IFELSE
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:   XCHG  s0, s2
; CHECK-NEXT:   BLKDROP 2
; CHECK-NEXT:   RET
; CHECK-NEXT: }
; CHECK-NEXT: BLKSWAP 1, 4
; CHECK-NEXT: PUSH s4
; CHECK-NEXT: EXECUTE
; CHECK-NEXT: BLKSWAP 4, 1
; CHECK-NEXT: BLKDROP 4
define i257 @for_loop(i257 %N) nounwind {
entry:
  %cmp6 = icmp sgt i257 %N, 0
  br i1 %cmp6, label %for.body, label %for.cond.cleanup

for.cond.cleanup:
  %sum.0.lcssa = phi i257 [ 0, %entry ], [ %add, %for.body ]
  ret i257 %sum.0.lcssa

for.body:
  %i.08 = phi i257 [ %inc, %for.body ], [ 0, %entry ]
  %sum.07 = phi i257 [ %add, %for.body ], [ 0, %entry ]
  %add = add nsw i257 %i.08, %sum.07
  %inc = add nuw nsw i257 %i.08, 1
  %cmp = icmp slt i257 %inc, %N
  br i1 %cmp, label %for.body, label %for.cond.cleanup
}

; CHECK-LABEL: while_loop
; CHECK:      PUSHCONT {
; CHECK-NEXT:   PUSHINT 0
; CHECK-NEXT:   PUSHINT 0
; CHECK-NEXT:   PUSH  s2
; CHECK-NEXT:   SWAP
; CHECK-NEXT:   GREATER
; CHECK-NEXT:   ZERO
; CHECK-NEXT:   ROLLREV 2
; CHECK-NEXT:   PUSH2 s6, s4
; CHECK-NEXT:   IFELSE
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:   PUSHINT 0
; CHECK-NEXT:   PUSHINT 0
; CHECK-NEXT:   ROLL  2
; CHECK-NEXT:   DROP
; CHECK-NEXT:   ROLL  2
; CHECK-NEXT:   DROP
; CHECK-NEXT:   PUSH  s4
; CHECK-NEXT:   JMPX
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:   PUSH  s1
; CHECK-NEXT:   INC
; CHECK-NEXT:   PUSH2 s0, s3
; CHECK-NEXT:   LESS
; CHECK-NEXT:   XCHG2 s3, s2
; CHECK-NEXT:   ADD
; CHECK-NEXT:   XCHG  s0, s2
; CHECK-NEXT:   ROLLREV 2
; CHECK-NEXT:   PUSH2 s5, s4
; CHECK-NEXT:   IFELSE
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:   XCHG  s0, s2
; CHECK-NEXT:   BLKDROP 2
; CHECK-NEXT:   RET
; CHECK-NEXT: }
; CHECK-NEXT: BLKSWAP 1, 4
; CHECK-NEXT: PUSH s4
; CHECK-NEXT: EXECUTE
; CHECK-NEXT: BLKSWAP 4, 1
; CHECK-NEXT: BLKDROP 4
define i257 @while_loop(i257 %N) nounwind {
entry:
  %cmp5 = icmp sgt i257 %N, 0
  br i1 %cmp5, label %while.body, label %while.end

while.body:
  %i.07 = phi i257 [ %inc, %while.body ], [ 0, %entry ]
  %sum.06 = phi i257 [ %add, %while.body ], [ 0, %entry ]
  %inc = add nuw nsw i257 %i.07, 1
  %add = add nsw i257 %i.07, %sum.06
  %cmp = icmp slt i257 %inc, %N
  br i1 %cmp, label %while.body, label %while.end

while.end:
  %sum.0.lcssa = phi i257 [ 0, %entry ], [ %add, %while.body ]
  ret i257 %sum.0.lcssa
}
