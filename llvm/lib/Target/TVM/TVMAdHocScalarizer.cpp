#include "TVM.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
using namespace llvm;

#define DEBUG_TYPE "tvm-ad-hoc-scalarizer"

namespace {
class TVMAdHocScalarizer : public ModulePass {
  struct Candidate {
    Function *F;
    std::vector<CallInst *> CallSites;
  };

public:
  static char ID;
  TVMAdHocScalarizer() : ModulePass(ID) {}
  bool runOnModule(Module &M) override;

private:
  Function *transformMethod(Function *F);
  void transformCallSite(CallInst *CI, Function *NewF);

  Value *pack(Value *Mem, IRBuilder<> &Builder);
  Value *packImpl(Value *V, Type *T, Value *Mem,
                  std::vector<unsigned> &Indices, IRBuilder<> &Builder);

  void unpack(Value *V, Value *Mem, IRBuilder<> &Builder);
  void unpackImpl(Value *V, Type *T, Value *Mem,
                  std::vector<unsigned> &Indices, IRBuilder<> &Builder);
};
} // end anonymous namespace

char TVMAdHocScalarizer::ID = 0;
INITIALIZE_PASS(TVMAdHocScalarizer, "tvm-ad-hoc-scalarizer",
                "TVM ad-hoc scalarizer", false, false)

ModulePass *llvm::createTVMAdHocScalarizer() {
  return new TVMAdHocScalarizer();
}

Function *TVMAdHocScalarizer::transformMethod(Function *F) {
  FunctionType *OldFuncTy = F->getFunctionType();

  SmallVector<Type *, 16> ArgTys;
  ArgTys.append(OldFuncTy->param_begin(), OldFuncTy->param_end());

  StructType *ThisTy = cast<StructType>(cast<PointerType>(
      OldFuncTy->getParamType(0))->getElementType());

  Type *OldRetTy = OldFuncTy->getReturnType();
  StructType *RetTy = OldRetTy->isVoidTy() ? ThisTy :
      StructType::get(ThisTy, OldRetTy);

  FunctionType *FuncTy = FunctionType::get(RetTy, ArgTys,
      OldFuncTy->isVarArg());

  Function *NewF = Function::Create(FuncTy, F->getLinkage(),
      F->getName().str() + "_wrapper", F->getParent());
  NewF->copyAttributesFrom(F);
  F->removeFnAttr(Attribute::NoInline);
  F->addFnAttr(Attribute::AlwaysInline);

  BasicBlock::Create(NewF->getContext(), "entry", NewF);
  IRBuilder<> Builder(&NewF->getEntryBlock(), NewF->getEntryBlock().begin());
  AllocaInst *AI = Builder.CreateAlloca(ThisTy, 0, "this");

  Value *Dst = Builder.CreateBitCast(AI, Builder.getIntBytePtrTy());
  Value *Src = Builder.CreateBitCast(&*NewF->arg_begin(),
      Builder.getIntBytePtrTy());

  unsigned Size = F->getParent()->getDataLayout().getTypeStoreSize(ThisTy);
  Builder.CreateMemCpy(Dst, 1, Src, 1, Size, false);

  std::vector<Value *> Args;
  unsigned n = OldFuncTy->getNumParams();
  for (Function::arg_iterator ai = NewF->arg_begin(); n != 0; ++ai, --n)
    Args.push_back(&*ai);
  Args[0] = AI;
  CallInst *CI = Builder.CreateCall(F, Args, "");
  CI->setCallingConv(F->getCallingConv());

  Value *NewThis = pack(AI, Builder);

  if (OldRetTy->isVoidTy()) {
    Builder.CreateRet(NewThis);
  } else {
    Value *R1 = Builder.CreateInsertValue(UndefValue::get(RetTy), NewThis, 0);
    Value *R2 = Builder.CreateInsertValue(R1, CI, 1);
    Builder.CreateRet(R2);
  }

  return NewF;
}

void TVMAdHocScalarizer::transformCallSite(CallInst *CI, Function *NewF) {
  Function *OldF = CI->getCalledFunction();

  IRBuilder<> Builder(CI);
  std::vector<Value *> Args;
  for (unsigned I = 0; I < CI->getNumArgOperands(); ++I)
    Args.push_back(CI->getArgOperand(I));
  CallInst *NewCI = Builder.CreateCall(NewF, Args, "");
  NewCI->setCallingConv(OldF->getCallingConv());

  Value *This;
  if (OldF->getReturnType()->isVoidTy()) {
    This = NewCI;
  } else {
    This = Builder.CreateExtractValue(NewCI, 0);
    Value *Ret = Builder.CreateExtractValue(NewCI, 1);
    CI->replaceAllUsesWith(Ret);
  }

  Value *ThisPtr = Args[0];
  unpack(This, ThisPtr, Builder);

  CI->eraseFromParent();
}

