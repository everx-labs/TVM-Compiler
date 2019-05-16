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
/// Replace backedges in CFG with loop instructions.
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
class TVMLoopInstructions final : public MachineFunctionPass {
  StringRef getPassName() const override { return "TVM peephole optimizer"; }

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
INITIALIZE_PASS(TVMLoopInstructions, DEBUG_TYPE, "TVM peephole optimizations",
                false, false)

FunctionPass *llvm::createTVMLoopInstructions() {
  return new TVMLoopInstructions();
}

bool TVMLoopInstructions::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG({
    dbgs() << "********** Loop Instructions **********\n"
           << "********** Function: " << MF.getName() << '\n';
  });
  const auto &MLI = getAnalysis<MachineLoopInfo>();
  const auto &TII = *MF.getSubtarget<TVMSubtarget>().getInstrInfo();

  for (auto &BB : MF) {
    MachineLoop *Loop = MLI.getLoopFor(&BB);
    if (Loop) {
      assert(Loop->findLoopControlBlock() == &BB &&
             "Loops with multible BBs are not supported yet");
      MachineInstr &FirstTerminator = *BB.getFirstTerminator();
      // TODO: assumption: all terminators have exactly 1 bb to go.
      switch (FirstTerminator.getOpcode()) {
      case TVM::IFJMP: {
        MachineOperand ArgBB = FirstTerminator.getOperand(1);
        assert(ArgBB.isMBB() && "Must be a basic block");
        assert(ArgBB.getMBB() == &BB &&
               "The backedge must be the first terminator, otherwise "
               "unimplemented yet.");
        BuildMI(&FirstTerminator, TII.get(TVM::UNTIL))
            .addReg(FirstTerminator.getOperand(0).getReg())
            .addMBB(FirstTerminator.getOperand(1).getMBB());
        FirstTerminator.eraseFromParent();
        break;
      }
      default:
        llvm_unreachable("Unimplemented");
      }
    }
  }
  return false;
}
