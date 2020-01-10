//===--- IRPrintingPasses.cpp - Module and Function printing passes -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// PrintModulePass and PrintFunctionPass implementations.
//
//===----------------------------------------------------------------------===//

#include "llvm/IR/IRPrintingPasses.h"
// TVM local begin
#include "llvm/IR/Constants.h"
// TVM local end
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

PrintModulePass::PrintModulePass() : OS(dbgs()) {}
PrintModulePass::PrintModulePass(raw_ostream &OS, const std::string &Banner,
                                 bool ShouldPreserveUseListOrder)
    : OS(OS), Banner(Banner),
      ShouldPreserveUseListOrder(ShouldPreserveUseListOrder) {}

PreservedAnalyses PrintModulePass::run(Module &M, ModuleAnalysisManager &) {
  OS << Banner;
  if (llvm::isFunctionInPrintList("*"))
    M.print(OS, nullptr, ShouldPreserveUseListOrder);
  else {
    for(const auto &F : M.functions())
      if (llvm::isFunctionInPrintList(F.getName()))
        F.print(OS);
  }
  return PreservedAnalyses::all();
}

PrintFunctionPass::PrintFunctionPass() : OS(dbgs()) {}
PrintFunctionPass::PrintFunctionPass(raw_ostream &OS, const std::string &Banner)
    : OS(OS), Banner(Banner) {}

PreservedAnalyses PrintFunctionPass::run(Function &F,
                                         FunctionAnalysisManager &) {
  if (isFunctionInPrintList(F.getName())) {
    if (forcePrintModuleIR())
      OS << Banner << " (function: " << F.getName() << ")\n" << *F.getParent();
    else
      OS << Banner << static_cast<Value &>(F);
  }
  return PreservedAnalyses::all();
}

namespace {

class PrintModulePassWrapper : public ModulePass {
  PrintModulePass P;

public:
  static char ID;
  PrintModulePassWrapper() : ModulePass(ID) {}
  PrintModulePassWrapper(raw_ostream &OS, const std::string &Banner,
                         bool ShouldPreserveUseListOrder)
      : ModulePass(ID), P(OS, Banner, ShouldPreserveUseListOrder) {}

  bool runOnModule(Module &M) override {
    ModuleAnalysisManager DummyMAM;
    P.run(M, DummyMAM);
    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  StringRef getPassName() const override { return "Print Module IR"; }
};

class PrintFunctionPassWrapper : public FunctionPass {
  PrintFunctionPass P;

public:
  static char ID;
  PrintFunctionPassWrapper() : FunctionPass(ID) {}
  PrintFunctionPassWrapper(raw_ostream &OS, const std::string &Banner)
      : FunctionPass(ID), P(OS, Banner) {}

  // This pass just prints a banner followed by the function as it's processed.
  bool runOnFunction(Function &F) override {
    FunctionAnalysisManager DummyFAM;
    P.run(F, DummyFAM);
    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  StringRef getPassName() const override { return "Print Function IR"; }
};

class PrintBasicBlockPass : public BasicBlockPass {
  raw_ostream &Out;
  std::string Banner;

public:
  static char ID;
  PrintBasicBlockPass() : BasicBlockPass(ID), Out(dbgs()) {}
  PrintBasicBlockPass(raw_ostream &Out, const std::string &Banner)
      : BasicBlockPass(ID), Out(Out), Banner(Banner) {}

  bool runOnBasicBlock(BasicBlock &BB) override {
    Out << Banner << BB;
    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  StringRef getPassName() const override { return "Print BasicBlock IR"; }
};

// TVM local begin
// Pass to print text global constant (specified by name).
// Used by json-abi exporter with "-emit-text-const=json_abi"
//  to print "json_abi" global text constant with prepared abi description.
class PrintTextConstantPass : public ModulePass {
  raw_ostream &Out;
  std::string ConstantName;

public:
  static char ID;
  PrintTextConstantPass() : ModulePass(ID), Out(dbgs()) {}
  PrintTextConstantPass(raw_ostream &Out, const std::string &ConstantName)
      : ModulePass(ID), Out(Out), ConstantName(ConstantName) {}

  bool runOnModule(Module &M) override {
    const auto *GV = M.getNamedGlobal(ConstantName);
    if (!GV) return false;
    const auto *Init = GV->getInitializer();
    if (!Init) return false;
    const auto *GV2 = dyn_cast<GlobalVariable>(Init->stripPointerCasts());
    if (!GV2) return false;
    const auto *Init2 = GV2->getInitializer();
    if (!Init2) return false;
    const auto *Arr = dyn_cast<ConstantArray>(Init2);
    if (!Arr) return false;
    for (unsigned i = 0, e = Arr->getNumOperands(); i < e; ++i) {
      auto *cInt = dyn_cast<ConstantInt>(Arr->getOperand(i));
      if (!cInt) return false;
      auto Val = cInt->getValue().getZExtValue();
      assert(Val < 256 && "Too big");
      auto Ch = static_cast<unsigned char>(Val);
      if (!Ch)
        break;
      Out << Ch;
    }
    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  StringRef getPassName() const override { return "Print text constant"; }
};

// TVM local end
}

char PrintModulePassWrapper::ID = 0;
INITIALIZE_PASS(PrintModulePassWrapper, "print-module",
                "Print module to stderr", false, true)
char PrintFunctionPassWrapper::ID = 0;
INITIALIZE_PASS(PrintFunctionPassWrapper, "print-function",
                "Print function to stderr", false, true)
char PrintBasicBlockPass::ID = 0;
INITIALIZE_PASS(PrintBasicBlockPass, "print-bb", "Print BB to stderr", false,
                true)

char PrintTextConstantPass::ID = 0;
INITIALIZE_PASS(PrintTextConstantPass, "print-constant", "Print text constant", false,
                true)

ModulePass *llvm::createPrintModulePass(llvm::raw_ostream &OS,
                                        const std::string &Banner,
                                        bool ShouldPreserveUseListOrder) {
  return new PrintModulePassWrapper(OS, Banner, ShouldPreserveUseListOrder);
}

FunctionPass *llvm::createPrintFunctionPass(llvm::raw_ostream &OS,
                                            const std::string &Banner) {
  return new PrintFunctionPassWrapper(OS, Banner);
}

BasicBlockPass *llvm::createPrintBasicBlockPass(llvm::raw_ostream &OS,
                                                const std::string &Banner) {
  return new PrintBasicBlockPass(OS, Banner);
}

// TVM local begin
ModulePass *llvm::createPrintTextConstantPass(llvm::raw_ostream &OS,
                                              const std::string &ConstantName) {
  return new PrintTextConstantPass(OS, ConstantName);
}
// TVM local end

bool llvm::isIRPrintingPass(Pass *P) {
  const char *PID = (const char*)P->getPassID();

  return (PID == &PrintModulePassWrapper::ID)
      || (PID == &PrintFunctionPassWrapper::ID)
      || (PID == &PrintBasicBlockPass::ID)
      // TVM local begin
      || (PID == &PrintTextConstantPass::ID);
      // TVM local end
}
