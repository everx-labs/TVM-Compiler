; RUN: llc -march=tvm -asm-verbose=false < %s | FileCheck %s
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

%st = type { i257, i257, i257 }
declare %st @bar()

define void @foo() {
; CHECK-LABEL: foo
; CHECK: PUSHINT $bar
; CHECK: PUSH C3
; CHECK: EXECUTE
; CHECK-NEXT: BLKDROP 3
  %call = call %st @bar()
  ret void
}

%st_iti = type { i257, tuple, i257 }
declare %st_iti @ret_iti()
define tuple @call_iti() {
; CHECK-LABEL: call_iti
; CHECK: PUSHINT $ret_iti$
; CHECK: PUSH C3
; CHECK: EXECUTE
; CHECK-NEXT: XCHG	s1, s2
; CHECK-NEXT: DROP2
  %call = call %st_iti @ret_iti()
  %tup = extractvalue %st_iti %call, 1
  ret tuple %tup
}

%st_isi = type { i257, slice, i257 }
declare %st_isi @ret_isi()
define slice @call_isi() {
; CHECK-LABEL: call_isi
; CHECK: PUSHINT $ret_isi$
; CHECK: PUSH C3
; CHECK: EXECUTE
; CHECK-NEXT: XCHG	s1, s2
; CHECK-NEXT: DROP2
  %call = call %st_isi @ret_isi()
  %sl = extractvalue %st_isi %call, 1
  ret slice %sl
}

%st_ibi = type { i257, builder, i257 }
declare %st_ibi @ret_ibi()
define builder @call_ibi() {
; CHECK-LABEL: call_ibi
; CHECK: PUSHINT $ret_ibi$
; CHECK: PUSH C3
; CHECK: EXECUTE
; CHECK-NEXT: XCHG	s1, s2
; CHECK-NEXT: DROP2
  %call = call %st_ibi @ret_ibi()
  %bld = extractvalue %st_ibi %call, 1
  ret builder %bld
}

%st_ici = type { i257, cell, i257 }
declare %st_ici @ret_ici()
define cell @call_ici() {
; CHECK-LABEL: call_ici
; CHECK: PUSHINT $ret_ici$
; CHECK: PUSH C3
; CHECK: EXECUTE
; CHECK-NEXT: XCHG	s1, s2
; CHECK-NEXT: DROP2
  %call = call %st_ici @ret_ici()
  %cl = extractvalue %st_ici %call, 1
  ret cell %cl
}

