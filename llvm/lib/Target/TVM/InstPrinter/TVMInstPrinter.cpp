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
  // TODO: A hack, we have the same logic in AsmPrinter, but it's not triggered
  // for instructions in TVM::PUSHCONT_MBB. Need to find a better solution.
  if (MI->getOpcode() == TVM::BACKEDGE_S ||
      MI->getOpcode() == TVM::FALLTHROUGH_RETURN)
    return;

  printInstruction(MI, O);

  if (MI->getOpcode() == TVM::PUSHCONT_MBB) {
    O << " {\n";
    depth++;

    for (const auto &op : *MI) {
      if (op.isInst()) {
        O << std::string(depth, '\t');
        printInst(op.getInst(), O, "", STI);
        O << "\n";
      }
    }

    depth--;
    O << std::string(depth, '\t') << "\t}";
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
    assert((Info.OperandType == TVM::OPERAND_FUNCTION) &&
           "Unimplemented expression type");

    // The actual label address is not known at the moment of
    // code generation; to simplify further linking, the label name
    // is surrounded with dollar signs ($callee$).
    O << "$";
    Op.getExpr()->print(O, &MAI);
    O << "$";
  } else {
    llvm_unreachable("Unexpected operand");
  }
}
