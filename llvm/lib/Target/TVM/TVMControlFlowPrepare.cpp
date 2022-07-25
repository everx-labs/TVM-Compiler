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
/// Implements a pass that for each basic block having 2 or more predecessors
/// excluding BBs that are loop headers inserts an empty basic block in between
/// of the processing BB and predecessor. Thus a newly created BB replaces the
/// original one for the processing BB and it becomes a successor of the
/// original predecessor.
/// Later TVM backend triggers stack cleaning logic in the beginning of each BB
/// (including newly created once) and removes all the registers that are
/// dead. It prevents cases when dead variables pass though an edge in CFG.
/// Consider a terminator br i1 %cond, label %bb1, label %bb2. Liveliness
/// analysis detect that after br both live variables of bb1 and bb2 has to
/// be kept in stack we pass to these basic block, since the compiler can't
/// know statically which branch will be chosen. It breaks the invariant that
/// only live registers are passed through CFG and ultimately leads to stack
/// model crashes. The pass introduces the fix by removing dead variable in the
/// BBs inserted between the original predecessor and a basic block.
///
/// TODO:
/// The pass makes the CFG more complicated, and it should be fixed either
/// by a separate blocks removing / inlining redundant basic blocks or by
/// trying to exploit the following pattern in the pass itself:
/// PUSHCONT {bb1}
/// IFJMP
/// <stack cleanup since we know the right successor here>
/// PUSHCONT {bb2}
/// JMPX
///
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Operator.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
using namespace llvm;

#define DEBUG_TYPE "tvm-control-flow-prepare"

namespace {
class TVMControlFlowPrepare final : public FunctionPass {
  StringRef getPassName() const override {
    return "Prepare control-flow for ISel";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  bool runOnFunction(Function &F) override;

public:
  static char ID;
  explicit TVMControlFlowPrepare() : FunctionPass(ID) {}
};

void replaceSuccessorWith(Instruction *Terminator, BasicBlock *OriginalBB,
                          BasicBlock *NewBB) {
  assert(Terminator && OriginalBB && NewBB);
  if (auto *Branch = dyn_cast<BranchInst>(Terminator)) {
    if (Branch->getSuccessor(0) == OriginalBB) {
      Branch->setSuccessor(0, NewBB);
      return;
    }
    assert(Branch->isConditional() && "OriginalBB missing");
    assert(Branch->getSuccessor(1) == OriginalBB && "OriginalBB is missing");
    Branch->setSuccessor(1, NewBB);
    return;
  }
  llvm_unreachable("Not implemented");
}

void insertCleanupBBs(BasicBlock *BB) {
  auto *F = BB->getParent();
  std::string BBName = BB->getName().str();
  std::vector<BasicBlock *> Predecessors(pred_begin(BB), pred_end(BB));
  for (auto *Predecessor : Predecessors) {
    auto *Terminator = Predecessor->getTerminator();
    std::string PredName = Predecessor->getName().str();

    auto NewBB = BasicBlock::Create(BB->getContext(),
                                    PredName + "." + BBName + ".cleanup", F);
    IRBuilder<> Builder(NewBB);
    Builder.CreateBr(BB);
    replaceSuccessorWith(Terminator, BB, NewBB);
    for (auto &Phi : BB->phis()) {
      // TODO: LLVM update - replaceIncomingBlockWith (BasicBlock *Old,
      // BasicBlock *New)
      for (size_t I = 0, E = Phi.getNumIncomingValues(); I < E; ++I)
        if (Phi.getIncomingBlock(I) == Predecessor)
          Phi.setIncomingBlock(I, NewBB);
    }
  }
}

} // End anonymous namespace

char TVMControlFlowPrepare::ID = 0;
INITIALIZE_PASS(TVMControlFlowPrepare, DEBUG_TYPE,
                "Prepare control-flow for ISel", false, false)

FunctionPass *llvm::createTVMControlFlowPrepare() {
  return new TVMControlFlowPrepare();
}

bool TVMControlFlowPrepare::runOnFunction(Function &F) {
  bool Changed = false;

  LLVM_DEBUG(dbgs() << "runnning TVMControlFlowPrepare on " << F.getName()
                    << "\n");
  std::vector<BasicBlock *> OriginalBBs;
  llvm::transform(F, std::back_inserter(OriginalBBs),
                  [](BasicBlock &BB) { return &BB; });
  for (auto *BB : OriginalBBs) {
    if (pred_size(BB) > 1) {
      insertCleanupBBs(BB);
      Changed = true;
    }
  }
  return Changed;
}
