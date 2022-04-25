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
/// Ensure that all loops are suitable for code generation.
///
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "llvm/InitializePasses.h"
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

#define DEBUG_TYPE "tvm-loop-prepare"

namespace {
class TVMLoopPrepare final : public FunctionPass {
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
  explicit TVMLoopPrepare() : FunctionPass(ID) {}
};
} // End anonymous namespace

char TVMLoopPrepare::ID = 0;
INITIALIZE_PASS_BEGIN(TVMLoopPrepare, DEBUG_TYPE,
                      "Prepare loops for codegen", false, false)
INITIALIZE_PASS_DEPENDENCY(LoopInfoWrapperPass)
INITIALIZE_PASS_END(TVMLoopPrepare, DEBUG_TYPE,
                    "Prepare loops for codegen", false, false)

FunctionPass *llvm::createTVMLoopPrepare() { return new TVMLoopPrepare(); }

/// Ensure that TrueBB for \p Term instruction belongs to loop \p L.
static bool canonicalizeTerminator(BranchInst *Term, Loop &L) {
  BasicBlock *BrSucc = Term->getSuccessor(0);
  if (Term->isUnconditional() || L.contains(BrSucc))
    return false;
  Term->swapSuccessors();
  Value *Cond = Term->getCondition();
  IRBuilder<> Builder(Term);
  Cond = Builder.CreateNot(Cond);
  Term->setCondition(Cond);
  return true;
}

bool TVMLoopPrepare::runOnFunction(Function &F) {
  LLVM_DEBUG(dbgs() << "runnning TVMLoopPrepare on " << F.getName() << "\n");
  const LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
  DenseMap<Loop *, bool> LoopsProcessed;
  bool Changed = false;
  for (auto &BB : F) {
    Loop *L = LI.getLoopFor(&BB);
    if (!L || LoopsProcessed[L])
      continue;
    LoopsProcessed[L] = true;
    BasicBlock *Predecessor = L->getLoopPredecessor();
    assert(Predecessor && "Unexpected shape of a loop");
    Instruction *Term = Predecessor->getTerminator();
    if (auto *Br = dyn_cast<BranchInst>(Term))
      Changed |= canonicalizeTerminator(Br, *L);
    BasicBlock *Latch = L->getLoopLatch();
    assert(Latch);
    Term = Latch->getTerminator();
    if (auto *Br = dyn_cast<BranchInst>(Term))
      Changed |= canonicalizeTerminator(Br, *L);
  }
  return Changed;
}
