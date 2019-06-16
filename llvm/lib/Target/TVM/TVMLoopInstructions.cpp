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
  MachineBasicBlock *Latch = Loop.findLoopControlBlock();
  assert(Latch && "Unexpected loop shape");
  auto FirstTerminatorIt = Latch->getFirstTerminator();
  MachineInstr &FirstTerminator = *FirstTerminatorIt;
  std::vector<MachineInstr *> InstructionsToErase;

  for (auto It = FirstTerminatorIt, E = Latch->end(); It != E; ++It)
    InstructionsToErase.push_back(&*It);

  if (FirstTerminator.getOpcode() == TVM::IFELSE) {
    MachineOperand ThenBB = FirstTerminator.getOperand(2);
    MachineOperand ElseBB = FirstTerminator.getOperand(1);
    assert(ThenBB.isMBB() && ElseBB.isMBB() && "Must be a basic block");
    assert((MLI.getLoopFor(ThenBB.getMBB()) == &Loop || MLI.getLoopFor(ElseBB.getMBB()) == &Loop) &&
           "The backedge must be the first terminator, otherwise "
           "unimplemented yet.");
    BuildMI(&FirstTerminator, TII.get(TVM::BACKEDGE))
        .addReg(FirstTerminator.getOperand(0).getReg())
        .addMBB(ThenBB.getMBB());
  } else if (FirstTerminator.getOpcode() == TVM::IFJMP) {
    MachineOperand ArgBB = FirstTerminator.getOperand(1);
    assert(ArgBB.isMBB() && "Must be a basic block");
    assert(ArgBB.getMBB() == Latch &&
           "The backedge must be the first terminator, otherwise "
           "unimplemented yet.");
    BuildMI(&FirstTerminator, TII.get(TVM::BACKEDGE))
        .addReg(FirstTerminator.getOperand(0).getReg())
        .addMBB(FirstTerminator.getOperand(1).getMBB());
  } else {
    llvm_unreachable("Unexpected loop shape");
  }

  for (auto *Inst : InstructionsToErase)
    Inst->eraseFromParent();

  auto *LoopPredecessor = [&Loop, Latch, &MLI]() {
    MachineBasicBlock *Header = *Latch->succ_begin();
    if (MLI.getLoopFor(Header) != &Loop)
      Header = *std::next(Latch->succ_begin());
    assert(MLI.getLoopFor(Header) == &Loop && "Unexpected loop shape");

    MachineBasicBlock *Predecessor = *Header->pred_begin();
    if (MLI.getLoopFor(Predecessor) != &Loop)
      return Predecessor;
    Predecessor = *std::next(Header->pred_begin());
    assert(MLI.getLoopFor(Predecessor) != &Loop);
    return Predecessor;
  }();
  for (MachineInstr &Term : LoopPredecessor->terminators()) {
    if (Term.getOpcode() == TVM::IFJMP) {
      MachineOperand ArgBB = Term.getOperand(1);
      assert(ArgBB.isMBB() && "Must be a basic block");
      assert(MLI.getLoopFor(ArgBB.getMBB()) == &Loop &&
             "Unexpected loop shape");
      BuildMI(&Term, TII.get(TVM::UNTIL))
          .addReg(Term.getOperand(0).getReg())
          .addMBB(Term.getOperand(1).getMBB());
      Term.eraseFromParent();
      break;
    } else if (Term.getOpcode() == TVM::IFELSE) {
      MachineOperand ThenBB = Term.getOperand(2);
      MachineOperand ElseBB = Term.getOperand(1);
      assert(ThenBB.isMBB() && ElseBB.isMBB() && "Must be a basic block");
      assert(MLI.getLoopFor(ThenBB.getMBB()) == &Loop ||
             MLI.getLoopFor(ElseBB.getMBB()) == &Loop);
      BuildMI(&Term, TII.get(TVM::UNTIL))
          .addReg(Term.getOperand(0).getReg())
          .addMBB(ThenBB.getMBB());
      BuildMI(&Term, TII.get(TVM::JMPX)).addMBB(ElseBB.getMBB());
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
