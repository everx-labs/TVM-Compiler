; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; CHECK-LABEL: brcond
define i64 @brcond(i1 %par) nounwind {
entry:
; CHECK: PUSHCONT .LBB{{[0-9]+}}_2
; CHECK-NEXT: IFJMP
; CHECK-NEXT: PUSHCONT .LBB{{[0-9]+}}_1
; CHECK-NEXT: JMPX
  br i1 %par, label %exit1, label %exit2
exit1:
  ret i64 42
exit2:
  ret i64 77
}
; TODO: The test shows 3 deficiencies in our code.
; The generated code is:
;    PUSHINT    1
;    AND
;    ISZERO
;    PUSHCONT    .LBB0_2
;    IFJMP
;    PUSHCONT    .LBB0_1
;    JMPX
;; %bb.1:          ; <-- machine basic block was removed by LLVM,
;                  ;     but JMPX wasn't.
;    PUSHINT    42 ; <-- fall through is incorrect, should be explicit RET.
;.LBB0_2:
;    PUSHINT    77
;    POP    s1     ; <-- depending on control flow there might not be
;                        an element at s1, so stack must be modeled separately
;                        for each basic block.
;.Lfunc_end0:
