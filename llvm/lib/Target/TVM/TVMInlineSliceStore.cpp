//===------------- TVMInlineSliceStore.cpp - Inline slice store -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Replace store slice with its content.
///
//===----------------------------------------------------------------------===//

#include <vector>

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

#include "TVMUtilities.h"

using namespace llvm;

#define DEBUG_TYPE "tvm-inline-slice-store"

namespace {
class TVMInlineSliceStore final : public BasicBlockPass {
  StringRef getPassName() const override {
    return "Replace store slice with its content";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  bool runOnBasicBlock(BasicBlock &BB) override;

public:
  static char ID;
  explicit TVMInlineSliceStore() : BasicBlockPass(ID) {}
};
} // End anonymous namespace

char TVMInlineSliceStore::ID = 0;
INITIALIZE_PASS(TVMInlineSliceStore, DEBUG_TYPE, "Inline store slice",
                false, false)

BasicBlockPass *llvm::createTVMInlineSliceStore() { return new TVMInlineSliceStore(); }

static CallSite storeCallSite(Instruction *I) {
  auto CS = CallSite(I);
  if (!CS)
    return CS;
  if (CS.getIntrinsicID() == Intrinsic::tvm_stslice)
    return CS;
  return {};
}

/// For a given store \p CS return its builder argument.
static Value* argBuilder(CallSite CS) {
  if (!CS)
    return nullptr;
  unsigned ID = CS.getIntrinsicID();
  if (TVM::isIntStore(ID))
    return CS.getArgument(1);
  if (TVM::isVarIntStore(ID))
    return CS.getArgument(0);
  return nullptr;
}

/// Return stores to a slice in reverse order
static std::vector<Value *> collectStores(Value *Slice) {
  assert(Slice);
  std::vector<Value *> Values;
  if (!Slice->hasOneUse())
    return {};
  auto CS = CallSite(Slice);
  if (!CS || CS.getIntrinsicID() != Intrinsic::tvm_ctos)
    return {};
  Value *Cell = CS.getArgument(0);
  if (!Cell->hasOneUse())
    return {};
  CS = CallSite(Cell);
  if (!CS || CS.getIntrinsicID() != Intrinsic::tvm_endc)
    return {};
  Value *Builder = CS.getArgument(0);
  CS = CallSite(Builder);
  while (Builder) {
    if (!Builder->hasOneUse())
      break;
    auto *NextBuilder = argBuilder(CS);
    if (!NextBuilder)
      break;
    Values.push_back(Builder);
    Builder = NextBuilder;
    CS = CallSite(Builder);
  }
  if (!CS || (CS.getIntrinsicID() != Intrinsic::tvm_newc))
    return {};
  return Values;
}

/// Provide arguments for \p Store replacement.
/// The argument are the same as for original Store but builder is substituted
/// with \p Builder.
static std::vector<Value *> fillArguments(Value *Store, Value *Builder) {
  auto CS = CallSite(Store);
  assert(CS);
  unsigned ID = CS.getIntrinsicID();
  if (TVM::isIntStore(ID))
    return {CS.getArgument(0), Builder, CS.getArgument(2)};
  if (TVM::isVarIntStore(ID))
    return {Builder, CS.getArgument(1)};
  llvm_unreachable("Unexpected Store");
  return {};
}

/// Inline \p Stores and return the address of the last builder
/// to replace the original one.
static Value *inlineStores(Instruction *I,
                           const std::vector<Value *> &Stores) {
  assert(!Stores.empty());
  IRBuilder<> Builder(I);
  auto CS = CallSite(I);
  Value *Prev = CS.getArgument(1);
  for (Value *Store : make_range(Stores.rbegin(), Stores.rend())) {
    CS = CallSite(Store);
    assert(CS);
    std::vector<Value *> Args = fillArguments(Store, Prev);
    auto *Fn = Intrinsic::getDeclaration(I->getModule(), CS.getIntrinsicID());
    Prev = Builder.CreateCall(Fn, Args);
  }
  return Prev;
}

bool TVMInlineSliceStore::runOnBasicBlock(BasicBlock &BB) {
  std::vector<Value *> RemoveInst;
  auto It = BB.begin(), End = BB.end();
  while (It != End) {
    It = std::find_if(It, End, [](Instruction &I) {
      return static_cast<bool>(I.hasOneUse() && storeCallSite(&I));
    });
    if (It != End) {
      auto CS = storeCallSite(&*It);
      Value *Slice = CS.getArgument(0);
      std::vector<Value *> Stores = collectStores(Slice);
      if (!Stores.empty()) {
        Value *LastStore = inlineStores(&*It, Stores);
        It->replaceAllUsesWith(LastStore);
        RemoveInst.push_back(&*It);
      }
    } else {
      break;
    }
    ++It;
  }
  return !RemoveInst.empty();
}
