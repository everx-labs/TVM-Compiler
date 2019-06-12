//===-- TVMAsmPrinter.cpp - TVM LLVM assembly writer ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the TVM assembly language.
//
//===----------------------------------------------------------------------===//

#include "InstPrinter/TVMInstPrinter.h"
#include "TVM.h"
#include "TVMInstrInfo.h"
#include "TVMMCInstLower.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMTargetMachine.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Mangler.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"

namespace {
class TVMAsmPrinter : public AsmPrinter {
public:
  TVMAsmPrinter(TargetMachine &TM, std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override { return "TVM Assembly Printer"; }

  void printOperand(const MachineInstr *MI, int OpNum, raw_ostream &O,
                    const char *Modifier = nullptr);
  void EmitInstruction(const MachineInstr *MI) override;
  std::string regToString(const MachineOperand &MO);
  void EmitFunctionBodyStart() override;
  bool runOnMachineFunction(MachineFunction &MF) override;

private:
  TVMFunctionInfo *MFI;
};
} // end of anonymous namespace

std::string TVMAsmPrinter::regToString(const MachineOperand &MO) {
  unsigned RegNo = MO.getReg();
  assert(TargetRegisterInfo::isVirtualRegister(RegNo) &&
         "Unlowered physical register encountered during assembly printing");
  assert(!MFI->isVRegStackified(RegNo));
  unsigned TVMReg = MFI->getTVMReg(RegNo);
  assert(TVMReg != TVMFunctionInfo::UnusedReg);
  return 's' + utostr(TVMReg);
}

void TVMAsmPrinter::printOperand(const MachineInstr *MI, int OpNum,
                                 raw_ostream &O, const char *Modifier) {
  const MachineOperand &MO = MI->getOperand(OpNum);
  switch (MO.getType()) {
  default:
    llvm_unreachable("Not implemented yet!");
  case MachineOperand::MO_Register:
    O << regToString(MO);
    return;
  case MachineOperand::MO_Immediate:
    if (!Modifier || strcmp(Modifier, "nohash"))
      O << '#';
    O << MO.getImm();
    return;
  }
}

//===----------------------------------------------------------------------===//
void TVMAsmPrinter::EmitInstruction(const MachineInstr *MI) {
  LLVM_DEBUG(dbgs() << "EmitInstruction: " << *MI << '\n');
  if (isVerbose())
    for (auto Comment : MFI->getStackModelComments(MI)) {
      OutStreamer->AddComment(Comment);
    }
  switch (MI->getOpcode()) {
  case TVM::ARGUMENT:
    llvm_unreachable("CG only instruction mustn't reach ASM printer");
    break;
  case TVM::BACKEDGE_S:
    break;
  case TVM::FALLTHROUGH_RETURN:
    if (isVerbose()) {
      OutStreamer->AddComment("fallthrough return");
      OutStreamer->AddBlankLine();
    }
    break;
  default:
    TVMMCInstLower MCInstLowering(OutContext, *this);

    MCInst TmpInst;
    MCInstLowering.lower(MI, TmpInst);
    EmitToStreamer(*OutStreamer, TmpInst);
  }
}

void TVMAsmPrinter::EmitFunctionBodyStart() {
  if (isVerbose())
    OutStreamer->emitRawComment(" " + MFI->getStartStackModelComment(), false);
}

bool TVMAsmPrinter::runOnMachineFunction(MachineFunction &MF) {
  MFI = MF.getInfo<TVMFunctionInfo>();
  return AsmPrinter::runOnMachineFunction(MF);
}

// Force static initialization.
extern "C" void LLVMInitializeTVMAsmPrinter() {
  RegisterAsmPrinter<TVMAsmPrinter> X(getTheTVMTarget());
}
