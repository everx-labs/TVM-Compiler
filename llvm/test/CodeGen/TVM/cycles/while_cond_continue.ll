; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readnone uwtable
define i64 @func(i64 %arg) nounwind {
entry:
  %cmp1114 = icmp sgt i64 %arg, 0
  br i1 %cmp1114, label %while.body.lr.ph, label %while.end

while.body.lr.ph:                                 ; preds = %entry, %if.end
  %iterator.0.ph16 = phi i64 [ %dec, %if.end ], [ %arg, %entry ]
  %res.0.ph15 = phi i64 [ %mul, %if.end ], [ 1, %entry ]
  br label %while.body

while.body:                                       ; preds = %while.body.lr.ph, %if.then
  %iterator.012 = phi i64 [ %iterator.0.ph16, %while.body.lr.ph ], [ %div9, %if.then ]
  %rem8 = urem i64 %iterator.012, 11
  %div9 = udiv i64 %iterator.012, 11
  %cmp1 = icmp eq i64 %rem8, 0
  br i1 %cmp1, label %if.then, label %if.end

if.then:                                          ; preds = %while.body
  %cmp = icmp sgt i64 %div9, 0
  br i1 %cmp, label %while.body, label %while.end

if.end:                                           ; preds = %while.body
  %mul = mul i64 %iterator.012, %res.0.ph15
  %dec = add i64 %iterator.012, -1
  %cmp11 = icmp sgt i64 %iterator.012, 1
  br i1 %cmp11, label %while.body.lr.ph, label %while.end

while.end:                                        ; preds = %if.end, %if.then, %entry
  %res.0.ph.lcssa = phi i64 [ 1, %entry ], [ %res.0.ph15, %if.then ], [ %mul, %if.end ]
  ret i64 %res.0.ph.lcssa
}
