; RUN: llc < %s -march=tvm -O0 -asm-verbose=false | FileCheck %s
target triple = "tvm"

%struct.Pair = type { i64, i64 }

; CHECK-LABEL: mk_pair
define void @mk_pair(%struct.Pair* noalias sret %agg.result) {
; CHECK: PUSHINT 11
; CHECK: CALL $:store$
  %left = getelementptr inbounds %struct.Pair, %struct.Pair* %agg.result, i32 0, i32 0
  store i64 11, i64* %left, align 8
; CHECK: ADDCONST 8
; CHECK: PUSHINT 17
; CHECK: CALL $:store$
  %right = getelementptr inbounds %struct.Pair, %struct.Pair* %agg.result, i32 0, i32 1
  store i64 17, i64* %right, align 8
  ret void
}

; CHECK-LABEL: test_pair
define i64 @test_pair() {
; CHECK: PUSHINT 16
; CHECK: CALL $:enter$
  %p = alloca %struct.Pair, align 8
  call void @mk_pair(%struct.Pair* sret %p)
  %left = getelementptr inbounds %struct.Pair, %struct.Pair* %p, i32 0, i32 0
  %1 = load i64, i64* %left, align 8
  %right = getelementptr inbounds %struct.Pair, %struct.Pair* %p, i32 0, i32 1
; CHECK: ADDCONST 8
; CHECK-NEXT: $:load$
  %2 = load i64, i64* %right, align 8
  %add = add nsw i64 %1, %2
; CHECK: PUSHINT 16
; CHECK: CALL $:leave$
  ret i64 %add
}
