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

bool TVMLoopInstructions::runOnMachineFunction(MachineFunction &) {
  return false;
}
