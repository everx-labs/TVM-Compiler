//===-- TVMExplicitLocals.cpp - Make Locals Explicit ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file converts any remaining registers into TVM locals.
///
/// After register stackification and register coloring, convert non-stackified
/// registers into locals, inserting explicit get_local and set_local
/// instructions.
///
//===----------------------------------------------------------------------===//
#include <iostream>

#include "MCTargetDesc/TVMMCTargetDesc.h"
#include "TVM.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMSubtarget.h"
#include "TVMUtilities.h"
#include "llvm/CodeGen/MachineBlockFrequencyInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "tvm-explicit-locals"

namespace {
class TVMExplicitLocals final : public MachineFunctionPass {
  StringRef getPassName() const override { return "TVM Explicit Locals"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    AU.addPreserved<MachineBlockFrequencyInfo>();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

public:
  static char ID; // Pass identification, replacement for typeid
  TVMExplicitLocals() : MachineFunctionPass(ID) {}
};
} // end anonymous namespace

char TVMExplicitLocals::ID = 0;
INITIALIZE_PASS(TVMExplicitLocals, DEBUG_TYPE,
                "Convert registers to TVM locals", false, false)

FunctionPass *llvm::createTVMExplicitLocals() {
  return new TVMExplicitLocals();
}

/// Return a local id number for the given register, assigning it a new one
/// if it doesn't yet have one.
static unsigned getLocalId(DenseMap<unsigned, unsigned> &Reg2Local,
                           unsigned &CurLocal, unsigned Reg) {
  auto P = Reg2Local.insert(std::make_pair(Reg, CurLocal));
  if (P.second)
    ++CurLocal;
  return P.first->second;
}

// TODO: we need to duplicate a def prior to use if it has multiple users.
#if 0
/// Get the appropriate Push opcode for the given register class.
static unsigned getPushOpcode(const TargetRegisterClass *RC) {
  if (RC == &TVM::I64RegClass)
    return TVM::PUSH;
  llvm_unreachable("Unexpected register class");
}
#endif

/// Get the appropriate Xchg opcode for the given register class.
/// If we don't need the stack manipulation, gives XCHG s0, s0 pseudo.
static unsigned getXchgOpcode(const TargetRegisterClass *RC,
                              bool needStackManipulation) {
  if (RC == &TVM::I64RegClass) {
    if (needStackManipulation)
      return TVM::XCHG;
    return TVM::CG_XCHG;
  }
  llvm_unreachable("Unexpected register class");
}

/// Get the appropriate Pop opcode for the given register class.
static unsigned getPopOpcode(const TargetRegisterClass *RC) {
  if (RC == &TVM::I64RegClass)
    return TVM::POP;
  llvm_unreachable("Unexpected register class");
}

// TODO: For now it only stackifies function arguments. Extend.
bool TVMExplicitLocals::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(dbgs() << "********** Make Locals Explicit **********\n"
                       "********** Function: "
                    << MF.getName() << '\n');

  bool Changed = false;
  MachineRegisterInfo &MRI = MF.getRegInfo();
  TVMFunctionInfo &MFI = *MF.getInfo<TVMFunctionInfo>();
  const auto *TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();

  // Map non-stackified virtual registers to their local ids.
  DenseMap<unsigned, unsigned> Reg2Local;

  // Handle ARGUMENTS first to ensure that they get the designated numbers.
  for (MachineBasicBlock::iterator I = MF.begin()->begin(),
                                   E = MF.begin()->end();
       I != E;) {
    MachineInstr &MI = *I++;
    if (!TVM::isArgument(MI))
      break;
    unsigned Reg = MI.getOperand(0).getReg();
    assert(!MFI.isVRegStackified(Reg));
    Reg2Local[Reg] = MI.getOperand(1).getImm();
    MI.eraseFromParent();
    Changed = true;
  }

  // Start assigning local numbers after the last parameter.
  unsigned CurLocal = MFI.getParams().size();

