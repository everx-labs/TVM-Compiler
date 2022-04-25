//===--------- TVMArgumentMove.cpp - Argument instruction moving ----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file moves ARGUMENT instructions after ScheduleDAG scheduling.
///
/// Arguments are really live-in registers, however, since we use virtual
/// registers and LLVM doesn't support live-in virtual registers, we're
/// currently making do with ARGUMENT instructions which are placed at the top
/// of the entry block. The trick is to get them to *stay* at the top of the
/// entry block.
///
/// The ARGUMENTS physical register keeps these instructions pinned in place
/// during liveness-aware CodeGen passes, however one thing which does not
/// respect this is the ScheduleDAG scheduler. This pass is therefore run
/// immediately after that.
///
/// This is all hopefully a temporary solution until we find a better solution
/// for describing the live-in nature of arguments.
///
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/TVMMCTargetDesc.h"
#include "TVM.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMSubtarget.h"
#include "TVMUtilities.h"
#include "llvm/CodeGen/MachineBlockFrequencyInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "tvm-argument-move"

namespace {
class TVMArgumentMove final : public MachineFunctionPass {
public:
  static char ID; // Pass identification, replacement for typeid
  TVMArgumentMove() : MachineFunctionPass(ID) {}

  StringRef getPassName() const override { return "TVM Argument Move"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    AU.addPreserved<MachineBlockFrequencyInfo>();
    AU.addPreservedID(MachineDominatorsID);
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;
};
} // end anonymous namespace

char TVMArgumentMove::ID = 0;
INITIALIZE_PASS(TVMArgumentMove, DEBUG_TYPE,
                "Move ARGUMENT instructions for TVM", false, false)

FunctionPass *llvm::createTVMArgumentMove() { return new TVMArgumentMove(); }

bool TVMArgumentMove::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG({
    dbgs() << "********** Argument Move **********\n"
           << "********** Function: " << MF.getName() << '\n';
  });

  bool Changed = false;
  MachineBasicBlock &EntryMBB = MF.front();
  MachineBasicBlock::iterator InsertPt = EntryMBB.end();

  // Look for the first NonArg instruction.
  for (MachineInstr &MI : EntryMBB) {
    if (!TVM::isArgument(MI)) {
      InsertPt = MI;
      break;
    }
  }

  // Now move any argument instructions later in the block
  // to before our first NonArg instruction.
  for (MachineInstr &MI : llvm::make_range(InsertPt, EntryMBB.end())) {
    if (TVM::isArgument(MI)) {
      EntryMBB.insert(InsertPt, MI.removeFromParent());
      Changed = true;
    }
  }

  return Changed;
}
