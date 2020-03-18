//===-- TVMInstPrinter.cpp - Convert TVM MCInst to assembly syntax --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an TVM MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "TVMInstPrinter.h"
#include "TVM.h"
#include "TVMMCExpr.h"
#include "TVMMachineFunctionInfo.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#include "TVMGenAsmWriter.inc"

void TVMInstPrinter::printInst(const MCInst *MI, raw_ostream &O,
                               StringRef Annot, const MCSubtargetInfo &STI) {
  {
    std::string Str;
    raw_string_ostream OStr(Str);
    printInstruction(MI, OStr);
    OStr.flush();
    // Auto-generated printInstruction prints \t before instruction name
    // For nice-looking 2-space offsets (inside PUSHCONT) we need to remove it.
    assert(Str.front() == '\t');
    Str.erase(0, 1);
    O << Str;
  }

  printAnnotation(O, Annot);
}

void TVMInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                  raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  const MCInstrDesc &Desc = MII.get(MI->getOpcode());
  const MCOperandInfo &Info = Desc.OpInfo[OpNo];

  if (Op.isImm()) {
    if (Info.OperandType == TVM::OPERAND_STACK)
      O << "s";
    O << Op.getImm();
  } else if (Op.isExpr()) {
    if (const auto *Expr = dyn_cast<TVMImmStringMCExpr>(Op.getExpr())) {
      O << Expr->getString();
    } else {
      // The actual label address is not known at the moment of
      // code generation; to simplify further linking, the label name
      // is surrounded with dollar signs ($callee$).
      O << "$";
      Op.getExpr()->print(O, &MAI);
      O << "$";
    }
  } else {
    llvm_unreachable("Unexpected operand");
  }
}

void TVMInstPrinter::printUimm257(const MCInst *MI, unsigned OpNo,
                                 raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  assert(Op.isImm() && "Wrong operand type in printUimm257");
  O << static_cast<uint64_t>(Op.getImm());
}

void TVMInstPrinter::printRegisterList(const MCInst *MI, unsigned OpNum,
                                       raw_ostream &O) {
  O << "{";
  for (unsigned i = OpNum, e = MI->getNumOperands(); i != e; ++i) {
    if (i != OpNum)
      O << ", ";
    printRegName(O, MI->getOperand(i).getReg());
  }
  O << "}";
}
