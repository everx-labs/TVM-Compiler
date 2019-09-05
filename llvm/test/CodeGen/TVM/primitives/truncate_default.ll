; RUN: llc < %s -O3 -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm-unknown-unknown"

define zeroext i8 @conv_i16_to_i8(i16 %c)  {
; CHECK-LABEL: conv_i16_to_i8:
; CHECK: PUSHINT 255
; CHECK-NEXT: AND
  %1 = trunc i16 %c to i8
  ret i8 %1
}

define i8 @conv_and_inc_i16_to_i8(i16 %c)  {
; CHECK-LABEL: conv_and_inc_i16_to_i8:
; CHECK-NOT: PUSHINT 255
; CHECK-NOT: AND
  %conv = trunc i16 %c to i8
  %test = add i8 1, %conv
  ret i8 %test
}
