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
  bit isStackForm    = 0;
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

def SImm8    : ImmAsmOperand<-128,127> { let Name = "SImm8"; }
def UImm11   : ImmAsmOperand<0,2047> { let Name = "UImm11"; }
def UImm10   : ImmAsmOperand<0,1023> { let Name = "UImm10"; }
def UImm8    : ImmAsmOperand<0,255> { let Name = "UImm8"; }
def UImm7    : ImmAsmOperand<0,127> { let Name = "UImm7"; }
def UImm6    : ImmAsmOperand<0,63> { let Name = "UImm6"; }
def UImm4    : ImmAsmOperand<0,15> { let Name = "UImm4"; }
def UImm3    : ImmAsmOperand<0,7> { let Name = "UImm3"; }
def UImm2    : ImmAsmOperand<0,3> { let Name = "UImm2"; }
def UImm1_256: ImmAsmOperand<1,256> { let Name = "UImm1_256"; }
def UImm1_31: ImmAsmOperand<1,31> { let Name = "UImm1_31"; }

// A list of registers separated by comma. Used by Tuple and call operations.
def RegListAsmOperand : AsmOperandClass { let Name = "RegList"; }
def reglist : Operand<i257> {
  let ParserMatchClass = RegListAsmOperand;
  let PrintMethod = "printRegisterList";
}

def UImm257 : Operand<i257>, IntImmLeaf<i257, [{ (void)Imm; return true; }]> {
  let PrintMethod = "printUimm257";
}

def simm8 : Operand<i257>, IntImmLeaf<i257, [{
  if (Imm.getMinSignedBits() > 64)
    return false;
  int64_t Val = Imm.getSExtValue();
  return Val >=-128 && Val <= 127 && Val != 1;
}]> {
  let ParserMatchClass = SImm8;
}

