	.text
	.file	"test_dict.cpp"
	.globl	_Z13skipdict_testTVMs
	.p2align	1
	.type	_Z13skipdict_testTVMs,@function
_Z13skipdict_testTVMs:
; %bb.0:
	SKIPDICT
.Lfunc_end0:
	.size	_Z13skipdict_testTVMs, .Lfunc_end0-_Z13skipdict_testTVMs

	.globl	_Z12lddicts_testTVMs
	.p2align	1
	.type	_Z12lddicts_testTVMs,@function
_Z12lddicts_testTVMs:
; %bb.0:
	LDDICTS
	SWAP
	DROP
.Lfunc_end1:
	.size	_Z12lddicts_testTVMs, .Lfunc_end1-_Z12lddicts_testTVMs

	.globl	_Z13plddicts_testTVMs
	.p2align	1
	.type	_Z13plddicts_testTVMs,@function
_Z13plddicts_testTVMs:
; %bb.0:
	PLDDICTS
.Lfunc_end2:
	.size	_Z13plddicts_testTVMs, .Lfunc_end2-_Z13plddicts_testTVMs

	.globl	_Z12lddictq_testTVMs
	.p2align	1
	.type	_Z12lddictq_testTVMs,@function
_Z12lddictq_testTVMs:
; %bb.0:
	LDDICTQ
	XCHG	s0, s2
	DROP2
.Lfunc_end3:
	.size	_Z12lddictq_testTVMs, .Lfunc_end3-_Z12lddictq_testTVMs

	.globl	_Z13plddictq_testTVMs
	.p2align	1
	.type	_Z13plddictq_testTVMs,@function
_Z13plddictq_testTVMs:
; %bb.0:
	PLDDICTQ
	SWAP
	DROP
.Lfunc_end4:
	.size	_Z13plddictq_testTVMs, .Lfunc_end4-_Z13plddictq_testTVMs

	.globl	_Z16dictisetget_testTVMsiTVMci
	.p2align	1
	.type	_Z16dictisetget_testTVMsiTVMci,@function
_Z16dictisetget_testTVMsiTVMci:
; %bb.0:
	DICTISETGET NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end5:
	.size	_Z16dictisetget_testTVMsiTVMci, .Lfunc_end5-_Z16dictisetget_testTVMsiTVMci

	.globl	_Z19dictisetgetref_testTVMciTVMci
	.p2align	1
	.type	_Z19dictisetgetref_testTVMciTVMci,@function
_Z19dictisetgetref_testTVMciTVMci:
; %bb.0:
	DICTISETGETREF NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end6:
	.size	_Z19dictisetgetref_testTVMciTVMci, .Lfunc_end6-_Z19dictisetgetref_testTVMciTVMci

	.globl	_Z19dictreplaceget_testTVMsTVMsTVMci
	.p2align	1
	.type	_Z19dictreplaceget_testTVMsTVMsTVMci,@function
_Z19dictreplaceget_testTVMsTVMsTVMci:
; %bb.0:
	DICTREPLACEGET NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end7:
	.size	_Z19dictreplaceget_testTVMsTVMsTVMci, .Lfunc_end7-_Z19dictreplaceget_testTVMsTVMsTVMci

	.globl	_Z22dictreplacegetref_testTVMcTVMsTVMci
	.p2align	1
	.type	_Z22dictreplacegetref_testTVMcTVMsTVMci,@function
_Z22dictreplacegetref_testTVMcTVMsTVMci:
; %bb.0:
	DICTREPLACEGETREF NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end8:
	.size	_Z22dictreplacegetref_testTVMcTVMsTVMci, .Lfunc_end8-_Z22dictreplacegetref_testTVMcTVMsTVMci

	.globl	_Z20dictireplaceget_testTVMsiTVMci
	.p2align	1
	.type	_Z20dictireplaceget_testTVMsiTVMci,@function
_Z20dictireplaceget_testTVMsiTVMci:
; %bb.0:
	DICTIREPLACEGET NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end9:
	.size	_Z20dictireplaceget_testTVMsiTVMci, .Lfunc_end9-_Z20dictireplaceget_testTVMsiTVMci

	.globl	_Z23dictireplacegetref_testTVMciTVMci
	.p2align	1
	.type	_Z23dictireplacegetref_testTVMciTVMci,@function
_Z23dictireplacegetref_testTVMciTVMci:
; %bb.0:
	DICTIREPLACEGETREF NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end10:
	.size	_Z23dictireplacegetref_testTVMciTVMci, .Lfunc_end10-_Z23dictireplacegetref_testTVMciTVMci

	.globl	_Z20dictureplaceget_testTVMsiTVMci
	.p2align	1
	.type	_Z20dictureplaceget_testTVMsiTVMci,@function
