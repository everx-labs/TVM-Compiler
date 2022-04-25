//===-------- TVMRematerializeDefs.cpp - Register Stackification ----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements a cheap definition rematerializtion pass.
///
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMSubtarget.h"
#include "TVMUtilities.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/CodeGen/LiveIntervals.h"
#include "llvm/CodeGen/MachineBlockFrequencyInfo.h"
#include "llvm/CodeGen/MachineDominators.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfoImpls.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "tvm-rematerialize"

namespace {
class TVMRematerialize final : public MachineFunctionPass {
  StringRef getPassName() const override {
    return "TVM Cheap Definitions Rematerialize";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    AU.addRequired<AAResultsWrapperPass>();
    AU.addRequired<LiveIntervals>();
    AU.addPreserved<LiveIntervals>();
    AU.addPreservedID(LiveVariablesID);
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

public:
  static char ID; // Pass identification, replacement for typeid
  TVMRematerialize() : MachineFunctionPass(ID) {}
};
} // end anonymous namespace

char TVMRematerialize::ID = 0;
INITIALIZE_PASS(TVMRematerialize, DEBUG_TYPE, "Rematerialize Cheap Definitions",
                false, false)

FunctionPass *llvm::createTVMRematerialize() { return new TVMRematerialize(); }

// Test whether Def is safe and profitable to rematerialize.
static bool ShouldRematerialize(const MachineInstr &Def, AliasAnalysis &AA,
                                const TVMInstrInfo *TII) {
  if (Def.getOpcode() == TVM::PUSH_GLOBAL_ADDRESS)
    return true;
  return Def.isAsCheapAsAMove() && TII->isTriviallyReMaterializable(Def, &AA);
}

// Shrink LI to its uses, cleaning up LI.
static void ShrinkToUses(LiveInterval &LI, LiveIntervals &LIS) {
  if (LIS.shrinkToUses(&LI)) {
    SmallVector<LiveInterval *, 4> SplitLIs;
    LIS.splitSeparateComponents(LI, SplitLIs);
  }
}

// Identify the definition for this register at this point. This is a
// generalization of MachineRegisterInfo::getUniqueVRegDef that uses
// LiveIntervals to handle complex cases.
static MachineInstr *GetVRegDef(unsigned Reg, const MachineInstr *Insert,
                                const MachineRegisterInfo &MRI,
                                const LiveIntervals &LIS) {
  // Most registers are in SSA form here so we try a quick MRI query first.
  if (MachineInstr *Def = MRI.getUniqueVRegDef(Reg))
    return Def;

  // MRI doesn't know what the Def is. Try asking LIS.
  if (const VNInfo *ValNo = LIS.getInterval(Reg).getVNInfoBefore(
          LIS.getInstructionIndex(*Insert)))
    return LIS.getInstructionFromIndex(ValNo->def);

  return nullptr;
}

/// A trivially cloneable instruction; clone it and nest the new copy with the
/// current instruction.
static MachineInstr *RematerializeCheapDef(
    unsigned Reg, MachineOperand &Op, MachineInstr &Def, MachineBasicBlock &MBB,
    MachineBasicBlock::instr_iterator Insert, LiveIntervals &LIS,
    TVMFunctionInfo &MFI, MachineRegisterInfo &MRI, const TVMInstrInfo *TII,
    const TargetRegisterInfo *TRI) {
  LLVM_DEBUG(dbgs() << "Rematerializing cheap def: "; Def.dump());
  LLVM_DEBUG(dbgs() << " - for use in "; Op.getParent()->dump());

  unsigned NewReg = MRI.createVirtualRegister(MRI.getRegClass(Reg));
  TII->reMaterialize(MBB, Insert, NewReg, 0, Def, *TRI);
  Op.setReg(NewReg);
  MachineInstr *Clone = &*std::prev(Insert);
  LIS.InsertMachineInstrInMaps(*Clone);
  LIS.createAndComputeVirtRegInterval(NewReg);
  MFI.stackifyVReg(NewReg);

  LLVM_DEBUG(dbgs() << " - Cloned to "; Clone->dump());

  // Shrink the interval.
  bool IsDead = MRI.use_empty(Reg);
  if (!IsDead) {
    LiveInterval &LI = LIS.getInterval(Reg);
    ShrinkToUses(LI, LIS);
    IsDead = !LI.liveAt(LIS.getInstructionIndex(Def).getDeadSlot());
  }

  // If that was the last use of the original, delete the original.
  if (IsDead) {
    LLVM_DEBUG(dbgs() << " - Deleting original\n");
    SlotIndex Idx = LIS.getInstructionIndex(Def).getRegSlot();
    LIS.removePhysRegDefAt(TVM::ARGUMENTS, Idx);
    LIS.removeInterval(Reg);
    LIS.RemoveMachineInstrFromMaps(Def);
    Def.eraseFromParent();
  }

  return Clone;
}

bool TVMRematerialize::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(
      dbgs() << "********** Cheap Definition Rematerializing **********\n"
                "********** Function: "
             << MF.getName() << '\n');

  bool Changed = false;
  MachineRegisterInfo &MRI = MF.getRegInfo();
  TVMFunctionInfo &MFI = *MF.getInfo<TVMFunctionInfo>();
  const auto *TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();
  const auto *TRI = MF.getSubtarget<TVMSubtarget>().getRegisterInfo();
  AliasAnalysis &AA = getAnalysis<AAResultsWrapperPass>().getAAResults();
  LiveIntervals &LIS = getAnalysis<LiveIntervals>();

  // Walk the instructions from the bottom up. Currently we don't look past
  // block boundaries, and the blocks aren't ordered so the block visitation
  // order isn't significant, but we may want to change this in the future.
  for (MachineBasicBlock &MBB : MF) {
    // Don't use a range-based for loop, because we modify the list as we're
    // iterating over it and the end iterator may change.
    for (auto MII = MBB.begin(), MIE = MBB.end(); MII != MIE; ++MII) {
      auto &MI = *MII;
      // Don't nest anything inside an inline asm, because we don't have
      // constraints for $push inputs.
      if (MI.getOpcode() == TargetOpcode::INLINEASM)
        continue;

      // Ignore debugging intrinsics.
      if (MI.getOpcode() == TargetOpcode::DBG_VALUE)
        continue;

      for (MachineOperand &Op : MI.operands()) {
        // We're only interested in explicit virtual register operands.
        if (!Op.isReg() || !Op.isUse() || Op.isImplicit())
          continue;

        unsigned Reg = Op.getReg();

        // Identify the definition for this register at this point.
        MachineInstr *Def = GetVRegDef(Reg, &MI, MRI, LIS);
        if (!Def)
          continue;

        // Don't nest an INLINE_ASM def into anything, because we don't have
        // constraints for $pop outputs.
        if (Def->getOpcode() == TargetOpcode::INLINEASM)
          continue;

        // Argument instructions represent live-in registers and not real
        // instructions.
        if (TVM::isArgument(*Def))
          continue;

        if (ShouldRematerialize(*Def, AA, TII)) {
          RematerializeCheapDef(Reg, Op, *Def, MBB, MI.getIterator(), LIS, MFI,
                                MRI, TII, TRI);
          Changed = true;
        }
      }
    }
  }

  return Changed;
}
