//= TVMInstPrinter.h - Convert TVM MCInst to assembly syntax -------*- C++ -*-//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints a TVM MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_INSTPRINTER_TVMINSTPRINTER_H
#define LLVM_LIB_TARGET_TVM_INSTPRINTER_TVMINSTPRINTER_H

#include "llvm/MC/MCInstPrinter.h"

namespace llvm {
class TVMInstPrinter : public MCInstPrinter {
public:
  TVMInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                 const MCRegisterInfo &MRI)
      : MCInstPrinter(MAI, MII, MRI) {}

  void printInst(const MCInst *MI, raw_ostream &O, StringRef Annot,
                 const MCSubtargetInfo &STI) override;

  // Autogenerated by tblgen.
  void printInstruction(const MCInst *MI, raw_ostream &O);
  void printOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);
  static const char *getRegisterName(unsigned RegNo);

protected:
  int depth = 0;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_INSTPRINTER_TVMINSTPRINTER_H
