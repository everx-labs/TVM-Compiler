#!/usr/bin/python

inst_classes = {

    # A.2 Stack manipulation primitives
    'stack_mapulation': [

        # A.2.1. Basic stack manipulation primitives
        'NOP',
        'XCHG',
        'PUSH',
        'POP',
        'DUP',
        'OVER',
        'DROP',
        'NIP',
        'SWAP',

        # A.2.2. Compound stack manipulation primitives
        'XCHG3',
        'XCHG2',
        'XCPU',
        'XCHG3',
        'XCHG2',
        'XCPU',
        'PUXC',
        'PUSH2',
        'XC2PU',
        'XCPUXC',
        'XCPU2',
        'PUXC2',
        'PUXCPU',
        'PU2XC',
        'PUSH3',

        # A.2.3. Exotic stack manipulation primitives
        'BLKSWAP',
        'ROT2',
        'ROLL',
        'ROLLREV',
        'PUSH',
        'POP',
        'ROT',
        'ROTREV',
        'SWAP2',
        'DROP2',
        'DUP2',
        'OVER2',
        'REVERSE',
        'BLKDROP',
        'BLKPUSH',
        'PICK',
        'ROLLX',
        'ROLLREVX',
        'BLKSWX',
        'REVX',
        'DROPX',
        'TUCK',
        'XCHGX',
        'DEPTH',
        'CHKDEPTH',
        'ONLYTOPX',
        'ONLYX',
    ],

    # A.3 Tuple, List, and Null primitives
    'tuples': [

        # A.3.1. Null primitives
        'NULL',
        'PUSHNULL',
        'ISNULL',

        # A.3.2. Tuple primitives
        'TUPLE',
        'NIL',
        'SINGLE',
        'PAIR',
        'TRIPLE',
        'INDEX',
        'FIRST',
        'SECOND',
        'THIRD',
        'UNTUPLE',
        'UNSINGLE',
        'UNPAIR',
        'UNTRIPLE',
        'UNPACKFIRST',
        'CHKTUPLE',
        'EXPLODE',
        'SETINDEX',
        'SETFIRST',
        'SETSECOND',
        'SETTHIRD',
        'INDEXQ',
        'SETINDEXQ',
        'TUPLEVAR',
        'INDEXVAR',
        'UNTUPLEVAR',
        'UNPACKFIRSTVAR',
        'EXPLODEVAR',
        'SETINDEXVAR',
        'INDEXVARQ',
        'SETINDEXVARQ',
        'TLEN',
        'QTLEN',
        'ISTUPLE',
        'LAST',
        'TPUSH',
        'TPOP',
        'NULLSWAPIF',
        'NULLSWAPIFNOT',
        'NULLROTRIF',
        'NULLROTRIFNOT',
        'INDEX2',
        'CADR',
        'CDDR',
        'INDEX3',
        'CADDR',
        'CDDDR',
    ],

    # A.4 Constant, or literal primitives
    'const_literal': [

        # A.4.1. Integer and boolean constants
        'PUSHINT',
        'ZERO',
        'ONE',
        'TWO',
        'TEN',
        'TRUE',
        'PUSHPOW2',
        'PUSHNAN',
        'PUSHPOW2DEC',
        'PUSHNEGPOW2',

        # A.4.2. Constant slices, continuations, cells, and references
        'PUSHREF',
        'PUSHREFSLICE',
        'PUSHREFCONT',
        'PUSHSLICE',
        'PUSHCONT',
    ],

    # A.5 Arithmetic primitives
    'arithmetic': [

        # A.5.1. Addition, subtraction, multiplication
        'ADD',
        'SUB',
        'SUBR',
        'NEGATE',
        'INC',
        'DEC',
        'ADDCONST',
        'MULCONST',
        'MUL',

        # A.5.2. Division.
        'DIV',
        'DIVR',
        'DIVC',
        'MOD',
        'DIVMOD',
        'DIVMODR',
        'DIVMODC',
        'MODPOW2',
        'MULDIVR',
        'MULDIVMOD',

        # A.5.3. Shifts, logical operations.
        'LSHIFT',
        'RSHIFT',
        'POW2',
        'AND',
        'OR',
        'XOR',
        'NOT',
        'FITS',
        'CHKBOOL',
        'UFITS',
        'CHKBIT',
        'FITSX',
        'UFITSX',
        'BITSIZE',
        'UBITSIZE',
        'MIN',
        'MAX',
        'MINMAX',
        'INTSORT2',
        'ABS',

        # A.5.4. Quiet arithmetic primitives
        'QABS',
        'QADD',
        'QADDCONST',
        'QAND',
        'QBITSIZE',
        'QDEC',
        'QDIV',
        'QDIVC',
        'QDIVR',
        'QDIVMOD',
        'QDIVMODC',
        'QDIVMODR',
        'QFITS',
        'QFITSX',
        'QINC',
        'QINTSORT2',
        'QMAX',
        'QMIN',
        'QMINMAX',
        'QMOD',
        'QMODC',
        'QMODR',
        'QMUL',
        'QMULCONST',
        'QMULDIV',
        'QMULDIVC',
        'QMULDIVR',
        'QMULDIVMOD',
        'QMULDIVMODC',
        'QMULDIVMODR',
        'QMULMOD',
        'QMULMODC',
        'QMULMODR',
        'QNEGATE',
        'QNOT',
        'QOR',
        'QPOW2',
        'QSUB',
        'QSUBR',
        'QTLEN',
        'QUBITSIZE',
        'QUFITS',
        'QUFITSX',
        'QXOR',
    ],

    # A.6 Comparison primitives
    'comparison': [

        # A.6.1 Integer comparison
        'SGN',
        'LESS',
        'EQUAL',
        'LEQ',
        'GREATER',
        'NEQ',
        'GEQ',
        'CMP',
        'EQINT',
        'ISZERO',
        'LESSINT',
        'ISNEG',
        'ISNPOS',
        'GTINT',
        'ISPOS',
        'ISNNEG',
        'NEQINT',
        'ISNAN',
        'CHKNAN',
        'QSGN',
        'QLESS',
        'QEQUAL',
        'QLEQ',
        'QGREATER',
        'QNEQ',
        'QGEQ',
        'QCMP',
        'QEQINT',
        'QISZERO',
        'QLESSINT',
        'QISNEG',
        'QISNPOS',
        'QGTINT',
        'QISPOS',
        'QISNNEG',
        'QNEQINT',
        'QISNAN',
        'QCHKNAN',

        # A.6.2. Other comparison
        'SEMPTY',
        'SDEMPTY',
        'SREMPTY',
        'SDFIRST',
        'SDLEXCMP',
        'SDEQ',
        'SDPFX',
        'SDPFXREV',
        'SDPPFX',
        'SDPPFXREV',
        'SDSFX',
        'SDSFXREV',
        'SDPSFX',
        'SDPSFXREV',
        'SDCNTLEAD0',
        'SDCNTLEAD1',
        'SDCNTTRAIL0',
        'SDCNTTRAIL1',
    ],

    # A.7 Cell primitives
    'cell': [

        # A.7.1 Cell serialization primitives
        'BBITREFS',
        'BBITS',
        'BCHKBITREFS',
        'BCHKBITREFSQ',
        'BCHKBITS',
        'BCHKBITSQ',
        'BCHKREFS',
        'BCHKREFSQ',
        'BREFS',
        'BREMBITREFS',
        'BREMBITS',
        'BREMREFS',
        'ENDC',
        'ENDXC',
        'NEWC',
        'STB',
        'STBQ',
        'STBR',
        'STBREF',
        'STBREFQ',
        'STBREFR',
        'STBREFRQ',
        'STBRQ',
        'STI',
        'STILE4',
        'STILE8',
        'STIQ',
        'STIR',
        'STIRQ',
        'STIX',
        'STIXQ',
        'STIXR',
        'STIXRQ',
        'STONES',
        'STREF',
        'STREF2CONST',
        'STREF3CONST',
        'STREFCONST',
        'STREFQ',
        'STREFR',
        'STREFRQ',
        'STSAME',
        'STSLICE',
        'STSLICECONST',
        'STSLICEQ',
        'STSLICER',
        'STSLICERQ',
        'STU',
        'STULE4',
        'STULE8',
        'STUQ',
        'STUR',
        'STURQ',
        'STUX',
        'STUXQ',
        'STUXR',
        'STUXRQ',
        'STZEROES',

        # A.7.2 Cell deserialization primitives.
        'CTOS',
        'ENDS',
        'LDI',
        'LDILE4',
        'LDILE4Q',
        'LDILE8',
        'LDILE8Q',
        'LDIQ',
        'LDIX',
        'LDIXQ',
        'LDONES',
        'LDREF',
        'LDREFRTOS',
        'LDSAME',
        'LDSLICE',
        'LDSLICEQ',
        'LDSLICEX',
        'LDSLICEXQ',
        'LDU',
        'LDULE4',
        'LDULE4Q',
        'LDULE8',
        'LDULE8Q',
        'LDUQ',
        'LDUX',
        'LDUXQ',
        'LDZEROES',
        'PLDI',
        'PLDILE4',
        'PLDILE4Q',
        'PLDILE8',
        'PLDILE8Q',
        'PLDIQ',
        'PLDIX',
        'PLDIXQ',
        'PLDREF',
        'PLDREFIDX',
        'PLDREFVAR',
        'PLDSLICE',
        'PLDSLICEQ',
        'PLDSLICEX',
        'PLDSLICEXQ',
        'PLDU',
        'PLDULE4',
        'PLDULE4Q',
        'PLDULE8',
        'PLDULE8Q',
        'PLDUQ',
        'PLDUX',
        'PLDUXQ',
        'PLDUZ',
        'SBITREFS',
        'SBITS',
        'SCHKBITREFS',
        'SCHKBITREFSQ',
        'SCHKBITS',
        'SCHKBITSQ',
        'SCHKREFS',
        'SCHKREFSQ',
        'SCUTFIRST',
        'SCUTLAST',
        'SDBEGINS',
        'SDBEGINSQ',
        'SDBEGINSX',
        'SDBEGINSXQ',
        'SDCUTFIRST',
        'SDCUTLAST',
        'SDSKIPFIRST',
        'SDSKIPLAST',
        'SDSUBSTR',
        'SPLIT',
        'SPLITQ',
        'SREFS',
        'SSKIPFIRST',
        'SSKIPLAST',
        'SUBSLICE',
        'XCTOS',
        'XLOAD',
        'XLOADQ',
    ],

    # A.8 Continuation and control flow primitives
    'control_flow': [

        # A.8.1 Unconditional control flow primitives.
        'BRANCH',
        'CALLCC',
        'CALLCCARGS',
        'CALLCCVARARGS',
        'CALLREF',
        'CALLXARGS',
        'CALLXVARARGS',
        'EXECUTE',
        'JMPREF',
        'JMPREFDATA',
        'JMPX',
        'JMPXARGS',
        'JMPXDATA',
        'JMPXVARARGS',
        'RET',
        'RETALT',
        'RETARGS',
        'RETDATA',
        'RETVARARGS',

        # A.8.2. Conditional control flow primitives
        'CONDSEL',
        'CONDSELCHK',
        'IF',
        'IFBITJMP',
        'IFBITJMPREF',
        'IFELSE',
        'IFJMP',
        'IFJMPREF',
        'IFNBITJMP',
        'IFNBITJMPREF',
        'IFNOT',
        'IFNOTJMP',
        'IFNOTJMPREF',
        'IFNOTREF',
        'IFNOTRET',
        'IFNOTRETALT',
        'IFREF',
        'IFRET',
        'IFRETALT',

        # A.8.3. Control flow primitives: loops
        'REPEAT',
        'REPEATEND',
        'UNTIL',
        'UNTILEND',
        'WHILE',
        'WHILEEND',
        'AGAIN',
        'AGAINEND',

        # A.8.4. Manipulating the stack of continuations.
        'SETCONTARGS',
        'SETNUMARGS',
        'SETCONTARGS',
        'RETURNARGS',
        'RETURNVARARGS',
        'SETCONTVARARGS',
        'SETNUMVARARGS',

        # A.8.5. Creating simple continuations and closures
        'BLESS',
        'BLESSVARARGS',
        'BLESSARGS',
        'BLESSNUMARGS',

        # A.8.6. Operations with continuation savelists and control registers.
        #'PUSH c(x)',
        'PUSHCTR',
        #'POP c(x)',
        'POPCTR',
        'PUSHROOT',
        'POPROOT',
        'SETCONT',
        'SETCONTCTR',
        'SETRETCTR',
        'SETALTCTR',
        'POPSAVE',
        'SAVE',
        'SAVEALT',
        'SAVEBOTH',
        'PUSHCTRX',
        'POPCTRX',
        'SETCONTCTRX',
        'COMPOS',
        'COMPOSALT',
        'COMPOSBOTH',
        'ATEXIT',
        'ATEXITALT',
        'SETEXITALT',
        'THENRET',
        'THENRETALT',
        'INVERT',
        'BOOLEVAL',
        'BLESSARGS',
        'BOOLOR',
        'BOOLAND',

        # A.8.7. Dictionary subroutine calls and jumps
        'CALL',
        'CALLDICT',
        'JMP',
        'PREPARE',

    ],

    'exceptions': [

        # A.9.1. Throwing exceptions.
        'THROW',
        'THROWANY',
        'THROWANYIF',
        'THROWANYIFNOT',
        'THROWARG',
        'THROWARGANY',
        'THROWARGANYIF',
        'THROWARGANYIFNOT',
        'THROWARGIF',
        'THROWARGIFNOT',
        'THROWIF',
        'THROWIFNOT',

        # A.9.2. Catching and handling exceptions.
        'TRY',
        'TRYARGS'
    ],

    'dictionary': [

        # A.10.1. Dictionary creation.
        'NEWDICT',
        'DICTEMPTY',

        # A.10.2. Dictionary serialization and deserialization.
        'LDDICT',
        'LDDICTQ',
        'LDDICTS',
        'PLDDICT',
        'PLDDICTQ',
        'PLDDICTS',
        'SKIPDICT',
        'STDICT',
        'STDICTS',

        # A.10.3. Get dictionary operations.
        'DICTGET',
        'DICTGETREF',
        'DICTIGET',
        'DICTIGETREF',
        'DICTUGET',
        'DICTUGETREF',

        # A.10.4. Set/Replace/Add dictionary operations.
        'DICTADD',
        'DICTADDGET',
        'DICTADDGETREF',
        'DICTADDREF',
        'DICTIADD',
        'DICTIADDGET',
        'DICTIADDGETREF',
        'DICTIADDREF',
        'DICTIREPLACE',
        'DICTIREPLACEGET',
        'DICTIREPLACEGETREF',
        'DICTIREPLACEREF',
        'DICTISET',
        'DICTISETGET',
        'DICTISETGETREF',
        'DICTISETREF',
        'DICTREPLACE',
        'DICTREPLACEGET',
        'DICTREPLACEGETREF',
        'DICTREPLACEREF',
        'DICTSET',
        'DICTSETGET',
        'DICTSETGETREF',
        'DICTSETREF',
        'DICTUADD',
        'DICTUADDGET',
        'DICTUADDGETREF',
        'DICTUADDREF',
        'DICTUREPLACE',
        'DICTUREPLACEGET',
        'DICTUREPLACEGETREF',
        'DICTUREPLACEREF',
        'DICTUSET',
        'DICTUSETGET',
        'DICTUSETGETREF',
        'DICTUSETREF',

        # A.10.5. Builder-accepting variants of Set dictionary operations
        'DICTSETB',
        'DICTISETB',
        'DICTUSETB',
        'DICTSETGETB',
        'DICTISETGETB',
        'DICTUSETGETB',
        'DICTREPLACEB',
        'DICTIREPLACEB',
        'DICTUREPLACEB',
        'DICTREPLACEGETB',
        'DICTIREPLACEGETB',
        'DICTUREPLACEGETB',
        'DICTADDB',
        'DICTIADDB',
        'DICTUADDB',
        'DICTADDGETB',
        'DICTIADDGETB',
        'DICTUADDGETB',

        # A.10.6. Delete dictionary operations.
        'DICTDEL',
        'DICTIDEL',
        'DICTUDEL',
        'DICTDELGET',
        'DICTDELGETREF',
        'DICTIDELGET',
        'DICTIDELGETREF',
        'DICTUDELGET',
        'DICTUDELGETREF',

        # A.10.7. "Maybe reference" dictionary operations
        'DICTGETOPTREF',
        'DICTIGETOPTREF',
        'DICTUGETOPTREF',
        'DICTSETGETOPTREF',
        'DICTISETGETOPTREF',
        'DICTUSETGETOPTREF',

        # A.10.8. Prefix code dictionary operations
        'PFXDICTSET',
        'PFXDICTREPLACE',
        'PFXDICTADD',
        'PFXDICTDEL',

        # A.10.9. Variants of GetNext and GetPrev operations
        'DICTGETNEXT',
        'DICTGETNEXTEQ',
        'DICTGETPREV',
        'DICTGETPREVEQ',
        'DICTIGETNEXT',
        'DICTIGETNEXTEQ',
        'DICTIGETPREV',
        'DICTIGETPREVEQ',
        'DICTUGETNEXT',
        'DICTUGETNEXTEQ',
        'DICTUGETPREV',
        'DICTUGETPREVEQ',

        # A.10.10. GetMin, GetMax, RemoveMin, RemoveMax operations
        'DICTMIN',
        'DICTMINREF',
        'DICTIMIN',
        'DICTIMINREF',
        'DICTUMIN',
        'DICTUMINREF',
        'DICTMAX',
        'DICTMAXREF',
        'DICTIMAX',
        'DICTIMAXREF',
        'DICTUMAX',
        'DICTUMAXREF',
        'DICTREMMIN',
        'DICTREMMINREF',
        'DICTIREMMIN',
        'DICTIREMMINREF',
        'DICTUREMMIN',
        'DICTUREMMINREF',
        'DICTREMMAX',
        'DICTREMMAXREF',
        'DICTIREMMAX',
        'DICTIREMMAXREF',
        'DICTUREMMAX',
        'DICTUREMMAXREF',

        # A.10.11. Special Get dictionary and prefix code dictionary operations, and constant dictionaries.
        'DICTIGETJMP',
        'DICTUGETJMP',
        'DICTIGETEXEC',
        'DICTUGETEXEC',
        'DICTPUSHCONST',
        'PFXDICTGETQ',
        'PFXDICTGET',
        'PFXDICTGETJMP',
        'PFXDICTGETEXEC',
        'PFXDICTCONSTGETJMP',

        # A.10.12. SubDict dictionary operations.
        'SUBDICTGET',
        'SUBDICTIGET',
        'SUBDICTUGET',
        'SUBDICTRPGET',
        'SUBDICTIRPGET',
        'SUBDICTURPGET',
    ],

    # A.11 Application-specific primitives
    'application': [

        # A.11.1. External actions and access to blockchain configuration data.

        # A.11.2. Gas-related primitives
        'ACCEPT',
        'SETGASLIMIT',
        'BUYGAS',
        'GRAMTOGAS',
        'GASTOGRAM',

        # A.11.3. Pseudo-random number generator primitives.

        # A.11.4. Configuration primitives
        'GETPARAM',
        'NOW',
        'BLOCKLT',
        'LTIME',
        'MYADDR',
        'CONFIGROOT',
        'CONFIGDICT',
        'CONFIGPARAM',
        'CONFIGOPTPARAM',

        # A.11.5. Global variable primitives
        'GETGLOBVAR',
        'GETGLOB',
        'SETGLOBVAR',
        'SETGLOB',

        # A.11.6. Hashing and cryptography primitives.
        'HASHCU',
        'HASHSU',
        'SHA256U',
        'CHKSIGNU',
        'CHKSIGNS',

        # A.11.7. Currency manipulation primitives.
        'LDGRAMS',
        'LDVARINT16',
        'STGRAMS',
        'STVARINT16',
        'LDVARUINT32',
        'LDVARINT32',
        'STVARUINT32',
        'STVARINT32',

        # A.11.8. Message and address manipulation primitives.
        'LDMSGADDR',
        'LDMSGADDRQ',
        'PARSEMSGADDR',
        'PARSEMSGADDRQ',
        'REWRITESTDADDR',
        'REWRITESTDADDRQ',
        'REWRITEVARADDR',
        'REWRITEVARADDRQ',

        # A.11.9. Outbound message and output action primitives
        'SENDRAWMSG',
        'RAWRESERVE',
        'RAWRESERVEX',
        'SETCODE',
    ],

    'debug' : [

        # A.12.1. Debug primitives as multibyte NOPs.
        'DEBUG',
        'DEBUGSTR',

        # A.12.2. Debug primitives as operations without side-effect
        'DUMPSTK',
        'DUMPSTKTOP',
        'HEXDUMP',
        'HEXPRINT',
        'BINDUMP',
        'BINPRINT',
        'STRDUMP',
        'STRPRINT',
        'DEBUGOFF',
        'DEBUGON',
        'DUMP',
        'PRINT',
        'DUMPTOSFMT',
        'LOGSTR',
        'LOGFLUSH',
        'PRINTSTR',
    ],

    # A.13 Codepage primitives
    'codepage_primitives': [
        'SETCP',
        'SETCP0',
        'SETCPX',
    ],

    'implicit': [
        'IMPLICIT'
    ]
}

