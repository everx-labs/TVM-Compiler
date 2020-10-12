; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: sched
define i257 @sched(i257 %par1, i257 %par2, i257 %par3, i257 %par4) nounwind {
; CHECK-NOT: XCHG
; CHECK: ADD
  %1 = add i257 %par3, %par4
  %2 = add i257 %par1, %par2
  %3 = add i257 %1, %2
  ret i257 %3
}
