; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: icmpeq
define i257 @icmpeq(i257 %par1, i257 %par2) nounwind {
; CHECK: EQUAL
; CHECK-NEXT: PUSHINT 42
; CHECK-NEXT: PUSHINT 77
; CHECK: CONDSEL
  %1 = icmp eq i257 %par1, %par2
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmpeq0
; (par == 0) ? 42 : 77  => (!par) ? 42 : 77 => par ? 77 : 42
define i257 @icmpeq0(i257 %par1) nounwind {
; CHECK-NOT: EQUAL
; CHECK: PUSHINT 77
; CHECK-NEXT: PUSHINT 42
; CHECK: CONDSEL
  %1 = icmp eq i257 %par1, 0
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmpneq
define i257 @icmpneq(i257 %par1, i257 %par2) nounwind {
; CHECK: NEQ
  %1 = icmp ne i257 %par1, %par2
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmpneq2
define i257 @icmpneq2(i257 %par1, i257 %par2) nounwind {
; CHECK-NOT: SWAP
; CHECK: NEQ
  %1 = icmp ne i257 %par2, %par1
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmpneq0
; (par != 0) ? 42 : 77  => (par) ? 42 : 77
define i257 @icmpneq0(i257 %par1) nounwind {
; CHECK-NOT: NEQ
; CHECK: PUSHINT 42
; CHECK-NEXT: PUSHINT 77
; CHECK: CONDSEL
  %1 = icmp ne i257 %par1, 0
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmplt
define i257 @icmplt(i257 %par1, i257 %par2) nounwind {
; CHECK: LESS
  %1 = icmp slt i257 %par1, %par2
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmplt_gt
define i257 @icmplt_gt(i257 %par1, i257 %par2) nounwind {
; CHECK-NOT: SWAP
; CHECK: GREATER
  %1 = icmp slt i257 %par2, %par1
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmpleq
define i257 @icmpleq(i257 %par1, i257 %par2) nounwind {
; CHECK: LEQ
  %1 = icmp sle i257 %par1, %par2
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmpleq_ge
define i257 @icmpleq_ge(i257 %par1, i257 %par2) nounwind {
; CHECK-NOT: SWAP
; CHECK: GEQ
  %1 = icmp sle i257 %par2, %par1
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmpgt
define i257 @icmpgt(i257 %par1, i257 %par2) nounwind {
; CHECK: GREATER
  %1 = icmp sgt i257 %par1, %par2
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmpgeq
define i257 @icmpgeq(i257 %par1, i257 %par2) nounwind {
; CHECK: GEQ
  %1 = icmp sge i257 %par1, %par2
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmpeqimm
define i257 @icmpeqimm(i257 %par1) nounwind {
; CHECK: EQINT
  %1 = icmp eq i257 %par1, 42
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmpneqimm
define i257 @icmpneqimm(i257 %par1) nounwind {
; CHECK: NEQINT
  %1 = icmp ne i257 %par1, 42
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmpsgtimm
define i257 @icmpsgtimm(i257 %par1) nounwind {
; CHECK: GTINT
  %1 = icmp sgt i257 %par1, 42
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmpugtimm
define i257 @icmpugtimm(i257 %par1) nounwind {
; CHECK: GTINT
  %1 = icmp ugt i257 %par1, 42
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmpsltimm
define i257 @icmpsltimm(i257 %par1) nounwind {
; CHECK: LESSINT
  %1 = icmp slt i257 %par1, 42
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}

; CHECK-LABEL: icmpultimm
define i257 @icmpultimm(i257 %par1) nounwind {
; CHECK: LESSINT
  %1 = icmp ult i257 %par1, 42
  %2 = select i1 %1, i257 42, i257 77
  ret i257 %2
}
