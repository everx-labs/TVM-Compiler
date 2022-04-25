//==-- TVMMsgAddressInstrInfo.td - TVM Instruction defs --*-- tablegen --*--==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
/// \file
/// Message and address manipulation primitives (A.11.8) in TableGen format.
//
//===----------------------------------------------------------------------===//

defm LDMSGADDR : I<(outs Slice:$prefix, Slice:$remainder), (ins Slice:$sl),
                   (outs), (ins),
                   [(set Slice:$prefix, Slice:$remainder,
                     (int_tvm_ldmsgaddr Slice:$sl))],
                   "LDMSGADDR\t$prefix, $remainder, $sl",
                   "LDMSGADDR", 0xfa40>;
defm LDMSGADDRQ : I<(outs Slice:$prefix, Slice:$remainder, I257:$succ),
                    (ins Slice:$sl),
                    (outs), (ins),
                    [(set Slice:$prefix, Slice:$remainder, I257:$succ,
                      (int_tvm_ldmsgaddrq Slice:$sl))],
                    "LDMSGADDRQ\t$prefix, $remainder, $sl",
                    "LDMSGADDRQ", 0xfa41>;

defm PARSEMSGADDR : I<(outs Tuple:$tp), (ins Slice:$sl), (outs), (ins),
                      [(set Tuple:$tp, (int_tvm_parsemsgaddr Slice:$sl))],
                      "PARSEMSGADDR\t$tp, $sl",
                      "PARSEMSGADDR", 0xfa42>;
defm PARSEMSGADDRQ : I<(outs Tuple:$tp, I257:$succ), (ins Slice:$sl),
                       (outs), (ins),
                       [(set Tuple:$tp, I257:$succ,
                         (int_tvm_parsemsgaddrq Slice:$sl))],
                       "PARSEMSGADDRQ\t$tp, $succ, $sl",
                       "PARSEMSGADDRQ", 0xfa43>;

defm REWRITESTDADDR : I<(outs I257:$workchain, I257:$address), (ins Slice:$sl),
                        (outs), (ins),
                        [(set I257:$workchain, I257:$address,
                          (int_tvm_rewritestdaddr Slice:$sl))],
                        "REWRITESTDADDR\t$workchain, $address, $sl",
                        "REWRITESTDADDR", 0xfa44>;
defm REWRITESTDADDRQ : I<(outs I257:$workchain, I257:$address, I257:$succ),
                         (ins Slice:$sl),
                         (outs), (ins),
                         [(set I257:$workchain, I257:$address, I257:$succ,
                           (int_tvm_rewritestdaddrq Slice:$sl))],
                         "REWRITESTDADDRQ\t$workchain, $address, $succ, $sl",
                         "REWRITESTDADDRQ", 0xfa45>;

defm REWRITEVARADDR : I<(outs I257:$workchain, Slice:$address), (ins Slice:$sl),
                        (outs), (ins),
                        [(set I257:$workchain, Slice:$address,
                          (int_tvm_rewritevaraddr Slice:$sl))],
                        "REWRITEVARADDR\t$workchain, $address, $sl",
                        "REWRITEVARADDR", 0xfa46>;
defm REWRITEVARADDRQ : I<(outs I257:$workchain, Slice:$address, I257:$succ),
                         (ins Slice:$sl),
                         (outs), (ins),
                         [(set I257:$workchain, Slice:$address, I257:$succ,
                           (int_tvm_rewritevaraddrq Slice:$sl))],
                         "REWRITEVARADDRQ\t$workchain, $address, $succ, $sl",
                         "REWRITEVARADDRQ", 0xfa47>;
