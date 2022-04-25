; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
; XFAIL:*

target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define i257 @cond_comp(i257 %sel, i257 %par) nounwind {
  switch i257 %sel, label %bb3 [ i257 0, label %bb1
                                i257 1, label %bb2 ]
; CHECK:	PUSHINT	1
; CHECK-NEXT:	PUSH s3
; CHECK-NEXT:	XCHG s0, s1
; CHECK-NEXT:	LESS
; CHECK-NEXT: PUSHCONT
; CHECK:        PUSHINT 1
; CHECK-NEXT:   XCHG  s1, s3
; CHECK-NEXT:   EQUAL
; CHECK:        PUSHCONT
; CHECK:          PUSH  s0
; CHECK-NEXT:     PUSH  s0
; CHECK-NEXT:     MUL
; CHECK-NEXT:     MUL
; CHECK-NEXT:     RET
; CHECK:        }
; CHECK:        PUSHCONT
; CHECK:          PUSH  s0
; CHECK-NEXT:     MUL
; CHECK-NEXT:     RET
; CHECK-NEXT:     }
; CHECK:        XCHG  s0, s1
; CHECK:        IFELSE
; CHECK-NEXT:   }
; CHECK:      PUSHCONT
; CHECK:        XCHG  s0, s2
; CHECK-NEXT:   ISZERO
; CHECK:        PUSHCONT
; CHECK:          PUSH  s0
; CHECK-NEXT:     PUSH  s0
; CHECK-NEXT:     MUL
; CHECK-NEXT:     MUL
; CHECK-NEXT:     RET
; CHECK-NEXT:     }
; CHECK:        PUSHCONT
; CHECK:        }
; CHECK:      IFELSE
bb1:
  %1 = add i257 0, %par
  br label %exit
bb2:
  %2 = mul i257 %par, %par
  ret i257 %2
bb3:
  %3 = mul i257 %par, %par
  %4 = mul i257 %3, %par
  ret i257 %4
exit:
  ret i257 %1
}