_Z20dictureplaceget_testTVMsiTVMci:
; %bb.0:
	DICTUREPLACEGET NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end11:
	.size	_Z20dictureplaceget_testTVMsiTVMci, .Lfunc_end11-_Z20dictureplaceget_testTVMsiTVMci

	.globl	_Z23dictureplacegetref_testTVMciTVMci
	.p2align	1
	.type	_Z23dictureplacegetref_testTVMciTVMci,@function
_Z23dictureplacegetref_testTVMciTVMci:
; %bb.0:
	DICTUREPLACEGETREF NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end12:
	.size	_Z23dictureplacegetref_testTVMciTVMci, .Lfunc_end12-_Z23dictureplacegetref_testTVMciTVMci

	.globl	_Z12dictadd_testTVMsTVMsTVMci
	.p2align	1
	.type	_Z12dictadd_testTVMsTVMsTVMci,@function
_Z12dictadd_testTVMsTVMsTVMci:
; %bb.0:
	DICTADD
	SWAP
	DROP
.Lfunc_end13:
	.size	_Z12dictadd_testTVMsTVMsTVMci, .Lfunc_end13-_Z12dictadd_testTVMsTVMsTVMci

	.globl	_Z15dictaddref_testTVMcTVMsTVMci
	.p2align	1
	.type	_Z15dictaddref_testTVMcTVMsTVMci,@function
_Z15dictaddref_testTVMcTVMsTVMci:
; %bb.0:
	DICTADDREF
	SWAP
	DROP
.Lfunc_end14:
	.size	_Z15dictaddref_testTVMcTVMsTVMci, .Lfunc_end14-_Z15dictaddref_testTVMcTVMsTVMci

	.globl	_Z13dictiadd_testTVMsiTVMci
	.p2align	1
	.type	_Z13dictiadd_testTVMsiTVMci,@function
_Z13dictiadd_testTVMsiTVMci:
; %bb.0:
	DICTIADD
	SWAP
	DROP
.Lfunc_end15:
	.size	_Z13dictiadd_testTVMsiTVMci, .Lfunc_end15-_Z13dictiadd_testTVMsiTVMci

	.globl	_Z16dictiaddref_testTVMciTVMci
	.p2align	1
	.type	_Z16dictiaddref_testTVMciTVMci,@function
_Z16dictiaddref_testTVMciTVMci:
; %bb.0:
	DICTIADDREF
	SWAP
	DROP
.Lfunc_end16:
	.size	_Z16dictiaddref_testTVMciTVMci, .Lfunc_end16-_Z16dictiaddref_testTVMciTVMci

	.globl	_Z13dictuadd_testTVMsiTVMci
	.p2align	1
	.type	_Z13dictuadd_testTVMsiTVMci,@function
_Z13dictuadd_testTVMsiTVMci:
; %bb.0:
	DICTUADD
	SWAP
	DROP
.Lfunc_end17:
	.size	_Z13dictuadd_testTVMsiTVMci, .Lfunc_end17-_Z13dictuadd_testTVMsiTVMci

	.globl	_Z16dictuaddref_testTVMciTVMci
	.p2align	1
	.type	_Z16dictuaddref_testTVMciTVMci,@function
_Z16dictuaddref_testTVMciTVMci:
; %bb.0:
	DICTUADDREF
	SWAP
	DROP
.Lfunc_end18:
	.size	_Z16dictuaddref_testTVMciTVMci, .Lfunc_end18-_Z16dictuaddref_testTVMciTVMci

	.globl	_Z15dictaddget_testTVMsTVMsTVMci
	.p2align	1
	.type	_Z15dictaddget_testTVMsTVMsTVMci,@function
_Z15dictaddget_testTVMsTVMsTVMci:
; %bb.0:
	DICTADDGET NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end19:
	.size	_Z15dictaddget_testTVMsTVMsTVMci, .Lfunc_end19-_Z15dictaddget_testTVMsTVMsTVMci

	.globl	_Z18dictaddgetref_testTVMcTVMsTVMci
	.p2align	1
	.type	_Z18dictaddgetref_testTVMcTVMsTVMci,@function
_Z18dictaddgetref_testTVMcTVMsTVMci:
; %bb.0:
	DICTADDGETREF NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end20:
	.size	_Z18dictaddgetref_testTVMcTVMsTVMci, .Lfunc_end20-_Z18dictaddgetref_testTVMcTVMsTVMci

	.globl	_Z16dictiaddget_testTVMsiTVMci
	.p2align	1
	.type	_Z16dictiaddget_testTVMsiTVMci,@function
_Z16dictiaddget_testTVMsiTVMci:
; %bb.0:
	DICTIADDGET NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end21:
	.size	_Z16dictiaddget_testTVMsiTVMci, .Lfunc_end21-_Z16dictiaddget_testTVMsiTVMci

	.globl	_Z19dictiaddgetref_testTVMciTVMci
	.p2align	1
	.type	_Z19dictiaddgetref_testTVMciTVMci,@function
