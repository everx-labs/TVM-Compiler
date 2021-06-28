	.text
	.file	"test_cell.cpp"
	.globl	_Z11endcst_testTVMbTVMb
	.p2align	1
	.type	_Z11endcst_testTVMbTVMb,@function
_Z11endcst_testTVMbTVMb:
; %bb.0:
	ENDCST
.Lfunc_end0:
	.size	_Z11endcst_testTVMbTVMb, .Lfunc_end0-_Z11endcst_testTVMbTVMb

	.globl	_Z9stir_testTVMbi
	.p2align	1
	.type	_Z9stir_testTVMbi,@function
_Z9stir_testTVMbi:
; %bb.0:
	STIR	7
.Lfunc_end1:
	.size	_Z9stir_testTVMbi, .Lfunc_end1-_Z9stir_testTVMbi

	.globl	_Z9stur_testTVMbi
	.p2align	1
	.type	_Z9stur_testTVMbi,@function
_Z9stur_testTVMbi:
; %bb.0:
	STUR	8
.Lfunc_end2:
	.size	_Z9stur_testTVMbi, .Lfunc_end2-_Z9stur_testTVMbi

	.globl	_Z11stbref_testTVMbTVMb
	.p2align	1
	.type	_Z11stbref_testTVMbTVMb,@function
_Z11stbref_testTVMbTVMb:
; %bb.0:
	STBREF
.Lfunc_end3:
	.size	_Z11stbref_testTVMbTVMb, .Lfunc_end3-_Z11stbref_testTVMbTVMb

	.globl	_Z8stb_testTVMbTVMb
	.p2align	1
	.type	_Z8stb_testTVMbTVMb,@function
_Z8stb_testTVMbTVMb:
; %bb.0:
	STB
.Lfunc_end4:
	.size	_Z8stb_testTVMbTVMb, .Lfunc_end4-_Z8stb_testTVMbTVMb

	.globl	_Z11strefr_testTVMbTVMc
	.p2align	1
	.type	_Z11strefr_testTVMbTVMc,@function
_Z11strefr_testTVMbTVMc:
; %bb.0:
	STREFR
.Lfunc_end5:
	.size	_Z11strefr_testTVMbTVMc, .Lfunc_end5-_Z11strefr_testTVMbTVMc

	.globl	_Z13stslicer_testTVMbTVMs
	.p2align	1
	.type	_Z13stslicer_testTVMbTVMs,@function
_Z13stslicer_testTVMbTVMs:
; %bb.0:
	STSLICER
.Lfunc_end6:
	.size	_Z13stslicer_testTVMbTVMs, .Lfunc_end6-_Z13stslicer_testTVMbTVMs

	.globl	_Z9stbr_testTVMbTVMb
	.p2align	1
	.type	_Z9stbr_testTVMbTVMb,@function
_Z9stbr_testTVMbTVMb:
; %bb.0:
	STBR
.Lfunc_end7:
	.size	_Z9stbr_testTVMbTVMb, .Lfunc_end7-_Z9stbr_testTVMbTVMb

	.globl	_Z11strefq_testTVMcTVMb
	.p2align	1
	.type	_Z11strefq_testTVMcTVMb,@function
_Z11strefq_testTVMcTVMb:
; %bb.0:
	STREFQ
	XCHG	s1, s2
	DROP2
.Lfunc_end8:
	.size	_Z11strefq_testTVMcTVMb, .Lfunc_end8-_Z11strefq_testTVMcTVMb

	.globl	_Z12stbrefq_testTVMbTVMb
	.p2align	1
	.type	_Z12stbrefq_testTVMbTVMb,@function
_Z12stbrefq_testTVMbTVMb:
; %bb.0:
	STBREFQ
	XCHG	s1, s2
	DROP2
.Lfunc_end9:
	.size	_Z12stbrefq_testTVMbTVMb, .Lfunc_end9-_Z12stbrefq_testTVMbTVMb

	.globl	_Z13stsliceq_testTVMsTVMb
	.p2align	1
	.type	_Z13stsliceq_testTVMsTVMb,@function
