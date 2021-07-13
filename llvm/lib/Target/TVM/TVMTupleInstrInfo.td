//===--- TVMTupleInstrInfo.td - TVM Instruction defs ---*--- tablegen --*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
/// \file
/// This file describes Tuple TVM instructions (A.3.2) in TableGen format.
//
//===----------------------------------------------------------------------===//

defm NIL : I<(outs Tuple:$result), (ins), (outs), (ins),
             [(set Tuple:$result, (int_tvm_nil))],
             "NIL\t$result",
             "NIL", 0x6f00>;

defm TUPLE : I<(outs Tuple:$result),
                (ins reglist:$regs, variable_ops, uimm4:$sz),
                (outs), (ins uimm4:$sz),
                [],
                "TUPLE\t$result, $sz, $regs",
                "TUPLE\t$sz", 0x6f00>;

defm TUPLEVAR : I<(outs Tuple:$result),
                  (ins reglist:$regs, variable_ops, I257:$sz),
                  (outs), (ins),
                  [],
                  "TUPLEVAR\t$result, $sz, $regs",
                  "TUPLEVAR", 0x6f80>;

defm UNTUPLE1 : I<(outs I257:$ret), (ins Tuple:$tuple), (outs), (ins), [],
                  "UNTUPLE\t1, $ret, $tuple",
                  "UNTUPLE\t1", 0x6f20>;

defm UNTUPLE : I<(outs reglist:$regs, variable_ops),
                 (ins Tuple:$tuple, uimm4:$sz),
                 (outs), (ins uimm4:$sz),
                 [],
                 "UNTUPLE\t$sz, $regs, $tuple",
                 "UNTUPLE\t$sz", 0x6f20>;

defm UNTUPLEVAR : I<(outs reglist:$regs, variable_ops),
                    (ins Tuple:$tuple, I257:$sz),
                    (outs), (ins),
                    [],
                    "UNTUPLEVAR\t$sz, $regs, $tuple",
                    "UNTUPLEVAR", 0x6f82>;

defm UNPACKFIRST1 : I<(outs I257:$ret), (ins Tuple:$tuple), (outs), (ins), [],
                     "UNPACKFIRST\t1, $ret, $tuple",
                     "UNPACKFIRST\t1", 0x6f20>;

defm UNPACKFIRST : I<(outs reglist:$regs, variable_ops),
                     (ins Tuple:$tuple, uimm4:$sz),
                     (outs), (ins uimm4:$sz),
                     [],
                     "UNPACKFIRST\t$sz, $regs, $tuple",
                     "UNPACKFIRST\t$sz", 0x6f20>;

defm UNPACKFIRSTVAR : I<(outs reglist:$regs, variable_ops),
                        (ins Tuple:$tuple, I257:$sz),
                        (outs), (ins),
                        [],
                        "UNPACKFIRSTVAR\t$sz, $regs, $tuple",
                        "UNPACKFIRSTVAR", 0x6f82>;

defm INDEX : I<(outs I257:$result), (ins Tuple:$tuple, uimm4:$idx),
               (outs), (ins uimm4:$idx),
               [(set I257:$result,
                (int_tvm_index Tuple:$tuple, uimm4:$idx))],
               "INDEX\t$result, $tuple, $idx",
               "INDEX\t$idx", 0x6f10>;
defm SETINDEX : I<(outs Tuple:$result),
                  (ins Tuple:$tuple, I257:$val, uimm4:$idx),
                  (outs), (ins uimm4:$idx),
                  [(set Tuple:$result,
                   (int_tvm_setindex Tuple:$tuple, uimm4:$idx, I257:$val))],
                  "SETINDEX\t$result, $tuple, $idx, $val",
                  "SETINDEX\t$idx", 0x6f50>;

defm INDEXQ : I<(outs I257:$result), (ins Tuple:$tuple, uimm4:$idx),
                (outs), (ins uimm4:$idx),
                [(set I257:$result,
                 (int_tvm_indexq Tuple:$tuple, uimm4:$idx))],
                "INDEXQ\t$result, $tuple, $idx",
                "INDEXQ\t$idx", 0x6f10>;
