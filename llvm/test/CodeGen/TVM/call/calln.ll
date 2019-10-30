; RUN: llc -march=tvm -asm-verbose=false < %s | FileCheck %s
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

%st = type { i257, i257, i257 }
declare %st @bar()

define void @foo() {
; CHECK-LABEL: foo
; CHECK: CALL $bar$
; CHECK-NEXT: BLKDROP 3
  %call = call %st @bar()
  ret void
}
