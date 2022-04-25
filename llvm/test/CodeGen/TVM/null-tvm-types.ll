; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: ret_tuple:
define tuple @ret_tuple() nounwind {
; CHECK: PUSHNULL
  %null = call i257 @llvm.tvm.pushnull()
  %null_tuple = bitcast i257 %null to tuple
  ret tuple %null_tuple
}

; CHECK-LABEL: ret_slice:
define slice @ret_slice() nounwind {
; CHECK: PUSHNULL
  %null = call i257 @llvm.tvm.pushnull()
  %null_slice = bitcast i257 %null to slice
  ret slice %null_slice
}

; CHECK-LABEL: ret_builder:
define builder @ret_builder() nounwind {
; CHECK: PUSHNULL
  %null = call i257 @llvm.tvm.pushnull()
  %null_builder = bitcast i257 %null to builder
  ret builder %null_builder
}

; CHECK-LABEL: ret_cell:
define cell @ret_cell() nounwind {
; CHECK: PUSHNULL
  %null = call i257 @llvm.tvm.pushnull()
  %null_cell = bitcast i257 %null to cell
  ret cell %null_cell
}

declare i257 @llvm.tvm.pushnull()

