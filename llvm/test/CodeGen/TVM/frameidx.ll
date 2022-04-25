; RUN: llc -march=tvm -asm-verbose=false < %s | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: frameidx
define i257 @frameidx(i257 %slot) {
entry:
; CHECK: PUSH c7
; CHECK-NEXT: INDEXQ 5
; CHECK-NEXT: ADD
  %c7 = call i257 @llvm.tvm.getreg(i257 7)
  %tuple = call tuple @llvm.tvm.cast.to.tuple(i257 %c7)
  %fp = call i257 @llvm.tvm.indexq(tuple %tuple, i257 5)
  %idx = add nsw i257 %fp, %slot
  ret i257 %idx
}

declare i257 @llvm.tvm.getreg(i257)
declare tuple @llvm.tvm.cast.to.tuple(i257)
declare i257 @llvm.tvm.indexq(tuple, i257)
