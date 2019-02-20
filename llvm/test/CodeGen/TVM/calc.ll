; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S1024-i256:256:256" 
target triple = "tvm"

define i64 @calc (i64 %arg1, i64 %arg2, i64 %op) nounwind {
; CHECK-LABEL: calc:
; CHECK-NEXT: DUP
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:    DEC
; CHECK-NEXT:    DUP
; CHECK-NEXT:    PUSHCONT {
; CHECK-NEXT:       DEC
; CHECK-NEXT:       DUP
; CHECK-NEXT:       PUSHCONT {
; CHECK-NEXT:          DEC
; CHECK-NEXT:          PUSHCONT {
; CHECK-NEXT:             DROP2
; CHECK-NEXT:             RET
; CHECK-NEXT:          }
; CHECK-NEXT:          PUSHCONT {
; CHECK-NEXT:             DIV
; CHECK-NEXT:             RET
; CHECK-NEXT:          }
; CHECK-NEXT:          IFELSE
; CHECK-NEXT:       }
; CHECK-NEXT:       PUSHCONT {
; CHECK-NEXT:          DROP
; CHECK-NEXT:          MUL
; CHECK-NEXT:          RET
; CHECK-NEXT:       }
; CHECK-NEXT:       IFELSE
; CHECK-NEXT:    }
; CHECK-NEXT:    PUSHCONT {
; CHECK-NEXT:       DROP
; CHECK-NEXT:       SUB
; CHECK-NEXT:       RET
; CHECK-NEXT:    }
; CHECK-NEXT:    IFELSE
; CHECK-NEXT: }
; CHECK-NEXT: PUSHCONT {
; CHECK-NEXT:    DROP
; CHECK-NEXT:    ADD
; CHECK-NEXT:    RET
; CHECK-NEXT: }
; CHECK-NEXT: IFELSE
 switch i64 %op, label %noop [ i64 0, label %sum
                               i64 1, label %sub
                               i64 2, label %mul
                               i64 3, label %div ]
sum:
 %1 = add i64 %arg1, %arg2
 ret i64 %1
sub:
 %2 = sub i64 %arg1, %arg2
 ret i64 %2
mul:
 %3 = mul i64 %arg1, %arg2
 ret i64 %3
div:
 %4 = sdiv i64 %arg1, %arg2
 ret i64 %4
noop:
 ret i64 0
}
