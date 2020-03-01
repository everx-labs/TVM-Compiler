//===------- TVMStoreCombine.cpp - Combine STU / STI with constants -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Implement pass for merging multiple STU / STI with const argument into
/// a bigger STU / STI.
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
using namespace llvm;

#define DEBUG_TYPE "tvm-store-combine"

namespace {
class TVMStoreCombine final : public BasicBlockPass {
  StringRef getPassName() const override {
    return "Combine STU / STI instructions";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  bool runOnBasicBlock(BasicBlock &BB) override;

public:
  static char ID;
  explicit TVMStoreCombine() : BasicBlockPass(ID) {}
};
} // End anonymous namespace

char TVMStoreCombine::ID = 0;
INITIALIZE_PASS(TVMStoreCombine, DEBUG_TYPE, "Combine STU and STI intrinsics",
                false, false)

BasicBlockPass *llvm::createTVMStoreCombine() { return new TVMStoreCombine(); }

/// Size limit for single Store.
/// Note that 257-bit stores are possible, but they need 2 instructions to be
/// generated.
static const unsigned SizeLimit = 256;

/// Provide call site for STI or STU.
/// Default constructed call site is returned if the instruction is not STI/STU.
static CallSite storeConstCallSite(Instruction *I) {
  auto CS = CallSite(I);
  if (!CS)
    return CS;
  if ((CS.getIntrinsicID() == Intrinsic::tvm_stu ||
       CS.getIntrinsicID() == Intrinsic::tvm_sti) &&
      isa<ConstantInt>(CS.getArgument(0)) &&
      isa<ConstantInt>(CS.getArgument(2)))
    return CS;
  return {};
}

enum class CanCombine {
  No,
  Yes,
  Last // Can combine, but no more stores can be added.
};

/// \brief Determine whether current STI / STU instruction can be merged with
/// previous ones.
/// \param size total size of STI / STU arguments preceeding \p CS that are
/// supposed to be merged.
/// \return flag if the instruction can be merged.
static CanCombine canCombine(unsigned Size, CallSite &CS) {
  auto *Op0 = dyn_cast<ConstantInt>(CS.getArgument(0));
  if (!Op0)
    return CanCombine::No;
  auto *Op2 = dyn_cast<ConstantInt>(CS.getArgument(2));
  if (!Op2)
    return CanCombine::No;
  if (Size + Op2->getZExtValue() > SizeLimit)
    return CanCombine::No;
  if (CS.getInstruction()->getNumUses() > 1u)
    return CanCombine::Last;
  return CanCombine::Yes;
}

/// \brief Combine STU / STI instructions in the given range.
static void combine(BasicBlock::iterator Start, BasicBlock::iterator End) {
  unsigned Size = 0;
  APInt Data(257, 0, false);
  auto It = Start;
  std::vector<Instruction *> RemoveInst;
  for (; It != End; ++It) {
    auto CS = CallSite(&*It);
    APInt Val = cast<ConstantInt>(CS.getArgument(0))->getValue();
    unsigned Sz = cast<ConstantInt>(CS.getArgument(2))->getZExtValue();
    if (Val.slt(0)) {
      APInt Pow2(257, 0, false);
      Pow2.setBit(Sz);
      Val = Pow2 - Val;
    }
    Data <<= Sz;
    Size += Sz;
    Data |= Val;
    RemoveInst.push_back(&*It);
  }
  auto *BuilderArg = CallSite(&*Start).getArgument(1);
  IRBuilder<> Builder(&*It);
  std::vector<Value *> Args{Builder.getInt(Data), BuilderArg,
                            Builder.getIntN(257, Size)};
  auto *Fn = Intrinsic::getDeclaration(It->getModule(), Intrinsic::tvm_stu);
  auto *Inst = Builder.CreateCall(Fn, Args);
  std::prev(std::prev(It))->replaceAllUsesWith(Inst);
  for (auto It = RemoveInst.rbegin(), E = RemoveInst.rend(); It != E; ++It) {
    (*It)->eraseFromParent();
  }
}

/// Try merge STU / STI calls starting from It.
/// \param It iterator of STI or STU intrinsic call.
/// \return iterator pointing at one instruction after the merged STI or STU.
/// TODO: better is to implement not as a peephole, but as def-use walk.
static BasicBlock::iterator tryCombine(BasicBlock::iterator Start,
                                       BasicBlock::iterator End) {
  assert(Start != End);
  auto CS = CallSite(&*Start);
  unsigned Size = cast<ConstantInt>(CS.getArgument(2))->getZExtValue();
  auto It = std::next(Start), Prev = Start;
  bool Combinable = false;
  for (; It != End; Prev = It++) {
    if (auto CurrentCS = storeConstCallSite(&*It)) {
      CanCombine Status = canCombine(Size, CurrentCS);
      if (Status != CanCombine::No && CallSite(&*It).getArgument(1) == &*Prev) {
        Combinable = true;
        Size += cast<ConstantInt>(CurrentCS.getArgument(2))->getZExtValue();
        if (Status == CanCombine::Last || !It->hasOneUse()) {
          ++It;
          break;
        }
        continue;
      }
    }
    break;
  }
  if (Combinable && std::next(Start) != It)
    combine(Start, It);
  return It;
}

bool TVMStoreCombine::runOnBasicBlock(BasicBlock &BB) {
  bool Changed = false;
  auto It = BB.begin(), End = BB.end();
  do {
    It = std::find_if(It, End, [](Instruction &I) {
      return static_cast<bool>(I.hasOneUse() && storeConstCallSite(&I));
    });
    if (It == End)
      break;
    auto Next = std::next(It);
    It = tryCombine(It, End);
    if (It != Next)
      Changed = true;
  } while (true);
  return Changed;
}
