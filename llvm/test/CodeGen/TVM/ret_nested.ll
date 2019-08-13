; RUN: llc -O0 < %s -march=tvm | FileCheck %s
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
; CHECK: XCHG s0, s1
; CHECK: CALL 1
; CHECK: XCHG s0, s1
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
