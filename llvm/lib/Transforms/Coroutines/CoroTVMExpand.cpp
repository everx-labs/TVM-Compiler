//===- CoroTVMExpand.cpp - Coroutine TVM intrinsics expansion Pass --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This pass expands tvm-specific coroutine intrinsics
//===----------------------------------------------------------------------===//

#include "CoroInternal.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/CallGraphSCCPass.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Pass.h"
#include "llvm/Support/ErrorHandling.h"
#include "TVMTypesSerialize.h"

using namespace llvm;

#define DEBUG_TYPE "coro-tvm-expand"

namespace {
// Created on demand if CoroTVMExpand pass has work to do.
class Lowerer : public coro::LowererBase {
  IRBuilder<> Builder;

  void lowerSerialize(IntrinsicInst *II);
  void lowerDeserialize(IntrinsicInst *II);
public:
  Lowerer(Module &M)
      : LowererBase(M), Builder(Context) {}
  bool lowerTVMIntrinsics(Function &F);
};

CoroIdInst *getFunctionCoroId(Function &F) {
  for (auto &I : instructions(F)) {
    if (auto *CII = dyn_cast<CoroIdInst>(&I)) {
      assert(CII->getInfo().isPostSplit());
      if (CII->getCoroutine() == CII->getFunction())
        return CII;
    }
  }
  return nullptr;
}

// Given a resume function @f.resume(%f.frame* %frame), returns %f.frame type.
Type *getFrameType(Function *Resume) {
  auto *ArgType = Resume->arg_begin()->getType();
  return cast<PointerType>(ArgType)->getElementType();
}
Type *getFrameTypeFromCoroId(const CoroIdInst *CoroId) {
  ConstantArray *Resumers = CoroId->getInfo().Resumers;
  assert(Resumers && "PostSplit coro.id Info argument must refer to an array"
                     "of coroutine subfunctions");
  auto *ResumeAddrConstant =
      ConstantExpr::getExtractValue(Resumers, CoroSubFnInst::ResumeIndex);
  return getFrameType(cast<Function>(ResumeAddrConstant));
}
Type *getFrameTypeFromCoroutine(Function &Coroutine) {
  const auto *CoroId = getFunctionCoroId(Coroutine);
  assert(CoroId);
  return getFrameTypeFromCoroId(CoroId);
}

void postExpandCleanup(Function &F) {
  removeUnreachableBlocks(F);
  legacy::FunctionPassManager FPM(F.getParent());

  FPM.add(createVerifierPass());
  FPM.add(createSCCPPass());
  FPM.add(createCFGSimplificationPass());
  FPM.add(createEarlyCSEPass());
  FPM.add(createCFGSimplificationPass());

  FPM.doInitialization();
  FPM.run(F);
  FPM.doFinalization();
}

} // namespace

static constexpr unsigned SliceSizeBits = 10;

#ifdef TVMINTR
#error TVMINTR override
#endif
#define TVMINTR(FUNC) \
  Intrinsic::getDeclaration(&TheModule, FUNC)

void Lowerer::lowerSerialize(IntrinsicInst *Intrin) {
  Value *CellBuilder = Intrin->getArgOperand(0);
  Value *Frame = Intrin->getArgOperand(1);
  auto *Func = Intrin->getArgOperand(2)->stripPointerCasts();
  assert(Func);
  auto *FrameTy = getFrameTypeFromCoroutine(*cast<Function>(Func));
  StructType *STy = dyn_cast<StructType>(FrameTy);
  assert(STy);

  Builder.SetInsertPoint(Intrin);
  Frame = Builder.CreateBitCast(Frame, FrameTy->getPointerTo());
  types_pattern pattern(TheModule, Builder, STy, Frame);
  // To see the serialization pattern, uncomment the next line
  // dbgs() << "serialize: " << pattern << "\n";
  Value *RetCell = pattern.store(TheModule, Builder, CellBuilder);

  Intrin->replaceAllUsesWith(RetCell);
  Intrin->eraseFromParent();
}

