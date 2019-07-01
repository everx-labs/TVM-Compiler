//===------ TVMStackBlockInfo.cpp - block details about stack model -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
/// \file
/// Define classes to manipulate with program model of TVM stack.
//
//===----------------------------------------------------------------------===//

#include "TVMStackBlockInfo.h"

#include <algorithm>

#include "TVMSubtarget.h"
#include "TVMUtilities.h"
#include "TVMStackFixup.h"
#include "TVMStack.h"

namespace llvm {

void TVMStackBlockInfo::
setFixedEndWithFixup(const Stack &stack, const TargetInstrInfo *TII,
                     TVMFunctionInfo *MFI) {
  FixedEnd = stack;

  auto MBBI = MBB->getFirstTerminator();

  StackFixup endFixup;
  // if we have terminator, we need to adjust its args
  if (MBBI != MBB->end()) {
    endFixup = fixedEnd().addArgs(TerminatorArgs) -
        calculatedEnd().addArgs(TerminatorArgs);
  } else {
    endFixup = fixedEnd() - calculatedEnd();
  }

  StackFixup::InstructionGenerator gen(TII, MFI, MBB, MBBI);
  gen(endFixup);

  if (MBBI != MBB->end())
    MFI->resetStackModelComment(&*MBBI, "fx^ => " + fixedEnd().toString());
}

void TVMStackBlockInfo::setFixedEndForLoopTail(const Stack &stack,
                                               const TargetInstrInfo *TII,
                                               TVMFunctionInfo *MFI) {
  auto MBBI = MBB->getFirstTerminator();

  StackFixup endFixup;
  // if we have terminator, we need to adjust its args
  if (MBBI != MBB->end()) {
    endFixup = stack.addArgs(TerminatorArgs) -
        calculatedEnd().addArgs(TerminatorArgs);

    FixedEnd = (calculatedEnd().addArgs(TerminatorArgs) + endFixup).
        delArgs(TerminatorArgs);
  } else {
    endFixup = stack - calculatedEnd();
    FixedEnd = calculatedEnd() + endFixup;
  }

  StackFixup::InstructionGenerator gen(TII, MFI, MBB, MBBI);
  gen(endFixup);

  if (MBBI != MBB->end())
    MFI->resetStackModelComment(&*MBBI, "fx^ => " + fixedEnd().toString());
}

} // namespace llvm