_Z13stsliceq_testTVMsTVMb:
; %bb.0:
	STSLICEQ
	XCHG	s1, s2
	DROP2
.Lfunc_end10:
	.size	_Z13stsliceq_testTVMsTVMb, .Lfunc_end10-_Z13stsliceq_testTVMsTVMb

	.globl	_Z9stbq_testTVMbTVMb
	.p2align	1
	.type	_Z9stbq_testTVMbTVMb,@function
_Z9stbq_testTVMbTVMb:
; %bb.0:
	STBQ
	XCHG	s1, s2
	DROP2
.Lfunc_end11:
	.size	_Z9stbq_testTVMbTVMb, .Lfunc_end11-_Z9stbq_testTVMbTVMb

	.globl	_Z13stbrefrq_testTVMbTVMb
	.p2align	1
	.type	_Z13stbrefrq_testTVMbTVMb,@function
_Z13stbrefrq_testTVMbTVMb:
; %bb.0:
	STBREFRQ
	XCHG	s1, s2
	DROP2
.Lfunc_end12:
	.size	_Z13stbrefrq_testTVMbTVMb, .Lfunc_end12-_Z13stbrefrq_testTVMbTVMb

	.globl	_Z14stslicerq_testTVMbTVMs
	.p2align	1
	.type	_Z14stslicerq_testTVMbTVMs,@function
_Z14stslicerq_testTVMbTVMs:
; %bb.0:
	STSLICERQ
	DROP2
.Lfunc_end13:
	.size	_Z14stslicerq_testTVMbTVMs, .Lfunc_end13-_Z14stslicerq_testTVMbTVMs

	.globl	_Z10stbrq_testTVMbTVMb
	.p2align	1
	.type	_Z10stbrq_testTVMbTVMb,@function
_Z10stbrq_testTVMbTVMb:
; %bb.0:
	SWAP
	STBRQ
	XCHG	s1, s2
	DROP2
.Lfunc_end14:
	.size	_Z10stbrq_testTVMbTVMb, .Lfunc_end14-_Z10stbrq_testTVMbTVMb

	.globl	_Z10endxc_testTVMbi
	.p2align	1
	.type	_Z10endxc_testTVMbi,@function
_Z10endxc_testTVMbi:
; %bb.0:
	ENDXC
.Lfunc_end15:
	.size	_Z10endxc_testTVMbi, .Lfunc_end15-_Z10endxc_testTVMbi

	.globl	_Z11stile4_testiTVMb
	.p2align	1
	.type	_Z11stile4_testiTVMb,@function
_Z11stile4_testiTVMb:
; %bb.0:
	STILE4
.Lfunc_end16:
	.size	_Z11stile4_testiTVMb, .Lfunc_end16-_Z11stile4_testiTVMb

	.globl	_Z11stule4_testiTVMb
	.p2align	1
	.type	_Z11stule4_testiTVMb,@function
_Z11stule4_testiTVMb:
; %bb.0:
	STULE4
.Lfunc_end17:
	.size	_Z11stule4_testiTVMb, .Lfunc_end17-_Z11stule4_testiTVMb

	.globl	_Z11stile8_testiTVMb
	.p2align	1
	.type	_Z11stile8_testiTVMb,@function
_Z11stile8_testiTVMb:
; %bb.0:
	STILE8
.Lfunc_end18:
	.size	_Z11stile8_testiTVMb, .Lfunc_end18-_Z11stile8_testiTVMb

	.globl	_Z11stule8_testiTVMb
	.p2align	1
	.type	_Z11stule8_testiTVMb,@function
_Z11stule8_testiTVMb:
; %bb.0:
	STULE8
.Lfunc_end19:
	.size	_Z11stule8_testiTVMb, .Lfunc_end19-_Z11stule8_testiTVMb

	.globl	_Z11bdepth_testTVMb
	.p2align	1
	.type	_Z11bdepth_testTVMb,@function