import re
import argparse
import sys

def main():

    inst2class = dict()
    for c, insts in inst_classes.items():
        for i in insts:
            inst2class[i] = c

    parser = argparse.ArgumentParser()
    parser.add_argument('--exec-log', help='TVM execution log (tvm_linker trace)')
    parser.add_argument('--dump-inst', action='store_true', help='dump instruction info')
    args = parser.parse_args()

    stats = {k: 0 for k in inst_classes.keys()}
    with open(args.exec_log) as f:
        l2 = ''
        l1 = ''
        l = f.readline()
        while l:
            # parse gas log
            m = re.match(r'Gas\: (\d+) \((\d+)\)', l)
            if m:
                inst_gas = int(m.group(2))
                # parse instruction log
                m = re.match(r'(\d+)\: ([^,\n]+)', l2)
                order = int(m.group(1))
                inst = m.group(2).split(' ')[0]
                if inst not in inst2class:
                    raise RuntimeError('unknown instruction: ' + inst)
                inst_class = inst2class[inst]
                if args.dump_inst:
                    print(order, inst, inst_class)

                # calc stats
                stats[inst_class] += inst_gas

            l2 = l1
            l1 = l
            l = f.readline()

    total = sum(stats.values())
    print('cumulative gas usage statistics:')
    if total == 0:
        sys.exit(0)
    for c, g in stats.items():
        print(c, g, '{0:.3g}'.format(100 * float(g) / total), '%')

if __name__=='__main__':
    main()