_Z19dictiaddgetref_testTVMciTVMci:
; %bb.0:
	DICTIADDGETREF NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end22:
	.size	_Z19dictiaddgetref_testTVMciTVMci, .Lfunc_end22-_Z19dictiaddgetref_testTVMciTVMci

	.globl	_Z16dictuaddget_testTVMsiTVMci
	.p2align	1
	.type	_Z16dictuaddget_testTVMsiTVMci,@function
_Z16dictuaddget_testTVMsiTVMci:
; %bb.0:
	DICTUADDGET NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end23:
	.size	_Z16dictuaddget_testTVMsiTVMci, .Lfunc_end23-_Z16dictuaddget_testTVMsiTVMci

	.globl	_Z19dictuaddgetref_testTVMciTVMci
	.p2align	1
	.type	_Z19dictuaddgetref_testTVMciTVMci,@function
_Z19dictuaddgetref_testTVMciTVMci:
; %bb.0:
	DICTUADDGETREF NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end24:
	.size	_Z19dictuaddgetref_testTVMciTVMci, .Lfunc_end24-_Z19dictuaddgetref_testTVMciTVMci

	.globl	_Z13dictsetb_testTVMbTVMsTVMci
	.p2align	1
	.type	_Z13dictsetb_testTVMbTVMsTVMci,@function
_Z13dictsetb_testTVMbTVMsTVMci:
; %bb.0:
	DICTSETB
.Lfunc_end25:
	.size	_Z13dictsetb_testTVMbTVMsTVMci, .Lfunc_end25-_Z13dictsetb_testTVMbTVMsTVMci

	.globl	_Z14dictisetb_testTVMbiTVMci
	.p2align	1
	.type	_Z14dictisetb_testTVMbiTVMci,@function
_Z14dictisetb_testTVMbiTVMci:
; %bb.0:
	DICTISETB
.Lfunc_end26:
	.size	_Z14dictisetb_testTVMbiTVMci, .Lfunc_end26-_Z14dictisetb_testTVMbiTVMci

	.globl	_Z14dictusetb_testTVMbiTVMci
	.p2align	1
	.type	_Z14dictusetb_testTVMbiTVMci,@function
_Z14dictusetb_testTVMbiTVMci:
; %bb.0:
	DICTUSETB
.Lfunc_end27:
	.size	_Z14dictusetb_testTVMbiTVMci, .Lfunc_end27-_Z14dictusetb_testTVMbiTVMci

	.globl	_Z16dictsetgetb_testTVMbTVMsTVMci
	.p2align	1
	.type	_Z16dictsetgetb_testTVMbTVMsTVMci,@function
_Z16dictsetgetb_testTVMbTVMsTVMci:
; %bb.0:
	DICTSETGETB NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end28:
	.size	_Z16dictsetgetb_testTVMbTVMsTVMci, .Lfunc_end28-_Z16dictsetgetb_testTVMbTVMsTVMci

	.globl	_Z17dictisetgetb_testTVMbiTVMci
	.p2align	1
	.type	_Z17dictisetgetb_testTVMbiTVMci,@function
_Z17dictisetgetb_testTVMbiTVMci:
; %bb.0:
	DICTISETGETB NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end29:
	.size	_Z17dictisetgetb_testTVMbiTVMci, .Lfunc_end29-_Z17dictisetgetb_testTVMbiTVMci

	.globl	_Z17dictusetgetb_testTVMbiTVMci
	.p2align	1
	.type	_Z17dictusetgetb_testTVMbiTVMci,@function
_Z17dictusetgetb_testTVMbiTVMci:
; %bb.0:
	DICTUSETGETB NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end30:
	.size	_Z17dictusetgetb_testTVMbiTVMci, .Lfunc_end30-_Z17dictusetgetb_testTVMbiTVMci

	.globl	_Z17dictreplaceb_testTVMbTVMsTVMci
	.p2align	1
	.type	_Z17dictreplaceb_testTVMbTVMsTVMci,@function
_Z17dictreplaceb_testTVMbTVMsTVMci:
; %bb.0:
	DICTREPLACEB
	SWAP
	DROP
.Lfunc_end31:
	.size	_Z17dictreplaceb_testTVMbTVMsTVMci, .Lfunc_end31-_Z17dictreplaceb_testTVMbTVMsTVMci

	.globl	_Z18dictireplaceb_testTVMbiTVMci
	.p2align	1
	.type	_Z18dictireplaceb_testTVMbiTVMci,@function
_Z18dictireplaceb_testTVMbiTVMci:
; %bb.0:
	DICTIREPLACEB
	SWAP
	DROP