_Z11bdepth_testTVMb:
; %bb.0:
	BDEPTH
.Lfunc_end20:
	.size	_Z11bdepth_testTVMb, .Lfunc_end20-_Z11bdepth_testTVMb

	.globl	_Z13bbitrefs_testTVMb
	.p2align	1
	.type	_Z13bbitrefs_testTVMb,@function
_Z13bbitrefs_testTVMb:
; %bb.0:
	BBITREFS
	ADD
.Lfunc_end21:
	.size	_Z13bbitrefs_testTVMb, .Lfunc_end21-_Z13bbitrefs_testTVMb

	.globl	_Z16brembitrefs_testTVMb
	.p2align	1
	.type	_Z16brembitrefs_testTVMb,@function
_Z16brembitrefs_testTVMb:
; %bb.0:
	BREMBITREFS
	ADD
.Lfunc_end22:
	.size	_Z16brembitrefs_testTVMb, .Lfunc_end22-_Z16brembitrefs_testTVMb

	.globl	_Z13bchkbits_testTVMbi
	.p2align	1
	.type	_Z13bchkbits_testTVMbi,@function
_Z13bchkbits_testTVMbi:
; %bb.0:
	BCHKBITS
.Lfunc_end23:
	.size	_Z13bchkbits_testTVMbi, .Lfunc_end23-_Z13bchkbits_testTVMbi

	.globl	_Z13bchkrefs_testTVMbi
	.p2align	1
	.type	_Z13bchkrefs_testTVMbi,@function
_Z13bchkrefs_testTVMbi:
; %bb.0:
	BCHKREFS
.Lfunc_end24:
	.size	_Z13bchkrefs_testTVMbi, .Lfunc_end24-_Z13bchkrefs_testTVMbi

	.globl	_Z16bchkbitrefs_testTVMbii
	.p2align	1
	.type	_Z16bchkbitrefs_testTVMbii,@function
_Z16bchkbitrefs_testTVMbii:
; %bb.0:
	BCHKBITREFS
.Lfunc_end25:
	.size	_Z16bchkbitrefs_testTVMbii, .Lfunc_end25-_Z16bchkbitrefs_testTVMbii

	.globl	_Z14bchkbitsq_testTVMbi
	.p2align	1
	.type	_Z14bchkbitsq_testTVMbi,@function
_Z14bchkbitsq_testTVMbi:
; %bb.0:
	BCHKBITSQ
.Lfunc_end26:
	.size	_Z14bchkbitsq_testTVMbi, .Lfunc_end26-_Z14bchkbitsq_testTVMbi

	.globl	_Z14bchkrefsq_testTVMbi
	.p2align	1
	.type	_Z14bchkrefsq_testTVMbi,@function
_Z14bchkrefsq_testTVMbi:
; %bb.0:
	BCHKREFSQ
.Lfunc_end27:
	.size	_Z14bchkrefsq_testTVMbi, .Lfunc_end27-_Z14bchkrefsq_testTVMbi

	.globl	_Z17bchkbitrefsq_testTVMbii
	.p2align	1
	.type	_Z17bchkbitrefsq_testTVMbii,@function
_Z17bchkbitrefsq_testTVMbii:
; %bb.0:
	BCHKBITREFSQ
.Lfunc_end28:
	.size	_Z17bchkbitrefsq_testTVMbii, .Lfunc_end28-_Z17bchkbitrefsq_testTVMbii

	.globl	_Z13stzeroes_testTVMbi
	.p2align	1
	.type	_Z13stzeroes_testTVMbi,@function
_Z13stzeroes_testTVMbi:
; %bb.0:
	STZEROES
.Lfunc_end29:
	.size	_Z13stzeroes_testTVMbi, .Lfunc_end29-_Z13stzeroes_testTVMbi

	.globl	_Z11stones_testTVMbi
	.p2align	1
	.type	_Z11stones_testTVMbi,@function
_Z11stones_testTVMbi:
; %bb.0:
	STONES