void Lowerer::lowerDeserialize(IntrinsicInst *Intrin) {
  Value *ReadSlice = Intrin->getArgOperand(0);
  auto *Func = Intrin->getArgOperand(1)->stripPointerCasts();
  assert(Func);
  Value *NewThis = Intrin->getArgOperand(2);

  auto *CoroId = getFunctionCoroId(*cast<Function>(Func));
  auto *FrameTy = getFrameTypeFromCoroId(CoroId);
  StructType *STy = dyn_cast<StructType>(FrameTy);
  assert(STy);

  auto *CoroIdClone = CoroId->clone();
  CoroIdClone->insertBefore(Intrin);

  const auto &DL = TheModule.getDataLayout();

  auto *IntPtrTy = Builder.getInt257Ty();
  Value *AllocSize = ConstantInt::get(IntPtrTy, DL.getStructLayout(STy)->getSizeInBytes());

  BasicBlock *CurrentBlock = Intrin->getParent();
  BasicBlock *AllocBB = CurrentBlock->splitBasicBlock(Intrin, "AllocBB");
  CurrentBlock->getTerminator()->eraseFromParent();
  BasicBlock *InitBB = AllocBB->splitBasicBlock(AllocBB->begin(), "InitBB");

  Builder.SetInsertPoint(CurrentBlock);
  Value *NullFrame = Builder.CreateBitCast(NullPtr, FrameTy->getPointerTo());
  // Backend is allowed to elide memory allocations, to help it, emit
  // auto mem = coro.alloc() ? ... allocation code ... : 0;
  auto *CoroAllocFn = Intrinsic::getDeclaration(&TheModule, Intrinsic::coro_alloc);
  auto *CoroAlloc = Builder.CreateCall(CoroAllocFn, {CoroIdClone});
  Builder.CreateCondBr(CoroAlloc, AllocBB, InitBB);

  Builder.SetInsertPoint(AllocBB);
  Value *AllocateCall =
      CallInst::CreateMalloc(AllocBB->getTerminator(), IntPtrTy, FrameTy,
                             AllocSize, Builder.getInt257(1),
                             nullptr, "Frame.Malloced");
  Builder.SetInsertPoint(Intrin);

  // Pass the result of the allocation to coro.begin.
  auto *Phi = Builder.CreatePHI(AllocateCall->getType(), 2);
  Phi->addIncoming(NullFrame, CurrentBlock);
  Phi->addIncoming(AllocateCall, AllocBB);

  Value *PhiPtr = Builder.CreateBitCast(Phi, Type::getIntBytePtrTy(Context));

  auto *CoroBeginFn = Intrinsic::getDeclaration(&TheModule, Intrinsic::coro_begin);
  Value *Frame = Builder.CreateCall(CoroBeginFn, {CoroIdClone, PhiPtr});
  Frame = Builder.CreateBitCast(Frame, FrameTy->getPointerTo());

  ConstantArray *Resumers = CoroId->getInfo().Resumers;
  assert(Resumers && "PostSplit coro.id Info argument must refer to an array"
                     "of coroutine subfunctions");
  auto *ResumeAddrConstant =
      ConstantExpr::getExtractValue(Resumers, CoroSubFnInst::ResumeIndex);
  auto *ResumePtr = Builder.CreateConstInBoundsGEP2_32(FrameTy, Frame, 0, 0);
  Builder.CreateStore(ResumeAddrConstant, ResumePtr);
  auto *DestroyAddrConstant =
      ConstantExpr::getExtractValue(Resumers, CoroSubFnInst::DestroyIndex);
  auto *DestroyPtr = Builder.CreateConstInBoundsGEP2_32(FrameTy, Frame, 0, 1);
  Builder.CreateStore(DestroyAddrConstant, DestroyPtr);

  types_pattern pattern(TheModule, Builder, STy, Frame);
  // To see the deserialization pattern, uncomment the next line
  // dbgs() << "deserialize: " << pattern << "\n";
  pattern.load(TheModule, Builder, ReadSlice, NewThis);

  auto *Rv = Builder.CreateBitCast(Frame, Intrin->getType());

  Intrin->replaceAllUsesWith(Rv);
  Intrin->eraseFromParent();
}
#undef TVMINTR

bool Lowerer::lowerTVMIntrinsics(Function &F) {
  bool Changed = false;
  SmallVector<Instruction *, 1> Insts;

  // Changing control-flow while iterating through it is a bad idea, so gather a
  // list of all atomic instructions before we start.
  for (auto II = inst_begin(F), E = inst_end(F); II != E;) {
    Instruction *I = &*II++;
    if (auto CS = dyn_cast<CallBase>(I)) {
      switch (CS->getIntrinsicID()) {
      case Intrinsic::coro_tvm_serialize:
      case Intrinsic::coro_tvm_deserialize:
        Insts.push_back(I);
        break;
      default:
        continue;
      }
    }
  }

  for (Instruction *I : Insts) {
     if (auto CS = dyn_cast<CallBase>(I)) {
      switch (CS->getIntrinsicID()) {
      default:
        continue;
      case Intrinsic::coro_tvm_serialize:
        lowerSerialize(cast<IntrinsicInst>(I));
        break;
      case Intrinsic::coro_tvm_deserialize:
        lowerDeserialize(cast<IntrinsicInst>(I));
        break;
      }
      Changed = true;
    }
  }
  return Changed;
}

//===----------------------------------------------------------------------===//
//                              Top Level Driver
//===----------------------------------------------------------------------===//

namespace {
struct CoroTVMExpand : FunctionPass {
  static char ID;
  CoroTVMExpand() : FunctionPass(ID) {
    initializeCoroTVMExpandPass(*PassRegistry::getPassRegistry());
  }

  std::unique_ptr<Lowerer> L;

  bool doInitialization(Module &M) override {
    if (coro::declaresIntrinsics(M, {"llvm.coro.tvm.serialize", "llvm.coro.tvm.deserialize"}))
      L = std::make_unique<Lowerer>(M);
    return false;
  }

  bool runOnFunction(Function &F) override {
    if (!L)
      return false;

    if (L->lowerTVMIntrinsics(F)) {
      postExpandCleanup(F);
      return true;
    }
    return false;
  }
  void getAnalysisUsage(AnalysisUsage &) const override {}
  StringRef getPassName() const override { return "Coroutine TVM intrinsics expansion"; }
};
}

char CoroTVMExpand::ID = 0;
INITIALIZE_PASS(CoroTVMExpand, "coro-tvm-expand",
                "Coroutine TVM intrinsics expansion",
                false, false)

Pass *llvm::createCoroTVMExpandPass() { return new CoroTVMExpand(); }
