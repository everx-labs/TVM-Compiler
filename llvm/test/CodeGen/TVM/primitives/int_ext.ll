; RUN: llc < %s -O0 -march=tvm -asm-verbose=false | FileCheck %s 
; XFAIL:*
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm-unknown-unknown"

define signext i8 @conv_i8_to_i8(i8 signext %x) {
; CHECK-LABEL: conv_i8_to_i8:
; CHECK: RET
  ret i8 %x
}

define zeroext i8 @conv_i8_to_u8(i8 signext %x) {
; CHECK-LABEL: conv_i8_to_u8:
; CHECK: PUSHINT 255
; CHECK-NEXT: AND
; CHECK: RET
  ret i8 %x
}

define signext i16 @conv_i8_to_i16(i8 %x) {
; CHECK-LABEL: conv_i8_to_i16:
; CHECK: LSHIFT 56
; CHECK-NEXT: RSHIFT 56
; CHECK: RET
  %conv = sext i8 %x to i16
  ret i16 %conv
}

define i16 @conv_i8_to_u16(i8 %x) {
; CHECK-LABEL: conv_i8_to_u16:
; CHECK: LSHIFT 56
; CHECK-NEXT: RSHIFT 56
; CHECK: RET
  %conv = sext i8 %x to i16
  ret i16 %conv
}

define i32 @conv_i8_to_i32(i8 %x) {
; CHECK-LABEL: conv_i8_to_i32:
; CHECK: LSHIFT 56
; CHECK-NEXT: RSHIFT 56
; CHECK: RET
  %conv = sext i8 %x to i32
  ret i32 %conv
}

define i32 @conv_i8_to_u32(i8 %x) {
; CHECK-LABEL: conv_i8_to_u32:
; CHECK: LSHIFT 56
; CHECK-NEXT: RSHIFT 56
; CHECK: RET
  %conv = sext i8 %x to i32
  ret i32 %conv
}

define i64 @conv_i8_to_i64(i8 %x) {
; CHECK-LABEL: conv_i8_to_i64:
; CHECK: LSHIFT 56
; CHECK-NEXT: RSHIFT 56
; CHECK: RET
  %conv = sext i8 %x to i64
  ret i64 %conv
}

define i64 @conv_i8_to_u64(i8 %x) {
; CHECK-LABEL: conv_i8_to_u64:
; CHECK: LSHIFT 56
; CHECK-NEXT: RSHIFT 56
; CHECK: RET
  %conv = sext i8 %x to i64
  ret i64 %conv
}

define signext i8 @conv_u8_to_i8(i8 %x) {
; CHECK-LABEL: conv_u8_to_i8:
; CHECK: LSHIFT 56
; CHECK-NEXT: RSHIFT 56
; CHECK: RET
  ret i8 %x
}

define zeroext i8 @conv_u8_to_u8(i8 zeroext %x) {
; CHECK-LABEL: conv_u8_to_u8:
; CHECK: RET
  ret i8 %x
}

define signext i16 @conv_u8_to_i16(i8 %x) {
; CHECK-LABEL: conv_u8_to_i16:
; CHECK: PUSHINT 255
; CHECK-NEXT: AND
; CHECK: RET
  %conv = zext i8 %x to i16
  ret i16 %conv
}

define zeroext i16 @conv_u8_to_u16(i8 %x) {
; CHECK-LABEL: conv_u8_to_u16:
; CHECK: PUSHINT 255
; CHECK-NEXT: AND
; CHECK: RET
  %conv = zext i8 %x to i16
  ret i16 %conv
}

define i32 @conv_u8_to_i32(i8 %x) {
; CHECK-LABEL: conv_u8_to_i32:
; CHECK: PUSHINT 255
; CHECK-NEXT: AND
; CHECK: RET
  %conv = zext i8 %x to i32
  ret i32 %conv
}

define i32 @conv_u8_to_u32(i8 %x) {
; CHECK-LABEL: conv_u8_to_u32:
; CHECK: PUSHINT 255
; CHECK-NEXT: AND
; CHECK: RET
  %conv = zext i8 %x to i32
  ret i32 %conv
}

define i64 @conv_u8_to_i64(i8 %x) {
; CHECK-LABEL: conv_u8_to_i64:
; CHECK: PUSHINT  255
; CHECK-NEXT: XCHG s1, s2
; CHECK-NEXT: AND
; CHECK: RET
  %conv = zext i8 %x to i64
  ret i64 %conv
}

define i64 @conv_u8_to_u64(i8 %x) {
; CHECK-LABEL: conv_u8_to_u64:
; CHECK: PUSHINT  255
; CHECK-NEXT: XCHG s1, s2
; CHECK-NEXT: AND
; CHECK: RET
  %conv = zext i8 %x to i64
  ret i64 %conv
}

define signext i16 @conv_i16_to_i16(i16 signext %x) {
; CHECK-LABEL: conv_i16_to_i16:
; CHECK: RET
  ret i16 %x
}

