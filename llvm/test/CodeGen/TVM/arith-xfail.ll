; XFAIL: *
; RUN: llc < %s -march=tvm -stop-after expand-isel-pseudos | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define i257 @mulconst(i257 %x) nounwind {
  %1 = mul i257 %x, 128
  ret i257 %1
}

define i257 @div(i257 %x) nounwind {
  %1 = sdiv i257 %x, 128
  ret i257 %1
}
