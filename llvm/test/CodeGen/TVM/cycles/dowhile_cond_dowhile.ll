; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg1, i64 %arg2) nounwind {
entry:
  br label %do.body

do.body:                                          ; preds = %do.cond39, %entry
  %res.0 = phi i64 [ 1, %entry ], [ %res.5, %do.cond39 ]
  %iter.0 = phi i64 [ %arg1, %entry ], [ %iter.1, %do.cond39 ]
  %rem = srem i64 %iter.0, 11
  %cmp = icmp eq i64 %rem, 0
  %rem1 = srem i64 %iter.0, 7
  %cmp2 = icmp eq i64 %rem1, 0
  %or.cond = or i1 %cmp, %cmp2
  br i1 %or.cond, label %cleanup42, label %if.else4

if.else4:                                         ; preds = %do.body
  %rem5 = srem i64 %iter.0, 5
  %cmp6 = icmp eq i64 %rem5, 0
  br i1 %cmp6, label %do.cond39, label %if.else8

if.else8:                                         ; preds = %if.else4
  %rem9 = srem i64 %iter.0, 3
  %cmp10 = icmp eq i64 %rem9, 0
  br i1 %cmp10, label %cleanup42, label %do.body13

do.body13:                                        ; preds = %if.else8, %do.cond
  %res.1 = phi i64 [ %res.2, %do.cond ], [ %res.0, %if.else8 ]
  %iter1.0 = phi i64 [ %iter1.1, %do.cond ], [ %arg2, %if.else8 ]
  %rem14 = srem i64 %iter1.0, 11
  %cmp15 = icmp eq i64 %rem14, 0
  br i1 %cmp15, label %cleanup42, label %if.else17

if.else17:                                        ; preds = %do.body13
  %rem18 = srem i64 %iter1.0, 7
  %cmp19 = icmp eq i64 %rem18, 0
  br i1 %cmp19, label %if.end36, label %if.else21

if.else21:                                        ; preds = %if.else17
  %rem22 = srem i64 %iter1.0, 5
  %cmp23 = icmp eq i64 %rem22, 0
  br i1 %cmp23, label %do.cond, label %if.else25

if.else25:                                        ; preds = %if.else21
  %rem26 = srem i64 %iter1.0, 3
  %cmp27 = icmp eq i64 %rem26, 0
  br i1 %cmp27, label %cleanup42, label %if.end31

if.end31:                                         ; preds = %if.else25
  %mul = mul i64 %iter1.0, %res.1
  %dec = add i64 %iter1.0, -1
  br label %do.cond

do.cond:                                          ; preds = %if.else21, %if.end31
  %res.2 = phi i64 [ %res.1, %if.else21 ], [ %mul, %if.end31 ]
  %iter1.1 = phi i64 [ %iter1.0, %if.else21 ], [ %dec, %if.end31 ]
  %cmp32 = icmp sgt i64 %iter1.1, 0
  br i1 %cmp32, label %do.body13, label %if.end36

if.end36:                                         ; preds = %if.else17, %do.cond
  %res.4.ph = phi i64 [ %res.2, %do.cond ], [ %res.1, %if.else17 ]
  %mul37 = mul i64 %res.4.ph, %iter.0
  %dec38 = add i64 %iter.0, -1
  br label %do.cond39

do.cond39:                                        ; preds = %if.else4, %if.end36
  %res.5 = phi i64 [ %res.0, %if.else4 ], [ %mul37, %if.end36 ]
  %iter.1 = phi i64 [ %iter.0, %if.else4 ], [ %dec38, %if.end36 ]
  %cmp40 = icmp sgt i64 %iter.1, 0
  br i1 %cmp40, label %do.body, label %cleanup42

cleanup42:                                        ; preds = %if.else8, %do.cond39, %do.body, %do.body13, %if.else25
  %retval.3 = phi i64 [ %res.1, %do.body13 ], [ -1, %if.else25 ], [ -1, %if.else8 ], [ %res.5, %do.cond39 ], [ %res.0, %do.body ]
  ret i64 %retval.3
}