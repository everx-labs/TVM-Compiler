; RUN: llc < %s -march=tvm 
; ModuleID = 'while_cond_continue.c'
source_filename = "while_cond_continue.c"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %arg) local_unnamed_addr #0 {
entry:
  %cmp1114 = icmp sgt i257 %arg, 0
  br i1 %cmp1114, label %while.body.lr.ph, label %while.end

while.body.lr.ph:                                 ; preds = %entry, %if.end
  %iterator.0.ph16 = phi i257 [ %dec, %if.end ], [ %arg, %entry ]
  %res.0.ph15 = phi i257 [ %mul, %if.end ], [ 1, %entry ]
  br label %while.body

while.body:                                       ; preds = %while.body.lr.ph, %if.then
  %iterator.012 = phi i257 [ %iterator.0.ph16, %while.body.lr.ph ], [ %div9, %if.then ]
  %div9 = udiv i257 %iterator.012, 11
  %0 = mul i257 %div9, 11
  %1 = sub i257 %iterator.012, %0
  %cmp1 = icmp eq i257 %1, 0
  br i1 %cmp1, label %if.then, label %if.end

if.then:                                          ; preds = %while.body
  %cmp = icmp sgt i257 %div9, 0
  br i1 %cmp, label %while.body, label %while.end

if.end:                                           ; preds = %while.body
  %mul = mul nsw i257 %iterator.012, %res.0.ph15
  %dec = add nsw i257 %iterator.012, -1
  %cmp11 = icmp sgt i257 %iterator.012, 1
  br i1 %cmp11, label %while.body.lr.ph, label %while.end

while.end:                                        ; preds = %if.end, %if.then, %entry
  %res.0.ph.lcssa = phi i257 [ 1, %entry ], [ %res.0.ph15, %if.then ], [ %mul, %if.end ]
  ret i257 %res.0.ph.lcssa
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
