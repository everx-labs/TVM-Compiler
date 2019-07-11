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
#include "llvm/ADT/DenseMap.h"
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
class TVMFunctionInfo;

/// TVMMCInstLower - This class is used to lower an MachineInstr
/// into an MCInst.
class LLVM_LIBRARY_VISIBILITY TVMMCInstLower {
  MCContext &Ctx;
  std::vector<std::shared_ptr<MCInst>> ContinuationInstructionStorage;
  DenseMap<const MCInst *, MachineInstr *> MCInstrsMap;

  [[maybe_unused]] AsmPrinter &Printer;

public:
  TVMMCInstLower(MCContext &ctx, AsmPrinter &printer)
      : Ctx(ctx), Printer(printer) {}
  void lower(const MachineInstr *MI, MCInst &OutMI);

  const decltype(MCInstrsMap) &getMCInstrsMap() const {
    return MCInstrsMap;
  }
};

} // namespace llvm

#endif
