; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

; Function Attrs: readnone
define dso_local i257 @_Z12get_msg_kindTVMs(slice %sl) local_unnamed_addr #0 {
entry:
; CHECK-LABEL: _Z12get_msg_kindTVMs
; CHECK: LDMSGADDR
; CHECK: PARSEMSGADDR

  %0 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %sl)
  %.fca.0.extract = extractvalue { slice, slice } %0, 0
  %1 = tail call tuple @llvm.tvm.parsemsgaddr(slice %.fca.0.extract)
  %2 = tail call i257 @llvm.tvm.index(tuple %1, i257 0)
  ret i257 %2
}

; Function Attrs: readnone
declare { slice, slice } @llvm.tvm.ldmsgaddr(slice) #1

; Function Attrs: readnone
declare tuple @llvm.tvm.parsemsgaddr(slice) #1

; Function Attrs: nounwind readnone
declare i257 @llvm.tvm.index(tuple, i257) #2

; Function Attrs: nounwind readnone
define dso_local i257 @_Z18get_msg_kind_quietTVMs(slice %sl) local_unnamed_addr #3 {
entry:
; CHECK-LABEL: _Z18get_msg_kind_quietTVMs
; CHECK: LDMSGADDRQ
; CHECK: PARSEMSGADDRQ

  %0 = tail call { slice, slice, i257 } @llvm.tvm.ldmsgaddrq(slice %sl)
  %.fca.2.extract = extractvalue { slice, slice, i257 } %0, 2
  %tobool = icmp eq i257 %.fca.2.extract, 0
  br i1 %tobool, label %cleanup5, label %if.end

if.end:                                           ; preds = %entry
  %.fca.0.extract7 = extractvalue { slice, slice, i257 } %0, 0
  %1 = tail call { tuple, i257 } @llvm.tvm.parsemsgaddrq(slice %.fca.0.extract7)
  %.fca.1.extract = extractvalue { tuple, i257 } %1, 1
  %tobool1 = icmp eq i257 %.fca.1.extract, 0
  br i1 %tobool1, label %cleanup5, label %if.end3

if.end3:                                          ; preds = %if.end
  %.fca.0.extract = extractvalue { tuple, i257 } %1, 0
  %2 = tail call i257 @llvm.tvm.index(tuple %.fca.0.extract, i257 0)
  br label %cleanup5

cleanup5:                                         ; preds = %if.end3, %if.end, %entry
  %retval.1 = phi i257 [ -1, %entry ], [ %2, %if.end3 ], [ -1, %if.end ]
  ret i257 %retval.1
}

; Function Attrs: nounwind readnone
declare { slice, slice, i257 } @llvm.tvm.ldmsgaddrq(slice) #2

; Function Attrs: nounwind readnone
declare { tuple, i257 } @llvm.tvm.parsemsgaddrq(slice) #2

; Function Attrs: readnone
define dso_local { i257, i257 } @_Z14rewritestdaddrTVMs(slice %sl) local_unnamed_addr #0 {
entry:
; CHECK-LABEL: _Z14rewritestdaddrTVMs
; CHECK: REWRITESTDADDR

  %0 = tail call { i257, i257 } @llvm.tvm.rewritestdaddr(slice %sl)
  ret { i257, i257 } %0
}

; Function Attrs: readnone
declare { i257, i257 } @llvm.tvm.rewritestdaddr(slice) #1

; Function Attrs: nounwind readnone
define dso_local { i257, { i257, i257 } } @_Z15rewritestdaddrqTVMs(slice %sl) local_unnamed_addr #3 {
entry:
; CHECK-LABEL: _Z15rewritestdaddrqTVMs
; CHECK: REWRITESTDADDRQ

  %0 = tail call { i257, i257, i257 } @llvm.tvm.rewritestdaddrq(slice %sl)
  %.fca.2.extract = extractvalue { i257, i257, i257 } %0, 2
  %tobool = icmp ne i257 %.fca.2.extract, 0
  %.fca.1.extract = extractvalue { i257, i257, i257 } %0, 1
  %.fca.0.extract = extractvalue { i257, i257, i257 } %0, 0
  %retval.sroa.0.0 = zext i1 %tobool to i257
  %.fca.0.insert = insertvalue { i257, { i257, i257 } } undef, i257 %retval.sroa.0.0, 0
  %.fca.1.0.insert = insertvalue { i257, { i257, i257 } } %.fca.0.insert, i257 %.fca.0.extract, 1, 0
  %.fca.1.1.insert = insertvalue { i257, { i257, i257 } } %.fca.1.0.insert, i257 %.fca.1.extract, 1, 1
  ret { i257, { i257, i257 } } %.fca.1.1.insert
}

; Function Attrs: nounwind readnone
declare { i257, i257, i257 } @llvm.tvm.rewritestdaddrq(slice) #2

; Function Attrs: readnone
define dso_local { i257, slice } @_Z14rewritevaraddrTVMs(slice %sl) local_unnamed_addr #0 {
entry:
; CHECK-LABEL: _Z14rewritevaraddrTVMs
; CHECK: REWRITEVARADDR

  %0 = tail call { i257, slice } @llvm.tvm.rewritevaraddr(slice %sl)
  ret { i257, slice } %0
}

; Function Attrs: readnone
declare { i257, slice } @llvm.tvm.rewritevaraddr(slice) #1

; Function Attrs: nounwind readnone
define dso_local { i257, { i257, slice } } @_Z15rewritevaraddrqTVMs(slice %sl) local_unnamed_addr #3 {
entry:
; CHECK-LABEL: _Z15rewritevaraddrqTVMs
; CHECK: REWRITEVARADDRQ

  %0 = tail call { i257, slice, i257 } @llvm.tvm.rewritevaraddrq(slice %sl)
  %.fca.2.extract = extractvalue { i257, slice, i257 } %0, 2
  %tobool = icmp ne i257 %.fca.2.extract, 0
  %.fca.1.extract = extractvalue { i257, slice, i257 } %0, 1
  %.fca.0.extract = extractvalue { i257, slice, i257 } %0, 0
  %retval.sroa.0.0 = zext i1 %tobool to i257
  %.fca.0.insert = insertvalue { i257, { i257, slice } } undef, i257 %retval.sroa.0.0, 0
  %.fca.1.0.insert = insertvalue { i257, { i257, slice } } %.fca.0.insert, i257 %.fca.0.extract, 1, 0
  %.fca.1.1.insert = insertvalue { i257, { i257, slice } } %.fca.1.0.insert, slice %.fca.1.extract, 1, 1
  ret { i257, { i257, slice } } %.fca.1.1.insert
}

; Function Attrs: nounwind readnone
declare { i257, slice, i257 } @llvm.tvm.rewritevaraddrq(slice) #2

attributes #0 = { readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { readnone }
attributes #2 = { nounwind readnone }
attributes #3 = { nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"clang version 7.0.0 "}
