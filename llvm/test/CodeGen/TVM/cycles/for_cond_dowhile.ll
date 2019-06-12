; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg1, i64 %arg2) nounwind {
entry:
  %cmp75 = icmp slt i64 %arg1, 1
  %rem6876 = urem i64 %arg1, 11
  %cmp177 = icmp eq i64 %rem6876, 0
  %or.cond78 = or i1 %cmp75, %cmp177
  %rem26979 = urem i64 %arg1, 7
  %cmp380 = icmp eq i64 %rem26979, 0
  %or.cond7281 = or i1 %cmp380, %or.cond78
  br i1 %or.cond7281, label %cleanup41, label %if.else5

if.else5:                                         ; preds = %entry, %for.inc
  %iter.083 = phi i64 [ %dec38, %for.inc ], [ %arg1, %entry ]
  %res.082 = phi i64 [ %res.5, %for.inc ], [ 1, %entry ]
  %rem670 = urem i64 %iter.083, 5
  %cmp7 = icmp eq i64 %rem670, 0
  br i1 %cmp7, label %for.inc, label %if.else9

if.else9:                                         ; preds = %if.else5
  %rem1071 = urem i64 %iter.083, 3
  %cmp11 = icmp eq i64 %rem1071, 0
  br i1 %cmp11, label %cleanup41, label %do.body

do.body:                                          ; preds = %if.else9, %do.cond
  %res.1 = phi i64 [ %res.2, %do.cond ], [ %res.082, %if.else9 ]
  %iter1.0 = phi i64 [ %iter1.1, %do.cond ], [ %arg2, %if.else9 ]
  %rem14 = srem i64 %iter1.0, 11
  %cmp15 = icmp eq i64 %rem14, 0
  br i1 %cmp15, label %cleanup41, label %if.else17

if.else17:                                        ; preds = %do.body
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
  br i1 %cmp27, label %cleanup41, label %if.end31

if.end31:                                         ; preds = %if.else25
  %mul = mul i64 %iter1.0, %res.1
  %dec = add i64 %iter1.0, -1
  br label %do.cond

do.cond:                                          ; preds = %if.else21, %if.end31
  %res.2 = phi i64 [ %res.1, %if.else21 ], [ %mul, %if.end31 ]
  %iter1.1 = phi i64 [ %iter1.0, %if.else21 ], [ %dec, %if.end31 ]
  %cmp32 = icmp sgt i64 %iter1.1, 0
  br i1 %cmp32, label %do.body, label %if.end36

if.end36:                                         ; preds = %if.else17, %do.cond
  %res.4.ph = phi i64 [ %res.2, %do.cond ], [ %res.1, %if.else17 ]
  %mul37 = mul i64 %res.4.ph, %iter.083
  br label %for.inc

for.inc:                                          ; preds = %if.else5, %if.end36
  %res.5 = phi i64 [ %res.082, %if.else5 ], [ %mul37, %if.end36 ]
  %dec38 = add i64 %iter.083, -1
  %cmp = icmp slt i64 %iter.083, 2
  %rem68 = urem i64 %dec38, 11
  %cmp1 = icmp eq i64 %rem68, 0
  %or.cond = or i1 %cmp, %cmp1
  %rem269 = urem i64 %dec38, 7
  %cmp3 = icmp eq i64 %rem269, 0
  %or.cond72 = or i1 %cmp3, %or.cond
  br i1 %or.cond72, label %cleanup41, label %if.else5

cleanup41:                                        ; preds = %if.else9, %for.inc, %do.body, %if.else25, %entry
  %retval.4 = phi i64 [ 1, %entry ], [ %res.1, %do.body ], [ -1, %if.else25 ], [ -1, %if.else9 ], [ %res.5, %for.inc ]
  ret i64 %retval.4
}