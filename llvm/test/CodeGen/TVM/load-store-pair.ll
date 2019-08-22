; RUN: llc < %s -march=tvm -O0 -asm-verbose=false | FileCheck %s
target triple = "tvm"

%struct.Pair = type { i257, i257 }

; CHECK-LABEL: mk_pair
define void @mk_pair(%struct.Pair* noalias sret %agg.result) {
; CHECK: PUSHINT 11
; CHECK: CALL $:store$
  %left = getelementptr inbounds %struct.Pair, %struct.Pair* %agg.result, i32 0, i32 0
  store i257 11, i257* %left, align 1
; CHECK: INC
; CHECK: PUSHINT 17
; CHECK: CALL $:store$
  %right = getelementptr inbounds %struct.Pair, %struct.Pair* %agg.result, i32 0, i32 1
  store i257 17, i257* %right, align 1
  ret void
}

; CHECK-LABEL: test_pair
define i257 @test_pair() {
; CHECK: PUSHINT 2
; CHECK: CALL $:enter$
  %p = alloca %struct.Pair, align 1
  call void @mk_pair(%struct.Pair* sret %p)
  %left = getelementptr inbounds %struct.Pair, %struct.Pair* %p, i32 0, i32 0
  %1 = load i257, i257* %left, align 1
  %right = getelementptr inbounds %struct.Pair, %struct.Pair* %p, i32 0, i32 1
; CHECK: INC
; CHECK-NEXT: $:load$
  %2 = load i257, i257* %right, align 1
  %add = add nsw i257 %1, %2
; CHECK: PUSHINT 2
; CHECK: CALL $:leave$
  ret i257 %add
}
