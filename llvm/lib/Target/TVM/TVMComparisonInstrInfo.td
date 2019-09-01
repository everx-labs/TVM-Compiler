//===----- TVMComparisonInstrInfo.td - A11 instructions ----*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes A.6 comparison instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instruction list..
defm SEMPTY  : I0<(outs Slice : $result), (ins Slice : $slice),
                 [(set Slice : $result, (int_tvm_sempty Slice : $slice))],
                 "SEMPTY", 0xc700>;

defm SDEMPTY : I0<(outs Slice : $result), (ins Slice : $slice),
                  [(set Slice : $result, (int_tvm_sdempty Slice : $slice))],
                  "SDEMPTY", 0xc701>;

defm SREMPTY : I0<(outs Slice : $result), (ins Slice : $slice),
                  [(set Slice : $result, (int_tvm_srempty Slice : $slice))],
                  "SREMPTY", 0xc702>;
