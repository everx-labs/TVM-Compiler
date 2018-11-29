//===-- TVMInstrFormats.td - TVM Instruction Formats -------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Describe TVM instructions format here
//
class Format<bits<2> val> {
  bits<2> Value = val;
}

def PseudoFrm : Format<0>;

// Generic TVM Format
class TVMInst<bits<64> inst, string asmstr> : Instruction {
  field bits<64> Inst = inst; // Instruction encoding

  let Namespace = "TVM";
  let Pattern = [];
  let AsmString = asmstr;
}

// Normal instructions.
class I<dag oops, dag iops, string asmstr, list<dag> pattern,
        bits<64> inst = -1> : TVMInst<inst, asmstr> {
  dag OutOperandList = oops;
  dag InOperandList = iops;
  let Pattern = pattern;
}

// Pseudo instructions
class Pseudo<dag oops, dag iops, string asmstr, list<dag> pattern>
    : I<oops, iops, asmstr, pattern, 0>;