class uimmN<int Bits> : Operand<i257>, IntImmLeaf<i257, [{
  if (Imm.getMinSignedBits() > 64)
    return false;
  int64_t Val = Imm.getSExtValue();
  return Val >=0 && ((unsigned long)Val) <= ((1ul << }]#Bits#[{) - 1);
}]> {
  let ParserMatchClass = !cast<AsmOperandClass>("UImm"#Bits);
}

def uimm2 : uimmN<2>;
def uimm3 : uimmN<3>;
def uimm4 : uimmN<4>;
def uimm6 : uimmN<6>;
def uimm7 : uimmN<7>;
def uimm8 : uimmN<8>;
def uimm10 : uimmN<10>;
def uimm11 : uimmN<11>;

def uimm1_256 : Operand<i257>, IntImmLeaf<i257, [{
  if (Imm.getMinSignedBits() > 64)
    return false;
  int64_t Val = Imm.getSExtValue();
  return Val >=1 && Val <= 256;
}]> {
  let ParserMatchClass = UImm1_256;
}
def uimm1_31 : Operand<i257>, IntImmLeaf<i257, [{
  if (Imm.getMinSignedBits() > 64)
    return false;
  int64_t Val = Imm.getSExtValue();
  return Val >=1 && Val <= 31;
}]> {
  let ParserMatchClass = UImm1_31;
}

//===----------------------------------------------------------------------===//
// Multiclasses for instructions
//===----------------------------------------------------------------------===//

multiclass I<dag oops_r, dag iops_r, dag oops_s, dag iops_s,
             list<dag> pattern_r, string asmstr_r = "", string asmstr_s = "",
             bits<32> inst = -1> {
  def "" : NI<oops_r, iops_r, pattern_r, 0, asmstr_r, inst>;
  let isStackForm = 1, isCodeGenOnly = 1 in
  def _S : NI<oops_s, iops_s, [], 1, asmstr_s, inst>;
}

multiclass I0<dag oops_r, dag iops_r, list<dag> pattern, string asmstr = "",
              bits<32> inst = -1> {
  def "" : NI<oops_r, iops_r, pattern, 0, asmstr, inst>;
  let isStackForm = 1, isCodeGenOnly = 1 in
  def _S : NI<(outs), (ins), [], 1, asmstr>;
}

// Stack manipulation instructions.
// They aren't support to be selected on DAG but instead later backend passes
// insert them.
multiclass SI<dag iops, string asmstr = "", bits<32> inst = -1> {
  def "" : NI<(outs), iops, [], 1, asmstr, inst>;
}

// For instructions that have no register ops, so both sets are the same.
multiclass NRI<dag oops, dag iops, list<dag> pattern, string asmstr = "",
               bits<32> inst = -1> {
  defm "": I<oops, iops, oops, iops, pattern, asmstr, asmstr, inst>;
}

// Unary and binary instructions, for the local types that TVM supports.
multiclass UnaryR<SDNode node, string name, bits<32> i257Inst> {
  defm "" : I<(outs I257:$dst), (ins I257:$src), (outs), (ins),
              [(set I257:$dst, (node I257:$src))],
              !strconcat(name, "\t$dst, $src"),
              name, i257Inst>;
}

// UnaryR multiclass with PatFrag
multiclass UnaryRx<PatFrag dg, string name, bits<32> i257Inst> {
  defm "" : I<(outs I257:$dst), (ins I257:$src), (outs), (ins),
              [(set I257:$dst, (dg I257:$src))],
              !strconcat(name, "\t$dst, $src"),
              name, i257Inst>;
}

// UnaryR multiclass with Intrinsics
multiclass UnaryRi<Intrinsic intr, string name, bits<32> i257Inst> {
  defm "" : I<(outs I257:$dst), (ins I257:$src), (outs), (ins),
              [(set I257:$dst, (intr I257:$src))],
              !strconcat(name, "\t$dst, $src"),
              name, i257Inst>;
}

multiclass BinaryRR<SDNode node, string name, bits<32> i257Inst> {
  defm "" : I<(outs I257:$dst), (ins I257:$lhs, I257:$rhs), (outs), (ins),
              [(set I257:$dst, (node I257:$lhs, I257:$rhs))],
              !strconcat(name, "\t$dst, $lhs, $rhs"),
              name, i257Inst>;
}


// BinaryRR multiclass with Intrincis
multiclass BinaryRRi<Intrinsic intr, string name, bits<32> i257Inst> {
  defm "" : I<(outs I257:$dst), (ins I257:$lhs, I257:$rhs), (outs), (ins),
              [(set I257:$dst, (intr I257:$lhs, I257:$rhs))],
              !strconcat(name, "\t$dst, $lhs, $rhs"),
              name, i257Inst>;
}

multiclass BinaryRI<SDNode node, string name, ImmAsmOperand immtype, bits<32> i257Inst> {
  defm "" : I<(outs I257:$dst), (ins I257:$lhs, immtype:$rhs), (outs), (ins immtype:$rhs),
              [(set I257:$dst, (node I257:$lhs, immtype:$rhs))],
              !strconcat(name, "\t$dst, $lhs, $rhs"),
              !strconcat(name, "\t$rhs"), i257Inst>;
}

// BinaryRI multiclass with IntImmLeaf
multiclass BinaryRIil<SDNode node, string name, IntImmLeaf immtype, bits<32> i257Inst> {
  defm "" : I<(outs I257:$dst), (ins I257:$lhs, immtype:$rhs), (outs), (ins immtype:$rhs),
              [(set I257:$dst, (node I257:$lhs, immtype:$rhs))],
              !strconcat(name, "\t$dst, $lhs, $rhs"),
              !strconcat(name, "\t$rhs"), i257Inst>;
}

// BinaryRI multiclass with Intrincis
multiclass BinaryRIi<Intrinsic intr, string name, ImmAsmOperand immtype, bits<32> i257Inst> {
  defm "" : I<(outs I257:$dst), (ins I257:$lhs, immtype:$rhs), (outs), (ins immtype:$rhs),
              [(set I257:$dst, (intr I257:$lhs, immtype:$rhs))],
              !strconcat(name, "\t$dst, $lhs, $rhs"),
              !strconcat(name, "\t$rhs"), i257Inst>;
}

// BinaryRI multiclass with Intrincis and IntImmLeaf
multiclass BinaryRIiil<Intrinsic intr, string name, IntImmLeaf immtype, bits<32> i257Inst> {
  defm "" : I<(outs I257:$dst), (ins I257:$lhs, immtype:$rhs), (outs), (ins immtype:$rhs),
              [(set I257:$dst, (intr I257:$lhs, immtype:$rhs))],
              !strconcat(name, "\t$dst, $lhs, $rhs"),
              !strconcat(name, "\t$rhs"), i257Inst>;
}

multiclass ComparisonInt<CondCode cond, string name, bits<32> inst> {
  defm "" : I<(outs I257:$dst), (ins I257:$lhs, I257:$rhs), (outs), (ins),
              [(set I257:$dst, (setcc I257:$lhs, I257:$rhs, cond))],
              !strconcat(name, "\t$dst, $lhs, $rhs"), name, inst>;
}

multiclass ComparisonImm<CondCode cond, string name, ImmAsmOperand immtype, bits<32> inst> {
  defm "" : I<(outs I257:$dst), (ins I257:$lhs, immtype:$rhs), (outs), (ins immtype:$rhs),
              [(set I257:$dst, (setcc I257:$lhs, immtype:$rhs, cond))],
              !strconcat(name, "\t$dst, $lhs, $rhs"), !strconcat(name, "\t$rhs"), inst>;
}

// ComparisonImm multiclass with IntImmLeaf
multiclass ComparisonImmil<CondCode cond, string name, IntImmLeaf immtype, bits<32> inst> {
  defm "" : I<(outs I257:$dst), (ins I257:$lhs, immtype:$rhs), (outs), (ins immtype:$rhs),
              [(set I257:$dst, (setcc I257:$lhs, immtype:$rhs, cond))],
              !strconcat(name, "\t$dst, $lhs, $rhs"), !strconcat(name, "\t$rhs"), inst>;
}