.Lfunc_end30:
	.size	_Z11stones_testTVMbi, .Lfunc_end30-_Z11stones_testTVMbi

	.globl	_Z11stsame_testTVMbii
	.p2align	1
	.type	_Z11stsame_testTVMbii,@function
_Z11stsame_testTVMbii:
; %bb.0:
	STSAME
.Lfunc_end31:
	.size	_Z11stsame_testTVMbii, .Lfunc_end31-_Z11stsame_testTVMbii

	.globl	_Z11stzero_testTVMb
	.p2align	1
	.type	_Z11stzero_testTVMb,@function
_Z11stzero_testTVMb:
; %bb.0:
	STZERO
.Lfunc_end32:
	.size	_Z11stzero_testTVMb, .Lfunc_end32-_Z11stzero_testTVMb

	.globl	_Z10stone_testTVMb
	.p2align	1
	.type	_Z10stone_testTVMb,@function
_Z10stone_testTVMb:
; %bb.0:
	STONE
.Lfunc_end33:
	.size	_Z10stone_testTVMb, .Lfunc_end33-_Z10stone_testTVMb

	.globl	_Z9ldiq_testTVMs
	.p2align	1
	.type	_Z9ldiq_testTVMs,@function
_Z9ldiq_testTVMs:
; %bb.0:
	LDIQ	 15
	XCHG	s0, s2
	DROP2
.Lfunc_end34:
	.size	_Z9ldiq_testTVMs, .Lfunc_end34-_Z9ldiq_testTVMs

	.globl	_Z10pldiq_testTVMs
	.p2align	1
	.type	_Z10pldiq_testTVMs,@function
_Z10pldiq_testTVMs:
; %bb.0:
	PLDIQ	 17
	SWAP
	DROP
.Lfunc_end35:
	.size	_Z10pldiq_testTVMs, .Lfunc_end35-_Z10pldiq_testTVMs

	.globl	_Z10plduq_testTVMs
	.p2align	1
	.type	_Z10plduq_testTVMs,@function
_Z10plduq_testTVMs:
; %bb.0:
	PLDUQ	 18
	SWAP
	DROP
.Lfunc_end36:
	.size	_Z10plduq_testTVMs, .Lfunc_end36-_Z10plduq_testTVMs

	.globl	_Z10plduz_testTVMs
	.p2align	1
	.type	_Z10plduz_testTVMs,@function
_Z10plduz_testTVMs:
; %bb.0:
	PLDUZ	 3
	DROP
.Lfunc_end37:
	.size	_Z10plduz_testTVMs, .Lfunc_end37-_Z10plduz_testTVMs

	.globl	_Z13pldslice_testTVMs
	.p2align	1
	.type	_Z13pldslice_testTVMs,@function
_Z13pldslice_testTVMs:
; %bb.0:
	PLDSLICE	 4
.Lfunc_end38:
	.size	_Z13pldslice_testTVMs, .Lfunc_end38-_Z13pldslice_testTVMs

	.globl	_Z13ldsliceq_testTVMs
	.p2align	1
	.type	_Z13ldsliceq_testTVMs,@function
_Z13ldsliceq_testTVMs:
; %bb.0:
	LDSLICEQ	 5
	XCHG	s1, s2
	DROP2
.Lfunc_end39:
	.size	_Z13ldsliceq_testTVMs, .Lfunc_end39-_Z13ldsliceq_testTVMs

	.globl	_Z14pldsliceq_testTVMs
	.p2align	1
	.type	_Z14pldsliceq_testTVMs,@function
_Z14pldsliceq_testTVMs:
; %bb.0:
	PLDSLICEQ	 6
	DROP
.Lfunc_end40:
	.size	_Z14pldsliceq_testTVMs, .Lfunc_end40-_Z14pldsliceq_testTVMs

	.globl	_Z14ldslicexq_testTVMsi
	.p2align	1
	.type	_Z14ldslicexq_testTVMsi,@function