  // Precompute the set of registers that are unused, so that we can insert
  // drops to their defs.
  BitVector UseEmpty(MRI.getNumVirtRegs());
  for (unsigned i = 0, e = MRI.getNumVirtRegs(); i < e; ++i)
    UseEmpty[i] = MRI.use_empty(TargetRegisterInfo::index2VirtReg(i));

  // Visit each instruction in the function.
  for (MachineBasicBlock &MBB : MF) {
    for (MachineBasicBlock::iterator I = MBB.begin(), E = MBB.end(); I != E;) {
      MachineInstr &MI = *I++;
      assert(!TVM::isArgument(MI));

      if (MI.isDebugInstr() || MI.isLabel())
        continue;

      // TODO: multiple defs should be handled separately.
      assert(MI.getDesc().getNumDefs() <= 1);

      // Insert set_locals for any defs that aren't stackified yet. Currently
      // we handle at most one def.
      if (MI.getDesc().getNumDefs() == 1) {
        unsigned OldReg = MI.getOperand(0).getReg();
        if (!MFI.isVRegStackified(OldReg)) {
          const TargetRegisterClass *RC = MRI.getRegClass(OldReg);
          unsigned NewReg = MRI.createVirtualRegister(RC);
          auto InsertPt = std::next(MachineBasicBlock::iterator(&MI));
          if (MI.getOpcode() == TVM::IMPLICIT_DEF) {
            MI.eraseFromParent();
            Changed = true;
            continue;
          }
          if (UseEmpty[TargetRegisterInfo::virtReg2Index(OldReg)]) {
            unsigned Opc = getPopOpcode(RC);
            MachineInstr *Pop =
                BuildMI(MBB, InsertPt, MI.getDebugLoc(), TII->get(Opc))
                    .addReg(NewReg);
            // After the drop instruction, this reg operand will not be used
            Pop->getOperand(0).setIsKill();
          }
          MI.getOperand(0).setReg(NewReg);
          // This register operand is now being used by the inserted drop
          // instruction, so make it undead.
          MI.getOperand(0).setIsDead(false);
          MFI.stackifyVReg(NewReg);
          Changed = true;
        }
      }

      // Insert XCHG for any uses that aren't stackified yet.
      MachineInstr *InsertPt = &MI;
      for (MachineOperand &MO : reverse(MI.explicit_uses())) {
        if (!MO.isReg())
          continue;

        unsigned OldReg = MO.getReg();

        // If we see a stackified register, prepare to insert subsequent
        // get_locals before the start of its tree.
        if (MFI.isVRegStackified(OldReg)) {
          llvm_unreachable("Not implemented");
          continue;
        }

        unsigned LocalId = getLocalId(Reg2Local, CurLocal, OldReg);
        const TargetRegisterClass *RC = MRI.getRegClass(OldReg);
        unsigned NewReg = MRI.createVirtualRegister(RC);
        // TODO: For now we could olny compile stackified programs.
        unsigned Opc = getXchgOpcode(RC, false);
        InsertPt =
            BuildMI(MBB, InsertPt, MI.getDebugLoc(), TII->get(Opc), NewReg)
                .addImm(LocalId);
        MO.setReg(NewReg);
        MFI.stackifyVReg(NewReg);
        Changed = true;
      }
    }
  }

#ifndef NDEBUG
  // Assert that all registers have been stackified at this point.
  for (const MachineBasicBlock &MBB : MF) {
    for (const MachineInstr &MI : MBB) {
      if (MI.isDebugInstr() || MI.isLabel())
        continue;
      for (const MachineOperand &MO : MI.explicit_operands()) {
        assert((!MO.isReg() || MRI.use_empty(MO.getReg()) ||
                MFI.isVRegStackified(MO.getReg())) &&
               "TVMExplicitLocals failed to stackify a register operand");
      }
    }
  }
#endif

  return Changed;
}
