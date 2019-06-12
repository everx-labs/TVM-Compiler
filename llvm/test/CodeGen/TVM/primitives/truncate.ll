; RUN: llc < %s -O0 -march=tvm -tvm-trunc-masks -asm-verbose=false | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm-unknown-unknown"

define zeroext i8 @conv_i16_to_i8(i16 %c)  {
; CHECK-LABEL: conv_i16_to_i8:
; CHECK: PUSHINT 255
; CHECK-NEXT: AND
  %1 = trunc i16 %c to i8
  ret i8 %1
}

define zeroext i8 @conv_i32_to_i8(i32 %c)  {
; CHECK-LABEL: conv_i32_to_i8:
; CHECK: PUSHINT 255
; CHECK-NEXT: AND
  %1 = trunc i32 %c to i8
  ret i8 %1
}

define zeroext i8 @conv_i64_to_i8(i64 %c)  {
; CHECK-LABEL: conv_i64_to_i8:
; CHECK: PUSHINT 255
; CHECK-NEXT: AND
  %1 = trunc i64 %c to i8
  ret i8 %1
}

define zeroext i16 @conv_i32_to_i16(i32 %c)  {
; CHECK-LABEL: conv_i32_to_i16:
; CHECK: PUSHINT 65535
; CHECK-NEXT: AND
  %1 = trunc i32 %c to i16
  ret i16 %1
}

define zeroext i16 @conv_i64_to_i16(i64 %c)  {
; CHECK-LABEL: conv_i64_to_i16:
; CHECK: PUSHINT 65535
; CHECK-NEXT: AND
  %1 = trunc i64 %c to i16
  ret i16 %1
}

define zeroext i32 @conv_i64_to_i32(i64 %c)  {
; CHECK-LABEL: conv_i64_to_i32:
; CHECK: PUSHINT 4294967295
; CHECK-NEXT: AND
  %1 = trunc i64 %c to i32
  ret i32 %1
}

define i8 @conv_and_inc_i16_to_i8(i16 %c)  {
; CHECK-LABEL: conv_and_inc_i16_to_i8:
; CHECK: PUSHINT 255
; CHECK-NEXT: AND
  %conv = trunc i16 %c to i8
  %test = add i8 1, %conv
  ret i8 %test
}

define i8 @conv_and_inc_i32_to_i8(i32 %c)  {
; CHECK-LABEL: conv_and_inc_i32_to_i8:
; CHECK: PUSHINT 255
; CHECK-NEXT: AND
  %conv = trunc i32 %c to i8
  %test = add i8 1, %conv
  ret i8 %test
}

define i8 @conv_and_inc_i64_to_i8(i64 %c)  {
; CHECK-LABEL: conv_and_inc_i64_to_i8:
; CHECK: PUSHINT 255
; CHECK-NEXT: AND
  %conv = trunc i64 %c to i8
  %test = add i8 1, %conv
  ret i8 %test
}

define i16 @conv_and_inc_i32_to_i16(i32 %c)  {
; CHECK-LABEL: conv_and_inc_i32_to_i16:
; CHECK: PUSHINT 65535
; CHECK-NEXT: AND
  %conv = trunc i32 %c to i16
  %test = add i16 1, %conv
  ret i16 %test
}

define i16 @conv_and_inc_i64_to_i16(i64 %c)  {
; CHECK-LABEL: conv_and_inc_i64_to_i16:
; CHECK: PUSHINT 65535
; CHECK-NEXT: AND
  %conv = trunc i64 %c to i16
  %test = add i16 1, %conv
  ret i16 %test
}

define i32 @conv_and_inc_i64_to_i32(i64 %c)  {
; CHECK-LABEL: conv_and_inc_i64_to_i32:
; CHECK: PUSHINT 4294967295
; CHECK-NEXT: AND
  %conv = trunc i64 %c to i32
  %test = add i32 1, %conv
  ret i32 %test
}