_Z14ldslicexq_testTVMsi:
; %bb.0:
	LDSLICEXQ NULLROTRIFNOT
	XCHG	s1, s2
	DROP2
.Lfunc_end41:
	.size	_Z14ldslicexq_testTVMsi, .Lfunc_end41-_Z14ldslicexq_testTVMsi

	.globl	_Z13pldslice_testTVMsi
	.p2align	1
	.type	_Z13pldslice_testTVMsi,@function
_Z13pldslice_testTVMsi:
; %bb.0:
	PLDSLICEXQ NULLSWAPIFNOT
	DROP
.Lfunc_end42:
	.size	_Z13pldslice_testTVMsi, .Lfunc_end42-_Z13pldslice_testTVMsi

	.globl	_Z15sdcutfirst_testTVMsi
	.p2align	1
	.type	_Z15sdcutfirst_testTVMsi,@function
_Z15sdcutfirst_testTVMsi:
; %bb.0:
	SDCUTFIRST
.Lfunc_end43:
	.size	_Z15sdcutfirst_testTVMsi, .Lfunc_end43-_Z15sdcutfirst_testTVMsi

	.globl	_Z16sdskipfirst_testTVMsi
	.p2align	1
	.type	_Z16sdskipfirst_testTVMsi,@function
_Z16sdskipfirst_testTVMsi:
; %bb.0:
	SDSKIPFIRST
.Lfunc_end44:
	.size	_Z16sdskipfirst_testTVMsi, .Lfunc_end44-_Z16sdskipfirst_testTVMsi

	.globl	_Z14sdcutlast_testTVMsi
	.p2align	1
	.type	_Z14sdcutlast_testTVMsi,@function
_Z14sdcutlast_testTVMsi:
; %bb.0:
	SDCUTLAST
.Lfunc_end45:
	.size	_Z14sdcutlast_testTVMsi, .Lfunc_end45-_Z14sdcutlast_testTVMsi

	.globl	_Z15sdskiplast_testTVMsi
	.p2align	1
	.type	_Z15sdskiplast_testTVMsi,@function
_Z15sdskiplast_testTVMsi:
; %bb.0:
	SDSKIPLAST
.Lfunc_end46:
	.size	_Z15sdskiplast_testTVMsi, .Lfunc_end46-_Z15sdskiplast_testTVMsi

	.globl	_Z13subslice_testTVMsiiii
	.p2align	1
	.type	_Z13subslice_testTVMsiiii,@function
_Z13subslice_testTVMsiiii:
; %bb.0:
	SUBSLICE
.Lfunc_end47:
	.size	_Z13subslice_testTVMsiiii, .Lfunc_end47-_Z13subslice_testTVMsiiii

	.globl	_Z10split_testTVMsii
	.p2align	1
	.type	_Z10split_testTVMsii,@function
_Z10split_testTVMsii:
; %bb.0:
	SPLIT
	SWAP
	DROP
.Lfunc_end48:
	.size	_Z10split_testTVMsii, .Lfunc_end48-_Z10split_testTVMsii

	.globl	_Z11splitq_testTVMsii
	.p2align	1
	.type	_Z11splitq_testTVMsii,@function
_Z11splitq_testTVMsii:
; %bb.0:
	SPLITQ
	XCHG	s1, s2
	DROP2
.Lfunc_end49:
	.size	_Z11splitq_testTVMsii, .Lfunc_end49-_Z11splitq_testTVMsii

	.globl	_Z10xctos_testTVMc
	.p2align	1
	.type	_Z10xctos_testTVMc,@function
_Z10xctos_testTVMc:
; %bb.0:
	XCTOS
	DROP
.Lfunc_end50:
	.size	_Z10xctos_testTVMc, .Lfunc_end50-_Z10xctos_testTVMc

	.globl	_Z10xload_testTVMc
	.p2align	1
	.type	_Z10xload_testTVMc,@function
_Z10xload_testTVMc:
; %bb.0:
	XLOAD
