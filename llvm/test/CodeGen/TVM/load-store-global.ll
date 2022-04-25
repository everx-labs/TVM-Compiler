; RUN: llc < %s -march=tvm | FileCheck %s
target triple = "tvm"

@value1 = global i257 1
@value2 = global i257 1234

; CHECK-LABEL: make_store
define void @make_store() {
; CHECK: PUSHINT $value2$
; CHECK: ONE
; CHECK: GETGLOB 14 CALLX
  store i257 1, i257* @value2
  ret void
}

; CHECK-LABEL: make_load
define i257 @make_load() {
; CHECK: PUSHINT $value1$
; CHECK: GETGLOB 13 CALLX
  %1 = load i257, i257* @value1
  ret i257 %1
}

; CHECK-LABEL: make_load_store
define i257 @make_load_store() {
; CHECK: PUSHINT $value2$
; CHECK: GETGLOB 13 CALLX
  %1 = load i257, i257* @value2
  %2 = add i257 %1, 1
; CHECK: INC
; CHECK: PUSHINT $value1$
; CHECK: PUSH s1
; CHECK-NEXT: GETGLOB 14 CALLX
  store i257 %2, i257* @value1
  ret i257 %2
}
