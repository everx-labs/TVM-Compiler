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
#include "llvm/IR/CallSite.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Pass.h"
#include "llvm/Support/ErrorHandling.h"

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
  Value *CellStack = Builder.CreateCall(TVMINTR(Intrinsic::tvm_tuple));
  CellStack = Builder.CreateBitCast(CellStack, Type::getTVMTupleTy(Context), "CellStack");

  const auto &DL = TheModule.getDataLayout();

  auto CheckOverflow = [this, Intrin, &CellBuilder, &CellStack](Value *CmpOK) {
    BasicBlock *CurrentBlock = Intrin->getParent();
    BasicBlock *OverflowBB = CurrentBlock->splitBasicBlock(Intrin, "OverflowBB");
    CurrentBlock->getTerminator()->eraseFromParent();
    BasicBlock *OkBB = OverflowBB->splitBasicBlock(OverflowBB->begin(), "OkBB");

    Builder.SetInsertPoint(CurrentBlock);
    Builder.CreateCondBr(CmpOK, OkBB, OverflowBB);

    // OverflowBB:
    Builder.SetInsertPoint(OverflowBB->getTerminator());
    auto *NextCell = Builder.CreateCall(TVMINTR(Intrinsic::tvm_newc), {});
    auto *OldCell = Builder.CreateBitCast(CellBuilder, Builder.getInt257Ty());
    auto *NewStack = Builder.CreateCall(TVMINTR(Intrinsic::tvm_tpush),
                                        {CellStack, OldCell});
    // OkBB:
    Builder.SetInsertPoint(Intrin);
    auto *CellBuilderPhi = Builder.CreatePHI(Type::getTVMBuilderTy(Context), 2);
    CellBuilderPhi->addIncoming(CellBuilder, CurrentBlock);
    CellBuilderPhi->addIncoming(NextCell, OverflowBB);

    auto *CellStackPhi = Builder.CreatePHI(Type::getTVMTupleTy(Context), 2);
    CellStackPhi->addIncoming(CellStack, CurrentBlock);
    CellStackPhi->addIncoming(NewStack, OverflowBB);

    CellBuilder = CellBuilderPhi;
    CellStack = CellStackPhi;
  };

  // skipping resume_ptr, cleanup_ptr and promise
  auto Elems = drop_begin(STy->elements(), 3);
  unsigned Idx = 3;
  for (Type *Ty : Elems) {
    auto *ElemPtr = Builder.CreateConstInBoundsGEP2_32(FrameTy, Frame, 0, Idx);
    auto *Load = Builder.CreateLoad(ElemPtr);

    if (Ty->isPointerTy()) {
      ++Idx;
      continue;
    }
    auto Sz = DL.getTypeSizeInBits(Ty);
    assert(Sz <= 257);

    if (Ty->isTVMSliceTy()) {
      auto *SlizeSz = Builder.CreateCall(TVMINTR(Intrinsic::tvm_sbits), {Load});
      auto *FullSz =
          Builder.CreateAdd(SlizeSz, Builder.getInt257(SliceSizeBits));
      auto *RemBits =
          Builder.CreateCall(TVMINTR(Intrinsic::tvm_brembits), {CellBuilder});
      auto *CmpOK = Builder.CreateICmpULE(FullSz, RemBits);
      CheckOverflow(CmpOK);

      CellBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_stu),
        {SlizeSz, CellBuilder, Builder.getInt257(SliceSizeBits)});
      CellBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_stslice),
        {Load, CellBuilder});
    } else if (Ty->isTVMCellTy()) {
      auto *RemRefs =
          Builder.CreateCall(TVMINTR(Intrinsic::tvm_bremrefs), {CellBuilder});
      auto *CmpOK = Builder.CreateICmpULE(Builder.getInt257(2), RemRefs);
      CheckOverflow(CmpOK);
      CellBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_stref),
                                       {Load, CellBuilder});
    } else if (Sz <= 256) {
      auto *RemBits = Builder.CreateCall(TVMINTR(Intrinsic::tvm_brembits),
                                         {CellBuilder});
      auto *CmpOK = Builder.CreateICmpULE(Builder.getInt257(Sz), RemBits);
      CheckOverflow(CmpOK);
      auto *Val = Builder.CreateZExt(Load, Builder.getInt257Ty());
      CellBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_stu),
        {Val, CellBuilder, Builder.getInt257(Sz)});
    } else {
      auto *RemBits = Builder.CreateCall(TVMINTR(Intrinsic::tvm_brembits),
                                         {CellBuilder});
      auto *CmpOK = Builder.CreateICmpULE(Builder.getInt257(257), RemBits);
      CheckOverflow(CmpOK);
      CellBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_sti),
        {Load, CellBuilder, Builder.getInt257(Sz)});
    }
    ++Idx;
  }
  Value *RetCell = Builder.CreateCall(TVMINTR(Intrinsic::tvm_endc),
                                     {CellBuilder});
  // while (tlen(tup)) {
  // prev_builder = tpop(tup)
  // prev_builder.stref(RetCell)
  // RetCell = prev_builder.endc()
  // }

  BasicBlock *PreloopBB = Intrin->getParent();
  BasicBlock *LoopBB = PreloopBB->splitBasicBlock(Intrin, "LoopBB");
  PreloopBB->getTerminator()->eraseFromParent();
  BasicBlock *ExitBB = LoopBB->splitBasicBlock(LoopBB->begin(), "ExitBB");
  LoopBB->getTerminator()->eraseFromParent();

  // Preloop:
  Builder.SetInsertPoint(PreloopBB);
  auto *Tlen = Builder.CreateCall(TVMINTR(Intrinsic::tvm_tlen), {CellStack});
  auto *HasPrev = Builder.CreateICmpNE(Tlen, Builder.getInt257(0));
  Builder.CreateCondBr(HasPrev, LoopBB, ExitBB);

  // Loop:
  Builder.SetInsertPoint(LoopBB);
  auto *StackPhi = Builder.CreatePHI(Type::getTVMTupleTy(Context), 2);
  auto *NextCellPhi = Builder.CreatePHI(Type::getTVMCellTy(Context), 2);
  StackPhi->addIncoming(CellStack, PreloopBB);
  NextCellPhi->addIncoming(RetCell, PreloopBB);

  auto *TpopCall = Builder.CreateCall(TVMINTR(Intrinsic::tvm_tpop), {StackPhi});
  auto *NewStack = Builder.CreateExtractValue(TpopCall, 0);
  auto *Tpop = Builder.CreateExtractValue(TpopCall, 1);
  auto *PrevBuilder = Builder.CreateBitCast(Tpop, Type::getTVMBuilderTy(Context));

  PrevBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_stref),
                                   {NextCellPhi, PrevBuilder});

  auto *NewNextCell = Builder.CreateCall(TVMINTR(Intrinsic::tvm_endc),
                                         {PrevBuilder});
  StackPhi->addIncoming(NewStack, LoopBB);
  NextCellPhi->addIncoming(NewNextCell, LoopBB);

  auto *TlenInner = Builder.CreateCall(TVMINTR(Intrinsic::tvm_tlen), {NewStack});
  auto *HasPrevInner = Builder.CreateICmpNE(TlenInner, Builder.getInt257(0));
  Builder.CreateCondBr(HasPrevInner, LoopBB, ExitBB);

  // Exit:
  Builder.SetInsertPoint(Intrin);
  auto *RetCellPhi = Builder.CreatePHI(Type::getTVMCellTy(Context), 2);
  RetCellPhi->addIncoming(RetCell, PreloopBB);
  RetCellPhi->addIncoming(NewNextCell, LoopBB);
  RetCell = RetCellPhi;

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

  auto CheckOverflow = [this, Intrin](Value *Sl, bool Refs)->Value* {
    Value *CmpOK;
    if (Refs) {
      auto *RefsCnt = Builder.CreateCall(TVMINTR(Intrinsic::tvm_srefs), {Sl});
      CmpOK = Builder.CreateICmpUGE(RefsCnt, Builder.getInt257(2));
    } else {
      auto *IsEmpty = Builder.CreateCall(TVMINTR(Intrinsic::tvm_sdempty), {Sl});
      CmpOK = Builder.CreateICmpEQ(IsEmpty, Builder.getInt257(0));
    }

    BasicBlock *CurrentBlock = Intrin->getParent();
    BasicBlock *OverflowBB = CurrentBlock->splitBasicBlock(Intrin, "OverflowBB");
    CurrentBlock->getTerminator()->eraseFromParent();
    BasicBlock *OkBB = OverflowBB->splitBasicBlock(OverflowBB->begin(), "OkBB");
    Builder.SetInsertPoint(CurrentBlock);
    Builder.CreateCondBr(CmpOK, OkBB, OverflowBB);
    // OverflowBB:
    Builder.SetInsertPoint(OverflowBB->getTerminator());
    auto *Ldref = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldrefrtos), {Sl});
    auto *NextSl = Builder.CreateExtractValue(Ldref, 1);
    // OkBB:
    Builder.SetInsertPoint(Intrin);
    auto *ReadSlicePhi = Builder.CreatePHI(Type::getTVMSliceTy(Context), 2);
    ReadSlicePhi->addIncoming(Sl, CurrentBlock);
    ReadSlicePhi->addIncoming(NextSl, OverflowBB);
    return ReadSlicePhi;
  };

  // skipping resume_ptr, cleanup_ptr and promise
  auto Elems = drop_begin(STy->elements(), 3);
  unsigned Idx = 3;
  for (Type *Ty : Elems) {
    if (Ty->isPointerTy()) {
      auto *ElemPtr = Builder.CreateConstInBoundsGEP2_32(FrameTy, Frame, 0, Idx);
      Builder.CreateStore(NewThis->stripPointerCasts(), ElemPtr);
      ++Idx;
      continue;
    }
    auto Sz = DL.getTypeSizeInBits(Ty);
    assert(Sz <= 257);
    auto *ElemSz = Builder.getInt257(Sz);
    if (Ty->isTVMSliceTy()) {
      ReadSlice = CheckOverflow(ReadSlice, false);
      auto *SzValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldu),
        {ReadSlice, Builder.getInt257(SliceSizeBits)});
      auto *SliceSz = Builder.CreateExtractValue(SzValAndSlice, 0);
      ReadSlice = Builder.CreateExtractValue(SzValAndSlice, 1);
      auto *ValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldslice),
                                             {ReadSlice, SliceSz});
      auto *Val = Builder.CreateExtractValue(ValAndSlice, 0);
      ReadSlice = Builder.CreateExtractValue(ValAndSlice, 1);

      auto *ElemPtr = Builder.CreateConstInBoundsGEP2_32(FrameTy, Frame, 0, Idx);
      Builder.CreateStore(Val, ElemPtr);
    } else if (Ty->isTVMCellTy()) {
      ReadSlice = CheckOverflow(ReadSlice, true);
      auto *ValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldref),
                                             {ReadSlice});
      auto *Val = Builder.CreateExtractValue(ValAndSlice, 0);
      ReadSlice = Builder.CreateExtractValue(ValAndSlice, 1);
      auto *ElemPtr = Builder.CreateConstInBoundsGEP2_32(FrameTy, Frame, 0, Idx);
      Builder.CreateStore(Val, ElemPtr);
    } else if (Sz <= 256) {
      ReadSlice = CheckOverflow(ReadSlice, false);
      auto *ValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldu),
                                             {ReadSlice, ElemSz});
      auto *Val = Builder.CreateExtractValue(ValAndSlice, 0);
      ReadSlice = Builder.CreateExtractValue(ValAndSlice, 1);
      Val = Builder.CreateTruncOrBitCast(Val, Ty);
      auto *ElemPtr = Builder.CreateConstInBoundsGEP2_32(FrameTy, Frame, 0, Idx);
      Builder.CreateStore(Val, ElemPtr);
    } else {
      ReadSlice = CheckOverflow(ReadSlice, false);
      auto *ValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldi),
                                             {ReadSlice, ElemSz});
      auto *Val = Builder.CreateExtractValue(ValAndSlice, 0);
      ReadSlice = Builder.CreateExtractValue(ValAndSlice, 1);
      auto *ElemPtr = Builder.CreateConstInBoundsGEP2_32(FrameTy, Frame, 0, Idx);
      Builder.CreateStore(Val, ElemPtr);
    }

    ++Idx;
  }

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
    if (auto CS = CallSite(I)) {
      switch (CS.getIntrinsicID()) {
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
     if (auto CS = CallSite(I)) {
      switch (CS.getIntrinsicID()) {
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
      L = llvm::make_unique<Lowerer>(M);
    return false;
  }

  bool runOnFunction(Function &F) override {
    if (!Triple(F.getParent()->getTargetTriple()).isTVM())
      return false;
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
