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
#include "TVMMachineFunctionInfo.h"
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
  printInstruction(MI, O);
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
    assert((Info.OperandType == TVM::OPERAND_BASIC_BLOCK ||
            Info.OperandType == TVM::OPERAND_FUNCTION) &&
           "Unimplemented expression type");
    Op.getExpr()->print(O, &MAI);
  } else {
    llvm_unreachable("Unexpected operand");
  }
}
