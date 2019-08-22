; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s 
; XFAIL:*

target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @urem_func(i257 %a, i257 %b) norecurse nounwind readnone noinline {
; CHECK-LABEL: urem_func:
; CHECK:	AND
; CHECK:	XCHG	s1, s2
; CHECK:	AND
; CHECK:	XCHG	s0, s1
; CHECK:	MOD

entry:
  %rem = urem i257 %a, %b
  ret i257 %rem
}

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @srem_func(i257 %a, i257 %b) norecurse nounwind readnone noinline {
; CHECK-LABEL: srem_func:
; CHECK:	PUSH	s1
; CHECK:	PUSH	s1
; CHECK:	XOR
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
  %rem = srem i257 %a, %b
  ret i257 %rem
}

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @udiv_func(i257 %a, i257 %b) norecurse nounwind readnone noinline {
; CHECK-LABEL: udiv_func:
; CHECK:	AND
; CHECK:	XCHG	s1, s2
; CHECK:	AND
; CHECK:	XCHG	s0, s1
; CHECK:	DIV

entry:
  %rem = udiv i257 %a, %b
  ret i257 %rem
}

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @sdiv_func(i257 %a, i257 %b) norecurse nounwind readnone noinline {
; CHECK-LABEL: sdiv_func:
; CHECK:	PUSH	s1
; CHECK:	PUSH	s1
; CHECK:	XOR
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
  %rem = sdiv i257 %a, %b
  ret i257 %rem
}

