; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: pushslice_x8
define slice @pushslice_x8() nounwind {
; CHECK: PUSHSLICE x8_
  %1 = call builder @llvm.tvm.newc()
  %2 = call cell @llvm.tvm.endc(builder %1)
  %3 = call slice @llvm.tvm.ctos(cell %2)
  ret slice %3
}

; CHECK-LABEL: pushslice_x4u
define slice @pushslice_x4u() nounwind {
; CHECK: PUSHSLICE x4_
  %1 = call builder @llvm.tvm.newc()
  %2 = call builder @llvm.tvm.stu(i257 0, builder %1, i257 1)
  %3 = call cell @llvm.tvm.endc(builder %2)
  %4 = call slice @llvm.tvm.ctos(cell %3)
  ret slice %4
}

; CHECK-LABEL: pushslice_x4s
define slice @pushslice_x4s() nounwind {
; CHECK: PUSHSLICE x4_
  %1 = call builder @llvm.tvm.newc()
  %2 = call builder @llvm.tvm.sti(i257 0, builder %1, i257 1)
  %3 = call cell @llvm.tvm.endc(builder %2)
  %4 = call slice @llvm.tvm.ctos(cell %3)
  ret slice %4
}

; CHECK-LABEL: pushslice_xcu
define slice @pushslice_xcu() nounwind {
; CHECK: PUSHSLICE xc_
  %1 = call builder @llvm.tvm.newc()
  %2 = call builder @llvm.tvm.stu(i257 1, builder %1, i257 1)
  %3 = call cell @llvm.tvm.endc(builder %2)
  %4 = call slice @llvm.tvm.ctos(cell %3)
  ret slice %4
}

; CHECK-LABEL: pushslice_xcs
define slice @pushslice_xcs() nounwind {
; CHECK: PUSHSLICE xc_
  %1 = call builder @llvm.tvm.newc()
  %2 = call builder @llvm.tvm.sti(i257 1, builder %1, i257 1)
  %3 = call cell @llvm.tvm.endc(builder %2)
  %4 = call slice @llvm.tvm.ctos(cell %3)
  ret slice %4
}

declare builder @llvm.tvm.newc() nounwind
declare cell @llvm.tvm.endc(builder %b) nounwind
declare slice @llvm.tvm.ctos(cell %cell)
declare builder @llvm.tvm.sti(i257 %value, builder %builder, i257 %size)
declare builder @llvm.tvm.stu(i257 %value, builder %builder, i257 %size)
