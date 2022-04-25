; RUN: llc -march=tvm < %s | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define void @store_int(i257* %pb, i257 %b) {
entry:
; CHECK-LABEL: store_int
; CHECK: GETGLOB 14 CALLX
  store i257 %b, i257* %pb
  ret void
}

define i257 @load_int(i257* %pb) {
entry:
; CHECK-LABEL: load_int
; CHECK: GETGLOB 13 CALLX
  %b = load i257, i257* %pb
  ret i257 %b
}

define void @store_builder(builder* %pb, builder %b) {
entry:
; CHECK-LABEL: store_builder
; CHECK: CALL $:store_builder$
  store builder %b, builder* %pb
  ret void
}

define builder @load_builder(builder* %pb) {
entry:
; CHECK-LABEL: load_builder
; CHECK: CALL $:load_builder$
  %b = load builder, builder* %pb
  ret builder %b
}

define void @store_slice(slice* %pb, slice %b) {
entry:
; CHECK-LABEL: store_slice
; CHECK: CALL $:store_slice$
  store slice %b, slice* %pb
  ret void
}

define slice @load_slice(slice* %pb) {
entry:
; CHECK-LABEL: load_slice
; CHECK: CALL $:load_slice$
  %b = load slice, slice* %pb
  ret slice %b
}

define void @store_cell(cell* %pb, cell %b) {
entry:
; CHECK-LABEL: store_cell
; CHECK: CALL $:store_cell$
  store cell %b, cell* %pb
  ret void
}

define cell @load_cell(cell* %pb) {
entry:
; CHECK-LABEL: load_cell
; CHECK: CALL $:load_cell$
  %b = load cell, cell* %pb
  ret cell %b
}
