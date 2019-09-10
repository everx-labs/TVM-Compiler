; RUN: llc < %s -march=tvm -filetype=asm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; CHECK-LABEL: g
define void @g(i64 %x) {
; CHECK: PUSHCONT ; >%5 =
; CHECK-NEXT: ; {{.*}}
; CHECK-NEXT: {
; CHECK-NEXT: ; %bb.1: ; %if.then
; CHECK-NEXT: ; {{.*}}
; CHECK-NEXT:   JMPX
; CHECK-NEXT: }
  %x.addr = alloca i64, align 8
  store i64 %x, i64* %x.addr, align 8
  %tobool = icmp ne i64 %x, 0
  br i1 %tobool, label %if.then, label %if.end

if.then:
  br label %return

if.end:
  %x.restored = load i64, i64* %x.addr, align 8
  %call = call i64 @f(i64 %x.restored)
  br label %return

return:
  ret void
}

declare dso_local i64 @f(i64) #1
