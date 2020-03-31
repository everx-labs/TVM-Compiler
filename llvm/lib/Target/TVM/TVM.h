//==-- TVM.h - Top-level interface for TVM representation --------*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM TVM backend.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVM_H
#define LLVM_LIB_TARGET_TVM_TVM_H

#include "MCTargetDesc/TVMMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class TVMTargetMachine;
class FunctionPass;
class LoopPass;
class formatted_raw_ostream;

FunctionPass *createTVMISelDag(TVMTargetMachine &TM,
                               CodeGenOpt::Level OptLevel);

FunctionPass *createTVMArgumentMove();
FunctionPass *createTVMControlFlowPrepare();
FunctionPass *createTVMReplacePhysRegs();
FunctionPass *createTVMPrepareForLiveIntervals();
FunctionPass *createTVMRematerialize();
FunctionPass *createTVMRegStackify();
FunctionPass *createTVMRegNumbering();
FunctionPass *createTVMStackModel();
FunctionPass *createTVMLoopInstructions();
FunctionPass *createTVMLoopPrepare();
FunctionPass *createTVMIfConversionTerm();
BasicBlockPass *createTVMLoadStoreReplace();

void initializeTVMArgumentMovePass(PassRegistry &);
void initializeTVMControlFlowPreparePass(PassRegistry &);
void initializeTVMReplacePhysRegsPass(PassRegistry &);
void initializeTVMPrepareForLiveIntervalsPass(PassRegistry &);
void initializeTVMRematerializePass(PassRegistry &);
void initializeTVMRegStackifyPass(PassRegistry &);
void initializeTVMRegNumberingPass(PassRegistry &);
void initializeTVMStackModelPass(PassRegistry &);
void initializeTVMLoopInstructionsPass(PassRegistry &);
void initializeTVMLoopPreparePass(PassRegistry &);
void initializeTVMLoadStoreReplacePass(PassRegistry &);
void initializeTVMIfConversionTermPass(PassRegistry &);

} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVM_H
