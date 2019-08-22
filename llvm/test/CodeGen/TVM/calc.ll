; XFAIL: *
; RUN: llc < %s -march=tvm | FileCheck %s 
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257" 
target triple = "tvm"

define i257 @calc (i257 %arg1, i257 %arg2, i257 %op) nounwind {
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
 switch i257 %op, label %noop [ i257 0, label %sum
                               i257 1, label %sub
                               i257 2, label %mul
                               i257 3, label %div ]
sum:
 %1 = add i257 %arg1, %arg2
 ret i257 %1
sub:
 %2 = sub i257 %arg1, %arg2
 ret i257 %2
mul:
 %3 = mul i257 %arg1, %arg2
 ret i257 %3
div:
 %4 = sdiv i257 %arg1, %arg2
 ret i257 %4
noop:
 ret i257 0
}
