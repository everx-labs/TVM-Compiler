; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm" 

define i64 @rshift (i64 %v) nounwind {
; CHECK-LABEL: rshift:
; CHECK: {{^[ ]*RSHIFT[ ]*256$}}
 %1 = lshr i64 %v, 256
 ret i64 %1
}