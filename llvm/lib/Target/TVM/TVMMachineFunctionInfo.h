//=== TVMMachineFunctionInfo.h - TVM machine function info -------*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares TVM-specific per-machine-function information.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMMACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_TVM_TVMMACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {

/// TVMMachineFunctionInfo - This class is derived from MachineFunction and
/// contains private TVM target-specific information for each MachineFunction.
class TVMMachineFunctionInfo : public MachineFunctionInfo {
  virtual void anchor();

public:
  TVMMachineFunctionInfo() {}
};

} // namespace llvm

#endif