defm SETINDEXQ : I<(outs Tuple:$result),
                   (ins Tuple:$tuple, I257:$val, uimm4:$idx),
                   (outs), (ins uimm4:$idx),
                   [(set Tuple:$result,
                    (int_tvm_setindex Tuple:$tuple, uimm4:$idx, I257:$val))],
                   "SETINDEXQ\t$result, $tuple, $idx, $val",
                   "SETINDEXQ\t$idx", 0x6f50>;

defm INDEXVAR : I<(outs I257:$result), (ins Tuple:$tuple, I257:$idx),
                  (outs), (ins),
                  [(set I257:$result,
                   (int_tvm_index Tuple:$tuple, I257:$idx))],
                  "INDEXVAR\t$result, $tuple, $idx",
                  "INDEXVAR", 0x6f81>;
defm SETINDEXVAR : I<(outs Tuple:$result),
                     (ins Tuple:$tuple, I257:$idx, I257:$val),
                     (outs), (ins),
                     [(set Tuple:$result,
                      (int_tvm_setindex Tuple:$tuple, I257:$idx, I257:$val))],
                     "SETINDEXVAR\t$result, $tuple, $idx, $val",
                     "SETINDEXVAR", 0x6f85>;

defm CHKTUPLE : I<(outs), (ins Tuple:$tuple),
                  (outs), (ins),
                  [(int_tvm_chktuple Tuple:$tuple)],
                  "CHKTUPLE\t$tuple",
                  "CHKTUPLE", 0x6f30>;

defm TLEN : I<(outs I257:$tlen), (ins Tuple:$tuple),
              (outs), (ins),
              [(set I257:$tlen, (int_tvm_tlen Tuple:$tuple))],
              "TLEN\t$tlen, $tuple",
              "TLEN", 0x6f88>;

defm QTLEN : I<(outs I257:$tlen), (ins Tuple:$tuple),
               (outs), (ins),
               [(set I257:$tlen, (int_tvm_qtlen Tuple:$tuple))],
               "QTLEN\t$tlen, $tuple",
               "QTLEN", 0x6f89>;

defm ISTUPLE : I<(outs I257:$result), (ins Tuple:$tuple),
                 (outs), (ins),
                 [(set I257:$result, (int_tvm_istuple Tuple:$tuple))],
                 "ISTUPLE\t$result, $tuple",
                 "ISTUPLE", 0x6f8a>;

defm LAST : I<(outs I257:$result), (ins Tuple:$tuple),
              (outs), (ins),
              [(set I257:$result, (int_tvm_last Tuple:$tuple))],
              "LAST\t$result, $tuple",
              "LAST", 0x6f8b>;

defm TPUSH : I<(outs Tuple:$result), (ins Tuple:$tuple, I257:$val),
               (outs), (ins),
               [(set Tuple:$result, (int_tvm_tpush Tuple:$tuple, I257:$val))],
               "TPUSH\t$result, $tuple, $val",
               "TPUSH", 0x6f8c>;

defm TPOP : I<(outs Tuple:$ret_tuple, I257:$ret_val), (ins Tuple:$tuple),
              (outs), (ins),
              [(set Tuple:$ret_tuple, I257:$ret_val,
                (int_tvm_tpop Tuple:$tuple))],
              "TPOP\t$ret_tuple, $ret_val, $tuple",
              "TPOP", 0x6f8d>;

defm INDEX2 : I<(outs I257:$result), (ins Tuple:$tuple, uimm2:$i, uimm2:$j),
                (outs), (ins uimm2:$i, uimm2:$j),
                [(set I257:$result,
                  (int_tvm_index2 Tuple:$tuple, uimm2:$i, uimm2:$j))],
                "INDEX2\t$result, $i, $j, $tuple",
                "INDEX2\t$i, $j", 0x6fb00>;

