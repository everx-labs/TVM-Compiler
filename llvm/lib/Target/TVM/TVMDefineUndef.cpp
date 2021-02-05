//===------------ TVMDefineUndef.cpp - define undef in stores -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Define undef in sti, stu, stref, stslice.
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

#define DEBUG_TYPE "tvm-define-undef"

namespace {
/// Define undef in sti, stu, stref, stslice.
class TVMDefineUndef final : public BasicBlockPass {
  StringRef getPassName() const override {
    return "Define undefs in store instructions";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {}

  bool runOnBasicBlock(BasicBlock &BB) override;

public:
  static char ID;
  explicit TVMDefineUndef() : BasicBlockPass(ID) {}
};
} // End anonymous namespace

char TVMDefineUndef::ID = 0;
INITIALIZE_PASS(TVMDefineUndef, DEBUG_TYPE,
                "Define undefs in store Instructions", false, false)

BasicBlockPass *llvm::createTVMDefineUndef() { return new TVMDefineUndef(); }

static bool definePlainStore(Instruction &I) {
  auto *Int257Ty = Type::getIntNTy(I.getModule()->getContext(), 257);
  auto *Int257Undef = UndefValue::get(Int257Ty);
  if (I.getOperand(0) != Int257Undef)
    return false;
  I.replaceUsesOfWith(Int257Undef, ConstantInt::get(Int257Ty, 0));
  return true;
}

static bool defineStref(Instruction &I) {
  auto *CellTy = Type::getTVMCellTy(I.getModule()->getContext());
  auto *CellUndef = UndefValue::get(CellTy);
  if (I.getOperand(0) != CellUndef)
    return false;
  IRBuilder<> Builder(&I);
  CallInst *Newc = Builder.CreateIntrinsic(Intrinsic::tvm_newc);
  auto *Fn = Intrinsic::getDeclaration(I.getModule(), Intrinsic::tvm_endc);
  CallInst *Endc = Builder.CreateCall(Fn, ArrayRef<Value *>{Newc});
  I.replaceUsesOfWith(CellUndef, Endc);
  return true;
}

static bool defineStdict(Instruction &I) {
  auto *CellTy = Type::getTVMCellTy(I.getModule()->getContext());
  auto *CellUndef = UndefValue::get(CellTy);
  if (I.getOperand(0) != CellUndef)
    return false;
  IRBuilder<> Builder(&I);
  CallInst *Pushnull = Builder.CreateIntrinsic(Intrinsic::tvm_pushnull);
  auto *Fn = Intrinsic::getDeclaration(I.getModule(), Intrinsic::tvm_cast_to_cell);
  CallInst *Cast = Builder.CreateCall(Fn, ArrayRef<Value *>{Pushnull});
  I.replaceUsesOfWith(CellUndef, Cast);
  return true;
}

static bool defineStslice(Instruction &I) {
  auto *SliceTy = Type::getTVMSliceTy(I.getModule()->getContext());
  auto *SliceUndef = UndefValue::get(SliceTy);
  if (I.getOperand(0) != SliceUndef)
    return false;
  IRBuilder<> Builder(&I);
  CallInst *Newc = Builder.CreateIntrinsic(Intrinsic::tvm_newc);
  auto *Fn = Intrinsic::getDeclaration(I.getModule(), Intrinsic::tvm_endc);
  CallInst *Endc = Builder.CreateCall(Fn, ArrayRef<Value *>{Newc});
  Fn = Intrinsic::getDeclaration(I.getModule(), Intrinsic::tvm_ctos);
  CallInst *Ctos = Builder.CreateCall(Fn, ArrayRef<Value *>{Endc});
  I.replaceUsesOfWith(SliceUndef, Ctos);
  return true;
}

bool TVMDefineUndef::runOnBasicBlock(BasicBlock &BB) {
  bool Changed = false;
  auto It = std::begin(BB);
  while (It != std::end(BB)) {
    auto CS = CallSite(&*It);
    auto &I = *It++;
    if (!CS)
      continue;
    if ((CS.getIntrinsicID() == Intrinsic::tvm_stu ||
         CS.getIntrinsicID() == Intrinsic::tvm_sti))
      Changed |= definePlainStore(I);
    if (CS.getIntrinsicID() == Intrinsic::tvm_stref)
      Changed |= defineStref(I);
    if (CS.getIntrinsicID() == Intrinsic::tvm_stdict)
      Changed |= defineStdict(I);
    if (CS.getIntrinsicID() == Intrinsic::tvm_stslice)
      Changed |= defineStslice(I);
  }
  return Changed;
}