.Lfunc_end32:
	.size	_Z18dictireplaceb_testTVMbiTVMci, .Lfunc_end32-_Z18dictireplaceb_testTVMbiTVMci

	.globl	_Z18dictureplaceb_testTVMbiTVMci
	.p2align	1
	.type	_Z18dictureplaceb_testTVMbiTVMci,@function
_Z18dictureplaceb_testTVMbiTVMci:
; %bb.0:
	DICTUREPLACEB
	SWAP
	DROP
.Lfunc_end33:
	.size	_Z18dictureplaceb_testTVMbiTVMci, .Lfunc_end33-_Z18dictureplaceb_testTVMbiTVMci

	.globl	_Z20dictreplacegetb_testTVMbTVMsTVMci
	.p2align	1
	.type	_Z20dictreplacegetb_testTVMbTVMsTVMci,@function
_Z20dictreplacegetb_testTVMbTVMsTVMci:
; %bb.0:
	DICTREPLACEGETB NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end34:
	.size	_Z20dictreplacegetb_testTVMbTVMsTVMci, .Lfunc_end34-_Z20dictreplacegetb_testTVMbTVMsTVMci

	.globl	_Z21dictireplacegetb_testTVMbiTVMci
	.p2align	1
	.type	_Z21dictireplacegetb_testTVMbiTVMci,@function
_Z21dictireplacegetb_testTVMbiTVMci:
; %bb.0:
	DICTIREPLACEGETB NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end35:
	.size	_Z21dictireplacegetb_testTVMbiTVMci, .Lfunc_end35-_Z21dictireplacegetb_testTVMbiTVMci

	.globl	_Z21dictureplacegetb_testTVMbiTVMci
	.p2align	1
	.type	_Z21dictureplacegetb_testTVMbiTVMci,@function
_Z21dictureplacegetb_testTVMbiTVMci:
; %bb.0:
	DICTUREPLACEGETB NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end36:
	.size	_Z21dictureplacegetb_testTVMbiTVMci, .Lfunc_end36-_Z21dictureplacegetb_testTVMbiTVMci

	.globl	_Z13dictaddb_testTVMbTVMsTVMci
	.p2align	1
	.type	_Z13dictaddb_testTVMbTVMsTVMci,@function
_Z13dictaddb_testTVMbTVMsTVMci:
; %bb.0:
	DICTADDB
	SWAP
	DROP
.Lfunc_end37:
	.size	_Z13dictaddb_testTVMbTVMsTVMci, .Lfunc_end37-_Z13dictaddb_testTVMbTVMsTVMci

	.globl	_Z14dictiaddb_testTVMbiTVMci
	.p2align	1
	.type	_Z14dictiaddb_testTVMbiTVMci,@function
_Z14dictiaddb_testTVMbiTVMci:
; %bb.0:
	DICTIADDB
	SWAP
	DROP
.Lfunc_end38:
	.size	_Z14dictiaddb_testTVMbiTVMci, .Lfunc_end38-_Z14dictiaddb_testTVMbiTVMci

	.globl	_Z14dictuaddb_testTVMbiTVMci
	.p2align	1
	.type	_Z14dictuaddb_testTVMbiTVMci,@function
_Z14dictuaddb_testTVMbiTVMci:
; %bb.0:
	DICTUADDB
	SWAP
	DROP
.Lfunc_end39:
	.size	_Z14dictuaddb_testTVMbiTVMci, .Lfunc_end39-_Z14dictuaddb_testTVMbiTVMci

	.globl	_Z16dictaddgetb_testTVMbTVMsTVMci
	.p2align	1
	.type	_Z16dictaddgetb_testTVMbTVMsTVMci,@function
_Z16dictaddgetb_testTVMbTVMsTVMci:
; %bb.0:
	DICTADDGETB NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end40:
	.size	_Z16dictaddgetb_testTVMbTVMsTVMci, .Lfunc_end40-_Z16dictaddgetb_testTVMbTVMsTVMci

	.globl	_Z17dictiaddgetb_testTVMbiTVMci
	.p2align	1
	.type	_Z17dictiaddgetb_testTVMbiTVMci,@function
_Z17dictiaddgetb_testTVMbiTVMci:
; %bb.0:
	DICTIADDGETB NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end41:
	.size	_Z17dictiaddgetb_testTVMbiTVMci, .Lfunc_end41-_Z17dictiaddgetb_testTVMbiTVMci

	.globl	_Z17dictuaddgetb_testTVMbiTVMci
	.p2align	1
	.type	_Z17dictuaddgetb_testTVMbiTVMci,@function
_Z17dictuaddgetb_testTVMbiTVMci:
; %bb.0:
	DICTUADDGETB NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end42:
	.size	_Z17dictuaddgetb_testTVMbiTVMci, .Lfunc_end42-_Z17dictuaddgetb_testTVMbiTVMci

	.globl	_Z13dictidel_testiTVMci
	.p2align	1
	.type	_Z13dictidel_testiTVMci,@function
