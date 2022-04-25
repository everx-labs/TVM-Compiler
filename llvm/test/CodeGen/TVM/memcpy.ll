; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

%struct.anon = type { [100 x i257] }

@X = global %struct.anon zeroinitializer, align 1
@Y = global %struct.anon zeroinitializer, align 1

; CHECK-LABEL: do_copy
define void @do_copy() {
entry:
  call void @llvm.memcpy.p0i8.p0i8.i257(i8* align 1 bitcast (%struct.anon* @X to i8*), i8* align 1 bitcast (%struct.anon* @Y to i8*), i257 100, i1 false)
  ret void
}

declare void @llvm.memcpy.p0i8.p0i8.i257(i8* nocapture writeonly, i8* nocapture readonly, i257, i1)
