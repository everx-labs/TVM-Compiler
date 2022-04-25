; RUN: llc < %s -march=tvm -filetype=asm | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; CHECK-LABEL: g
define void @g(i257 %x) {
; CHECK: PUSHCONT
; CHECK: ; {{.*}}
; CHECK-NEXT: {
; CHECK: IFJMP
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

; CHECK-LABEL: k
define dso_local void @k(i257 %x) #0 {
; CHECK: PUSHCONT ; {{.*}}
; CHECK-NEXT: ; {{.*}}
; CHECK-NEXT: {
; CHECK-NEXT: ; %bb.3: ; %if.end
; CHECK: }
; CHECK: PUSHCONT ; {{.*}}
; CHECK-NEXT: ; {{.*}}
; CHECK-NEXT: {
; CHECK-NEXT: ; %bb.2: ; %if.else
; CHECK: }
; CHECK: IFNOTJMP
entry:
  %x.addr = alloca i257, align 1
  %y = alloca i257, align 1
  store i257 %x, i257* %x.addr, align 1
  store i257 0, i257* %y, align 1
  %0 = load i257, i257* %x.addr, align 1
  %tobool = icmp ne i257 %0, 0
  br i1 %tobool, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %1 = load i257, i257* %x.addr, align 1
  %call = call i257 @f1(i257 %1)
  %2 = load i257, i257* %x.addr, align 1
  %call1 = call i257 @f1(i257 %2)
  %mul = mul nsw i257 %call, %call1
  store i257 %mul, i257* %y, align 1
  br label %if.end

if.else:                                          ; preds = %entry
  %3 = load i257, i257* %x.addr, align 1
  %call2 = call i257 @f2(i257 %3)
  %4 = load i257, i257* %x.addr, align 1
  %call3 = call i257 @f2(i257 %4)
  %add = add nsw i257 %call2, %call3
  store i257 %add, i257* %y, align 1
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %5 = load i257, i257* %y, align 1
  %call4 = call i257 @f3(i257 %5)
  ret void
}

declare dso_local i257 @f1(i257) #1
declare dso_local i257 @f2(i257) #1
declare dso_local i257 @f3(i257) #1
