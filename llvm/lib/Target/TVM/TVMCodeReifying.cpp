//===-- TVMRegStackify.cpp - Register Stackification ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements a Machine Instruction hoisting.
/// 
/// If two instructions in different blocks are equal, in some cases we can 
/// instead insert one instruciton in Machine Block,
/// which is least common dominator of blocks, containing those instructions.
/// Such instruction pairs sometimes generated on some TVM backend stage,
/// for example during "while" loops processing (and maybe in other cases).
///
///
//===----------------------------------------------------------------------===//

#include "TVM.h"
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

#include <map>
#include <unordered_map>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "tvm-code-reifying"

namespace {
class TVMCodeReifying final : public MachineFunctionPass {
  StringRef getPassName() const override { return "TVM Code Reifying"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<LiveIntervals>();
    AU.addPreservedID(LiveVariablesID);
    AU.addRequired<MachineDominatorTree>();
    AU.addPreserved<MachineDominatorTree>();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

 // Calculate list of instructions which defined some virtual register
  void calculateDefinitions(MachineFunction &MF,
     unordered_map<unsigned, vector<MachineInstr *>> &defs);

  // Place some calculations near operations that use them like JUMPX and maybe some other operations at the end of blocks (todo)
  void reify(MachineFunction &MF, unordered_map<unsigned, vector<MachineInstr *>> defs);

  bool runOnMachineFunction(MachineFunction &MF) override;

public:
  static char ID; // Pass identification, replacement for typeid
  TVMCodeReifying() : MachineFunctionPass(ID) {}

private:
  bool runOnBasicBlocks(MachineFunction &MF);

  const TargetInstrInfo *TII;
  MachineDominatorTree *MDT;
};
} // end anonymous namespace

char TVMCodeReifying::ID = 0;
INITIALIZE_PASS(TVMCodeReifying, DEBUG_TYPE,
                "Machine instructions reifying", false, false)

FunctionPass *llvm::createTVMCodeReifying() { return new TVMCodeReifying(); }

bool TVMCodeReifying::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(dbgs() << "********** Machine Instruction Reifying **********\n"
                       "********** Function: "
                    << MF.getName() << '\n');

  TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();
  MDT = &getAnalysis<MachineDominatorTree>();

  bool changed = runOnBasicBlocks(MF);
  return changed;
}

void TVMCodeReifying::calculateDefinitions(MachineFunction &MF,
  unordered_map<unsigned, vector<MachineInstr *>> &defs) {
  for (MachineBasicBlock &MBB : MF) {
    for (MachineInstr &instr : MBB) {
      for (MachineOperand &def : instr.defs()) {
        if (def.isReg()) {
          unsigned reg = def.getReg();
          auto it = defs.find(reg);
          if (it != defs.end()) {
            vector<MachineInstr *> &list = it->second;
            list.push_back(&instr);
          }
          else {
            vector<MachineInstr *> list;
            list.push_back(&instr);
            defs.insert(pair<unsigned, vector<MachineInstr *>>(reg, list));
          }
        }
      }
    }
  }
}

void TVMCodeReifying::reify(MachineFunction &MF, unordered_map<unsigned, vector<MachineInstr *>> defs) {
  for (MachineBasicBlock &MBB : MF) {
    if (MBB.empty())
      continue;
    MachineInstr& instr = MBB.back();
    if (instr.getOpcode() == TVM::JMPX) {
      unsigned reg = instr.getOperand(0).getReg();
      vector<MachineInstr *> regDefs = defs[reg];
      if (regDefs.size() != 1)
        continue;
      MachineInstr *defInstr = regDefs[0];
      if (defInstr->getOpcode() != TVM::PUSHCONT_MBB)
        continue;
      MachineBasicBlock *defMBB = defInstr->getParent();

      // This check may be redundant
      if (!MDT->dominates(defMBB, &MBB))
        continue;

      MachineInstrBuilder MIB = BuildMI(MBB, &instr, instr.getDebugLoc(), TII->get(TVM::PUSHCONT_MBB))
        .addReg(reg)
        .addMBB(defInstr->getOperand(1).getMBB());
    }
  }
}

bool TVMCodeReifying::runOnBasicBlocks(MachineFunction &MF) {
  bool changed = false;

  unordered_map<unsigned, vector<MachineInstr *>> defs;
  calculateDefinitions(MF, defs);
  reify(MF, defs);


	return changed;
}

