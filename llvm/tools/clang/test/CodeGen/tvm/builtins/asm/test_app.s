	.text
	.file	"test_app.cpp"
	.globl	_Z13randu256_testv
	.p2align	1
	.type	_Z13randu256_testv,@function
_Z13randu256_testv:
; %bb.0:
	RANDU256
.Lfunc_end0:
	.size	_Z13randu256_testv, .Lfunc_end0-_Z13randu256_testv

	.globl	_Z9rand_testi
	.p2align	1
	.type	_Z9rand_testi,@function
_Z9rand_testi:
; %bb.0:
	RAND
.Lfunc_end1:
	.size	_Z9rand_testi, .Lfunc_end1-_Z9rand_testi

	.globl	_Z12setrand_testi
	.p2align	1
	.type	_Z12setrand_testi,@function
_Z12setrand_testi:
; %bb.0:
	SETRAND
.Lfunc_end2:
	.size	_Z12setrand_testi, .Lfunc_end2-_Z12setrand_testi

	.globl	_Z12addrand_testi
	.p2align	1
	.type	_Z12addrand_testi,@function
_Z12addrand_testi:
; %bb.0:
	ADDRAND
.Lfunc_end3:
	.size	_Z12addrand_testi, .Lfunc_end3-_Z12addrand_testi

	.globl	_Z13randseed_testv
	.p2align	1
	.type	_Z13randseed_testv,@function
_Z13randseed_testv:
; %bb.0:
	RANDSEED
.Lfunc_end4:
	.size	_Z13randseed_testv, .Lfunc_end4-_Z13randseed_testv

	.globl	_Z12balance_testv
	.p2align	1
	.type	_Z12balance_testv,@function
_Z12balance_testv:
; %bb.0:
	BALANCE
.Lfunc_end5:
	.size	_Z12balance_testv, .Lfunc_end5-_Z12balance_testv

	.globl	_Z12sha256u_testTVMs
	.p2align	1
	.type	_Z12sha256u_testTVMs,@function
_Z12sha256u_testTVMs:
; %bb.0:
	SHA256U
.Lfunc_end6:
	.size	_Z12sha256u_testTVMs, .Lfunc_end6-_Z12sha256u_testTVMs

	.globl	_Z13chksigns_testTVMsii
	.p2align	1
	.type	_Z13chksigns_testTVMsii,@function
_Z13chksigns_testTVMsii:
; %bb.0:
	CHKSIGNS
.Lfunc_end7:
	.size	_Z13chksigns_testTVMsii, .Lfunc_end7-_Z13chksigns_testTVMsii

	.globl	_Z15cdatasizeq_testTVMci
	.p2align	1
	.type	_Z15cdatasizeq_testTVMci,@function
_Z15cdatasizeq_testTVMci:
; %bb.0:
	CDATASIZEQ
	XCHG	s0, s3
	BLKDROP	3
.Lfunc_end8:
	.size	_Z15cdatasizeq_testTVMci, .Lfunc_end8-_Z15cdatasizeq_testTVMci

	.globl	_Z16rawreservex_testiTVMci
	.p2align	1
	.type	_Z16rawreservex_testiTVMci,@function
_Z16rawreservex_testiTVMci:
; %bb.0:
	RAWRESERVEX
.Lfunc_end9:
	.size	_Z16rawreservex_testiTVMci, .Lfunc_end9-_Z16rawreservex_testiTVMci

	.globl	_Z15setlibcode_testTVMci
	.p2align	1
	.type	_Z15setlibcode_testTVMci,@function
_Z15setlibcode_testTVMci:
; %bb.0:
	SETLIBCODE
.Lfunc_end10:
	.size	_Z15setlibcode_testTVMci, .Lfunc_end10-_Z15setlibcode_testTVMci

	.globl	_Z14changelib_testii
	.p2align	1
	.type	_Z14changelib_testii,@function
_Z14changelib_testii:
; %bb.0:
	CHANGELIB
.Lfunc_end11:
	.size	_Z14changelib_testii, .Lfunc_end11-_Z14changelib_testii


	.ident	"TON Labs clang for TVM. Version 7.0.0  (based on LLVM 7.0.0)"
	.section	".note.GNU-stack","",@progbits