.Lfunc_end51:
	.size	_Z10xload_testTVMc, .Lfunc_end51-_Z10xload_testTVMc

	.globl	_Z11xloadq_testTVMc
	.p2align	1
	.type	_Z11xloadq_testTVMc,@function
_Z11xloadq_testTVMc:
; %bb.0:
	XLOADQ
	DROP
.Lfunc_end52:
	.size	_Z11xloadq_testTVMc, .Lfunc_end52-_Z11xloadq_testTVMc

	.globl	_Z13schkbits_testTVMsi
	.p2align	1
	.type	_Z13schkbits_testTVMsi,@function
_Z13schkbits_testTVMsi:
; %bb.0:
	SCHKBITS
.Lfunc_end53:
	.size	_Z13schkbits_testTVMsi, .Lfunc_end53-_Z13schkbits_testTVMsi

	.globl	_Z13schkrefs_testTVMsi
	.p2align	1
	.type	_Z13schkrefs_testTVMsi,@function
_Z13schkrefs_testTVMsi:
; %bb.0:
	SCHKREFS
.Lfunc_end54:
	.size	_Z13schkrefs_testTVMsi, .Lfunc_end54-_Z13schkrefs_testTVMsi

	.globl	_Z16schkbitrefs_testTVMsii
	.p2align	1
	.type	_Z16schkbitrefs_testTVMsii,@function
_Z16schkbitrefs_testTVMsii:
; %bb.0:
	SCHKBITREFS
.Lfunc_end55:
	.size	_Z16schkbitrefs_testTVMsii, .Lfunc_end55-_Z16schkbitrefs_testTVMsii

	.globl	_Z14schkbitsq_testTVMsi
	.p2align	1
	.type	_Z14schkbitsq_testTVMsi,@function
_Z14schkbitsq_testTVMsi:
; %bb.0:
	SCHKBITSQ
.Lfunc_end56:
	.size	_Z14schkbitsq_testTVMsi, .Lfunc_end56-_Z14schkbitsq_testTVMsi

	.globl	_Z14schkrefsq_testTVMsi
	.p2align	1
	.type	_Z14schkrefsq_testTVMsi,@function
_Z14schkrefsq_testTVMsi:
; %bb.0:
	SCHKREFSQ
.Lfunc_end57:
	.size	_Z14schkrefsq_testTVMsi, .Lfunc_end57-_Z14schkrefsq_testTVMsi

	.globl	_Z17schkbitrefsq_testTVMsii
	.p2align	1
	.type	_Z17schkbitrefsq_testTVMsii,@function
_Z17schkbitrefsq_testTVMsii:
; %bb.0:
	SCHKBITREFSQ
.Lfunc_end58:
	.size	_Z17schkbitrefsq_testTVMsii, .Lfunc_end58-_Z17schkbitrefsq_testTVMsii

	.globl	_Z14pldrefvar_testTVMs
	.p2align	1
	.type	_Z14pldrefvar_testTVMs,@function
_Z14pldrefvar_testTVMs:
; %bb.0:
	PLDREFVAR
.Lfunc_end59:
	.size	_Z14pldrefvar_testTVMs, .Lfunc_end59-_Z14pldrefvar_testTVMs

	.globl	_Z14pldrefidx_testTVMs
	.p2align	1
	.type	_Z14pldrefidx_testTVMs,@function
_Z14pldrefidx_testTVMs:
; %bb.0:
	PLDREFIDX
.Lfunc_end60:
	.size	_Z14pldrefidx_testTVMs, .Lfunc_end60-_Z14pldrefidx_testTVMs

	.globl	_Z11pldref_testTVMs
	.p2align	1
	.type	_Z11pldref_testTVMs,@function
_Z11pldref_testTVMs:
; %bb.0:
	PLDREF
.Lfunc_end61:
	.size	_Z11pldref_testTVMs, .Lfunc_end61-_Z11pldref_testTVMs

	.globl	_Z11ldile4_testTVMs
	.p2align	1
	.type	_Z11ldile4_testTVMs,@function
