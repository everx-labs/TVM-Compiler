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

#include "llvm/InitializePasses.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

namespace llvm {

extern "C" void LLVMInitializeTVMTarget() {
  RegisterTargetMachine<TVMTargetMachine> X(getTheTVMTarget());
  auto &PR = *PassRegistry::getPassRegistry();
  initializeLowerSwitchLegacyPassPass(PR);
  initializeTVMArgumentMovePass(PR);
  initializeTVMControlFlowPreparePass(PR);
  initializeTVMReplacePhysRegsPass(PR);
  initializeTVMPrepareForLiveIntervalsPass(PR);
  initializeTVMRematerializePass(PR);
  initializeTVMRegStackifyPass(PR);
  initializeTVMRegNumberingPass(PR);
  initializeTVMPeepholePass(PR);
  initializeTVMStackModelPass(PR);
  initializeTVMLoopInstructionsPass(PR);
  initializeTVMLoopPreparePass(PR);
  initializeTVMContinuationsHoistPass(PR);
  initializeTVMLoadStoreReplacePass(PR);
  initializeTVMMoveMaterializablePass(PR);
  initializeTVMStoreCombinePass(PR);
  initializeTVMLowerIntrinsicsPass(PR);
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
  return "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-"
         "i64:257:257-i257:257:257-p:257:257-a:257:257";
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
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, std::string(CPU), std::string(CPU), *this) {
  setRequiresStructuredCFG(true);
  initAsmInfo();

  // TVM doesn't support .bss sections, everything is either .text or .data.
  // Adjust TargetLoweringObjectFile::getKindForGlobal() behavior by overriding
  // the corresponding target option.
  this->Options.NoZerosInBSS = true;
}

void TVMTargetMachine::adjustPassManager(PassManagerBuilder &Builder) {
  Builder.addExtension(
    PassManagerBuilder::EP_CGSCCOptimizerLate,
    [](const PassManagerBuilder &, legacy::PassManagerBase &PM) {
      //PM.add(createTVMReFuncPass());
      PM.add(createArgumentPromotionPass(0));
  });
  Builder.addExtension(
    PassManagerBuilder::EP_ModuleOptimizerEarly,
    [](const PassManagerBuilder &, legacy::PassManagerBase &PM) {
      PM.add(createTVMLowerIntrinsicsPass());
  });
}

namespace {
/// TVM Code Generator Pass Configuration Options.
class TVMPassConfig : public TargetPassConfig {
public:
  TVMPassConfig(TVMTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {
    EnableTailMerge = false;
  }

  TVMTargetMachine &getTVMTargetMachine() const {
    return getTM<TVMTargetMachine>();
  }

  FunctionPass *createTargetRegisterAllocator(bool) override;

  void addIRPasses() override;
  bool addInstSelector() override;
  bool addILPOpts() override;
  void addPreEmitPass() override;
  void addPreRegAlloc() override;
  void addPostRegAlloc() override;
};
} // namespace

FunctionPass *TVMPassConfig::createTargetRegisterAllocator(bool) {
  return nullptr; // No reg alloc
}

TargetPassConfig *TVMTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new TVMPassConfig(*this, PM);
}

void TVMPassConfig::addIRPasses() {
  addPass(createTVMLowerIntrinsicsPass());
  // TODO: once setcc is supported, we need to remove it.
  addPass(createLowerSwitchPass());
  addPass(createTVMLoopPrepare());
  addPass(createTVMControlFlowPrepare());
  addPass(createTVMDefineUndef());
  addPass(createTVMReFuncPass());
  addPass(createTVMStoreCombine());
}

bool TVMPassConfig::addInstSelector() {
  (void)TargetPassConfig::addInstSelector();
  // Install an instruction selector.
  addPass(createTVMISelDag(getTVMTargetMachine(), getOptLevel()));
  // Run the argument-move pass immediately after the ScheduleDAG scheduler
  // so that we can fix up the ARGUMENT instructions before anything else
  // sees them in the wrong place.
  addPass(createTVMArgumentMove());
  return false;
}

bool TVMPassConfig::addILPOpts() {
  addPass(createTVMIfConversionTerm());
  return true;
}

void TVMPassConfig::addPreEmitPass() {
  TargetPassConfig::addPreEmitPass();

  addPass(createTVMContinuationsHoist());

  // Now that we have a prologue and epilogue and all frame indices are
  // rewritten, eliminate SP and FP. This allows them to be stackified,
  // colored, and numbered with the rest of the registers.
  addPass(createTVMReplacePhysRegs());
  addPass(createTVMPrepareForLiveIntervals());
  addPass(createTVMRematerialize());
  addPass(createTVMRegStackify());
  addPass(createTVMLoopInstructions());
  addPass(createTVMMoveMaterializable());
  addPass(createTVMStackModel());

  // Perform the very last peephole optimizations on the code.
  if (getOptLevel() != CodeGenOpt::None)
    addPass(createTVMPeephole());

  // Create a mapping from LLVM CodeGen virtual registers to tvm registers.
  addPass(createTVMRegNumbering());
}

void TVMPassConfig::addPreRegAlloc() {
  addPass(createTVMContinuationsHoist());
  TargetPassConfig::addPreRegAlloc();
}

void TVMPassConfig::addPostRegAlloc() {
  // TODO: The following CodeGen passes don't currently support code containing
  // virtual registers. Consider removing their restrictions and re-enabling
  // them.

  // These functions all require the NoVRegs property.
  disablePass(&MachineCopyPropagationID);
  disablePass(&PostRAMachineSinkingID);
  disablePass(&PostRASchedulerID);
  disablePass(&FuncletLayoutID);
  disablePass(&StackMapLivenessID);
  disablePass(&LiveDebugValuesID);
  disablePass(&PatchableFunctionID);
  disablePass(&ShrinkWrapID);

  TargetPassConfig::addPostRegAlloc();
}

} // end of namespace llvm
