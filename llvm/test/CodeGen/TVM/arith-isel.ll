; RUN: llc < %s -march=tvm -stop-after expand-isel-pseudos | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define i64 @addi64(i64 %par) nounwind {
; CHECK: %[[VR1:[0-9]+]]:i64 = ARGUMENT 0
; CHECK: %[[VR0:[0-9]+]]:i64 = CONST_I64 1
; CHECK: %[[VR2:[0-9]+]]:i64 = ADD killed %[[VR1]], killed %[[VR0]]
  %1 = add i64 %par, 1
; CHECK: RETURN_I64 killed %[[VR2]]
	ret i64 %1
}