_Z11ldile4_testTVMs:
; %bb.0:
	LDILE4
	SWAP
	DROP
.Lfunc_end62:
	.size	_Z11ldile4_testTVMs, .Lfunc_end62-_Z11ldile4_testTVMs

	.globl	_Z11ldule4_testTVMs
	.p2align	1
	.type	_Z11ldule4_testTVMs,@function
_Z11ldule4_testTVMs:
; %bb.0:
	LDULE4
	SWAP
	DROP
.Lfunc_end63:
	.size	_Z11ldule4_testTVMs, .Lfunc_end63-_Z11ldule4_testTVMs

	.globl	_Z11ldile8_testTVMs
	.p2align	1
	.type	_Z11ldile8_testTVMs,@function
_Z11ldile8_testTVMs:
; %bb.0:
	LDILE8
	SWAP
	DROP
.Lfunc_end64:
	.size	_Z11ldile8_testTVMs, .Lfunc_end64-_Z11ldile8_testTVMs

	.globl	_Z11ldule8_testTVMs
	.p2align	1
	.type	_Z11ldule8_testTVMs,@function
_Z11ldule8_testTVMs:
; %bb.0:
	LDILE8
	SWAP
	DROP
.Lfunc_end65:
	.size	_Z11ldule8_testTVMs, .Lfunc_end65-_Z11ldule8_testTVMs

	.globl	_Z12pldile4_testTVMs
	.p2align	1
	.type	_Z12pldile4_testTVMs,@function
_Z12pldile4_testTVMs:
; %bb.0:
	PLDILE4
.Lfunc_end66:
	.size	_Z12pldile4_testTVMs, .Lfunc_end66-_Z12pldile4_testTVMs

	.globl	_Z12pldule4_testTVMs
	.p2align	1
	.type	_Z12pldule4_testTVMs,@function
_Z12pldule4_testTVMs:
; %bb.0:
	PLDULE4
.Lfunc_end67:
	.size	_Z12pldule4_testTVMs, .Lfunc_end67-_Z12pldule4_testTVMs

	.globl	_Z12pldile8_testTVMs
	.p2align	1
	.type	_Z12pldile8_testTVMs,@function
_Z12pldile8_testTVMs:
; %bb.0:
	PLDILE8
.Lfunc_end68:
	.size	_Z12pldile8_testTVMs, .Lfunc_end68-_Z12pldile8_testTVMs

	.globl	_Z12pldule8_testTVMs
	.p2align	1
	.type	_Z12pldule8_testTVMs,@function
_Z12pldule8_testTVMs:
; %bb.0:
	PLDULE8
.Lfunc_end69:
	.size	_Z12pldule8_testTVMs, .Lfunc_end69-_Z12pldule8_testTVMs

	.globl	_Z12ldile4q_testTVMs
	.p2align	1
	.type	_Z12ldile4q_testTVMs,@function
_Z12ldile4q_testTVMs:
; %bb.0:
	LDILE4Q
	XCHG	s1, s2
	DROP2
.Lfunc_end70:
	.size	_Z12ldile4q_testTVMs, .Lfunc_end70-_Z12ldile4q_testTVMs

	.globl	_Z12ldule4q_testTVMs
	.p2align	1
	.type	_Z12ldule4q_testTVMs,@function
_Z12ldule4q_testTVMs:
; %bb.0:
	LDULE4Q
	XCHG	s1, s2
	DROP2
.Lfunc_end71:
	.size	_Z12ldule4q_testTVMs, .Lfunc_end71-_Z12ldule4q_testTVMs

	.globl	_Z12ldile8q_testTVMs
	.p2align	1
	.type	_Z12ldile8q_testTVMs,@function
_Z12ldile8q_testTVMs:
; %bb.0:
	LDILE8Q
	XCHG	s1, s2
	DROP2
