//===---------- TVMUtilities - TVM Utility Functions -----------*- C++ -*-====//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the declaration of the TVM-specific utility functions.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMUTILITIES_H
#define LLVM_LIB_TARGET_TVM_TVMUTILITIES_H

#include "llvm/ADT/DenseMap.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"

namespace llvm {

class TVMFunctionInfo;

// A shortcut overload for BuildMI() function
MachineInstrBuilder BuildMI(MachineInstr *InsertPoint,
                            const MCInstrDesc &InstrDesc);

template <typename KeyT, typename ValT>
ValT &element(DenseMap<KeyT, ValT> &Container, KeyT Key) {
  auto It = Container.find(Key);
  assert(It != std::end(Container) && "Element not found");
  return It->getSecond();
}

namespace TVM {

bool isArgument(const MachineInstr &MI);
bool isArgumentNum(const MachineInstr &MI);

} // end namespace TVM

} // end namespace llvm

#endif
