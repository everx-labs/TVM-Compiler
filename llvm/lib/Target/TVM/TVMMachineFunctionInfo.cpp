//===------- TVMMachineFunctionInfo.cpp - TVM Machine Function Info -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements TVM-specific per-machine-function information.
///
//===----------------------------------------------------------------------===//

#include "TVMMachineFunctionInfo.h"
#include "TVMISelLowering.h"
#include "TVMSubtarget.h"
#include "llvm/CodeGen/Analysis.h"
using namespace llvm;

TVMFunctionInfo::TVMFunctionInfo(MachineFunction &MF) : MF(MF) {
  StackModelComments[nullptr]; // Empty vector reservation
}

TVMFunctionInfo::~TVMFunctionInfo() = default;

void TVMFunctionInfo::initTVMRegs() {
  assert(TVMRegs.empty());
  unsigned Reg = UnusedReg;
  TVMRegs.resize(MF.getRegInfo().getNumVirtRegs(), Reg);
}

void llvm::ComputeLegalValueVTs(const Function &F, const TargetMachine &TM,
                                Type *Ty, SmallVectorImpl<MVT> &ValueVTs) {
  const DataLayout &DL(F.getParent()->getDataLayout());
  const TVMTargetLowering &TLI =
      *TM.getSubtarget<TVMSubtarget>(F).getTargetLowering();
  SmallVector<EVT, 4> VTs;
  ComputeValueVTs(TLI, DL, Ty, VTs);

  for (EVT VT : VTs) {
    unsigned NumRegs = TLI.getNumRegisters(F.getContext(), VT);
    MVT RegisterVT = TLI.getRegisterType(F.getContext(), VT);
    for (unsigned i = 0; i != NumRegs; ++i)
      ValueVTs.push_back(RegisterVT);
  }
}

void llvm::ComputeSignatureVTs(const Function &F, const TargetMachine &TM,
                               SmallVectorImpl<MVT> &Params,
                               SmallVectorImpl<MVT> &Results) {
  ComputeLegalValueVTs(F, TM, F.getReturnType(), Results);

  if (Results.size() > 1) {
    // TVM currently can't lower returns of multiple values without
    // demoting to sret (see TVMTargetLowering::CanLowerReturn). So
    // replace multiple return values with a pointer parameter.
    Results.clear();
    Params.push_back(
        MVT::getIntegerVT(TM.createDataLayout().getPointerSizeInBits()));
  }

  for (auto &Arg : F.args())
    ComputeLegalValueVTs(F, TM, Arg.getType(), Params);
}
