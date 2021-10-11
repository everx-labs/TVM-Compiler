//===-- TVMRegStackify.cpp - Register Stackification ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements a Code Statistics pass.
///
/// This pass print to debug stream some useful statistics about code,
/// mainly code sizes of module, functions and machine blocks.
/// This information may be used by optimization passes to adjust
/// optimization algorithms and parameters.
///
///
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "TVMSubtarget.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include <vector>
#include <map>

using namespace llvm;

#define DEBUG_TYPE "tvm-code-statistics"

namespace {
class TVMCodeStatistics final : public ModulePass {
  StringRef getPassName() const override { return "TVM Code Statistics"; }

  bool runOnModule(Module &M) override;

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<MachineModuleInfo>();
    AU.setPreservesAll();
    ModulePass::getAnalysisUsage(AU);
  }

public:
  static char ID; // Pass identification, replacement for typeid
  TVMCodeStatistics() : ModulePass(ID) {}
};
} // end anonymous namespace

char TVMCodeStatistics::ID = 0;
INITIALIZE_PASS(TVMCodeStatistics, DEBUG_TYPE,
                "Print Code statistics to debug stream", false, false)

ModulePass *llvm::createTVMCodeStatistics() {
  return new TVMCodeStatistics();
}

bool TVMCodeStatistics::runOnModule(Module &M) {
  bool Changed = false;

	unsigned moduleCodeSize = 0;
	std::vector<unsigned> functionCodeSize;
	std::vector<unsigned> machineBlockSize;


	MachineModuleInfo &MMI = getAnalysis<MachineModuleInfo>();
	for (Function &F : M) {
    MachineFunction *MF = MMI.getMachineFunction(F);
    if (!MF)
			continue;
    unsigned functionSize = 0;

		const TargetInstrInfo *TII = MF->getSubtarget<TVMSubtarget>().getInstrInfo();

    for (MachineBasicBlock &MBB : *MF) {
      unsigned blockSize = 0;
			for (MachineInstr &MI : MBB) {
        unsigned instrSize = TII->getInstSizeInBytes(MI);
        MI.dump();
        dbgs() << "size = " << instrSize << "\n";
				blockSize += instrSize;
			}
			functionSize += blockSize;
      machineBlockSize.push_back(blockSize);
		}
		moduleCodeSize += functionSize;
		functionCodeSize.push_back(functionSize);
  }

	LLVM_DEBUG(dbgs() << "********** Code Statistics **********\n" 
                    << "********** Module: "
                    << M.getName()  
		                << " size = " << moduleCodeSize << " bytes" << '\n');
	
	for (Function &F : M) {
    MachineFunction *MF = MMI.getMachineFunction(F);
    if (!MF)
      continue;
    unsigned functionIndex = 0;
		unsigned blockIndex = 0;

     LLVM_DEBUG(dbgs() << "  Machine Function: " << MF->getName() 
			     << " size = " << functionCodeSize[functionIndex++] << " bytes"  
			     << "\n");
    for (MachineBasicBlock &MBB : *MF) {
       LLVM_DEBUG(dbgs() << "    Machine BasicBlock: " << MBB.getName()
             << " size = " << machineBlockSize[blockIndex++] << " bytes"
             << "\n");
    }
  }
  return Changed;
}
