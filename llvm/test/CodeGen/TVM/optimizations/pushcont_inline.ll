; RUN: llc < %s -march=tvm -filetype=asm | FileCheck %s
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: g
define void @g(i257 %x) {
; CHECK: PUSHCONT
; CHECK: {
; CHECK: }
; CHECK: IFJMP
; CHECK: JMPX
  %x.addr = alloca i257, align 1
  store i257 %x, i257* %x.addr, align 1
  %tobool = icmp ne i257 %x, 0
  br i1 %tobool, label %if.then, label %if.end

if.then:
  br label %return

if.end:
  %x.restored = load i257, i257* %x.addr, align 1
  %call = call i257 @f(i257 %x.restored)
  br label %return

return:
  ret void
}

declare dso_local i257 @f(i257) #1
