//===-- TVMLowerIntrinsics.cpp -----------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "TVMSubtarget.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Module.h"
#include "llvm/Transforms/Utils/LowerMemIntrinsics.h"

#define DEBUG_TYPE "tvm-lower-intrinsics"

using namespace llvm;

namespace {

class TVMLowerIntrinsics : public ModulePass {
public:
  static char ID;

  TVMLowerIntrinsics() : ModulePass(ID) {}

  bool runOnModule(Module &M) override;
  bool expandMemIntrinsicUses(Function &F);
  StringRef getPassName() const override {
    return "TVM Lower Intrinsics";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<TargetTransformInfoWrapperPass>();
  }
};

}

char TVMLowerIntrinsics::ID = 0;

INITIALIZE_PASS(TVMLowerIntrinsics, DEBUG_TYPE, "TVM Lower intrinsics", false,
                false)

bool TVMLowerIntrinsics::expandMemIntrinsicUses(Function &F) {
  Intrinsic::ID ID = F.getIntrinsicID();
  bool Changed = false;

  for (auto I = F.user_begin(), E = F.user_end(); I != E;) {
    Instruction *Inst = cast<Instruction>(*I);
    ++I;

    switch (ID) {
    default:
      continue;
    case Intrinsic::memcpy: {
      auto *Memcpy = cast<MemCpyInst>(Inst);
      Function *ParentFunc = Memcpy->getParent()->getParent();
      const TargetTransformInfo &TTI =
          getAnalysis<TargetTransformInfoWrapperPass>().getTTI(*ParentFunc);
      expandMemCpyAsLoopTVM(Memcpy, TTI);
      break;
    }
    case Intrinsic::memmove:
      expandMemMoveAsLoopTVM(cast<MemMoveInst>(Inst));
      break;
    case Intrinsic::memset:
      expandMemSetAsLoopTVM(cast<MemSetInst>(Inst));
      break;
    }
    Changed = true;
    Inst->eraseFromParent();
  }
  return Changed;
}

bool TVMLowerIntrinsics::runOnModule(Module &M) {
  bool Changed = false;

  for (Function &F : M) {
    if (!F.isDeclaration())
      continue;

    switch (F.getIntrinsicID()) {
    case Intrinsic::memcpy:
    case Intrinsic::memmove:
    case Intrinsic::memset:
      if (expandMemIntrinsicUses(F))
        Changed = true;
      break;
    default:
      break;
    }
  }

  return Changed;
}

ModulePass *llvm::createTVMLowerIntrinsicsPass() {
  return new TVMLowerIntrinsics();
}
