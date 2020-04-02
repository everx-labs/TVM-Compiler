#include "TVM.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/Utils/CodeExtractor.h"
using namespace llvm;

#define DEBUG_TYPE "tvm-ad-hoc-outliner"

namespace {
class TVMAdHocOutliner : public FunctionPass {
public:
  static char ID;
  TVMAdHocOutliner() : FunctionPass(ID) {}
  bool runOnFunction(Function &F) override;
  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<DominatorTreeWrapperPass>();
  }
};
} // end anonymous namespace

char TVMAdHocOutliner::ID = 0;
INITIALIZE_PASS(TVMAdHocOutliner, "tvm-ad-hoc-outliner",
                "TVM ad-hoc outliner", false, false)

FunctionPass *llvm::createTVMAdHocOutliner() {
  return new TVMAdHocOutliner();
}

bool TVMAdHocOutliner::runOnFunction(Function &F) {
  auto &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
  bool Changed = false;

  if (auto *SI = dyn_cast<SwitchInst>(F.getEntryBlock().getTerminator())) {
    for (auto Case : SI->cases()) {
      auto *BB = Case.getCaseSuccessor();
      SmallVector<BasicBlock *, 16> Blocks;
      DT.getDescendants(BB, Blocks);
      CodeExtractor(Blocks).extractCodeRegion();
    }
    Changed = true;
  }

  return Changed;
}
