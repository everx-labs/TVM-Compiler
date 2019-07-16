; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; CHECK-LABEL: icmpeq
define i64 @icmpeq(i64 %par1, i64 %par2) nounwind {
; CHECK: EQUAL
; CHECK-NEXT: PUSHINT 42
; CHECK-NEXT: PUSHINT 77
; CHECK: CONDSEL
  %1 = icmp eq i64 %par1, %par2
  %2 = select i1 %1, i64 42, i64 77
  ret i64 %2
}

; CHECK-LABEL: icmpeq0
define i64 @icmpeq0(i64 %par1) nounwind {
; CHECK-NOT: EQUAL
; CHECK: PUSHINT 42
; CHECK-NEXT: PUSHINT 77
; CHECK: CONDSEL
  %1 = icmp eq i64 %par1, 0
  %2 = select i1 %1, i64 42, i64 77
  ret i64 %2
}

; CHECK-LABEL: icmpneq
define i64 @icmpneq(i64 %par1, i64 %par2) nounwind {
; CHECK: NEQ
  %1 = icmp ne i64 %par1, %par2
  %2 = select i1 %1, i64 42, i64 77
  ret i64 %2
}

; CHECK-LABEL: icmpneq0
define i64 @icmpneq0(i64 %par1) nounwind {
; CHECK-NOT: NEQ
; CHECK: PUSHINT 77
; CHECK-NEXT: PUSHINT 42
; CHECK: CONDSEL
  %1 = icmp ne i64 %par1, 0
  %2 = select i1 %1, i64 42, i64 77
  ret i64 %2
}

; CHECK-LABEL: icmple
define i64 @icmple(i64 %par1, i64 %par2) nounwind {
; CHECK: LESS
  %1 = icmp slt i64 %par1, %par2
  %2 = select i1 %1, i64 42, i64 77
  ret i64 %2
}

; CHECK-LABEL: icmpleq
define i64 @icmpleq(i64 %par1, i64 %par2) nounwind {
; CHECK: LEQ
  %1 = icmp sle i64 %par1, %par2
  %2 = select i1 %1, i64 42, i64 77
  ret i64 %2
}

; CHECK-LABEL: icmpge
define i64 @icmpge(i64 %par1, i64 %par2) nounwind {
; CHECK: GREATER
  %1 = icmp sgt i64 %par1, %par2
  %2 = select i1 %1, i64 42, i64 77
  ret i64 %2
}

; CHECK-LABEL: icmpgeq
define i64 @icmpgeq(i64 %par1, i64 %par2) nounwind {
; CHECK: GEQ
  %1 = icmp sge i64 %par1, %par2
  %2 = select i1 %1, i64 42, i64 77
  ret i64 %2
}
