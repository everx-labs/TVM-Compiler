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
// TVM Instruction Format.
// We instantiate 2 of these for every actual instruction (register based
// and stack based), see below.
class TVMInst<bits<32> inst, string asmstr, bit stack> : Instruction {
  field bits<32> Inst = inst; // Instruction encoding.
  field bit StackBased = stack;
  let Namespace   = "TVM";
  let Pattern     = [];
  let AsmString   = asmstr;
}

// Normal instructions. Default instantiation of a TVMInst.
class NI<dag oops, dag iops, list<dag> pattern, bit stack, string asmstr = "",
         bits<32> inst = -1>
    : TVMInst<inst, asmstr, stack> {
  dag OutOperandList = oops;
  dag InOperandList  = iops;
  let Pattern        = pattern;
}

// Generates both register and stack based versions of one actual instruction.
// We have 2 sets of operands (oops & iops) for the register and stack
// based version of this instruction, as well as the corresponding asmstr.
// The register versions have virtual-register operands which correspond to wasm
// locals or stack locations. Each use and def of the register corresponds to an
// implicit get_local / set_local or access of stack operands in wasm. These
// instructions are used for ISel and all MI passes. The stack versions of the
// instructions do not have register operands (they implicitly operate on the
// stack), and get_locals and set_locals are explicit. The register instructions
// are converted to their corresponding stack instructions before lowering to
// MC.
// Every instruction should want to be based on this multi-class to guarantee
// there is always an equivalent pair of instructions.

//===----------------------------------------------------------------------===//
// Imm value in range
//===----------------------------------------------------------------------===//

class ImmAsmOperand<int Low, int High> : AsmOperandClass {
  let RenderMethod = "addImmOperands";
  let PredicateMethod = "isImmediate<" # Low # "," # High # ">";
  let DiagnosticString = "operand must be an immediate in the range [" # Low # "," # High # "]";
}

def SImm8: ImmAsmOperand<-128,127> { let Name = "SImm8"; }
def UImm8: ImmAsmOperand<0,255> { let Name = "UImm8"; }

def simm8 : Operand<i64>, ImmLeaf<i64, [{
  return Imm >=-128 && Imm <= 127 && Imm != 1;
}]> {
  let ParserMatchClass = SImm8;
}

def uimm8 : Operand<i64>, ImmLeaf<i64, [{
  return Imm >=0 && Imm <= 255;
}]> {
  let ParserMatchClass = UImm8;
}

//===----------------------------------------------------------------------===//
// Multiclasses for instructions
//===----------------------------------------------------------------------===//

multiclass I<dag oops_r, dag iops_r, dag oops_s, dag iops_s,
             list<dag> pattern_r, string asmstr_r = "", string asmstr_s = "",
             bits<32> inst = -1> {
  def "" : NI<oops_r, iops_r, pattern_r, 0, asmstr_r, inst>;
  def _S : NI<oops_s, iops_s, [], 1, asmstr_s, inst>;
}

// For instructions that have no register ops, so both sets are the same.
multiclass NRI<dag oops, dag iops, list<dag> pattern, string asmstr = "",
               bits<32> inst = -1> {
  defm "": I<oops, iops, oops, iops, pattern, asmstr, asmstr, inst>;
}

// Unary and binary instructions, for the local types that TVM supports.
multiclass UnaryR<SDNode node, string name, bits<32> i64Inst> {
  defm "" : I<(outs I64:$dst), (ins I64:$src), (outs), (ins),
              [(set I64:$dst, (node I64:$src))],
              !strconcat(name, "\t$dst, $src"),
              name, i64Inst>;
}

multiclass BinaryRR<SDNode node, string name, bits<32> i64Inst> {
  defm "" : I<(outs I64:$dst), (ins I64:$lhs, I64:$rhs), (outs), (ins),
              [(set I64:$dst, (node I64:$lhs, I64:$rhs))],
              !strconcat(name, "\t$dst, $lhs, $rhs"),
              name, i64Inst>;
}

multiclass BinaryRI<SDNode node, string name, ImmAsmOperand immtype, bits<32> i64Inst> {
  defm "" : I<(outs I64:$dst), (ins I64:$lhs, immtype:$rhs), (outs), (ins),
              [(set I64:$dst, (node I64:$lhs, immtype:$rhs))],
              !strconcat(name, "\t$dst, $lhs, $rhs"),
              name, i64Inst>;
}
