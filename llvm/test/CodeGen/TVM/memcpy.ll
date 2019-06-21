; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

%struct.anon = type { [100 x i64] }

@X = global %struct.anon zeroinitializer, align 8
@Y = global %struct.anon zeroinitializer, align 8

; CHECK-LABEL: do_copy
define void @do_copy() {
entry:
; CHECK: memcpy
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 bitcast (%struct.anon* @X to i8*), i8* align 8 bitcast (%struct.anon* @Y to i8*), i64 800, i1 false)
  ret void
}

declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1)
