//===---- TVMApplicationInstrInfo.td - A11 instructions ----*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes A11 application specific instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instruction list..
defm ACCEPT       : I0<(outs), (ins), [(int_tvm_accept)],
                       "ACCEPT", 0xf800>;

defm SETGASLIMIT  : I0<(outs), (ins I257:$limit), [(int_tvm_setgaslimit I257:$limit)],
                       "SETGASLIMIT", 0xf801>;

defm BUYGAS    : I0<(outs), (ins I257:$tons), [(int_tvm_buygas I257:$tons)],
                    "BUYGAS", 0xf802>;

defm GRAMTOGAS : I0<(outs I257:$gas), (ins I257:$tons),
                    [(set I257:$gas, (int_tvm_gramtogas I257:$tons))],
                     "GRAMTOGAS", 0xf804>;

defm GASTOGRAM : I0<(outs I257:$tons), (ins I257:$gas),
                    [(set I257:$tons, (int_tvm_gastogram I257:$gas))],
                     "GASTOGRAM", 0xf805>;

defm COMMIT   : I0<(outs), (ins), [(int_tvm_commit)],
                   "COMMIT", 0xf80f>;

defm HASHCU   : I0<(outs I257 : $result), (ins Cell : $cell),
                   [(set I257 : $result, (int_tvm_hashcu Cell : $cell))],
                   "HASHCU", 0xf900>;

defm HASHSU   : I0<(outs I257 : $result), (ins Slice : $slice),
                   [(set I257 : $result, (int_tvm_hashsu Slice : $slice))],
                   "HASHSU", 0xf901>;

defm MYCODE : I0<(outs Cell:$code), (ins),
                 [(set Cell:$code, (int_tvm_mycode))],
                 "MYCODE", 0xffff>; // TODO: fix MYCODE hex id

defm CHKSIGNU : I0<(outs I257 : $result),
                   (ins I257 : $hash, Slice : $signature, I257 : $key),
                   [(set I257 : $result, (int_tvm_chksignu I257  : $hash,
                                                           Slice : $signature,
                                                           I257  : $key))],
                   "CHKSIGNU", 0xf910>;

def : Pat<(int_tvm_get_temporary_data), (TO_TUPLE_COPY (PUSHC 7))>;

// A.11.4. Configuration primitives.

defm GETPARAM : I<(outs I257:$rv), (ins uimm4:$idx), (outs), (ins uimm4:$idx),
                  [(set I257:$rv, (int_tvm_getparam uimm4:$idx))],
                  "GETPARAM\t$idx", "GETPARAM\t$idx", 0xf820>;
def : Pat<(int_tvm_now), (GETPARAM 3)>;
def : Pat<(int_tvm_blocklt), (GETPARAM 4)>;
def : Pat<(int_tvm_ltime), (GETPARAM 5)>;
def : Pat<(int_tvm_myaddr), (TO_SLICE_COPY (GETPARAM 8))>;
def : Pat<(int_tvm_configroot), (TO_CELL_COPY (GETPARAM 9))>;

defm CONFIGDICT : I<(outs Cell:$dict, I257:$keylen), (ins), (outs), (ins),
                    [(set Cell:$dict, I257:$keylen, (int_tvm_configdict))],
                    "CONFIGDICT\t$dict, $keylen", "CONFIGDICT", 0xf830>;

defm CONFIGPARAM : I<(outs Cell:$param, I257:$succ), (ins I257:$idx),
                     (outs), (ins),
                     [(set Cell:$param, I257:$succ,
                       (int_tvm_configparam I257:$idx))],
                     "CONFIGPARAM\t$param, $succ, $idx", "CONFIGPARAM NULLSWAPIFNOT", 0xf832>;

defm CONFIGOPTPARAM : I<(outs Cell:$param), (ins I257:$idx), (outs), (ins),
                        [(set Cell:$param, (int_tvm_configoptparam I257:$idx))],
                        "CONFIGOPTPARAM\t$param, $idx", "CONFIGOPTPARAM",
                        0xf833>;

