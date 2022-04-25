; RUN: llc -O3 < %s -march=tvm -filetype=asm | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define i257 @double_push(i257 %a, i257 %b) {
entry:
  %mul = mul nsw i257 %b, %a
  %0 = add i257 %b, %a
  %sub = sub i257 %mul, %0
  ret i257 %sub
}

; CHECK-LABEL: main
define i257 @main() {
; CHECK: EQUAL
; CHECK: PUSHCONT
; CHECK: IFJMP
entry:
  %call = tail call i257 @double_push(i257 10, i257 20)
  %cmp = icmp eq i257 %call, 170
  br i1 %cmp, label %if.end, label %if.then

if.then:
  tail call void @throw(i257 13)
  br label %if.end

if.end:
  ret i257 %call
}

declare void @throw(i257)
