; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
; XFAIL:*
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: blkdrop
define i257 @blkdrop(i257 %x1, i257 %x2, i257 %x3, i257 %x4, i257 %x5, i257 %x6, i257 %x7, i257 %x8, i257 %x9, i257 %x10, i257 %x11, i257 %x12, i257 %x13, i257 %x14, i257 %x15, i257 %x16, i257 %x17) nounwind {
  ; CHECK-NOT: DROP
  ; CHECK: BLKDROP
  ret i257 %x17
}

; CHECK-LABEL: blkpush
define i257 @blkpush() {
  ; CHECK: PUSHINT 0
  ; CHECK: BLKPUSH 15, 0
  ; CHECK: PUSHINT 0
  %res = call i257 @blkdrop(i257 0, i257 0, i257 0, i257 0, i257 0, i257 0, i257 0, i257 0, i257 0, i257 0, i257 0, i257 0, i257 0, i257 0, i257 0, i257 0, i257 0)
  ret i257 %res
}

; CHECK-LABEL: blkpush2
define i257 @blkpush2(i257 %x) {
  ; CHECK: BLKPUSH 15, 0
  ; CHECK: DUP
  %res = call i257 @blkdrop(i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x)
  ret i257 %res
}

; CHECK-LABEL: blkpush3
define i257 @blkpush3(i257 %x, i257 %y) {
  ; CHECK: SWAP
  ; CHECK: BLKPUSH 15, 0
  ; CHECK: DUP
  ; CHECK: CALL
  ; CHECK: ADD
  %res = call i257 @blkdrop(i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x)
  %res.1 = add i257 %res, %y
  ret i257 %res.1
}

; CHECK-LABEL: blkpush-neg
define i257 @blkpush-neg(i257 %x, i257 %x1, i257 %x2, i257 %x3, i257 %x4, i257 %x5, i257 %x6, i257 %x7, i257 %x8, i257 %x9, i257 %x10, i257 %x11, i257 %x12, i257 %x13, i257 %x14, i257 %x15, i257 %x16) {
  ; CHECK: ROLL	16
  ; CHECK: BLKPUSH 15, 0
  ; CHECK: DUP
  %res = call i257 @blkdrop(i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x, i257 %x)
  %res.1 = call i257 @blkdrop(i257 %x1, i257 %x2, i257 %x3, i257 %x4, i257 %x5, i257 %x6, i257 %x7, i257 %x8, i257 %x9, i257 %x10, i257 %x11, i257 %x12, i257 %x13, i257 %x14, i257 %x15, i257 %x16, i257 %x16)
  %res.2 = add i257 %res, %res.1
  ret i257 %res.2
}