define zeroext i16 @conv_i16_to_u16(i16 signext %x) {
; CHECK-LABEL: conv_i16_to_u16:
; CHECK: PUSHINT 65535
; CHECK-NEXT: AND
; CHECK: RET
  ret i16 %x
}

define i32 @conv_i16_to_i32(i16 %x) {
; CHECK-LABEL: conv_i16_to_i32:
; CHECK: LSHIFT 48
; CHECK-NEXT: RSHIFT 48
; CHECK: RET
  %conv = sext i16 %x to i32
  ret i32 %conv
}

define i32 @conv_i16_to_u32(i16 %x) {
; CHECK-LABEL: conv_i16_to_u32:
; CHECK: LSHIFT 48
; CHECK-NEXT: RSHIFT 48
; CHECK: RET
  %conv = sext i16 %x to i32
  ret i32 %conv
}

define i64 @conv_i16_to_i64(i16 %x) {
; CHECK-LABEL: conv_i16_to_i64:
; CHECK: LSHIFT 48
; CHECK-NEXT: RSHIFT 48
; CHECK: RET
  %conv = sext i16 %x to i64
  ret i64 %conv
}

define i64 @conv_i16_to_u64(i16 %x) {
; CHECK-LABEL: conv_i16_to_u64:
; CHECK: LSHIFT 48
; CHECK-NEXT: RSHIFT 48
; CHECK: RET
  %conv = sext i16 %x to i64
  ret i64 %conv
}

define signext i16 @conv_u16_to_i16(i16 zeroext %x) {
; CHECK-LABEL: conv_u16_to_i16:
; CHECK: LSHIFT 48
; CHECK-NEXT: RSHIFT 48
; CHECK: RET
  ret i16 %x
}

define zeroext i16 @conv_u16_to_u16(i16 zeroext %x) {
; CHECK-LABEL: conv_u16_to_u16:
; CHECK: RET
  ret i16 %x
}

define i32 @conv_u16_to_i32(i16 %x) {
; CHECK-LABEL: conv_u16_to_i32:
; CHECK: PUSHINT 65535
; CHECK-NEXT: AND
; CHECK: RET
  %conv = zext i16 %x to i32
  ret i32 %conv
}

define i32 @conv_u16_to_u32(i16 %x) {
; CHECK-LABEL: conv_u16_to_u32:
; CHECK: PUSHINT 65535
; CHECK-NEXT: AND
; CHECK: RET
  %conv = zext i16 %x to i32
  ret i32 %conv
}

define i64 @conv_u16_to_i64(i16 %x) {
; CHECK-LABEL: conv_u16_to_i64:
; CHECK: PUSHINT 65535
; CHECK-NEXT: XCHG s1, s2
; CHECK-NEXT: AND
; CHECK: RET
  %conv = zext i16 %x to i64
  ret i64 %conv
}

define i64 @conv_u16_to_u64(i16 zeroext %x) {
; CHECK-LABEL: conv_u16_to_u64:
; CHECK: RET
  %conv = zext i16 %x to i64
  ret i64 %conv
}

define i32 @conv_i32_to_i32(i32 %x) {
; CHECK-LABEL: conv_i32_to_i32:
; CHECK: RET
  ret i32 %x
}

define zeroext i32 @conv_i32_to_u32(i32 signext %x) {
; CHECK-LABEL: conv_i32_to_u32:
; CHECK: PUSHINT 4294967295
; CHECK-NEXT: AND
; CHECK: RET
  ret i32 %x
}

define i64 @conv_i32_to_i64(i32 %x) {
; CHECK-LABEL: conv_i32_to_i64:
; CHECK: LSHIFT 32
; CHECK-NEXT: RSHIFT 32
; CHECK: RET
  %conv = sext i32 %x to i64
  ret i64 %conv
}

define i64 @conv_i32_to_u64(i32 %x) {
; CHECK-LABEL: conv_i32_to_u64:
; CHECK: LSHIFT 32
; CHECK-NEXT: RSHIFT 32
; CHECK: RET
  %conv = sext i32 %x to i64
  ret i64 %conv
}

define i32 @conv_u32_to_i32(i32 %x) {
; CHECK-LABEL: conv_u32_to_i32:
; CHECK: RET
  ret i32 %x
}

define i32 @conv_u32_to_u32(i32 %x) {
; CHECK-LABEL: conv_u32_to_u32:
; CHECK: RET
  ret i32 %x
}

define i64 @conv_u32_to_i64(i32 %x) {
; CHECK-LABEL: conv_u32_to_i64:
; CHECK: PUSHINT  4294967295
; CHECK-NEXT: XCHG s1, s2
; CHECK-NEXT: AND
; CHECK: RET
  %conv = zext i32 %x to i64
  ret i64 %conv
}

define i64 @conv_u32_to_u64(i32 %x) {
; CHECK-LABEL: conv_u32_to_u64:
; CHECK: PUSHINT  4294967295
; CHECK-NEXT: XCHG s1, s2
; CHECK-NEXT: AND
; CHECK: RET
  %conv = zext i32 %x to i64
  ret i64 %conv
}
