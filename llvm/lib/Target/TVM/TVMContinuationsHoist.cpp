//===- TVMContinuationsHoist.cpp - hoist PUSHCONTs into common def --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "TVMInstrInfo.h"
#include "TVMRegisterInfo.h"
#include "TVMSubtarget.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineDominators.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Pass.h"

#define DEBUG_TYPE "tvm-cont-hoist"

using namespace llvm;

namespace llvm {
  void initializeTVMContinuationsHoistPass(PassRegistry&);
  FunctionPass *createTVMContinuationsHoist();
}

namespace {
  class TVMContinuationsHoist : public MachineFunctionPass {
  public:
    static char ID;
    TVMContinuationsHoist() : MachineFunctionPass(ID) {}

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.addRequired<MachineDominatorTree>();
      AU.addPreserved<MachineDominatorTree>();
      MachineFunctionPass::getAnalysisUsage(AU);
    }

    StringRef getPassName() const override {
      return "TVM continuations hoisting pass";
    }
    bool runOnMachineFunction(MachineFunction &MF) override;

  private:
    const TVMInstrInfo *TII = nullptr;
    MachineDominatorTree *MDT = nullptr;
    MachineRegisterInfo *MRI = nullptr;

    struct OccuranciesInfo {
      SmallVector<MachineInstr *, 4> Instructions;
    };
    std::map<MachineBasicBlock *, OccuranciesInfo> Info;

    void collectInstr(MachineInstr &MI);
    void collect(MachineFunction &MF);
    bool optimize();
  };
}

INITIALIZE_PASS_BEGIN(TVMContinuationsHoist, "tvm-cont-hoist",
      "TVM continuations hoisting pass", false, false)
INITIALIZE_PASS_DEPENDENCY(MachineDominatorTree)
INITIALIZE_PASS_END(TVMContinuationsHoist, "tvm-cont-hoist",
      "TVM continuations hoisting pass", false, false)

char TVMContinuationsHoist::ID = 0;

void TVMContinuationsHoist::collect(MachineFunction &MF) {
  Info.clear();
  for (MachineBasicBlock &MBB : MF)
    for (MachineInstr &MI : MBB)
      collectInstr(MI);
}

bool TVMContinuationsHoist::optimize() {
  bool Changed = false;
  for (const auto &Pair : Info) {
    auto TargetMBB = Pair.first;
    const auto &Instrs = Pair.second.Instructions;
    if (Instrs.size() < 2)
      continue;
    Changed = true;

    auto CommonDom = Instrs.front()->getParent();
    for (auto CurNextMI : drop_begin(Instrs, 1)) {
      assert(CommonDom && "PUSHCONT_MBB with null parent");
      assert(CurNextMI->getParent() && "PUSHCONT_MBB with null parent");
      CommonDom = MDT->findNearestCommonDominator(CommonDom,
                                                  CurNextMI->getParent());
    }

    MachineInstr *FoundMI = nullptr;
    for (MachineInstr &MI : *CommonDom) {
      if (MI.getOpcode() == TVM::PUSHCONT_MBB &&
          MI.getOperand(1).getMBB() == TargetMBB) {
        FoundMI = &MI;
        break;
      }
      if (MI.isTerminator())
        break;
    }

    unsigned MBBReg = 0;
    if (FoundMI) {
      MBBReg = FoundMI->getOperand(0).getReg();
      FoundMI->clearRegisterDeads(MBBReg);
    } else {
      MBBReg = MRI->createVirtualRegister(&TVM::I64RegClass);
      BuildMI(*CommonDom, CommonDom->getFirstInstrTerminator(), DebugLoc(),
              TII->get(TVM::PUSHCONT_MBB), MBBReg).addMBB(TargetMBB).addImm(0);
    }

    for (auto CurDefMI : Instrs) {
      if (FoundMI && FoundMI == CurDefMI)
        continue;
      auto OldDefMO = CurDefMI->getOperand(0);
      assert(OldDefMO.isReg() && "PUSHCONT_MBB def is not reg");
      CurDefMI->eraseFromParent();

      MRI->replaceRegWith(OldDefMO.getReg(), MBBReg);
    }
    MRI->clearKillFlags(MBBReg);
  }
  return Changed;
}

void TVMContinuationsHoist::collectInstr(MachineInstr &MI) {
  if (MI.getOpcode() == TVM::PUSHCONT_MBB) {
    auto BB = MI.getOperand(1);
    assert(BB.isMBB() && "PUSHCONT_MBB operand 1 is not MBB");
    auto MBB = BB.getMBB();
    Info[MBB].Instructions.push_back(&MI);
  }
}

bool TVMContinuationsHoist::runOnMachineFunction(MachineFunction &MF) {
  if (skipFunction(MF.getFunction()))
    return false;
  LLVM_DEBUG(MF.print(dbgs() << "Before " << getPassName() << '\n', nullptr));

  TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();
  MDT = &getAnalysis<MachineDominatorTree>();
  MRI = &MF.getRegInfo();

  collect(MF);
  bool Changed = optimize();

  LLVM_DEBUG({
    if (Changed)
      MF.print(dbgs() << "After " << getPassName() << '\n', nullptr);
    else
      dbgs() << "No changes\n";
  });
  return Changed;
}

FunctionPass *llvm::createTVMContinuationsHoist() {
  return new TVMContinuationsHoist();
}
