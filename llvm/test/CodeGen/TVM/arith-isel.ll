; RUN: llc < %s -march=tvm -stop-after expand-isel-pseudos | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define i257 @addi257(i257 %par) nounwind {
; CHECK: %[[VR1:[0-9]+]]:i257 = ARGUMENT i257 0
; CHECK: %[[VR0:[0-9]+]]:i257 = CONST_I257 i257 12345
; CHECK: %[[VR2:[0-9]+]]:i257 = ADD killed %[[VR1]], killed %[[VR0]]
  %1 = add i257 %par, 12345
; CHECK: RETURN_N killed %[[VR2]]
	ret i257 %1
}

define i257 @subi257(i257 %par, i257 %par2) nounwind {
; CHECK-DAG: %[[VR0:[0-9]+]]:i257 = ARGUMENT i257 0
; CHECK-DAG: %[[VR1:[0-9]+]]:i257 = ARGUMENT i257 1
; CHECK: %[[VR2:[0-9]+]]:i257 = SUB killed %[[VR0]], killed %[[VR1]]
  %1 = sub i257 %par, %par2
; CHECK: RETURN_N killed %[[VR2]]
  ret i257 %1
}

;TODO complete the test, should be permutation optimization
define i257 @subri257(i257 %x, i257 %y) nounwind {
 %1 = sub i257  0, %x
 %2 = add i257 %1, %y
 ret i257 %2
}

define i257 @negate(i257 %x) nounwind {
; CHECK: %[[VR0:[0-9]+]]:i257 = ARGUMENT i257 0, implicit $arguments
; CHECK: %[[VR1:[0-9]+]]:i257 = NEGATE killed %[[VR0]]
 %1 = sub i257 0, %x
; CHECK: RETURN_N killed %[[VR1]]
 ret i257 %1
}

define i257 @increment(i257 %x) nounwind {
; CHECK: %[[VR0:[0-9]+]]:i257 = ARGUMENT i257 0, implicit $arguments
; CHECK: %[[VR1:[0-9]+]]:i257 = INC killed %[[VR0]]
 %1 = add i257 1, %x
; CHECK: RETURN_N killed %[[VR1]]
 ret i257 %1
}

define i257 @decrement(i257 %x) nounwind {
; CHECK: %[[VR0:[0-9]+]]:i257 = ARGUMENT i257 0, implicit $arguments
; CHECK: %[[VR1:[0-9]+]]:i257 = DEC killed %[[VR0]]
 %1 = add i257 -1, %x
; CHECK: RETURN_N killed %[[VR1]]
 ret i257 %1
}

define i257 @mul (i257 %x) nounwind {
; CHECK: %[[VR1:[0-9]+]]:i257 = ARGUMENT i257 0, implicit $arguments
; CHECK: %[[VR0:[0-9]+]]:i257 = CONST_I257 i257 1666
; CHECK: %[[VR2:[0-9]+]]:i257 = MUL killed %[[VR1]], killed %[[VR0]]
 %1 = mul i257 1666, %x
; CHECK: RETURN_N killed %[[VR2]]
 ret i257 %1
}

define i257 @div (i257 %x) nounwind {
; CHECK: %[[VR0:[0-9]+]]:i257 = ARGUMENT i257 0, implicit $arguments
; CHECK: %[[VR1:[0-9]+]]:i257 = CONST_I257 i257 1666
; CHECK: %[[VR2:[0-9]+]]:i257 = DIV killed %[[VR1]], killed %[[VR0]]
 %1 = call i257 @llvm.tvm.div(i257 1666, i257 %x)
; CHECK: RETURN_N killed %[[VR2]]
 ret i257 %1
}

define i257 @and (i257 %x, i257 %y) nounwind {
; CHECK-DAG: %[[VR0:[0-9]+]]:i257 = ARGUMENT i257 1
; CHECK-DAG: %[[VR1:[0-9]+]]:i257 = ARGUMENT i257 0
; CHECK: %[[VR2:[0-9]+]]:i257 = AND killed %[[VR1]], killed %[[VR0]]
 %1 = and i257 %x, %y
; CHECK: RETURN_N killed %[[VR2]]
 ret i257 %1
}

define i257 @or (i257 %x, i257 %y) nounwind {
; CHECK-DAG: %[[VR0:[0-9]+]]:i257 = ARGUMENT i257 1
; CHECK-DAG: %[[VR1:[0-9]+]]:i257 = ARGUMENT i257 0
; CHECK: %[[VR2:[0-9]+]]:i257 = OR killed %[[VR1]], killed %[[VR0]]
 %1 = or i257 %x, %y
; CHECK: RETURN_N killed %[[VR2]]
 ret i257 %1
}

define i257 @xor (i257 %x, i257 %y) nounwind {
; CHECK-DAG: %[[VR0:[0-9]+]]:i257 = ARGUMENT i257 1
; CHECK-DAG: %[[VR1:[0-9]+]]:i257 = ARGUMENT i257 0
; CHECK: %[[VR2:[0-9]+]]:i257 = XOR killed %[[VR1]], killed %[[VR0]]
 %1 = xor i257 %x, %y
; CHECK: RETURN_N killed %[[VR2]]
 ret i257 %1
}

define i257 @not (i257 %x) nounwind {
; CHECK: %[[VR2:[0-9]+]]:i257 = NOT killed %[[VR0]]
 %1 = xor i257 %x, -1
; CHECK: RETURN_N killed %[[VR2]]
 ret i257 %1
}


define i257 @mod (i257 %x, i257 %y) nounwind {
; CHECK-DAG: %[[VR0:[0-9]+]]:i257 = ARGUMENT i257 1
; CHECK-DAG: %[[VR1:[0-9]+]]:i257 = ARGUMENT i257 0
; CHECK: %[[VR2:[0-9]+]]:i257 = MOD killed %[[VR1]], killed %[[VR0]]
  %1 = call i257 @llvm.tvm.mod(i257 %x, i257 %y)
; CHECK: RETURN_N killed %[[VR2]]
  ret i257 %1
}

define i257 @addconst(i257 %x) nounwind {
; CHECK: %[[VR0:[0-9]+]]:i257 = ARGUMENT i257 0
; CHECK: %[[VR1:[0-9]+]]:i257 = ADDCONST killed %[[VR0]], i257 12
  %1 = add i257 %x, 12
; CHECK: RETURN_N killed %[[VR1]]
  ret i257 %1
}

define i257 @mulconst(i257 %x) nounwind {
; CHECK: %[[VR0:[0-9]+]]:i257 = ARGUMENT i257 0
; CHECK: %[[VR1:[0-9]+]]:i257 = MULCONST killed %[[VR0]], i257 127
  %1 = mul i257 %x, 127
; CHECK: RETURN_N killed %[[VR1]]
  ret i257 %1
}

declare i257 @llvm.tvm.div(i257, i257) nounwind
declare i257 @llvm.tvm.mod(i257, i257) nounwind
