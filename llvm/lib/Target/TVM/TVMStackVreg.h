//===----- TVMStackVreg.h - virtual register in stack model -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
/// \file
/// Virtual register in stack model.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMSTACKVREG_H
#define LLVM_LIB_TARGET_TVM_TVMSTACKVREG_H

#include <deque>
#include <variant>

#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/Support/Debug.h"

namespace llvm {

class Stack;

struct StackVreg {
  unsigned VirtReg = 0;
  const DILocalVariable *DbgVar = nullptr;

  explicit StackVreg(unsigned VirtReg, const DILocalVariable *DbgVar = nullptr)
      : VirtReg(VirtReg), DbgVar(DbgVar) {}

  bool operator<(const StackVreg &R) const { return VirtReg < R.VirtReg; }
  bool operator==(const StackVreg &R) const { return VirtReg == R.VirtReg; }
  bool operator!=(const StackVreg &R) const { return VirtReg != R.VirtReg; }
  void print(const Stack &S) const;
  bool operator==(unsigned R) const { return VirtReg == R; }
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMSTACKVREG_H