defm INDEX3 : I<(outs I257:$result),
                (ins Tuple:$tuple, uimm2:$i, uimm2:$j, uimm2:$k),
                (outs), (ins uimm2:$i, uimm2:$j, uimm2:$k),
                [(set I257:$result,
                  (int_tvm_index3 Tuple:$tuple, uimm2:$i, uimm2:$j, uimm2:$k))],
                "INDEX3\t$result, $i, $j, $k, $tuple",
                "INDEX3\t$i, $j, $k", 0x6fe000>;

defm CADR : I<(outs I257:$result), (ins Tuple:$tuple),
              (outs), (ins),
              [(set I257:$result, (int_tvm_cadr Tuple:$tuple))],
              "CADR\t$result, $tuple",
              "CADR", 0x6fb4>;

defm CDDR : I<(outs I257:$result), (ins Tuple:$tuple),
              (outs), (ins),
              [(set I257:$result, (int_tvm_cddr Tuple:$tuple))],
              "CDDR\t$result, $tuple",
              "CDDR", 0x6fb5>;

defm CADDR : I<(outs I257:$result), (ins Tuple:$tuple),
               (outs), (ins),
               [(set I257:$result, (int_tvm_caddr Tuple:$tuple))],
               "CADDR\t$result, $tuple",
               "CADDR", 0x6fd4>;

defm CDDDR : I<(outs I257:$result), (ins Tuple:$tuple),
               (outs), (ins),
               [(set I257:$result, (int_tvm_cdddr Tuple:$tuple))],
               "CDDDR\t$result, $tuple",
               "CDDDR", 0x6fd5>;

// ******************************************* 
//
// A.3  Tuple, List, and Null primitives
//
// *******************************************/

// 6F00 - NIL ( - t), pushes the only Tuple t = () of length zero
//defm NIL : I<(outs Tuple:$t), (ins), (outs), (ins),
//             [(set Tuple:$t, (int_tvm_nil))],
//             "NIL\t$t",
//             "NIL", 0x6f00>;

// 6F01 - SINGLE (x - t), creates a singleton t := (x), i.e., a Tuple of length one
defm SIGNLE : I<(outs Tuple:$t), (ins I257:$x), (outs), (ins),
             [(set Tuple:$t, (int_tvm_single I257:$x))],
             "SINGLE\t$t, $x",
             "SIGNLE", 0x6f01>;
// 6F02 - PAIR or CONS (x y - t), creates pair t := (x, y)
defm PAIR : I<(outs Tuple:$t), (ins I257:$x, I257:$y), (outs), (ins),
             [(set Tuple:$t, (int_tvm_pair I257:$x, I257:$y))],
             "PAIR\t$t, $x, $y",
             "PAIR", 0x6f02>;

// 6F03 - TRIPLE (x y z - t), creates triple t := (x, y, z)
defm TRIPLE : I<(outs Tuple:$t), (ins I257:$x, I257:$y, I257:$z), (outs), (ins),
             [(set Tuple:$t, (int_tvm_triple I257:$x, I257:$y, I257:$z))],
             "TRIPLE\t$t, $x, $y, $z",
             "TRIPLE", 0x6f03>;

// 6F10 - FIRST or CAR (t - x), returns the fist element of a Tuple
defm FIRST : I<(outs I257:$x), (ins Tuple:$t), (outs), (ins),
             [(set I257:$x, (int_tvm_first Tuple:$t))],
             "FIRST\t$x, $t",
             "FIRST", 0x6f10>;

// 6F11 - SECOND or CDR (t - y), returns the second element of a Tuple
defm SECOND : I<(outs I257:$y), (ins Tuple:$t), (outs), (ins),
             [(set I257:$y, (int_tvm_second Tuple:$t))],
             "SECOND\t$y, $t",
             "SECOND", 0x6f11>;

// 6F12 - THIRD (t - z), returns the third element of a Tuple
defm THIRD : I<(outs I257:$z), (ins Tuple:$t), (outs), (ins),
             [(set I257:$z, (int_tvm_third Tuple:$t))],
             "SECOND\t$z, $t",
             "SECOND", 0x6f12>;

