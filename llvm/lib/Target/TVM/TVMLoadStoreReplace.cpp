//===-- TVMLoadStoreReplace.cpp - Replace loads/stores with library calls -===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Operator.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
using namespace llvm;

#define DEBUG_TYPE "tvm-load-store-replace"

namespace {
class TVMLoadStoreReplace final : public BasicBlockPass {
  StringRef getPassName() const override {
    return "Replace loads/stores with library calls";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  bool runOnBasicBlock(BasicBlock &BB) override;

public:
  static char ID;
  explicit TVMLoadStoreReplace() : BasicBlockPass(ID) {}
};
} // End anonymous namespace

char TVMLoadStoreReplace::ID = 0;
INITIALIZE_PASS(TVMLoadStoreReplace, DEBUG_TYPE,
                "Replace loads/stores with library calls", false, false)

BasicBlockPass *llvm::createTVMLoadStoreReplace() {
  return new TVMLoadStoreReplace();
}

bool TVMLoadStoreReplace::runOnBasicBlock(BasicBlock &BB) {
  bool Changed = false;

  Module *M = BB.getModule();
  LLVMContext &Context = M->getContext();
  Value *Load = M->getOrInsertFunction("__tvm_load", Type::getByteTy(Context),
      Type::getIntBytePtrTy(Context));
  Value *Store = M->getOrInsertFunction("__tvm_store", Type::getVoidTy(Context),
      Type::getIntBytePtrTy(Context), Type::getByteTy(Context));

  for (auto II = BB.begin(), IE = BB.end(); II != IE; ) {
    Instruction *I = &*II++;
    if (LoadInst *LI = dyn_cast<LoadInst>(I)) {
      CallInst *C = CallInst::Create(Load, {LI->getPointerOperand()}, "", LI);
      BasicBlock::iterator It(LI);
      ReplaceInstWithValue(LI->getParent()->getInstList(), It, C);
      Changed = true;
    } else if (StoreInst *SI = dyn_cast<StoreInst>(I)) {
      Value *Args[] = {SI->getPointerOperand(), SI->getValueOperand()};
      CallInst *C = CallInst::Create(Store, Args);
      ReplaceInstWithInst(I, C);
      Changed = true;
    }
  }

  return Changed;
}
