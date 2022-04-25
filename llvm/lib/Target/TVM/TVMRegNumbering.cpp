//===---------- TVMRegNumbering.cpp - Register Numbering ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements a pass which assigns TVM register
/// numbers for CodeGen virtual registers.
///
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/TVMMCTargetDesc.h"
#include "TVM.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMSubtarget.h"
#include "TVMUtilities.h"
#include "llvm/ADT/SCCIterator.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineLoopInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "tvm-reg-numbering"

namespace {
class TVMRegNumbering final : public MachineFunctionPass {
  StringRef getPassName() const override { return "TVM Register Numbering"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

public:
  static char ID; // Pass identification, replacement for typeid
  TVMRegNumbering() : MachineFunctionPass(ID) {}
};
} // end anonymous namespace

char TVMRegNumbering::ID = 0;
INITIALIZE_PASS(TVMRegNumbering, DEBUG_TYPE,
                "Assigns TVM register numbers for virtual registers", false,
                false)

FunctionPass *llvm::createTVMRegNumbering() { return new TVMRegNumbering(); }

bool TVMRegNumbering::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(dbgs() << "********** Register Numbering **********\n"
                       "********** Function: "
                    << MF.getName() << '\n');

  TVMFunctionInfo &MFI = *MF.getInfo<TVMFunctionInfo>();
  MachineRegisterInfo &MRI = MF.getRegInfo();

  MFI.initTVMRegs();

  // TVM argument registers are in the same index space as local
  // variables. Assign the numbers for them first.
  MachineBasicBlock &EntryMBB = MF.front();
  for (MachineInstr &MI : EntryMBB) {
    if (!TVM::isArgument(MI))
      break;

    int64_t Imm = MI.getOperand(1).getImm();
    LLVM_DEBUG(dbgs() << "Arg VReg " << MI.getOperand(0).getReg()
                      << " -> TVMReg " << Imm << "\n");
    MFI.setTVMReg(MI.getOperand(0).getReg(), Imm);
  }

  // Then assign regular TVM registers for all remaining used
  // virtual registers. TODO: Consider sorting the registers by frequency of
  // use, to maximize usage of small immediate fields.
  unsigned NumVRegs = MF.getRegInfo().getNumVirtRegs();
  unsigned NumStackRegs = 0;
  // Start the numbering for locals after the arg regs
  unsigned CurReg = MFI.getParams().size();
  for (unsigned VRegIdx = 0; VRegIdx < NumVRegs; ++VRegIdx) {
    unsigned VReg = Register::index2VirtReg(VRegIdx);
    // Skip unused registers.
    if (MRI.use_empty(VReg))
      continue;
    // Handle stackified registers.
    if (MFI.isVRegStackified(VReg)) {
      LLVM_DEBUG(dbgs() << "VReg " << VReg << " -> TVMReg "
                        << (INT32_MIN | NumStackRegs) << "\n");
      MFI.setTVMReg(VReg, INT32_MIN | NumStackRegs++);
      continue;
    }
    if (MFI.getTVMReg(VReg) == TVMFunctionInfo::UnusedReg) {
      LLVM_DEBUG(dbgs() << "VReg " << VReg << " -> TVMReg " << CurReg << "\n");
      MFI.setTVMReg(VReg, CurReg++);
    }
  }

  return true;
}
