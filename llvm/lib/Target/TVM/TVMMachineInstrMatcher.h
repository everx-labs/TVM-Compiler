//===-- TVMMachineInstrMatcher.h - Define TVM subtarget ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMMACHINEINSTRMATCHER_H
#define LLVM_LIB_TARGET_TVM_TVMMACHINEINSTRMATCHER_H

#include "llvm/CodeGen/MachineBasicBlock.h"

namespace llvm {

/// Implements MI matcher with several immediate arguments
class MachineInstrMatcher {
public:
  MachineInstrMatcher() : It(), End() {}

  MachineInstrMatcher(MachineBasicBlock::instr_iterator It,
                      MachineBasicBlock::instr_iterator End)
      : It(It), End(End) {}

  /// Returns match if instruction has required opcode
  MachineInstr *match(unsigned int Opcode) {
    MachineInstr *Instr = next();

    if (!Instr)
      return nullptr;

    if (Instr->getOpcode() != Opcode)
      return nullptr;

    return Instr;
  }

  /// Returns match if instruction has required opcode and specified
  /// argument values
  template <typename... ArgsT>
  MachineInstr *match(unsigned int Opcode, ArgsT... Args) {
    return matchImpl(match(Opcode), 0, Args...);
  }

  template <typename... ArgsT>
  bool match(MachineInstr *&Instr, unsigned int Opcode, ArgsT... Args) {
    Instr = match(Opcode, Args...);

    return Instr != nullptr;
  }

  MachineBasicBlock::instr_iterator iter() { return It; }

private:
  MachineInstr *next() {
    if (It == End)
      return nullptr;

    return &*It++;
  }

  bool checkOperand(MachineInstr *Instr, unsigned int Index, int ImmValue) {
    if (!Instr)
      return false;

    if (Index >= Instr->getNumOperands())
      return false;

    const MachineOperand &Operand = Instr->getOperand(Index);

    if (!Operand.isImm())
      return false;

    if (ImmValue != Operand.getImm())
      return false;

    return true;
  }

  MachineInstr *matchImpl(MachineInstr *Instr, unsigned int Index, int First) {
    if (!checkOperand(Instr, Index, First))
      return nullptr;

    return Instr;
  }

  template <typename... ArgsT>
  MachineInstr *matchImpl(MachineInstr *Instr, unsigned int Index, int First,
                          ArgsT... Args) {
    if (!checkOperand(Instr, Index, First))
      return nullptr;

    return matchImpl(Instr, Index + 1, Args...);
  }

private:
  MachineBasicBlock::instr_iterator It;
  MachineBasicBlock::instr_iterator End;
};

} // namespace llvm

#endif
