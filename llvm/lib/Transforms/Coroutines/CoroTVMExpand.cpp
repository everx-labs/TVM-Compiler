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
#include "TVMTypesSerialize.h"

using namespace llvm;

#define DEBUG_TYPE "coro-tvm-expand"

namespace {
// Created on demand if CoroTVMExpand pass has work to do.
class Lowerer : public coro::LowererBase {
  IRBuilder<> Builder;

  void lowerSerialize(IntrinsicInst *II);
  void lowerDeserialize(IntrinsicInst *II);
  void prepareStructStore(StructType *STy, Value *Ptr,
                          unsigned StartIdx, Value *&CellBuilder,
                          IntrinsicInst *Intrin, Value *&CellStack);
  void prepareElementStore(Type *Ty, Value *Ptr, Value *&CellBuilder,
                           IntrinsicInst *Intrin, Value *&CellStack);
  void prepareElementLoad(Type *Ty, Value *Ptr, Value *&ReadSlice,
                          IntrinsicInst *Intrin);

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

void Lowerer::prepareElementStore(Type *Ty, Value *Ptr, Value *&CellBuilder,
                                  IntrinsicInst *Intrin, Value *&CellStack) {
  // TODO: generate error if pointer is not contract class type
  if (Ty->isPointerTy())
    return;
  const auto &DL = TheModule.getDataLayout();

  if (StructType *ElemSTy = dyn_cast<StructType>(Ty)) {
    prepareStructStore(ElemSTy, Ptr, 0, CellBuilder, Intrin, CellStack);
    return;
  } else if (ArrayType *ElemArray = dyn_cast<ArrayType>(Ty)) {
    Type *SubTy = ElemArray->getElementType();
    for (unsigned Idx = 0; Idx < ElemArray->getNumElements(); ++Idx) {
      auto *ElemPtr = Builder.CreateConstInBoundsGEP2_32(ElemArray, Ptr, 0, Idx);
      prepareElementStore(SubTy, ElemPtr, CellBuilder, Intrin, CellStack);
    }
    return;
  }
  auto Sz = DL.getTypeSizeInBits(Ty);
  assert(Sz <= 257);
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
  auto *Load = Builder.CreateLoad(Ptr);
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
}

void Lowerer::prepareStructStore(StructType *STy, Value *Ptr,
                                 unsigned StartIdx, Value *&CellBuilder,
                                 IntrinsicInst *Intrin, Value *&CellStack) {
  auto Elems = drop_begin(STy->elements(), StartIdx);
  unsigned Idx = StartIdx;
  for (Type *Ty : Elems) {
    auto *ElemPtr = Builder.CreateConstInBoundsGEP2_32(STy, Ptr, 0, Idx);
    prepareElementStore(Ty, ElemPtr, CellBuilder, Intrin, CellStack);
    ++Idx;
  }
}

void Lowerer::prepareElementLoad(Type *Ty, Value *Ptr, Value *&ReadSlice,
                                 IntrinsicInst *Intrin) {
  if (StructType *STy = dyn_cast<StructType>(Ty)) {
    unsigned Idx = 0;
    for (Type *Ty : STy->elements()) {
      auto *ElemPtr = Builder.CreateConstInBoundsGEP2_32(STy, Ptr, 0, Idx);
      prepareElementLoad(Ty, ElemPtr, ReadSlice, Intrin);
      ++Idx;
    }
    return;
  } else if (ArrayType *ElemArray = dyn_cast<ArrayType>(Ty)) {
    Type *SubTy = ElemArray->getElementType();
    for (unsigned Idx = 0; Idx < ElemArray->getNumElements(); ++Idx) {
      auto *ElemPtr = Builder.CreateConstInBoundsGEP2_32(ElemArray, Ptr, 0, Idx);
      prepareElementLoad(SubTy, ElemPtr, ReadSlice, Intrin);
    }
    return;
  }
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
  const auto &DL = TheModule.getDataLayout();
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

    Builder.CreateStore(Val, Ptr);
  } else if (Ty->isTVMCellTy()) {
    ReadSlice = CheckOverflow(ReadSlice, true);
    auto *ValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldref),
                                           {ReadSlice});
    auto *Val = Builder.CreateExtractValue(ValAndSlice, 0);
    ReadSlice = Builder.CreateExtractValue(ValAndSlice, 1);
    Builder.CreateStore(Val, Ptr);
  } else if (Sz <= 256) {
    ReadSlice = CheckOverflow(ReadSlice, false);
    auto *ValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldu),
                                           {ReadSlice, ElemSz});
    auto *Val = Builder.CreateExtractValue(ValAndSlice, 0);
    ReadSlice = Builder.CreateExtractValue(ValAndSlice, 1);
    Val = Builder.CreateTruncOrBitCast(Val, Ty);
    Builder.CreateStore(Val, Ptr);
  } else {
    ReadSlice = CheckOverflow(ReadSlice, false);
    auto *ValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldi),
                                           {ReadSlice, ElemSz});
    auto *Val = Builder.CreateExtractValue(ValAndSlice, 0);
    ReadSlice = Builder.CreateExtractValue(ValAndSlice, 1);
    Builder.CreateStore(Val, Ptr);
  }
}

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
