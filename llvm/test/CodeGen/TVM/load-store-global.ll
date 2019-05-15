; RUN: llc < %s -march=tvm | FileCheck %s
target triple = "tvm"

@value1 = global i64 1
@value2 = global i64 1234

; CHECK-LABEL: make_store
define void @make_store() {
; CHECK: PUSHINT $value2$
; CHECK: PUSHINT 1
; CHECK: CALL $:store$
  store i64 1, i64* @value2
  ret void
}

; CHECK-LABEL: make_load
define i64 @make_load() {
; CHECK: PUSHINT $value1$
; CHECK: CALL $:load$
  %1 = load i64, i64* @value1
  ret i64 %1
}

; CHECK-LABEL: make_load_store
define i64 @make_load_store() {
; CHECK: PUSHINT $value2$
; CHECK: CALL $:load$
  %1 = load i64, i64* @value2
  %2 = add i64 %1, 1
; CHECK: INC
; CHECK: PUSHINT $value1$
; CHECK: PUSH s1
; CHECK-NEXT: CALL $:store$
  store i64 %2, i64* @value1
  ret i64 %2
}