_Z13dictidel_testiTVMci:
; %bb.0:
	DICTDEL
	SWAP
	DROP
.Lfunc_end43:
	.size	_Z13dictidel_testiTVMci, .Lfunc_end43-_Z13dictidel_testiTVMci

	.globl	_Z16dictidelget_testiTVMci
	.p2align	1
	.type	_Z16dictidelget_testiTVMci,@function
_Z16dictidelget_testiTVMci:
; %bb.0:
	DICTDELGET NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end44:
	.size	_Z16dictidelget_testiTVMci, .Lfunc_end44-_Z16dictidelget_testiTVMci

	.globl	_Z19dictidelgetref_testiTVMci
	.p2align	1
	.type	_Z19dictidelgetref_testiTVMci,@function
_Z19dictidelgetref_testiTVMci:
; %bb.0:
	DICTDELGETREF NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end45:
	.size	_Z19dictidelgetref_testiTVMci, .Lfunc_end45-_Z19dictidelgetref_testiTVMci

	.globl	_Z16dictudelget_testiTVMci
	.p2align	1
	.type	_Z16dictudelget_testiTVMci,@function
_Z16dictudelget_testiTVMci:
; %bb.0:
	DICUDELGET NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end46:
	.size	_Z16dictudelget_testiTVMci, .Lfunc_end46-_Z16dictudelget_testiTVMci

	.globl	_Z19dictudelgetref_testiTVMci
	.p2align	1
	.type	_Z19dictudelgetref_testiTVMci,@function
_Z19dictudelgetref_testiTVMci:
; %bb.0:
	DICUDELGETREF NULLSWAPIFNOT
	XCHG	s0, s2
	DROP2
.Lfunc_end47:
	.size	_Z19dictudelgetref_testiTVMci, .Lfunc_end47-_Z19dictudelgetref_testiTVMci

	.globl	_Z18dictgetoptref_testTVMsTVMci
	.p2align	1
	.type	_Z18dictgetoptref_testTVMsTVMci,@function
_Z18dictgetoptref_testTVMsTVMci:
; %bb.0:
	DICTGETOPTREF
	DROP
.Lfunc_end48:
	.size	_Z18dictgetoptref_testTVMsTVMci, .Lfunc_end48-_Z18dictgetoptref_testTVMsTVMci

	.globl	_Z19dictigetoptref_testiTVMci
	.p2align	1
	.type	_Z19dictigetoptref_testiTVMci,@function
_Z19dictigetoptref_testiTVMci:
; %bb.0:
	DICTIGETOPTREF
.Lfunc_end49:
	.size	_Z19dictigetoptref_testiTVMci, .Lfunc_end49-_Z19dictigetoptref_testiTVMci

	.globl	_Z20_dictugetoptref_testiTVMci
	.p2align	1
	.type	_Z20_dictugetoptref_testiTVMci,@function
_Z20_dictugetoptref_testiTVMci:
; %bb.0:
	DICTUGETOPTREF
	DROP
.Lfunc_end50:
	.size	_Z20_dictugetoptref_testiTVMci, .Lfunc_end50-_Z20_dictugetoptref_testiTVMci

	.globl	_Z21dictsetgetoptref_testTVMcTVMsTVMci
	.p2align	1
	.type	_Z21dictsetgetoptref_testTVMcTVMsTVMci,@function
_Z21dictsetgetoptref_testTVMcTVMsTVMci:
; %bb.0:
	DICTSETGETOPTREF
	SWAP
	DROP
.Lfunc_end51:
	.size	_Z21dictsetgetoptref_testTVMcTVMsTVMci, .Lfunc_end51-_Z21dictsetgetoptref_testTVMcTVMsTVMci

	.globl	_Z22dictisetgetoptref_testTVMciTVMci
	.p2align	1
	.type	_Z22dictisetgetoptref_testTVMciTVMci,@function
_Z22dictisetgetoptref_testTVMciTVMci:
; %bb.0:
	DICTISETGETOPTREF
	SWAP
	DROP
.Lfunc_end52:
	.size	_Z22dictisetgetoptref_testTVMciTVMci, .Lfunc_end52-_Z22dictisetgetoptref_testTVMciTVMci

	.globl	_Z22dictusetgetoptref_testTVMciTVMci
	.p2align	1
	.type	_Z22dictusetgetoptref_testTVMciTVMci,@function
_Z22dictusetgetoptref_testTVMciTVMci:
; %bb.0:
	DICTUSETGETOPTREF
	SWAP
	DROP
.Lfunc_end53:
	.size	_Z22dictusetgetoptref_testTVMciTVMci, .Lfunc_end53-_Z22dictusetgetoptref_testTVMciTVMci

	.globl	_Z15pfxdictset_testTVMsTVMsTVMci
	.p2align	1
	.type	_Z15pfxdictset_testTVMsTVMsTVMci,@function
