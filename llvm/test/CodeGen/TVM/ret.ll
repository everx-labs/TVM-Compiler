; XFAIL: *

; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define i8 @ret42() nounwind {
	ret i8 42
}
