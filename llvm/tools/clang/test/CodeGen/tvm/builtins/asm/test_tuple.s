	.text
	.file	"test_tuple.cpp"
	.globl	_Z8nil_testv
	.p2align	1
	.type	_Z8nil_testv,@function
_Z8nil_testv:
; %bb.0:
	NIL
.Lfunc_end0:
	.size	_Z8nil_testv, .Lfunc_end0-_Z8nil_testv

	.globl	_Z11single_testi
	.p2align	1
	.type	_Z11single_testi,@function
_Z11single_testi:
; %bb.0:
	SIGNLE
.Lfunc_end1:
	.size	_Z11single_testi, .Lfunc_end1-_Z11single_testi

	.globl	_Z9pair_testii
	.p2align	1
	.type	_Z9pair_testii,@function
_Z9pair_testii:
; %bb.0:
	PAIR
.Lfunc_end2:
	.size	_Z9pair_testii, .Lfunc_end2-_Z9pair_testii

	.globl	_Z11triple_testiii
	.p2align	1
	.type	_Z11triple_testiii,@function
_Z11triple_testiii:
; %bb.0:
	TRIPLE
.Lfunc_end3:
	.size	_Z11triple_testiii, .Lfunc_end3-_Z11triple_testiii

	.globl	_Z10first_testTVMt
	.p2align	1
	.type	_Z10first_testTVMt,@function
_Z10first_testTVMt:
; %bb.0:
	FIRST
.Lfunc_end4:
	.size	_Z10first_testTVMt, .Lfunc_end4-_Z10first_testTVMt

	.globl	_Z11second_testTVMt
	.p2align	1
	.type	_Z11second_testTVMt,@function
_Z11second_testTVMt:
; %bb.0:
	SECOND
.Lfunc_end5:
	.size	_Z11second_testTVMt, .Lfunc_end5-_Z11second_testTVMt

	.globl	_Z10third_testTVMt
	.p2align	1
	.type	_Z10third_testTVMt,@function
_Z10third_testTVMt:
; %bb.0:
	SECOND
.Lfunc_end6:
	.size	_Z10third_testTVMt, .Lfunc_end6-_Z10third_testTVMt

	.globl	_Z13unsingle_testTVMt
	.p2align	1
	.type	_Z13unsingle_testTVMt,@function
_Z13unsingle_testTVMt:
; %bb.0:
	UNSIGNLE
.Lfunc_end7:
	.size	_Z13unsingle_testTVMt, .Lfunc_end7-_Z13unsingle_testTVMt

	.globl	_Z11unpair_testTVMt
	.p2align	1
	.type	_Z11unpair_testTVMt,@function
_Z11unpair_testTVMt:
; %bb.0:
	UNPAIR
	SWAP
	DROP
.Lfunc_end8:
	.size	_Z11unpair_testTVMt, .Lfunc_end8-_Z11unpair_testTVMt

	.globl	_Z13untriple_testTVMt
	.p2align	1
	.type	_Z13untriple_testTVMt,@function
_Z13untriple_testTVMt:
; %bb.0:
	UNTRIPLE
	XCHG	s0, s2
	DROP2
.Lfunc_end9:
	.size	_Z13untriple_testTVMt, .Lfunc_end9-_Z13untriple_testTVMt

	.globl	_Z13setfirst_testTVMti
	.p2align	1
	.type	_Z13setfirst_testTVMti,@function
_Z13setfirst_testTVMti:
; %bb.0:
	SETFIRST
.Lfunc_end10:
	.size	_Z13setfirst_testTVMti, .Lfunc_end10-_Z13setfirst_testTVMti

	.globl	_Z14setsecond_testTVMti
	.p2align	1
	.type	_Z14setsecond_testTVMti,@function
_Z14setsecond_testTVMti:
; %bb.0:
	SETSECOND
.Lfunc_end11:
	.size	_Z14setsecond_testTVMti, .Lfunc_end11-_Z14setsecond_testTVMti

	.globl	_Z13setthird_testTVMti
	.p2align	1
	.type	_Z13setthird_testTVMti,@function
_Z13setthird_testTVMti:
; %bb.0:
	SETTHIRD
.Lfunc_end12:
	.size	_Z13setthird_testTVMti, .Lfunc_end12-_Z13setthird_testTVMti

	.globl	_Z14indexvarq_testTVMti
	.p2align	1
	.type	_Z14indexvarq_testTVMti,@function
_Z14indexvarq_testTVMti:
; %bb.0:
	INDEXVARQ
.Lfunc_end13:
	.size	_Z14indexvarq_testTVMti, .Lfunc_end13-_Z14indexvarq_testTVMti

	.globl	_Z17setindexvarq_testTVMtii
	.p2align	1
	.type	_Z17setindexvarq_testTVMtii,@function
_Z17setindexvarq_testTVMtii:
; %bb.0:
	SETINDEXVARQ
.Lfunc_end14:
	.size	_Z17setindexvarq_testTVMtii, .Lfunc_end14-_Z17setindexvarq_testTVMtii


	.ident	"TON Labs clang for TVM. Version 7.0.0  (based on LLVM 7.0.0)"
	.section	".note.GNU-stack","",@progbits