Value *TVMAdHocScalarizer::pack(Value *Mem, IRBuilder<> &Builder) {
  StructType *T = cast<StructType>(cast<PointerType>(Mem->getType())
      ->getElementType());
  std::vector<unsigned> Empty;
  return packImpl(UndefValue::get(T), T, Mem, Empty, Builder);
}

Value *TVMAdHocScalarizer::packImpl(Value *V, Type *T, Value *Mem,
                                    std::vector<unsigned> &Indices,
                                    IRBuilder<> &Builder) {
  if (isa<PointerType>(T)) {
    // TODO skip for now
  } else if (isa<IntegerType>(T) || T->isTVMCellTy()) {
    Type *Int32Ty = Builder.getInt32Ty();
    std::vector<Value *> IdxList;
    IdxList.push_back(ConstantInt::get(Int32Ty, 0));
    for (unsigned Idx : Indices)
      IdxList.push_back(ConstantInt::get(Int32Ty, Idx));
    Value *Ptr = Builder.CreateGEP(Mem, IdxList);
    Value *Val = Builder.CreateLoad(Ptr);
    V = Builder.CreateInsertValue(V, Val, Indices);
  } else if (StructType *STy = dyn_cast<StructType>(T)) {
    for (unsigned I = 0; I < STy->getNumElements(); ++I) {
      auto NewIndices = Indices;
      NewIndices.push_back(I);
      V = packImpl(V, STy->getElementType(I), Mem, NewIndices, Builder);
    }
  } else {
    assert(0 && "unsupported");
  }

  return V;
}

void TVMAdHocScalarizer::unpackImpl(Value *V, Type *T, Value *Mem,
                                    std::vector<unsigned> &Indices,
                                    IRBuilder<> &Builder) {
  if (isa<PointerType>(T)) {
    // TODO skip for now
  } else if (isa<IntegerType>(T) || T->isTVMCellTy()) {
    Type *Int32Ty = Builder.getInt32Ty();
    std::vector<Value *> IdxList;

    IdxList.push_back(ConstantInt::get(Int32Ty, 0));
    for (unsigned Idx : Indices)
      IdxList.push_back(ConstantInt::get(Int32Ty, Idx));

    Value *Val = Builder.CreateExtractValue(V, Indices);
    Value *Ptr = Builder.CreateGEP(Mem, IdxList);
    Builder.CreateStore(Val, Ptr);
  } else if (StructType *STy = dyn_cast<StructType>(T)) {
    for (unsigned I = 0; I < STy->getNumElements(); ++I) {
      auto NewIndices = Indices;
      NewIndices.push_back(I);
      unpackImpl(V, STy->getElementType(I), Mem, NewIndices, Builder);
    }
  } else {
    LLVM_DEBUG(T->dump());
    assert(0 && "unsupported");
  }
}

void TVMAdHocScalarizer::unpack(Value *V, Value *Mem, IRBuilder<> &Builder) {
  assert(isa<StructType>(V->getType()) && "unsupported");
  std::vector<unsigned> Empty;
  unpackImpl(V, V->getType(), Mem, Empty, Builder);
}

bool TVMAdHocScalarizer::runOnModule(Module &M) {
  std::vector<Candidate> Candidates;
  for (auto &F : M) {
    if (F.arg_begin() == F.arg_end())
      continue;

    auto &FirstArg = *F.arg_begin();
    auto FirstArgTy = FirstArg.getType();
    if (!isa<PointerType>(FirstArgTy))
      continue;
    auto EltTy = cast<PointerType>(FirstArgTy)->getElementType();
    if (!isa<StructType>(EltTy))
      continue;

    std::vector<CallInst *> CallSites;
    bool BadCS = false;
    for (User *U : F.users()) {
      CallInst *CI = dyn_cast<CallInst>(U);
      if (!CI || CI->getCalledFunction() != &F) {
        BadCS = true;
        break;
      }
      CallSites.push_back(CI);
    }
    if (BadCS)
      continue;

    LLVM_DEBUG(llvm::dbgs() << "TVMAdHocScalarizer " << F.getName() << "\n");
    Candidates.push_back({ &F, CallSites });
  }

  bool Changed = false;
  for (Candidate &C : Candidates) {
    Function *NewF = transformMethod(C.F);
    for (CallInst *CI : C.CallSites)
      transformCallSite(CI, NewF);
    Changed = true;
  }

  return Changed;
}
