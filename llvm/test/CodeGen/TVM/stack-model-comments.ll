; RUN: llc < %s -march=tvm -filetype=asm | FileCheck %s
; XFAIL:*
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: brcond
define i64 @brcond(i1 %par) nounwind {
entry:
; CHECK:      PUSHCONT {
; CHECK:        PUSHINT	77
; CHECK:      } ; %[[VRBB2:[0-9]+]] = PUSHCONT_MBB %bb.2
; CHECK:      ; { %[[VRBB2]] | %[[VR0:[0-9]+]] }
; CHECK:      PUSHCONT {
; CHECK:        PUSHINT	42
; CHECK:        RET
; CHECK:      } ; %[[VRBB1:[0-9]+]] = PUSHCONT_MBB %bb.1
; CHECK:      ; { %[[VRBB1]] | {{.*}} | {{.*}} }
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

