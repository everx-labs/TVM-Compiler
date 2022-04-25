//========--- TVMIfConversionTerm.cpp - If-convert TVM terminators ---========//
//  to produce IFRET/IFNOTRET/THROWIF/THROWIFNOT
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Implements a pass that converts branch to return/throw into conditional
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "llvm/InitializePasses.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/CodeGen/MachineDominators.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineLoopInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
using namespace llvm;

#define DEBUG_TYPE "tvm-if-conversion-term"

// TODO: Support also conditional return
namespace {
// Converts conditional branch to a block with single THROW instruction
//  into THROWIF instruction with condition provided as THROWIF argument.
// Pseudo:
// if (Cond) THROW(Err); => THROWIF(Cond, Err);
class TVMIfConversionTerm final : public MachineFunctionPass {
  const TargetInstrInfo *TII;
  MachineDominatorTree *DomTree;
  MachineLoopInfo *Loops;
public:
  StringRef getPassName() const override {
    return "If-convert TVM terminators";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<MachineDominatorTree>();
    AU.addPreserved<MachineDominatorTree>();
    AU.addRequired<MachineLoopInfo>();
    AU.addPreserved<MachineLoopInfo>();
    MachineFunctionPass::getAnalysisUsage(AU);
  }
  bool runOnMachineFunction(MachineFunction &MF) override;
public:
  static char ID;
  explicit TVMIfConversionTerm() : MachineFunctionPass(ID) {}
private:
  bool tryConvertIf(MachineBasicBlock*);
  void updateDomTree(MachineBasicBlock *Head,
                     MachineBasicBlock *ThrowBB,
                     MachineBasicBlock *ContBB);
  void updateLoops(MachineBasicBlock *ThrowBB,
                   MachineBasicBlock *ContBB);
};
} // end anonymous namespace

char TVMIfConversionTerm::ID = 0;

INITIALIZE_PASS_BEGIN(TVMIfConversionTerm, DEBUG_TYPE,
                      "If-convert TVM terminators", false, false)
INITIALIZE_PASS_DEPENDENCY(MachineDominatorTree)
INITIALIZE_PASS_END(TVMIfConversionTerm, DEBUG_TYPE,
                    "If-convert TVM terminators", false, false)

/// Update the dominator tree after if-conversion erased some blocks.
void TVMIfConversionTerm::updateDomTree(MachineBasicBlock *Head,
                                        MachineBasicBlock *ThrowBB,
                                        MachineBasicBlock *ContBB) {
  // tryConvertIf will remove ThrowBB, ContBB and merge content into Head.
  // ThrowBB should not dominate any blocks.
  // ContBB children should be transferred to Head.

  MachineDomTreeNode *HeadNode = DomTree->getNode(Head);
  MachineDomTreeNode *ContNode = DomTree->getNode(ContBB);
  while (ContNode->getNumChildren()) {
    DomTree->changeImmediateDominator(ContNode->back(), HeadNode);
  }
  DomTree->eraseNode(ContBB);
  [[maybe_unused]] MachineDomTreeNode *ThrowNode = DomTree->getNode(ThrowBB);
  assert((!ThrowNode->getNumChildren()) && "Unexpected dom children");
  DomTree->eraseNode(ThrowBB);
}

/// Update LoopInfo after if-conversion.
void TVMIfConversionTerm::updateLoops(MachineBasicBlock *ThrowBB,
                                      MachineBasicBlock *ContBB) {
  if (!Loops)
    return;
  // If-conversion doesn't change loop structure, and it doesn't mess with back
  // edges, so updating LoopInfo is simply removing the dead blocks.
  Loops->removeBlock(ThrowBB);
  Loops->removeBlock(ContBB);
}

bool TVMIfConversionTerm::tryConvertIf(MachineBasicBlock* MBB) {
  SmallVector<MachineBasicBlock*, 4> RemovedBlocks;

  MachineBasicBlock *Head = MBB;
  MachineBasicBlock *TBB = nullptr, *FBB = nullptr;
  if (Head->succ_size() != 2)
    return false;
  MachineBasicBlock *Succ0 = Head->succ_begin()[0];
  MachineBasicBlock *Succ1 = Head->succ_begin()[1];
  if (Succ0->pred_size() != 1 || Succ1->pred_size() != 1)
    return false;
  // Canonicalize so Succ0 is throw-finalized.
  if (!Succ0->succ_empty() || Succ0->getFirstTerminator() == Succ0->end() ||
      Succ0->getFirstTerminator()->getOpcode() != TVM::THROW)
    std::swap(Succ0, Succ1);
  if (!Succ0->succ_empty())
    return false;
  if (Succ1->phis().begin() != Succ1->phis().end())
    return false;
  if (Succ0->size() != 1)
    return false;
  auto &MI = Succ0->front();
  if (MI.getOpcode() != TVM::THROW)
    return false;
  SmallVector<MachineOperand, 4> Cond;
  if (TII->analyzeBranch(*Head, TBB, FBB, Cond)) {
    LLVM_DEBUG(dbgs() << "Branch not analyzable.\n");
    return false;
  }
  if (TBB != Succ0)
    Cond[0].setImm(!Cond[0].getImm());
  if (!TII->PredicateInstruction(MI, Cond))
    return false;
  Head->removeSuccessor(TBB);
  Head->removeSuccessor(FBB, true);

  TII->removeBranch(*Head);
  assert(Head->succ_empty() && "Additional head successors?");
  Head->splice(Head->end(), Succ0, Succ0->begin(), Succ0->end());
  Head->splice(Head->end(), Succ1, Succ1->begin(), Succ1->end());
  Head->transferSuccessorsAndUpdatePHIs(Succ1);
  RemovedBlocks.push_back(Succ0);
  Succ0->eraseFromParent();
  RemovedBlocks.push_back(Succ1);
  Succ1->eraseFromParent();

  updateDomTree(Head, Succ0, Succ1);
  updateLoops(Succ0, Succ1);

  return true;
}

bool TVMIfConversionTerm::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(dbgs() << "********** TVM IF-CONVERSION TERM **********\n"
                    << "********** Function: " << MF.getName() << '\n');
  if (skipFunction(MF.getFunction()))
    return false;
  TII = MF.getSubtarget().getInstrInfo();
  DomTree = &getAnalysis<MachineDominatorTree>();
  Loops = getAnalysisIfAvailable<MachineLoopInfo>();
  bool Changed = false;
  for (auto DomNode : post_order(DomTree))
    if (tryConvertIf(DomNode->getBlock()))
      Changed = true;
  return Changed;
}

FunctionPass *llvm::createTVMIfConversionTerm() {
  return new TVMIfConversionTerm();
}
