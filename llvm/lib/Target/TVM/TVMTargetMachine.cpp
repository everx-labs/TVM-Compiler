//===-- TVMTargetMachine.cpp - Define TargetMachine for TVM ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the TVM specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#include "TVMTargetMachine.h"
#include "TVM.h"

#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"

namespace llvm {

extern "C" void LLVMInitializeTVMTarget() {
  RegisterTargetMachine<TVMTargetMachine> X(getTheTVMTarget());
}

static Reloc::Model getEffectiveRelocModel(Optional<Reloc::Model> RM) {
  if (!RM.hasValue())
    return Reloc::Static;
  return *RM;
}

static CodeModel::Model getEffectiveCodeModel(Optional<CodeModel::Model> CM) {
  if (CM)
    return *CM;
  return CodeModel::Small;
}

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options) {
  return "E-S1024-i256:256:256";
}

TVMTargetMachine::TVMTargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   Optional<Reloc::Model> RM,
                                   Optional<CodeModel::Model> CM,
                                   CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options), TT, CPU, FS,
                        Options, getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CM), OL),
      TLOF(make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, CPU, FS, *this) {
  initAsmInfo();
}

namespace {
/// TVM Code Generator Pass Configuration Options.
class TVMPassConfig : public TargetPassConfig {
public:
  TVMPassConfig(TVMTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  TVMTargetMachine &getTVMTargetMachine() const {
    return getTM<TVMTargetMachine>();
  }

  bool addInstSelector() override;
};
} // namespace

TargetPassConfig *TVMTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new TVMPassConfig(*this, PM);
}

bool TVMPassConfig::addInstSelector() {
  // Install an instruction selector.
  addPass(createTVMISelDag(getTVMTargetMachine(), getOptLevel()));
  return false;
}

} // end of namespace llvm