// 6F21 - UNSINGLE (t - x), unpacks a singleton t = (x)
defm UNSIGNLE : I<(outs I257:$x), (ins Tuple:$t), (outs), (ins),
             [(set I257:$x, (int_tvm_unsingle Tuple:$t))],
             "UNSINGLE\t$x, $t",
             "UNSIGNLE", 0x6f21>;

// 6F22 - UNPAIR or UNCONS (t - x y), unpacks a pair t = (x, y)
defm UNPAIR : I<(outs I257:$x, I257:$y), (ins Tuple:$t), (outs), (ins),
             [(set I257:$x, I257:$y, (int_tvm_unpair Tuple:$t))],
             "UNPAIR\t$x, $y, $t",
             "UNPAIR", 0x6f22>;

// 6F23 - UNTRIPLE (t - x y z), unpacks a triple t = (x, y, z)
defm UNTRIPLE : I<(outs I257:$x, I257:$y, I257:$z), (ins Tuple:$t), (outs), (ins),
             [(set I257:$x, I257:$y, I257:$z, (int_tvm_untriple Tuple:$t))],
             "UNTRIPLE\t$x, $y, $z, $t",
             "UNTRIPLE", 0x6f23>;

// 6F4n - EXPLODE	n (t - x1. . . xmm), unpacks a Tuple t = (x1, . . . , xm) and returns its length m, 
//  but only if m <= n <= 15. Otherwise throws a type check exception
// TODO

// 6F50 - SETFIRST (t x - t0), sets the first component of Tuple t to x and returns the resulting Tuple t0
defm SETFIRST : I<(outs Tuple:$ot), (ins Tuple:$it, I257:$x), (outs), (ins),
             [(set Tuple:$ot, (int_tvm_setfirst Tuple:$it, I257:$x))],
             "SETFIRST\t$ot, $it, $x",
             "SETFIRST", 0x6f50>;

// 6F51 - SETSECOND (t x - t0), sets the second component of Tuple t to x and returns the resulting Tuple t0
defm SETSECOND : I<(outs Tuple:$ot), (ins Tuple:$it, I257:$x), (outs), (ins),
             [(set Tuple:$ot, (int_tvm_setsecond Tuple:$it, I257:$x))],
             "SETSECOND\t$ot, $it, $x",
             "SETSECOND", 0x6f51>;

// 6F52 - SETTHIRD (t x - t0), sets the third component of Tuple t to x and returns the resulting Tuple t0
defm SETTHIRD : I<(outs Tuple:$ot), (ins Tuple:$it, I257:$x), (outs), (ins),
             [(set Tuple:$ot, (int_tvm_setthird Tuple:$it, I257:$x))],
             "SETTHIRD\t$ot, $it, $x",
             "SETTHIRD", 0x6f52>;

// 6F83 - UNPACKFIRSTVAR (t n - x1. . . xn), similar to UNPACKFIRST n, but with 0 <= n <= 255 taken from the stack
// TODO

// 6F84 - EXPLODEVAR (t n - x1. . . xmm), similar to EXPLODE n, but with 0 <= n <= 255 taken from the stack
// TODO

// 6F86 - INDEXVARQ (t k - x), similar to INDEXQ n, but with 0 <= k <= 254 taken from the stack
defm INDEXVARQ : I<(outs I257:$x), (ins Tuple:$t, I257:$k),
                  (outs), (ins),
                  [(set I257:$x, (int_tvm_indexvarq Tuple:$t, I257:$k))],
                  "INDEXVARQ\t$x, $t, $k",
                  "INDEXVARQ", 0x6f86>;

// 6F87 - SETINDEXVARQ (t x k - t0), similar to SETINDEXQ	k, but with 0 <= k <= 254 taken from the stack
defm SETINDEXVARQ : I<(outs Tuple:$ot), (ins Tuple:$it, I257:$x, I257:$k),
                  (outs), (ins),
                  [(set Tuple:$ot, (int_tvm_setindexvarq Tuple:$it, I257:$x, I257:$k))],
                  "SETINDEXVARQ\t$ot, $it, $x, $k",
                  "SETINDEXVARQ", 0x6f87>;