// A.11.5. Global variable primitives.
defm GETGLOB : I<(outs I257:$rv), (ins uimm1_31:$idx), (outs), (ins uimm1_31:$idx),
                 [(set I257:$rv, (int_tvm_getglobal uimm1_31:$idx))],
                 "GETGLOB\t$rv, $idx", "GETGLOB\t$idx", 0xf8500>;
defm GETGLOBVAR : I<(outs I257:$rv), (ins I257:$idx), (outs), (ins),
                    [(set I257:$rv, (int_tvm_getglobal I257:$idx))],
                    "GETGLOBVAR\t$rv, $idx", "GETGLOBVAR", 0xf840>;
defm SETGLOB : I<(outs), (ins I257:$val, uimm1_31:$idx), (outs), (ins uimm1_31:$idx),
                 [(int_tvm_setglobal uimm1_31:$idx, I257:$val)],
                 "SETGLOB\t$idx, $val", "SETGLOB\t$idx", 0xf8700>;
defm SETGLOBVAR : I<(outs), (ins I257:$val, I257:$idx), (outs), (ins),
                    [(int_tvm_setglobal I257:$idx, I257:$val)],
                    "SETGLOBVAR\t$idx, $val", "SETGLOBVAR", 0xf860>;

// A.11.7. Misc primitives.
defm CDATASIZE : I<(outs I257:$numCells, I257:$numBits, I257:$numRefs),
                   (ins Cell:$cell, I257:$maxCells), (outs), (ins),
                   [(set I257:$numCells, I257:$numBits, I257:$numRefs, (int_tvm_cdatasize Cell:$cell, I257:$maxCells))],
                   "CDATASIZE\t$numCells, $numBits, $numRefs", "CDATASIZE", 0xf941>;

// A.11.10 Outbound message and output action primitives.
defm RAWRESERVE : I<(outs), (ins I257:$x, I257:$y), (outs), (ins),
                    [(int_tvm_rawreserve I257:$x, I257:$y)],
                    "RAWRESERVE\t$x, $y", "RAWRESERVE", 0xfb02>;

defm SETCODE : I<(outs), (ins Cell:$cell), (outs), (ins),
                 [(int_tvm_setcode Cell:$cell)],
                 "SETCODE\t$cell", "SETCODE", 0xfb04>;

// ******************************************* 
//
// A.11.3. Pseudo-random number generator primitives
//
// *******************************************/

// F810	- RANDU256 ( -	x), generates a new pseudo-random unsigned 256-bit Integer x
let hasSideEffects = 1 in
defm RANDU256 : I0<(outs I257:$x), (ins),
                    [(set I257:$x, (int_tvm_randu256))],
                     "RANDU256", 0xf810>;

// F811 - RAND (y - z), generates a new pseudo-random integer z in the range 0...y-1 (or y...-1, if y < 0)
let hasSideEffects = 1 in
defm RAND : I<(outs I257:$z), (ins I257:$y),
               (outs), (ins), 
               [(set I257:$z, (int_tvm_rand I257:$y))],
                     "RAND\t$y", "RAND", 0xf811>;

// F814	- SETRAND  (x	- ), sets the random seed to unsigned 256-bit Integer x
let hasSideEffects = 1 in
defm SETRAND : I<(outs), (ins I257:$x),
                 (outs), (ins),
                    [(int_tvm_setrand I257:$x)],
                     "SETRAND\t$x", "SETRAND", 0xf814>;

// F815 - ADDRAND (x - ), mixes unsigned 256-bit Integer x into the random seed r
let hasSideEffects = 1 in
defm ADDRAND : I<(outs), (ins I257:$x),
                  (outs), (ins),
                  [(int_tvm_addrand I257:$x)],
                     "ADDRAND\t$x", "ADDRAND", 0xf815>;

// ******************************************* 
//
// A.11.4. Configuration primitives
//
// *******************************************/

// F826	- RANDSEED ( -	x), returns the current random seed as an unsigned 256-bit Integer. Equivalent to GETPARAM 6
defm RANDSEED : I<(outs I257:$x), (ins),
                  (outs), (ins),
                  [(set I257:$x, (int_tvm_randseed))],
                     "RANDSEED\t$x", "RANDSEED", 0xf826>;

