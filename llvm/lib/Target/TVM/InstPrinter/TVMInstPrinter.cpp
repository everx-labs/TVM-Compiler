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
#define PRINT_ALIAS_INSTR
#include "TVMGenAsmWriter.inc"

void TVMInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                               StringRef Annot,
                 const MCSubtargetInfo &STI, raw_ostream &O) {
  // TODO: A hack, we have the same logic in AsmPrinter, but it's not triggered
  // for instructions in TVM::PUSHCONT_MBB. Need to find a better solution.
  switch (MI->getOpcode()) {
  case TVM::TO_TUPLE_COPY_S:
  case TVM::TO_SLICE_COPY_S:
  case TVM::TO_BUILDER_COPY_S:
  case TVM::TO_CELL_COPY_S:
  case TVM::FROM_TUPLE_COPY_S:
  case TVM::FROM_SLICE_COPY_S:
  case TVM::FROM_BUILDER_COPY_S:
  case TVM::FROM_CELL_COPY_S:
  case TVM::REG_TO_REG_COPY_S:
  case TVM::PUSH_GLOBAL_ADDRESS_S:
  case TVM::FALLTHROUGH_RETURN:
    return;
  default:
    break;
  }

  {
    std::string Str;
    raw_string_ostream OStr(Str);
    if (!printAliasInstr(MI, Address, /* STI, */ OStr))
      printInstruction(MI, Address, /* STI, */ OStr);
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
      assert((Info.OperandType == TVM::OPERAND_FUNCTION) &&
             "Unimplemented expression type");
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