_Z15pfxdictset_testTVMsTVMsTVMci:
; %bb.0:
	PFXDICTSET
	SWAP
	DROP
.Lfunc_end54:
	.size	_Z15pfxdictset_testTVMsTVMsTVMci, .Lfunc_end54-_Z15pfxdictset_testTVMsTVMsTVMci

	.globl	_Z19pfxdictreplace_testTVMsTVMsTVMci
	.p2align	1
	.type	_Z19pfxdictreplace_testTVMsTVMsTVMci,@function
_Z19pfxdictreplace_testTVMsTVMsTVMci:
; %bb.0:
	PFXDICTREPLACE
	SWAP
	DROP
.Lfunc_end55:
	.size	_Z19pfxdictreplace_testTVMsTVMsTVMci, .Lfunc_end55-_Z19pfxdictreplace_testTVMsTVMsTVMci

	.globl	_Z15pfxdictadd_testTVMsTVMsTVMci
	.p2align	1
	.type	_Z15pfxdictadd_testTVMsTVMsTVMci,@function
_Z15pfxdictadd_testTVMsTVMsTVMci:
; %bb.0:
	PFXDICTADD
	SWAP
	DROP
.Lfunc_end56:
	.size	_Z15pfxdictadd_testTVMsTVMsTVMci, .Lfunc_end56-_Z15pfxdictadd_testTVMsTVMsTVMci

	.globl	_Z15pfxdictdel_testTVMsTVMci
	.p2align	1
	.type	_Z15pfxdictdel_testTVMsTVMci,@function
_Z15pfxdictdel_testTVMsTVMci:
; %bb.0:
	PFXDICTDEL
	SWAP
	DROP
.Lfunc_end57:
	.size	_Z15pfxdictdel_testTVMsTVMci, .Lfunc_end57-_Z15pfxdictdel_testTVMsTVMci

	.globl	_Z18dictgetnexteq_testTVMsTVMci
	.p2align	1
	.type	_Z18dictgetnexteq_testTVMsTVMci,@function
_Z18dictgetnexteq_testTVMsTVMci:
; %bb.0:
	DICTGETNEXTQ NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end58:
	.size	_Z18dictgetnexteq_testTVMsTVMci, .Lfunc_end58-_Z18dictgetnexteq_testTVMsTVMci

	.globl	_Z16dictgetprev_testTVMsTVMci
	.p2align	1
	.type	_Z16dictgetprev_testTVMsTVMci,@function
_Z16dictgetprev_testTVMsTVMci:
; %bb.0:
	DICTGETPREV NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end59:
	.size	_Z16dictgetprev_testTVMsTVMci, .Lfunc_end59-_Z16dictgetprev_testTVMsTVMci

	.globl	_Z18dictgetpreveq_testTVMsTVMci
	.p2align	1
	.type	_Z18dictgetpreveq_testTVMsTVMci,@function
_Z18dictgetpreveq_testTVMsTVMci:
; %bb.0:
	DICTGETPREVQ NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end60:
	.size	_Z18dictgetpreveq_testTVMsTVMci, .Lfunc_end60-_Z18dictgetpreveq_testTVMsTVMci

	.globl	_Z17dictigetnext_testiTVMci
	.p2align	1
	.type	_Z17dictigetnext_testiTVMci,@function
_Z17dictigetnext_testiTVMci:
; %bb.0:
	DICTIGETNEXT NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end61:
	.size	_Z17dictigetnext_testiTVMci, .Lfunc_end61-_Z17dictigetnext_testiTVMci

	.globl	_Z19dictigetnexteq_testiTVMci
	.p2align	1
	.type	_Z19dictigetnexteq_testiTVMci,@function
_Z19dictigetnexteq_testiTVMci:
; %bb.0:
	DICTIGETNEXTEQ NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end62:
	.size	_Z19dictigetnexteq_testiTVMci, .Lfunc_end62-_Z19dictigetnexteq_testiTVMci

	.globl	_Z17dictigetprev_testiTVMci
	.p2align	1
	.type	_Z17dictigetprev_testiTVMci,@function
_Z17dictigetprev_testiTVMci:
; %bb.0:
	DICTIGETPREV NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end63:
	.size	_Z17dictigetprev_testiTVMci, .Lfunc_end63-_Z17dictigetprev_testiTVMci

	.globl	_Z19dictigetpreveq_testiTVMci
	.p2align	1
	.type	_Z19dictigetpreveq_testiTVMci,@function
_Z19dictigetpreveq_testiTVMci:
; %bb.0:
	DICTIGETPREVEQ NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end64:
	.size	_Z19dictigetpreveq_testiTVMci, .Lfunc_end64-_Z19dictigetpreveq_testiTVMci

	.globl	_Z19dictugetnexteq_testiTVMci
	.p2align	1
	.type	_Z19dictugetnexteq_testiTVMci,@function
