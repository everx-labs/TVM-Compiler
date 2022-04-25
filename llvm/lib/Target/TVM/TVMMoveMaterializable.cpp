//===--------- TVMMoveMaterializable.cpp - Rewrite Reg-forms with S-forms
//---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Introduce explicit stack manipulation. Rewrite all register forms of
/// instructions with their stack counterparts.
///
//===----------------------------------------------------------------------===//

#include <unordered_map>

#include "MCTargetDesc/TVMMCTargetDesc.h"
#include "TVM.h"
#include "TVMExtras.h"
#include "TVMStack.h"
#include "TVMStackBlockInfo.h"
#include "TVMSubtarget.h"
#include "TVMUtilities.h"

#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/CodeGen/LiveIntervals.h"
#include "llvm/CodeGen/MachineBlockFrequencyInfo.h"
#include "llvm/CodeGen/MachineDominators.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineLoopInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "tvm-move-materializable"

static cl::opt<bool>
    DisableTVMMoveMaterializable("disable-tvm-move-materializable", cl::Hidden,
                                 cl::desc("TVM: Move materializable."),
                                 cl::init(false));

namespace {

/// Move materializable VRs s.t. they follow the order of argument consuption,
/// thus StackModel needs less reorderings.
class TVMMoveMaterializable final : public MachineFunctionPass {
public:
  using RegUsage = std::unordered_map<unsigned, unsigned>;
  StringRef getPassName() const override { return "TVM move materializable"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<LiveIntervals>();
    AU.addPreservedID(LiveVariablesID);
    AU.addRequired<MachineLoopInfo>();
    AU.addPreserved<MachineLoopInfo>();
    AU.addRequired<AAResultsWrapperPass>();
    AU.addRequired<MachineDominatorTree>();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

  static char ID; // Pass identification, replacement for typeid
  TVMMoveMaterializable() : MachineFunctionPass(ID) {}

private:
  bool processInstruction(MachineInstr &MI);
  bool runOnBasicBlocks(MachineFunction &MF);

  MachineRegisterInfo *MRI;
  const TargetInstrInfo *TII;
  LiveIntervals *LIS;
  AliasAnalysis *AA;
  MachineDominatorTree *MDT;
  LLVMContext *C;
};

} // end anonymous namespace

char TVMMoveMaterializable::ID = 0;
INITIALIZE_PASS(TVMMoveMaterializable, DEBUG_TYPE,
                "Move materializable values close to their usage", false, false)

FunctionPass *llvm::createTVMMoveMaterializable() {
  return new TVMMoveMaterializable();
}

// Identify the definition for this register at this point. This is a
// generalization of MachineRegisterInfo::getUniqueVRegDef that uses
// LiveIntervals to handle complex cases.
static MachineInstr *GetVRegDef(unsigned Reg, const MachineInstr *Insert,
                                const MachineRegisterInfo &MRI,
                                const LiveIntervals &LIS) {
  // Most registers are in SSA form here so we try a quick MRI query first.
  if (MachineInstr *Def = MRI.getUniqueVRegDef(Reg))
    return Def;

  // MRI doesn't know what the Def is. Try asking LIS.
  if (const VNInfo *ValNo = LIS.getInterval(Reg).getVNInfoBefore(
          LIS.getInstructionIndex(*Insert)))
    return LIS.getInstructionFromIndex(ValNo->def);

  return nullptr;
}

// Test whether Reg, as defined at Def, has exactly one use. This is a
// generalization of MachineRegisterInfo::hasOneUse that uses LiveIntervals
// to handle complex cases.
static bool HasOneUse(unsigned Reg, MachineInstr *Def, MachineRegisterInfo &MRI,
                      MachineDominatorTree &MDT, LiveIntervals &LIS) {
  // Most registers are in SSA form here so we try a quick MRI query first.
  if (MRI.hasOneUse(Reg))
    return true;

  bool HasOne = false;
  const LiveInterval &LI = LIS.getInterval(Reg);
  const VNInfo *DefVNI =
      LI.getVNInfoAt(LIS.getInstructionIndex(*Def).getRegSlot());
  assert(DefVNI);
  for (auto &I : MRI.use_nodbg_operands(Reg)) {
    const auto &Result = LI.Query(LIS.getInstructionIndex(*I.getParent()));
    if (Result.valueIn() == DefVNI) {
      if (!Result.isKill())
        return false;
      if (HasOne)
        return false;
      HasOne = true;
    }
  }
  return HasOne;
}

bool TVMMoveMaterializable::processInstruction(MachineInstr &MI) {
  bool Changed = false;
  auto Uses = reverse(MI.uses());
  MachineInstr *InsertBefore = &MI;
  MachineBasicBlock *MBB = MI.getParent();
  for (MachineOperand &Use : Uses) {
    if (Use.isReg()) {
      unsigned Reg = Use.getReg();
      MachineInstr *Def = GetVRegDef(Reg, &MI, *MRI, *LIS);
      if (Def) {
        if (HasOneUse(Reg, Def, *MRI, *MDT, *LIS) && Def->getParent() == MBB &&
            Def->getDesc().isRematerializable() && !TVM::isArgument(*Def) &&
            !TVM::isArgument(*InsertBefore)) {
          MBB->splice(InsertBefore, MBB, Def);
          InsertBefore = Def;
        }
        Changed = true;
      } else if (Use.isUndef()) {
        const ConstantInt *Value;
        if (TVM::isConstInt(*InsertBefore))
          Value = InsertBefore->getOperand(1).getCImm();
        else
          Value = cimm(*C, 0);
        unsigned RegConst = MRI->createVirtualRegister(&TVM::I257RegClass);
        InsertBefore = BuildMI(*MBB, *InsertBefore, InsertBefore->getDebugLoc(),
                               TII->get(TVM::CONST_I257), RegConst)
                           .addCImm(Value);
        Use.ChangeToRegister(RegConst, false /*IsDef*/, false /*IsImp*/,
                             true /*IsKill*/);
        Changed = true;
      }
    }
  }
  return Changed;
}

bool TVMMoveMaterializable::runOnBasicBlocks(MachineFunction &MF) {
  bool Changed = false;

  using RPOTType = ReversePostOrderTraversal<MachineFunction *>;
  RPOTType RPOT(&MF);

  for (RPOTType::rpo_iterator I = RPOT.begin(), E = RPOT.end(); I != E; ++I) {
    auto MBB = *I;

    auto Instr = MBB->begin();
    while (Instr != MBB->end()) {
      if (TVM::isArgument(*Instr) || TVM::isArgumentNum(*Instr)) {
        ++Instr;
        continue;
      }

      Changed |= processInstruction(*Instr);

      MachineInstr &MI = *Instr++;
      assert(!TVM::isArgument(MI));

      if (MI.isDebugInstr() || MI.isLabel())
        continue;
    }
  }
  return Changed;
}

bool TVMMoveMaterializable::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(
      dbgs()
      << "********** Moving materializable close to their usage **********\n"
         "********** Function: "
      << MF.getName() << '\n');

  LIS = &getAnalysis<LiveIntervals>();
  MRI = &MF.getRegInfo();
  AA = &getAnalysis<AAResultsWrapperPass>().getAAResults();
  MDT = &getAnalysis<MachineDominatorTree>();
  C = &MF.getFunction().getContext();
  TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();

  bool Changed = runOnBasicBlocks(MF);

  return Changed;
}
