//===-- TVMTargetMachine.h - Define TargetMachine for AVR -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the TVM specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMTARGETMACHINE_H
#define LLVM_LIB_TARGET_TVM_TVMTARGETMACHINE_H

#include "TVMSubtarget.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class TVMTargetMachine : public LLVMTargetMachine {
public:
  static inline constexpr size_t SmallTupleLimit = 15;

  TVMTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                   StringRef FS, const TargetOptions &Options,
                   Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                   CodeGenOpt::Level OL, bool JIT);
  virtual ~TVMTargetMachine() override = default;
  const TVMSubtarget *getSubtargetImpl(const Function &F) const override {
    return &Subtarget;
  }
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
  void adjustPassManager(PassManagerBuilder &) override;

  //bool usesPhysRegsForPEI() const override { return false; }

  bool usesPhysRegsForValues() const override { return false; }

private:
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  TVMSubtarget Subtarget;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMTARGETMACHINE_H