_Z19dictugetnexteq_testiTVMci:
; %bb.0:
	DICTUGETNEXTEQ NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end65:
	.size	_Z19dictugetnexteq_testiTVMci, .Lfunc_end65-_Z19dictugetnexteq_testiTVMci

	.globl	_Z19dictugetpreveq_testiTVMci
	.p2align	1
	.type	_Z19dictugetpreveq_testiTVMci,@function
_Z19dictugetpreveq_testiTVMci:
; %bb.0:
	DICTUGETPREVEQ NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end66:
	.size	_Z19dictugetpreveq_testiTVMci, .Lfunc_end66-_Z19dictugetpreveq_testiTVMci

	.globl	_Z15dictminref_testTVMci
	.p2align	1
	.type	_Z15dictminref_testTVMci,@function
_Z15dictminref_testTVMci:
; %bb.0:
	DICTMINREF NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end67:
	.size	_Z15dictminref_testTVMci, .Lfunc_end67-_Z15dictminref_testTVMci

	.globl	_Z13dictimin_testTVMci
	.p2align	1
	.type	_Z13dictimin_testTVMci,@function
_Z13dictimin_testTVMci:
; %bb.0:
	DICTIMIN NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end68:
	.size	_Z13dictimin_testTVMci, .Lfunc_end68-_Z13dictimin_testTVMci

	.globl	_Z16dictiminref_testTVMci
	.p2align	1
	.type	_Z16dictiminref_testTVMci,@function
_Z16dictiminref_testTVMci:
; %bb.0:
	DICTIMINREF NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end69:
	.size	_Z16dictiminref_testTVMci, .Lfunc_end69-_Z16dictiminref_testTVMci

	.globl	_Z12dictmax_testTVMci
	.p2align	1
	.type	_Z12dictmax_testTVMci,@function
_Z12dictmax_testTVMci:
; %bb.0:
	DICTMAX NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end70:
	.size	_Z12dictmax_testTVMci, .Lfunc_end70-_Z12dictmax_testTVMci

	.globl	_Z15dictmaxref_testTVMci
	.p2align	1
	.type	_Z15dictmaxref_testTVMci,@function
_Z15dictmaxref_testTVMci:
; %bb.0:
	DICTMAXREF NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end71:
	.size	_Z15dictmaxref_testTVMci, .Lfunc_end71-_Z15dictmaxref_testTVMci

	.globl	_Z13dictimax_testTVMci
	.p2align	1
	.type	_Z13dictimax_testTVMci,@function
_Z13dictimax_testTVMci:
; %bb.0:
	DICTIMAX NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end72:
	.size	_Z13dictimax_testTVMci, .Lfunc_end72-_Z13dictimax_testTVMci

	.globl	_Z16dictimaxref_testTVMci
	.p2align	1
	.type	_Z16dictimaxref_testTVMci,@function
_Z16dictimaxref_testTVMci:
; %bb.0:
	DICTIMAXREF NULLSWAPIFNOT2
	XCHG	s0, s2
	DROP2
.Lfunc_end73:
	.size	_Z16dictimaxref_testTVMci, .Lfunc_end73-_Z16dictimaxref_testTVMci

	.globl	_Z15dictremmin_testTVMci
	.p2align	1
	.type	_Z15dictremmin_testTVMci,@function
_Z15dictremmin_testTVMci:
; %bb.0:
	DICTREMMIN NULLSWAPIFNOT2
	XCHG	s0, s3
	BLKDROP	3
.Lfunc_end74:
	.size	_Z15dictremmin_testTVMci, .Lfunc_end74-_Z15dictremmin_testTVMci

	.globl	_Z18dictremminref_testTVMci
	.p2align	1
	.type	_Z18dictremminref_testTVMci,@function
_Z18dictremminref_testTVMci:
; %bb.0:
	DICTREMMINREF NULLSWAPIFNOT2
	XCHG	s0, s3
	BLKDROP	3
.Lfunc_end75:
	.size	_Z18dictremminref_testTVMci, .Lfunc_end75-_Z18dictremminref_testTVMci

	.globl	_Z16dictiremmin_testTVMci
	.p2align	1
	.type	_Z16dictiremmin_testTVMci,@function
_Z16dictiremmin_testTVMci:
; %bb.0:
	DICTIREMMIN NULLSWAPIFNOT2
	XCHG	s0, s3
	BLKDROP	3
.Lfunc_end76:
	.size	_Z16dictiremmin_testTVMci, .Lfunc_end76-_Z16dictiremmin_testTVMci

	.globl	_Z19dictiremminref_testTVMci
	.p2align	1
	.type	_Z19dictiremminref_testTVMci,@function
