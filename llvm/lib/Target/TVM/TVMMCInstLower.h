//===-- TVMMCInstLower.h - Lower MachineInstr to MCInst ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMMCINSTLOWER_H
#define LLVM_LIB_TARGET_TVM_TVMMCINSTLOWER_H

#include "llvm/Support/Compiler.h"
#include <memory>
#include <vector>

namespace llvm {
class AsmPrinter;
class MCContext;
class MCInst;
class MCOperand;
class MCSymbol;
class MachineInstr;
class MachineModuleInfoMachO;
class MachineOperand;

/// TVMMCInstLower - This class is used to lower an MachineInstr
/// into an MCInst.
class LLVM_LIBRARY_VISIBILITY TVMMCInstLower {
  MCContext &Ctx;
  std::vector<std::shared_ptr<MCInst>> ContinuationInstructionStorage;

  [[maybe_unused]] AsmPrinter &Printer;

public:
  TVMMCInstLower(MCContext &ctx, AsmPrinter &printer)
      : Ctx(ctx), Printer(printer) {}
  void lower(const MachineInstr *MI, MCInst &OutMI);
};

} // namespace llvm

#endif
