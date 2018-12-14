; RUN: llc < %s -march=tvm -stop-after expand-isel-pseudos | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

define i64 @addi64(i64 %par) nounwind {
; CHECK: %[[VR1:[0-9]+]]:i64 = ARGUMENT 0
; CHECK: %[[VR0:[0-9]+]]:i64 = CONST_I64 12345
; CHECK: %[[VR2:[0-9]+]]:i64 = ADD killed %[[VR1]], killed %[[VR0]]
  %1 = add i64 %par, 12345
; CHECK: RETURN_I64 killed %[[VR2]]
	ret i64 %1
}

define i64 @subi64(i64 %par, i64 %par2) nounwind {
; CHECK-DAG: %[[VR0:[0-9]+]]:i64 = ARGUMENT 0
; CHECK-DAG: %[[VR1:[0-9]+]]:i64 = ARGUMENT 1
; CHECK: %[[VR2:[0-9]+]]:i64 = SUB killed %[[VR0]], killed %[[VR1]]
  %1 = sub i64 %par, %par2
; CHECK: RETURN_I64 killed %[[VR2]]
  ret i64 %1
}

;TODO complete the test, should be permutation optimization
define i64 @subri64(i64 %x, i64 %y) nounwind {
 %1 = sub i64  0, %x
 %2 = add i64 %1, %y
 ret i64 %2
}

define i64 @negate(i64 %x) nounwind {
; CHECK: %[[VR0:[0-9]+]]:i64 = ARGUMENT 0, implicit $arguments
; CHECK: %[[VR1:[0-9]+]]:i64 = NEGATE killed %[[VR0]]
 %1 = sub i64 0, %x
; CHECK: RETURN_I64 killed %[[VR1]]
 ret i64 %1
}

define i64 @increment(i64 %x) nounwind {
; CHECK: %[[VR0:[0-9]+]]:i64 = ARGUMENT 0, implicit $arguments
; CHECK: %[[VR1:[0-9]+]]:i64 = INC killed %[[VR0]]
 %1 = add i64 1, %x
; CHECK: RETURN_I64 killed %[[VR1]]
 ret i64 %1
}

define i64 @decrement(i64 %x) nounwind {
; CHECK: %[[VR0:[0-9]+]]:i64 = ARGUMENT 0, implicit $arguments
; CHECK: %[[VR1:[0-9]+]]:i64 = DEC killed %[[VR0]]
 %1 = add i64 -1, %x
; CHECK: RETURN_I64 killed %[[VR1]]
 ret i64 %1
}

define i64 @mul (i64 %x) nounwind {
; CHECK: %[[VR1:[0-9]+]]:i64 = ARGUMENT 0, implicit $arguments
; CHECK: %[[VR0:[0-9]+]]:i64 = CONST_I64 1666
; CHECK: %[[VR2:[0-9]+]]:i64 = MUL killed %[[VR1]], killed %[[VR0]]
 %1 = mul i64 1666, %x
; CHECK: RETURN_I64 killed %[[VR2]]
 ret i64 %1
}

define i64 @div (i64 %x) nounwind {
; CHECK: %[[VR0:[0-9]+]]:i64 = ARGUMENT 0, implicit $arguments
; CHECK: %[[VR1:[0-9]+]]:i64 = CONST_I64 1666
; CHECK: %[[VR2:[0-9]+]]:i64 = DIV killed %[[VR1]], killed %[[VR0]]
 %1 = sdiv i64 1666, %x
; CHECK: RETURN_I64 killed %[[VR2]]
 ret i64 %1
}

define i64 @and (i64 %x, i64 %y) nounwind {
; CHECK-DAG: %[[VR0:[0-9]+]]:i64 = ARGUMENT 1
; CHECK-DAG: %[[VR1:[0-9]+]]:i64 = ARGUMENT 0
; CHECK: %[[VR2:[0-9]+]]:i64 = AND killed %[[VR1]], killed %[[VR0]]
 %1 = and i64 %x, %y
; CHECK: RETURN_I64 killed %[[VR2]]
 ret i64 %1
}

define i64 @or (i64 %x, i64 %y) nounwind {
; CHECK-DAG: %[[VR0:[0-9]+]]:i64 = ARGUMENT 1
; CHECK-DAG: %[[VR1:[0-9]+]]:i64 = ARGUMENT 0
; CHECK: %[[VR2:[0-9]+]]:i64 = OR killed %[[VR1]], killed %[[VR0]]
 %1 = or i64 %x, %y
; CHECK: RETURN_I64 killed %[[VR2]]
 ret i64 %1
}

define i64 @xor (i64 %x, i64 %y) nounwind {
; CHECK-DAG: %[[VR0:[0-9]+]]:i64 = ARGUMENT 1
; CHECK-DAG: %[[VR1:[0-9]+]]:i64 = ARGUMENT 0
; CHECK: %[[VR2:[0-9]+]]:i64 = XOR killed %[[VR1]], killed %[[VR0]]
 %1 = xor i64 %x, %y
; CHECK: RETURN_I64 killed %[[VR2]]
 ret i64 %1
}

define i64 @not (i64 %x) nounwind {
; CHECK: %[[VR2:[0-9]+]]:i64 = NOT killed %[[VR0]]
 %1 = xor i64 %x, -1
; CHECK: RETURN_I64 killed %[[VR2]]
 ret i64 %1
}


define i64 @mod (i64 %x, i64 %y) nounwind {
; CHECK-DAG: %[[VR0:[0-9]+]]:i64 = ARGUMENT 1
; CHECK-DAG: %[[VR1:[0-9]+]]:i64 = ARGUMENT 0
; CHECK: %[[VR2:[0-9]+]]:i64 = MOD killed %[[VR1]], killed %[[VR0]]
  %1 = srem i64 %x, %y
; CHECK: RETURN_I64 killed %[[VR2]]
  ret i64 %1
}