_Z19dictiremminref_testTVMci:
; %bb.0:
	DICTIREMMINREF NULLSWAPIFNOT2
	XCHG	s0, s3
	BLKDROP	3
.Lfunc_end77:
	.size	_Z19dictiremminref_testTVMci, .Lfunc_end77-_Z19dictiremminref_testTVMci

	.globl	_Z15dictremmax_testTVMci
	.p2align	1
	.type	_Z15dictremmax_testTVMci,@function
_Z15dictremmax_testTVMci:
; %bb.0:
	DICTREMMAX NULLSWAPIFNOT2
	XCHG	s0, s3
	BLKDROP	3
.Lfunc_end78:
	.size	_Z15dictremmax_testTVMci, .Lfunc_end78-_Z15dictremmax_testTVMci

	.globl	_Z18dictremmaxref_testTVMci
	.p2align	1
	.type	_Z18dictremmaxref_testTVMci,@function
_Z18dictremmaxref_testTVMci:
; %bb.0:
	DICTREMMINREF NULLSWAPIFNOT2
	XCHG	s0, s3
	BLKDROP	3
.Lfunc_end79:
	.size	_Z18dictremmaxref_testTVMci, .Lfunc_end79-_Z18dictremmaxref_testTVMci

	.globl	_Z16dictiremmax_testTVMci
	.p2align	1
	.type	_Z16dictiremmax_testTVMci,@function
_Z16dictiremmax_testTVMci:
; %bb.0:
	DICTIREMMAX NULLSWAPIFNOT2
	XCHG	s0, s3
	BLKDROP	3
.Lfunc_end80:
	.size	_Z16dictiremmax_testTVMci, .Lfunc_end80-_Z16dictiremmax_testTVMci

	.globl	_Z19dictiremmaxref_testTVMci
	.p2align	1
	.type	_Z19dictiremmaxref_testTVMci,@function
_Z19dictiremmaxref_testTVMci:
; %bb.0:
	DICTIREMMAXREF NULLSWAPIFNOT2
	XCHG	s0, s3
	BLKDROP	3
.Lfunc_end81:
	.size	_Z19dictiremmaxref_testTVMci, .Lfunc_end81-_Z19dictiremmaxref_testTVMci

	.globl	_Z15subdictget_testTVMsiTVMci
	.p2align	1
	.type	_Z15subdictget_testTVMsiTVMci,@function
_Z15subdictget_testTVMsiTVMci:
; %bb.0:
	SUBDICTGET
.Lfunc_end82:
	.size	_Z15subdictget_testTVMsiTVMci, .Lfunc_end82-_Z15subdictget_testTVMsiTVMci

	.globl	_Z16subdictiget_testiiTVMci
	.p2align	1
	.type	_Z16subdictiget_testiiTVMci,@function
_Z16subdictiget_testiiTVMci:
; %bb.0:
	SUBDICTIGET
.Lfunc_end83:
	.size	_Z16subdictiget_testiiTVMci, .Lfunc_end83-_Z16subdictiget_testiiTVMci

	.globl	_Z16subdictuget_testiiTVMci
	.p2align	1
	.type	_Z16subdictuget_testiiTVMci,@function
_Z16subdictuget_testiiTVMci:
; %bb.0:
	SUBDICTUGET
.Lfunc_end84:
	.size	_Z16subdictuget_testiiTVMci, .Lfunc_end84-_Z16subdictuget_testiiTVMci

	.globl	_Z17subdictrpget_testTVMsiTVMci
	.p2align	1
	.type	_Z17subdictrpget_testTVMsiTVMci,@function
_Z17subdictrpget_testTVMsiTVMci:
; %bb.0:
	SUBDICTRPGET
.Lfunc_end85:
	.size	_Z17subdictrpget_testTVMsiTVMci, .Lfunc_end85-_Z17subdictrpget_testTVMsiTVMci

	.globl	_Z18subdictirpget_testiiTVMci
	.p2align	1
	.type	_Z18subdictirpget_testiiTVMci,@function
_Z18subdictirpget_testiiTVMci:
; %bb.0:
	SUBDICTIRPGET
.Lfunc_end86:
	.size	_Z18subdictirpget_testiiTVMci, .Lfunc_end86-_Z18subdictirpget_testiiTVMci

	.globl	_Z18subdicturpget_testiiTVMci
	.p2align	1
	.type	_Z18subdicturpget_testiiTVMci,@function
_Z18subdicturpget_testiiTVMci:
; %bb.0:
	SUBDICTURPGET
.Lfunc_end87:
	.size	_Z18subdicturpget_testiiTVMci, .Lfunc_end87-_Z18subdicturpget_testiiTVMci


	.ident	"TON Labs clang for TVM. Version 7.0.0  (based on LLVM 7.0.0)"
	.section	".note.GNU-stack","",@progbits
