; RUN: llc -O3 < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define i64 @g(i64 %N) nounwind {
  ret i64 %N
}

; CHECK-LABEL: f:
define i64 @f(i64 %N) nounwind {
; CHECK: PUSH c0
; CHECK: PUSHCONT
; CHECK: PUSHINT $g$
; CHECK: PUSHINT 1
; CHECK: SWAP
; CHECK: CALL 1
; CHECK: SWAP
; CHECK: POP	c0
; CHECK: }
; CHECK: PUSHCONT
; CHECK: IFELSE

  %status = icmp sgt i64 %N, 0
  br i1 %status, label %then, label %else

else:
  %1 = call i64 @g(i64 1)
  ret i64 %1

then:
  %2 = call i64 @g(i64 2)
  ret i64 %2
}

; the part below has been added only for runtime testing
define void @test() nounwind {
  %1 = call i64 @f(i64 0)
  %2 = sub i64 %1, 2
  call void @llvm.tvm.throwif(i64 %2, i64 13)
  ret void
}

declare void @llvm.tvm.throwif(i64 %cond, i64 %exception)

declare i64 @undefined_f(i64)

; CHECK-LABEL: l:
define i64 @l(i64 %x) {
; CHECK: PUSH c0
; CHECK: PUSHCONT
; CHECK: {
; CHECK:   PUSHCONT
; CHECK:   {
; CHECK:     POP c0
; CHECK:   }
; CHECK:   PUSHCONT
; CHECK:   {
; CHECK:     POP c0
; CHECK:   }
; CHECK:   IFELSE
; CHECK: }
; CHECK: PUSHCONT
; CHECK: {
; CHECK:   POP c0
; CHECK: }
; CHECK: IFELSE
entry:
  %0 = icmp eq i64 %x, 0
  br i1 %0, label %if.else, label %if.then

if.then:
  %1 = tail call i64 @undefined_f(i64 %x)
  %2 = icmp ne i64 %1, 0
  br i1 %2, label %if.then2, label %if.else2

if.then2:
  %3 = tail call i64 @undefined_f(i64 %x)
  ret i64 %3

if.else2:
  ret i64 4

if.else:
  ret i64 0
}

; CHECK-LABEL: k:
define i64 @k(i64 %x) {
entry:
; CHECK-NOT: PUSH c0
; CHECK: ADDCONST 3
; CHECK-NOT: POP c0
  %add = add nsw i64 %x, 3
  ret i64 %add
}
