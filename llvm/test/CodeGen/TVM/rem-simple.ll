; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s 

target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @rem_func(i257 %a) norecurse nounwind readnone {
; CHECK-LABEL: rem_func:
; CHECK:	PUSHINT	26385
; CHECK:	MOD
entry:
  %rem = srem i257 %a, 26385
  ret i257 %rem
}

