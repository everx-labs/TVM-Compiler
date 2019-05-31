//===-- TVMLoopCanonicalize.cpp - Canonicalize loops to simplify codegen --===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Ensure that for a loop predecessor terminating with conditional br
/// true successor is the loop header.
///
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Operator.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
using namespace llvm;

#define DEBUG_TYPE "tvm-loop-entry"

namespace {
class TVMLoopEntry final : public FunctionPass {
  StringRef getPassName() const override {
    return "Fix loop entry to simplify codegen";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<LoopInfoWrapperPass>();
    AU.setPreservesAll();
  }

  bool runOnFunction(Function &F) override;

public:
  static char ID;
  explicit TVMLoopEntry() : FunctionPass(ID) {}
};
} // End anonymous namespace

char TVMLoopEntry::ID = 0;
INITIALIZE_PASS_BEGIN(TVMLoopEntry, DEBUG_TYPE,
                      "Fix loop entry to simplify codegen", false, false)
INITIALIZE_PASS_DEPENDENCY(LoopInfoWrapperPass)
INITIALIZE_PASS_END(TVMLoopEntry, DEBUG_TYPE,
                    "Fix loop entry to simplify codegen", false, false)

FunctionPass *llvm::createTVMLoopEntry() { return new TVMLoopEntry(); }

bool TVMLoopEntry::runOnFunction(Function &F) {
  LLVM_DEBUG(dbgs() << "runnning TVMLoopEntry on " << F.getName() << "\n");
  const LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
  DenseMap<Loop *, bool> LoopsProcessed;
  for (auto &BB : F) {
    Loop *L = LI.getLoopFor(&BB);
    if (!L || LoopsProcessed[L])
      continue;
    LoopsProcessed[L] = true;
    BasicBlock *Predecessor = L->getLoopPredecessor();
    assert(Predecessor && "Unexpected shape of a loop");
    Instruction *Term = Predecessor->getTerminator();
    if (auto *Br = dyn_cast<BranchInst>(Term)) {
      BasicBlock *BrSucc = Br->getSuccessor(0);
      if (LI.getLoopFor(BrSucc) == L || Br->isUnconditional())
        continue;
      Br->swapSuccessors();
      Value *Cond = Br->getCondition();
      IRBuilder<> Builder(Br);
      Cond = Builder.CreateNot(Cond);
      Br->setCondition(Cond);
    }
  }
  return false;
}
