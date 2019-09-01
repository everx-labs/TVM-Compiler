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
defm ACCEPT   : I0<(outs), (ins), [(int_tvm_accept)],
                   "ACCEPT", 0xf800>;

defm HASHCU   : I0<(outs I257 : $result), (ins Cell : $cell),
                   [(set I257 : $result, (int_tvm_hashcu Cell : $cell))],
                   "HASHCU", 0xf900>;

defm HASHSU   : I0<(outs I257 : $result), (ins Cell : $cell),
                   [(set I257 : $result, (int_tvm_hashsu Cell : $cell))],
                   "HASHSU", 0xf901>;

defm CHKSIGNU : I0<(outs I257 : $result),
                   (ins I257 : $hash, Slice : $signature, I257 : $key),
                   [(set I257 : $result, (int_tvm_chksignu I257  : $hash,
                                                           Slice : $signature,
                                                           I257  : $key))],
                   "CHKSIGNU", 0xf910>;
