; RUN: llc -march=tvm < %s | FileCheck %s
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define void @store_builder(builder* %pb, builder %b) {
entry:
; CHECK-LABEL: store_builder
; CHECK: CALL $:store$
  store builder %b, builder* %pb
  ret void
}

define builder @load_builder(builder* %pb) {
entry:
; CHECK-LABEL: load_builder
; CHECK: CALL $:load$
  %b = load builder, builder* %pb
  ret builder %b
}

define void @store_slice(slice* %pb, slice %b) {
entry:
; CHECK-LABEL: store_slice
; CHECK: CALL $:store$
  store slice %b, slice* %pb
  ret void
}

define slice @load_slice(slice* %pb) {
entry:
; CHECK-LABEL: load_slice
; CHECK: CALL $:load$
  %b = load slice, slice* %pb
  ret slice %b
}
