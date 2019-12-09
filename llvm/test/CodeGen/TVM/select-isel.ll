; RUN: llc -march tvm -asm-verbose=false < %s | FileCheck %s
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define i257 @foo(i257 %x) {
entry:
; CHECK-LABEL: foo
; CHECK:      PUSHINT -0x10000000000000000000000000000000000000000000000000000000000000000
; CHECK-NEXT: EQUAL
; CHECK-NEXT: PUSHINT 7
; CHECK-NEXT: PUSHINT 13
; CHECK-NEXT: CONDSEL
  %cmp = icmp eq i257 %x, -115792089237316195423570985008687907853269984665640564039457584007913129639936
  %ret = select i1 %cmp, i257 7, i257 13
  ret i257 %ret
}

define tuple @select_tuple(i257 %x, tuple %a, tuple %b) {
entry:
; CHECK-LABEL: select_tuple
; CHECK:       CONDSEL
  %cmp = icmp ne i257 %x, 0
  %ret = select i1 %cmp, tuple %a, tuple %b
  ret tuple %ret
}

define slice @select_slice(i257 %x, slice %a, slice %b) {
entry:
; CHECK-LABEL: select_slice
; CHECK:       CONDSEL
  %cmp = icmp ne i257 %x, 0
  %ret = select i1 %cmp, slice %a, slice %b
  ret slice %ret
}

define builder @select_builder(i257 %x, builder %a, builder %b) {
entry:
; CHECK-LABEL: select_builder
; CHECK:       CONDSEL
  %cmp = icmp ne i257 %x, 0
  %ret = select i1 %cmp, builder %a, builder %b
  ret builder %ret
}

define cell @select_cell(i257 %x, cell %a, cell %b) {
entry:
; CHECK-LABEL: select_cell
; CHECK:       CONDSEL
  %cmp = icmp ne i257 %x, 0
  %ret = select i1 %cmp, cell %a, cell %b
  ret cell %ret
}