// F827	- BALANCE ( -	t), returns the remaining balance of the smart contract as a Tuple consisting of an Integer (the remaining Gram balance in nanograms) 
//   and a Maybe Cell (a dictionary with 32-bit keys representing the balance of "extra currencies"). Equivalent to GETPARAM 7
defm BALANCE : I<(outs Tuple:$t), (ins),
                  (outs), (ins),
                  [(set Tuple:$t, (int_tvm_balance))],
                     "BALANCE\t$t", "BALANCE", 0xf827>;

// ******************************************* 
//
// A.11.6. Hashing and cryptography primitives
//
// *******************************************/

// F902 - SHA256U (s - x), computes sha256 of the data bits of Slice s.
//  If the bit length of s is not divisible by eight, throws a cell underflow exception. The hash value is returned as a 256-bit unsigned integer x
defm SHA256U : I<(outs I257:$x), (ins Slice:$s),
                  (outs), (ins),
                  [(set I257:$x, (int_tvm_sha256u Slice:$s))],
                     "SHA256U\t$x, $s", "SHA256U", 0xf902>;

// F911	- CHKSIGNS  (d s k - ?) checks whether	s is a valid Ed25519-signature of the data portion of Slice	d using public key k, similarly to CHKSIGNU
defm CHKSIGNS : I<(outs I257:$result), (ins Slice:$d, I257:$s, I257:$k),
                  (outs), (ins),
                  [(set I257:$result, (int_tvm_chksigns Slice:$d, I257:$s, I257:$k))],
                     "CHKSIGNS\t$result, $d, $s, $k", "CHKSIGNS", 0xf911>;

// ******************************************* 
//
// A.11.7. Miscellaneous primitives
//
// *******************************************/

// F940	- CDATASIZEQ (c n - x y z -1 or 0), recursively computes the count of distinct cells x, data bits y, 
//   and cell references z in the dag rooted at Cell c
defm CDATASIZEQ : I<(outs I257:$x, I257:$y, I257:$z, I257:$result), (ins Cell:$c, I257:$n),
                  (outs), (ins),
                  [(set I257:$x, I257:$y, I257:$z, I257:$result, (int_tvm_cdatasizeq Cell:$c, I257:$n))],
                     "CDATASIZEQ\t$x, $y, $z,$result, $c, $n", "CDATASIZEQ", 0xf940>;


// ******************************************* 
//
// A.11.10. Outbound message and output action primitives
//
// *******************************************/

// FB03	- RAWRESERVEX  (x D y - ), similar to RAWRESERVE, but also accepts a dictionary D (represented by a Cell or Null ) with extra currencies
let hasSideEffects = 1 in
defm RAWRESERVEX : I<(outs), (ins I257:$x, Cell:$d, I257:$y),
                  (outs), (ins),
                  [(int_tvm_rawreservex I257:$x, Cell:$d, I257:$y)],
                     "RAWRESERVEX\t$x, $d, $y", "RAWRESERVEX", 0xfb03>;

// FB06 - SETLIBCODE (c x - ), creates an output action that would modify the collection of this smart contract libraries by adding or removing
//   library with code given in	Cell c
let hasSideEffects = 1 in
defm SETLIBCODE : I<(outs), (ins Cell:$c, I257:$x),
                  (outs), (ins),
                  [(int_tvm_setlibcode Cell:$c, I257:$x)],
                     "SETLIBCODE\t$c, $x", "SETLIBCODE", 0xfb06>;

// FB07	- CHANGELIB (h x - ), creates an output action similarly to SETLIBCODE, 
//   but instead of the library code accepts its hash as an unsigned 256-bit integer
let hasSideEffects = 1 in
defm CHANGELIB : I<(outs), (ins I257:$h, I257:$x),
                  (outs), (ins),
                  [(int_tvm_changelib i257:$h, I257:$x)],
                     "CHANGELIB\t$h, $x", "CHANGELIB", 0xfb07>;
