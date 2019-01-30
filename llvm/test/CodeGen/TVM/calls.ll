; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define void @nop() nounwind {
  ret void
}

; CHECK-LABEL: test0
define void @test0() nounwind {
; CHECK: PUSHCONT nop
; CHECK-NEXT: CALLX
  call void @nop()
  ret void
}

; CHECK-LABEL: test00
define void @test00() nounwind {
; CHECK: PUSHCONT nop
; CHECK-NEXT: CALLX
; CHECK-NEXT: PUSHCONT nop
; CHECK-NEXT: CALLX
  call void @nop()
  call void @nop()
  ret void
}
