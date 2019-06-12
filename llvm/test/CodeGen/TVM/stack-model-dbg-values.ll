; RUN: llc < %s -march=tvm -filetype=asm | FileCheck %s

source_filename = "ifelse.c"
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: nounwind readnone
define dso_local i64 @test_ifelse(i64 %a, i64 %b, i64 %c) local_unnamed_addr #0 !dbg !7 {
entry:
    ; CHECK: XCHG	s0, s2                  ; { %0(a) | %1(b) | %3(c) }
    ; CHECK: ADD                            ; %2 = ADD %1(b), %0(a)
    ; CHECK-NEXT:                           ; { %2 | %3(c) }
    ; CHECK: ADD                            ; %4(sum) = ADD %2, %3(c)
    ; CHECK-NEXT:                           ; { %4(sum) }
    ; CHECK: PUSHINT	1000                ; %11 = CONST_I64 1000
    ; CHECK-NEXT:                           ; { %11 | %4(sum) }
    ; CHECK: PUSH	s1                      ; { %4(sum) | %11 | %4(sum) }
    ; CHECK: XCHG	s0, s1                  ; { %11 | %4(sum) | %4(sum) }
    ; CHECK: GREATER                        ; %6 = SGT %4(sum), %11
    ; CHECK-NEXT:                           ; { %6 | %4(sum) }
    ; CHECK: PUSHINT	100                 ; %12 = CONST_I64 100
    ; CHECK-NEXT:                           ; { %12 | %6 | %4(sum) }
    ; CHECK: PUSHINT	-200                ; %13 = CONST_I64 -200
    ; CHECK-NEXT:                           ; { %13 | %12 | %6 | %4(sum) }
    ; CHECK: XCHG	s0, s2                  ; { %6 | %12 | %13 | %4(sum) }
    ; CHECK: XCHG	s1, s2                  ; { %6 | %13 | %12 | %4(sum) }
    ; CHECK: CONDSEL                        ; %9 = CONDSEL %12, %13, %6
    ; CHECK-NEXT:                           ; { %9 | %4(sum) }
    ; CHECK: ADD                            ; %10 = ADD %9, %4(sum)
    ; CHECK-NEXT:                           ; { %10 }

  call void @llvm.dbg.value(metadata i64 %a, metadata !13, metadata !DIExpression()), !dbg !17
  call void @llvm.dbg.value(metadata i64 %b, metadata !14, metadata !DIExpression()), !dbg !18
  call void @llvm.dbg.value(metadata i64 %c, metadata !15, metadata !DIExpression()), !dbg !19
  %add = add nsw i64 %b, %a, !dbg !20
  %add1 = add nsw i64 %add, %c, !dbg !21
  call void @llvm.dbg.value(metadata i64 %add1, metadata !16, metadata !DIExpression()), !dbg !22
  %cmp = icmp sgt i64 %add1, 1000, !dbg !23
  %retval.0.v = select i1 %cmp, i64 100, i64 -200, !dbg !25
  %retval.0 = add i64 %retval.0.v, %add1, !dbg !25
  ret i64 %retval.0, !dbg !26
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.value(metadata, metadata, metadata) #1

attributes #0 = { nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 7.0.0 (tags/RELEASE_700/final)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "ifelse.c", directory: "")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"clang version 7.0.0 (tags/RELEASE_700/final)"}
!7 = distinct !DISubprogram(name: "test_ifelse", scope: !1, file: !1, line: 3, type: !8, isLocal: false, isDefinition: true, scopeLine: 3, flags: DIFlagPrototyped, isOptimized: true, unit: !0, retainedNodes: !12)
!8 = !DISubroutineType(types: !9)
!9 = !{!10, !10, !10, !10}
!10 = !DIDerivedType(tag: DW_TAG_typedef, name: "i64", file: !1, line: 1, baseType: !11)
!11 = !DIBasicType(name: "long long int", size: 64, encoding: DW_ATE_signed)
!12 = !{!13, !14, !15, !16}
!13 = !DILocalVariable(name: "a", arg: 1, scope: !7, file: !1, line: 3, type: !10)
!14 = !DILocalVariable(name: "b", arg: 2, scope: !7, file: !1, line: 3, type: !10)
!15 = !DILocalVariable(name: "c", arg: 3, scope: !7, file: !1, line: 3, type: !10)
!16 = !DILocalVariable(name: "sum", scope: !7, file: !1, line: 4, type: !10)
!17 = !DILocation(line: 3, column: 22, scope: !7)
!18 = !DILocation(line: 3, column: 29, scope: !7)
!19 = !DILocation(line: 3, column: 36, scope: !7)
!20 = !DILocation(line: 4, column: 14, scope: !7)
!21 = !DILocation(line: 4, column: 18, scope: !7)
!22 = !DILocation(line: 4, column: 6, scope: !7)
!23 = !DILocation(line: 5, column: 13, scope: !24)
!24 = distinct !DILexicalBlock(scope: !7, file: !1, line: 5, column: 9)
!25 = !DILocation(line: 5, column: 9, scope: !7)
!26 = !DILocation(line: 9, column: 1, scope: !7)
