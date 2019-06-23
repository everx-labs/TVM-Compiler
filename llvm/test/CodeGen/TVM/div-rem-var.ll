; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s 

target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i64 @urem_func(i64 %a, i64 %b) norecurse nounwind readnone noinline {
; CHECK-LABEL: urem_func:
; CHECK:	PUSHINT	18446744073709551615
; CHECK:	AND
; CHECK:	PUSHINT	18446744073709551615
; CHECK:	XCHG	s1, s2
; CHECK:	AND
; CHECK:	XCHG	s0, s1
; CHECK:	MOD

entry:
  %rem = urem i64 %a, %b
  ret i64 %rem
}

; Function Attrs: norecurse nounwind readnone
define dso_local i64 @srem_func(i64 %a, i64 %b) norecurse nounwind readnone noinline {
; CHECK-LABEL: srem_func:
; CHECK:	PUSH	s1
; CHECK:	PUSH	s1
; CHECK:	XOR
; CHECK:	PUSHINT	9223372036854775808
; CHECK:	AND
; CHECK:	XCHG	s0, s1
; CHECK:	ABS
; CHECK:	XCHG	s0, s2
; CHECK:	ABS
; CHECK:	XCHG	s0, s2
; CHECK:	XCHG	s1, s2
; CHECK:	MOD
; CHECK:	AND

entry:
  %rem = srem i64 %a, %b
  ret i64 %rem
}

; Function Attrs: norecurse nounwind readnone
define dso_local i64 @udiv_func(i64 %a, i64 %b) norecurse nounwind readnone noinline {
; CHECK-LABEL: udiv_func:
; CHECK:	PUSHINT	18446744073709551615
; CHECK:	AND
; CHECK:	PUSHINT	18446744073709551615
; CHECK:	XCHG	s1, s2
; CHECK:	AND
; CHECK:	XCHG	s0, s1
; CHECK:	DIV

entry:
  %rem = udiv i64 %a, %b
  ret i64 %rem
}

; Function Attrs: norecurse nounwind readnone
define dso_local i64 @sdiv_func(i64 %a, i64 %b) norecurse nounwind readnone noinline {
; CHECK-LABEL: sdiv_func:
; CHECK:	PUSH	s1
; CHECK:	PUSH	s1
; CHECK:	XOR
; CHECK:	PUSHINT	9223372036854775808
; CHECK:	AND
; CHECK:	XCHG	s0, s1
; CHECK:	ABS
; CHECK:	XCHG	s0, s2
; CHECK:	ABS
; CHECK:	XCHG	s0, s2
; CHECK:	XCHG	s1, s2
; CHECK:	DIV
; CHECK:	AND

entry:
  %rem = sdiv i64 %a, %b
  ret i64 %rem
}

