; RUN: opt -deadargelim -S < %s | FileCheck %s
; REQUIRES: tvm-registered-target

target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define internal i257 @cond_func(i257 %a) local_unnamed_addr noinline {
entry:
  %cmp = icmp sgt i257 %a, 0
  %conv = zext i1 %cmp to i257
  ret i257 %conv
}

define internal i257 @bar(i257 %a, slice %sl, i257 %c) local_unnamed_addr noinline {
entry:
  %add = add nsw i257 %c, %a
  ret i257 %add
}

define internal i257 @baz(i257 %a, i257 %b, slice %sl) local_unnamed_addr noinline {
entry:
  %sub = sub nsw i257 %a, %b
  ret i257 %sub
}

define internal i257 @foo(i257 %a, slice %sl, i257 %c) local_unnamed_addr noinline {
entry:
  %call = tail call i257 @cond_func(i257 %a)
  %tobool = icmp eq i257 %call, 0
  br i1 %tobool, label %if.end, label %if.then

if.then:                                          ; preds = %entry
  %call1 = tail call i257 @bar(i257 %a, slice %sl, i257 %c)
  br label %return

if.end:                                           ; preds = %entry
  %call2 = tail call i257 @baz(i257 %a, i257 %c, slice %sl)
  br label %return

return:                                           ; preds = %if.end, %if.then
  %retval.0 = phi i257 [ %call1, %if.then ], [ %call2, %if.end ]
  ret i257 %retval.0
}

; CHECK-LABEL: define i257 @test(i257 %a, slice %sl, i257 %b)
define i257 @test(i257 %a, slice %sl, i257 %b) local_unnamed_addr #1 {
entry:
; CHECK: call i257 @foo(i257 %a, i257 %b)
  %call = tail call i257 @foo(i257 %a, slice %sl, i257 %b)
  ret i257 %call
}

