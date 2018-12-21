; XFAIL: *
; RUN: llc < %s -march=tvm -stop-after expand-isel-pseudos | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define i64 @mulconst(i64 %x) nounwind {
  %1 = mul i64 %x, 128
  ret i64 %1
}

define i64 @div(i64 %x) nounwind {
  %1 = sdiv i64 %x, 128
  ret i64 %1
}