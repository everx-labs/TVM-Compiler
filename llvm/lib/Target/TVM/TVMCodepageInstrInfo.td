//===------ TVMCodepageInstrInfo.td - A11 instructions -----*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes A13 codepage instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Immediate operands
def CPImm : ImmAsmOperand<0, 239> { let Name = "CPImm"; }
def cpimm : Operand<i257>, ImmLeaf<i257, [{
  return Imm >= 0 && Imm <= 239;
}]> {
  let ParserMatchClass = CPImm;
}

//===----------------------------------------------------------------------===//
// Instruction list..
defm SETCP : I<(outs), (ins cpimm : $codepage),
               (outs), (ins cpimm : $codepage),
               [(int_tvm_setcp cpimm : $codepage)],
               "SETCP\t$codepage", "SETCP\t$codepage", 0xff>;
