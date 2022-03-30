; RUN: llc -O3 < %s -march=tvm -filetype=asm | FileCheck %s
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: ret1
define i257 @ret1() {
; CHECK: PUSHPOW2 7
  ret i257 128
}

; CHECK-LABEL: ret2
define i257 @ret2() {
; CHECK: PUSHNEGPOW2 8
  ret i257 -256
}

; CHECK-LABEL: ret3
define i257 @ret3() {
; CHECK: PUSHPOW2DEC	8
  ret i257 255
}

; CHECK-LABEL: retneg
define i257 @retneg() {
  ret i257 0
}
