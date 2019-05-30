//===---- TVMLoopInstructions.cpp - TVM loop instruction insertion pass ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Replace IFJMP forming a loop with proper loop instructions.
///
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMSubtarget.h"
#include "TVMUtilities.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineLoopInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "tvm-loop"

namespace {
/// Replace IFJMP forming a loop with proper loop instructions.
/// The loop must be in the following form:
/// -----------------------------
/// |      LoopPredecessor      |
/// | ...                       |
/// | IFJMP LoopCond LoopHeader |-----------------------
/// | JMPX ExitBlock            |                      |
/// -----------------------------                      ∨
///               |                       -----------------------------
///               |                       |         ExitBlock         |
///               ∨                       -----------------------------
/// -----------------------------                      ∧
/// |        LoopHeader         |                      |
/// | IFJMP LoopCond LoopBody   |-----------------------
/// | JMPX ExitBlock            |
/// -----------------------------
///          |         ∧
///          |         |
///          |         ~
///          ∨         |
/// -----------------------------
/// |         LoopBody          |
/// -----------------------------
///
/// The pass rewrites terminators so that the following is generated:
/// ------------------------------
/// |      LoopPredecessor       |
/// | ...                        |
/// | UNTIL LoopCond LoopHeader  |-----------------------
/// | JMPX ExitBlock             |                      |
/// ------------------------------                      ∨
///               |                        -----------------------------
///               |                        |         ExitBlock         |
///               ∨                        -----------------------------
/// ------------------------------                      ∧
/// |        LoopHeader          |                      |
/// | BACKEDGE LoopCond LoopBody |-----------------------
/// | <DELETED TERMINATOR>       |
/// ------------------------------
///          |         ∧
///          |         |
///          |         ~
///          ∨         |
/// ------------------------------
/// |         LoopBody           |
/// ------------------------------
/// Note: the transformation only deletes code following BACKEDGE, but preserves
/// LoopHeader successors.
class TVMLoopInstructions final : public MachineFunctionPass {
  StringRef getPassName() const override {
    return "TVM loop instructions inserter";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<MachineLoopInfo>();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

public:
  static char ID;
  TVMLoopInstructions() : MachineFunctionPass(ID) {}
};
} // end anonymous namespace

char TVMLoopInstructions::ID = 0;
INITIALIZE_PASS(TVMLoopInstructions, DEBUG_TYPE,
                "TVM loop instructions inserter", false, false)

FunctionPass *llvm::createTVMLoopInstructions() {
  return new TVMLoopInstructions();
}

static void processLoop(MachineLoop &Loop, const MachineLoopInfo &MLI,
                        const TargetInstrInfo &TII) {
  MachineBasicBlock *Header = Loop.findLoopControlBlock();
  assert(Header && "Unexpected loop shape");
  auto FirstTerminatorIt = Header->getFirstTerminator();
  MachineInstr &FirstTerminator = *FirstTerminatorIt;
  std::vector<MachineInstr *> InstuctionsToErace;

  for (auto It = FirstTerminatorIt, E = Header->end(); It != E; ++It)
    InstuctionsToErace.push_back(&*It);

  assert(FirstTerminator.getOpcode() == TVM::IFJMP && "Unexpected loop shape");

  MachineOperand ArgBB = FirstTerminator.getOperand(1);
  assert(ArgBB.isMBB() && "Must be a basic block");
  assert(ArgBB.getMBB() == Header &&
         "The backedge must be the first terminator, otherwise "
         "unimplemented yet.");
  BuildMI(&FirstTerminator, TII.get(TVM::BACKEDGE))
      .addReg(FirstTerminator.getOperand(0).getReg())
      .addMBB(FirstTerminator.getOperand(1).getMBB());

  for (auto *Inst : InstuctionsToErace)
    Inst->eraseFromParent();

  auto *LoopPredecessor = [&Loop, Header, &MLI]() {
    MachineBasicBlock *Pred1 = *Header->pred_begin();
    MachineBasicBlock *Pred2 = *std::next(Header->pred_begin());
    if (MLI.getLoopFor(Pred1) != &Loop)
      return Pred1;
    assert(MLI.getLoopFor(Pred2) != &Loop);
    return Pred2;
  }();
  for (MachineInstr &Term : LoopPredecessor->terminators()) {
    if (Term.getOpcode() == TVM::IFJMP) {
      MachineOperand ArgBB = Term.getOperand(1);
      assert(ArgBB.isMBB() && "Must be a basic block");
      if (ArgBB.getMBB() != Header)
        continue;
      BuildMI(&Term, TII.get(TVM::UNTIL))
          .addReg(Term.getOperand(0).getReg())
          .addMBB(Term.getOperand(1).getMBB());
      Term.eraseFromParent();
      break;
    }
  }
}

bool TVMLoopInstructions::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG({
    dbgs() << "********** Loop Instructions **********\n"
           << "********** Function: " << MF.getName() << '\n';
  });

  const auto &MLI = getAnalysis<MachineLoopInfo>();
  const auto &TII = *MF.getSubtarget<TVMSubtarget>().getInstrInfo();
  llvm::DenseMap<MachineLoop *, bool> LoopsProcessed;

  for (auto &BB : MF) {
    MachineLoop *Loop = MLI.getLoopFor(&BB);
    if (!Loop || LoopsProcessed[Loop])
      continue;

    processLoop(*Loop, MLI, TII);
    LoopsProcessed[Loop] = true;
  }
  return !LoopsProcessed.empty();
}
