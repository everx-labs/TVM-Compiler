; RUN: llc < %s -march=tvm -asm-verbose=false | FileCheck %s

target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define i257 @baz() {
; CHECK-LABEL: baz
entry:
; CHECK: ZERO
  ret i257 undef
}

define void @phi() {
; CHECK-LABEL: phi
entry:
  br label %loop
loop:
  %0 = phi i257* [ undef, %entry ], [ %1, %loop ]
  %1 = getelementptr i257, i257* %0, i257 3
  br label %loop
}

; CHECK-LABEL: two1
define void @two1() {
; CHECK: PUSHINT 12345
; CHECK-NEXT: ZERO
; CHECK-NEXT: CALL
  call void @two(i257 12345, i257 undef)
  ret void
}

; CHECK-LABEL: two2
define void @two2() {
; CHECK: PUSHINT 12345
; CHECK-NEXT: PUSHINT 12345
; CHECK-NEXT: CALL
  call void @two(i257 undef, i257 12345)
  ret void
}

; CHECK-LABEL: two3
define void @two3() {
; CHECK: ZERO
; CHECK-NEXT: ZERO
; CHECK-NEXT: CALL
  call void @two(i257 undef, i257 undef)
  ret void
}
declare void @two(i257, i257)

; CHECK-LABEL: three1
define void @three1() {
; CHECK: PUSHINT 12345
; CHECK-NEXT: PUSHINT 12345
; CHECK-NEXT: ZERO
; CHECK-NEXT: CALL
  call void @three(i257 12345, i257 12345, i257 undef)
  ret void
}

; CHECK-LABEL: three2
define void @three2() {
; CHECK: PUSHINT 12345
; CHECK-NEXT: BLKPUSH 2, 0
; CHECK-NEXT: CALL
  call void @three(i257 12345, i257 undef, i257 12345)
  ret void
}

; CHECK-LABEL: three3
define void @three3() {
; CHECK: PUSHINT 12345
; CHECK-NEXT: ZERO
; CHECK-NEXT: ZERO
; CHECK-NEXT: CALL
  call void @three(i257 12345, i257 undef, i257 undef)
  ret void
}

; CHECK-LABEL: three4
define void @three4() {
; CHECK: PUSHINT 12345
; CHECK-NEXT: BLKPUSH 2, 0
; CHECK-NEXT: CALL
  call void @three(i257 undef, i257 12345, i257 12345)
  ret void
}

; CHECK-LABEL: three5
define void @three5() {
; CHECK: PUSHINT 12345
; CHECK-NEXT: PUSHINT 12345
; CHECK-NEXT: ZERO
; CHECK-NEXT: CALL
  call void @three(i257 undef, i257 12345, i257 undef)
  ret void
}

; CHECK-LABEL: three6
define void @three6() {
; CHECK: PUSHINT 12345
; CHECK-NEXT: BLKPUSH 2, 0
; CHECK-NEXT: CALL
  call void @three(i257 undef, i257 undef, i257 12345)
  ret void
}

; CHECK-LABEL: three7
define void @three7() {
; CHECK: ZERO
; CHECK-NEXT: BLKPUSH 2, 0
; CHECK-NEXT: CALL
  call void @three(i257 undef, i257 undef, i257 undef)
  ret void
}
declare void @three(i257, i257, i257)

; CHECK-LABEL: four1
define void @four1() {
; CHECK: ZERO
; CHECK-NEXT: BLKPUSH 3, 0
; CHECK-NEXT: CALL
  call void @four(i257 undef, i257 undef, i257 undef, i257 undef)
  ret void
}

; CHECK-LABEL: four2
define void @four2() {
; CHECK: PUSHINT 12345
; CHECK-NEXT: BLKPUSH 3, 0
; CHECK-NEXT: CALL
  call void @four(i257 undef, i257 12345, i257 12345, i257 12345)
  ret void
}

; CHECK-LABEL: four3
define void @four3() {
; CHECK: PUSHINT 12345
; CHECK-NEXT: BLKPUSH 2, 0
; CHECK-NEXT: ZERO
; CHECK-NEXT: CALL
  call void @four(i257 12345, i257 12345, i257 12345, i257 undef)
  ret void
}
declare void @four(i257, i257, i257, i257)

; CHECK-LABEL: sti
define builder @sti(builder %builder) {
; CHECK: ZERO
; CHECK: STIR 5
  %builder.1 = call builder @llvm.tvm.sti(i257 undef, builder %builder, i257 5)
  ret builder %builder.1
}

; CHECK-LABEL: stref
define builder @stref(builder %builder) {
; CHECK: NEWC
; CHECK: ENDC
; CHECK: SWAP
; CHECK: STREF
  %builder.1 = call builder @llvm.tvm.stref(cell undef, builder %builder)
  ret builder %builder.1
}

; CHECK-LABEL: stslice
define builder @stslice(builder %builder) {
; CHECK: PUSHSLICE x8_
; CHECK: STSLICE
  %builder.1 = call builder @llvm.tvm.stslice(slice undef, builder %builder)
  ret builder %builder.1
}

declare builder @llvm.tvm.sti(i257 %value, builder %builder, i257 %size)
declare builder @llvm.tvm.stu(i257 %value, builder %builder, i257 %size)
declare builder @llvm.tvm.stref(cell %value, builder %builder)
declare builder @llvm.tvm.stslice(slice %value, builder %builder)
