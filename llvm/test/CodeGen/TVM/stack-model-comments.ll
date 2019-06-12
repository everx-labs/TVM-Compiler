; RUN: llc < %s -march=tvm -filetype=asm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; CHECK-LABEL: brcond
define i64 @brcond(i1 %par) nounwind {
entry:
; CHECK:      PUSHCONT {
; CHECK-NEXT:   PUSHINT	77
; CHECK-NEXT: } ; { bb.2.exit2 | %[[VR0:[0-9]+]] }
; CHECK:      PUSHCONT {
; CHECK-NEXT:   PUSHINT	42
; CHECK-NEXT:   RET
; CHECK-NEXT: } ; { bb.1.exit1 | bb.2.exit2 | %0 }
; CHECK:      IFELSE            ; {  }
  br i1 %par, label %exit1, label %exit2
exit1:
; CHECK: PUSHINT 42       ; %[[VR1:[0-9]+]] = CONST_I64 42
; CHECK-NEXT:             ; { %[[VR1]] }
; CHECK-NEXT: RET
  ret i64 42
exit2:
; CHECK: PUSHINT 77       ; %[[VR2:[0-9]+]] = CONST_I64 77
; CHECK-NEXT:             ; { %[[VR2]] }
  ret i64 77
}

