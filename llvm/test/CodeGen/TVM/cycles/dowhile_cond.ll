; RUN: llc < %s -march=tvm 
; ModuleID = 'dowhile_cond.c'
source_filename = "dowhile_cond.c"
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone
define dso_local i257 @func(i257 %arg) local_unnamed_addr #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %do.cond, %entry
  %res.0 = phi i257 [ %arg, %entry ], [ %res.1, %do.cond ]
  %iterator.0 = phi i257 [ %arg, %entry ], [ %dec, %do.cond ]
  %dec = add nsw i257 %iterator.0, -1
  %rem = srem i257 %dec, 11
  %cmp = icmp eq i257 %rem, 0
  %rem1 = srem i257 %dec, 7
  %cmp2 = icmp eq i257 %rem1, 0
  %or.cond = or i1 %cmp, %cmp2
  br i1 %or.cond, label %cleanup, label %if.else4

if.else4:                                         ; preds = %do.body
  %rem5 = srem i257 %dec, 5
  %cmp6 = icmp eq i257 %rem5, 0
  br i1 %cmp6, label %do.cond, label %if.else8

if.else8:                                         ; preds = %if.else4
  %rem9 = srem i257 %dec, 3
  %cmp10 = icmp eq i257 %rem9, 0
  br i1 %cmp10, label %cleanup, label %if.end14

if.end14:                                         ; preds = %if.else8
  %mul = mul nsw i257 %dec, %res.0
  br label %do.cond

do.cond:                                          ; preds = %if.else4, %if.end14
  %res.1 = phi i257 [ %res.0, %if.else4 ], [ %mul, %if.end14 ]
  %cmp15 = icmp sgt i257 %iterator.0, 1
  br i1 %cmp15, label %do.body, label %cleanup

cleanup:                                          ; preds = %if.else8, %do.cond, %do.body
  %retval.0 = phi i257 [ %res.0, %do.body ], [ %res.1, %do.cond ], [ -1, %if.else8 ]
  ret i257 %retval.0
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
