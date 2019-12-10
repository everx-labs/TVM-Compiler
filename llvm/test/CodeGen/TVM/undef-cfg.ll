; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s

target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

declare i257 @cfg_left(i257, i257, i257)
declare i257 @cfg_right(i257, i257, i257)
define { i257, i257 } @cfg_undefs(i257 %v0, i257 %v1, i257 %v2) {
; CHECK-LABEL: cfg_undefs
entry:
  %cond = icmp ne i257 %v0, 0
  br i1 %cond, label %true_bb, label %false_bb
true_bb:
  %true_call_v = call i257 @cfg_left(i257 %v0, i257 %v1, i257 %v2)
  br label %tail
false_bb:
  %false_call_v = call i257 @cfg_right(i257 %v0, i257 %v1, i257 %v2)
  br label %tail
tail:
  %ph0 = phi i257 [undef, %true_bb], [%false_call_v, %false_bb]
  %ph1 = phi i257 [%true_call_v, %true_bb], [undef, %false_bb]
  %ret0 = insertvalue { i257, i257 } undef, i257 %ph0, 0
  %ret1 = insertvalue { i257, i257 } %ret0, i257 %ph1, 1
  ret { i257, i257 } %ret1
}

