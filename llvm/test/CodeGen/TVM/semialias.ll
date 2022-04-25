; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

declare i257 @foo(i257 %a, i257 %b, i257 %c, i257 %d)
declare i257 @bar(i257 %a, i257 %b)
declare i257 @baz(i257 %a, i257 %b, i257 %c)

; CHECK-LABEL: swap2
define i257 @swap2(i257 %a, i257 %b, i257 %c, i257 %d) nounwind {
  ; TODO: ROLL ROLL is generated, should be SWAP2
  %1 = call i257 @foo(i257 %c, i257 %d, i257 %a, i257 %b)
  ret i257 %1
}

; CHECK-LABEL: drop2
define i257 @drop2(i257 %a, i257 %b, i257 %c) nounwind {
  ; CHECK: DROP2
  ret i257 %a
}

; CHECK-LABEL: dup2
define i257 @dup2(i257 %a, i257 %b) nounwind {
  ; CHECK: DUP2
  %1 = call i257 @bar(i257 %a, i257 %b)
  %2 = add i257 %1, %a
  %3 = add i257 %2, %b
  ret i257 %3
}

; CHECK-LABEL: over2
define i257 @over2(i257 %a, i257 %b, i257 %c, i257 %d) nounwind {
  ; CHECK: OVER2
  %1 = call i257 @bar(i257 %a, i257 %b)
  %2 = add i257 %1, %a
  %3 = add i257 %2, %b
  %4 = add i257 %3, %c
  %5 = add i257 %4, %d
  ret i257 %5
}

; CHECK-LABEL: rot
define i257 @rot(i257 %a, i257 %b, i257 %c) nounwind {
  ; CHECK: ROT
  %1 = call i257 @baz(i257 %b, i257 %c, i257 %a)
  ret i257 %1
}

; CHECK-LABEL: rotrev
define i257 @rotrev(i257 %a, i257 %b, i257 %c) nounwind {
  ; CHECK: ROTREV
  %1 = call i257 @baz(i257 %c, i257 %a, i257 %b)
  ret i257 %1
}

; CHECK-LABEL: tuck
define i257 @tuck(i257 %a, i257 %b) nounwind {
  ; CHECK: TUCK
  %1 = call i257 @baz(i257 %b, i257 %a, i257 %b)
  ret i257 %1
}
