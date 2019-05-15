; RUN: llc < %s -march=tvm | FileCheck %s

target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define i64 @cond_comp(i64 %sel, i64 %par) nounwind {
  switch i64 %sel, label %bb3 [ i64 0, label %bb1
                                i64 1, label %bb2 ]
; CHECK:	PUSHINT	1
; CHECK-NEXT:	PUSH s2
; CHECK-NEXT:	XCHG s0, s1
; CHECK-NEXT:	LESS
; CHECK-NEXT: PUSHCONT
; CHECK-NEXT:   PUSHINT 1
; CHECK-NEXT:   XCHG  s1, s2
; CHECK-NEXT:   EQUAL
; CHECK-NEXT:   PUSHCONT
; CHECK-NEXT:     PUSH  s0
; CHECK-NEXT:     PUSH  s0
; CHECK-NEXT:     MUL
; CHECK-NEXT:     MUL
; CHECK-NEXT:     RET
; CHECK-NEXT:     }
; CHECK-NEXT:   PUSHCONT
; CHECK-NEXT:     PUSH  s0
; CHECK-NEXT:     MUL
; CHECK-NEXT:     RET
; CHECK-NEXT:     }
; CHECK-NEXT:   IFELSE
; CHECK-NEXT:   }
; CHECK-NEXT: PUSHCONT
; CHECK-NEXT:   XCHG  s0, s1
; CHECK-NEXT:   ISZERO
; CHECK-NEXT:   PUSHCONT
; CHECK-NEXT:     PUSH  s0
; CHECK-NEXT:     PUSH  s0
; CHECK-NEXT:     MUL
; CHECK-NEXT:     MUL
; CHECK-NEXT:     RET
; CHECK-NEXT:     }
; CHECK-NEXT:   PUSHCONT
; CHECK-NEXT:   }
; CHECK-NEXT:   IFELSE
; CHECK-NEXT:   }
; CHECK-NEXT: IFELSE
bb1:
  %1 = add i64 0, %par
  br label %exit
bb2:
  %2 = mul i64 %par, %par
  ret i64 %2
bb3:
  %3 = mul i64 %par, %par
  %4 = mul i64 %3, %par
  ret i64 %4
exit:
  ret i64 %1
}