.Lfunc_end72:
	.size	_Z12ldile8q_testTVMs, .Lfunc_end72-_Z12ldile8q_testTVMs

	.globl	_Z12ldule8q_testTVMs
	.p2align	1
	.type	_Z12ldule8q_testTVMs,@function
_Z12ldule8q_testTVMs:
; %bb.0:
	LDULE8Q
	XCHG	s1, s2
	DROP2
.Lfunc_end73:
	.size	_Z12ldule8q_testTVMs, .Lfunc_end73-_Z12ldule8q_testTVMs

	.globl	_Z13pldile4q_testTVMs
	.p2align	1
	.type	_Z13pldile4q_testTVMs,@function
_Z13pldile4q_testTVMs:
; %bb.0:
	PLDILE4Q
	SWAP
	DROP
.Lfunc_end74:
	.size	_Z13pldile4q_testTVMs, .Lfunc_end74-_Z13pldile4q_testTVMs

	.globl	_Z13pldule4q_testTVMs
	.p2align	1
	.type	_Z13pldule4q_testTVMs,@function
_Z13pldule4q_testTVMs:
; %bb.0:
	PLDULE4Q
	SWAP
	DROP
.Lfunc_end75:
	.size	_Z13pldule4q_testTVMs, .Lfunc_end75-_Z13pldule4q_testTVMs

	.globl	_Z13pldile8q_testTVMs
	.p2align	1
	.type	_Z13pldile8q_testTVMs,@function
_Z13pldile8q_testTVMs:
; %bb.0:
	PLDILE8Q
	SWAP
	DROP
.Lfunc_end76:
	.size	_Z13pldile8q_testTVMs, .Lfunc_end76-_Z13pldile8q_testTVMs

	.globl	_Z13pldule8q_testTVMs
	.p2align	1
	.type	_Z13pldule8q_testTVMs,@function
_Z13pldule8q_testTVMs:
; %bb.0:
	PLDULE8Q
	SWAP
	DROP
.Lfunc_end77:
	.size	_Z13pldule8q_testTVMs, .Lfunc_end77-_Z13pldule8q_testTVMs

	.globl	_Z13ldzeroes_testTVMs
	.p2align	1
	.type	_Z13ldzeroes_testTVMs,@function
_Z13ldzeroes_testTVMs:
; %bb.0:
	LDZEROES
	SWAP
	DROP
.Lfunc_end78:
	.size	_Z13ldzeroes_testTVMs, .Lfunc_end78-_Z13ldzeroes_testTVMs

	.globl	_Z11ldones_testTVMs
	.p2align	1
	.type	_Z11ldones_testTVMs,@function
_Z11ldones_testTVMs:
; %bb.0:
	LDONES
	SWAP
	DROP
.Lfunc_end79:
	.size	_Z11ldones_testTVMs, .Lfunc_end79-_Z11ldones_testTVMs

	.globl	_Z11ldsame_testTVMsi
	.p2align	1
	.type	_Z11ldsame_testTVMsi,@function
_Z11ldsame_testTVMsi:
; %bb.0:
	LDSAME
	SWAP
	DROP
.Lfunc_end80:
	.size	_Z11ldsame_testTVMsi, .Lfunc_end80-_Z11ldsame_testTVMsi

	.globl	_Z11sdepth_testTVMs
	.p2align	1
	.type	_Z11sdepth_testTVMs,@function
_Z11sdepth_testTVMs:
; %bb.0:
	SDEPTH
.Lfunc_end81:
	.size	_Z11sdepth_testTVMs, .Lfunc_end81-_Z11sdepth_testTVMs

	.globl	_Z11cdepth_testTVMc
	.p2align	1
	.type	_Z11cdepth_testTVMc,@function
_Z11cdepth_testTVMc:
; %bb.0:
	CDEPTH
.Lfunc_end82:
	.size	_Z11cdepth_testTVMc, .Lfunc_end82-_Z11cdepth_testTVMc


	.ident	"TON Labs clang for TVM. Version 7.0.0  (based on LLVM 7.0.0)"
	.section	".note.GNU-stack","",@progbits
