; RUN: llc < %s -march=tvm -O0 -asm-verbose=false | FileCheck %s
target triple = "tvm"

%struct.Pair = type { i257, i257 }

; CHECK-LABEL: mk_pair
define void @mk_pair(%struct.Pair* noalias sret(%struct.Pair*) %agg.result) {
; CHECK: PUSHINT 11
; CHECK: GETGLOB 14 CALLX
  %left = getelementptr inbounds %struct.Pair, %struct.Pair* %agg.result, i32 0, i32 0
  store i257 11, i257* %left, align 1
; CHECK: INC
; CHECK: PUSHINT 17
; CHECK: GETGLOB 14 CALLX
  %right = getelementptr inbounds %struct.Pair, %struct.Pair* %agg.result, i32 0, i32 1
  store i257 17, i257* %right, align 1
  ret void
}

; CHECK-LABEL: test_pair
define i257 @test_pair() {
; CHECK: GETGLOB 5
; CHECK-NEXT: ADDCONST -2
; CHECK-NEXT: SETGLOB
  %p = alloca %struct.Pair, align 1
  call void @mk_pair(%struct.Pair* sret(%struct.Pair*) %p)
  %left = getelementptr inbounds %struct.Pair, %struct.Pair* %p, i32 0, i32 0
  %1 = load i257, i257* %left, align 1
  %right = getelementptr inbounds %struct.Pair, %struct.Pair* %p, i32 0, i32 1
; CHECK: INC
; CHECK-NEXT: GETGLOB 13 CALLX
  %2 = load i257, i257* %right, align 1
  %add = add nsw i257 %1, %2
; CHECK: GETGLOB 5
; CHECK-NEXT: ADDCONST 2
; CHECK-NEXT: SETGLOB 5
  ret i257 %add
}
