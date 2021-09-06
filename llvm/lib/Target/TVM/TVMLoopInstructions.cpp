//===---- TVMLoopInstructions.cpp - TVM loop instruction insertion pass ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Replace IFJMP forming a loop with proper loop instructions.
///
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "TVMInstrInfo.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMSubtarget.h"
#include "TVMUtilities.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineLoopInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

#include <unordered_set>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "tvm-loop"

#define DEBUG_INITIAL_CODE 1
#define DEBUG_REMOVE_EMPTY 1
#define DEBUG_COLLECT_CHAINS 1
#define DEBUG_MERGE 1
#define DEBUG_LOOP_ANALYZE 1
#define DEBUG_LOOP_TRANSFORM 1

namespace {
/// Replace IFJMP forming a loop with proper loop instructions.
/// The loop must be in the following form:
/// -----------------------------
/// |      LoopPredecessor      |
/// | ...                       |
/// | IFJMP LoopCond LoopHeader |-----------------------
/// | JMPX ExitBlock            |                      |
/// -----------------------------                      ∨
///               |                       -----------------------------
///               |                       |         ExitBlock         |
///               ∨                       -----------------------------
/// -----------------------------                      ∧
/// |        LoopHeader         |                      |
/// | IFJMP LoopCond LoopBody   |-----------------------
/// | JMPX ExitBlock            |
/// -----------------------------
///          |         ∧
///          |         |
///          |         ~
///          ∨         |
/// -----------------------------
/// |         LoopBody          |
/// -----------------------------
///
/// The pass rewrites terminators so that the following is generated:
/// ------------------------------
/// |      LoopPredecessor       |
/// | ...                        |
/// | UNTIL LoopCond LoopHeader  |-----------------------
/// | JMPX ExitBlock             |                      |
/// ------------------------------                      ∨
///               |                        -----------------------------
///               |                        |         ExitBlock         |
///               ∨                        -----------------------------
/// ------------------------------                      ∧
/// |        LoopHeader          |                      |
/// | BACKEDGE LoopCond LoopBody |-----------------------
/// | <DELETED TERMINATOR>       |
/// ------------------------------
///          |         ∧
///          |         |
///          |         ~
///          ∨         |
/// ------------------------------
/// |         LoopBody           |
/// ------------------------------
/// Note: the transformation only deletes code following BACKEDGE, but preserves
/// LoopHeader successors.

enum LoopKind { 
	WhileDo,  //  "while { }" or "for (...;...;...) {}" loop
	DoWhile,  // "do {} while(...)" loop
	Irregular 
};

class TVMLoopInstructions final : public MachineFunctionPass {
  StringRef getPassName() const override {
    return "TVM loop instructions inserter";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<MachineLoopInfo>();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

public:
  static char ID;
  TVMLoopInstructions() : MachineFunctionPass(ID) {}

private:
  // Remove empty blocks, i.e,
  //  blocks with 1 input and 1 output and with empty contents
  bool removeEmptyBlocks(MachineFunction &MF);

  // Collect machine block chains which can be collapsed to one machine block
  void collectMachineBlockChains(MachineFunction &MF,
                                 vector<vector<MachineBasicBlock *>> &chains);

  // Merge machine block chan to one machine block.
  // Remove excessive jump operations
  void mergeMachineBlockChain(vector<MachineBasicBlock *> const &chain);

  // Analyze MachineLoop, infer loop kind
  LoopKind analyzeLoop(MachineLoop *loop);

	// Test if Basic Block contains an acceptable condition
	//   like IFELSE on the end of block
	bool testAcceptableLoopCondition(MachineBasicBlock &MBB);

	// Transform "while { }" or "for (...;...;...) {}" loop
	bool transformWhileLoop(MachineLoop *loop);

	// Transform "do {} while(...)" loop
	bool transformDoLoop(MachineLoop *loop);

	const TargetInstrInfo *TII;
	MachineLoopInfo *Loops;
};
} // end anonymous namespace

char TVMLoopInstructions::ID = 0;
INITIALIZE_PASS(TVMLoopInstructions, DEBUG_TYPE,
                "TVM loop instructions inserter", false, false)

FunctionPass *llvm::createTVMLoopInstructions() {
  return new TVMLoopInstructions();
}

static string blockToString(MachineBasicBlock *MBB) {
  string str("bb.");
  char buf[10];
  itoa(MBB->getNumber(), buf, 10);
  str += buf;
  str += ".";
  str += MBB->getName();
  return str;
}

// Convert MachineBasicBlock list to string
static string blockListToString(vector<MachineBasicBlock *> const &list) {
  string str;
  str += "[ ";
  bool first = true;
  for (MachineBasicBlock *MBB : list) {
    if (!first)
      str += ", ";
    str += blockToString(MBB);
    first = false;
  }
  str += " ]";
  return str;
}

// Print edges of MachineFunctio CFG
static void printCFGEdges(MachineFunction &MF) {
  dbgs() << "CFG Edges :\n";
  using RPOTType = ReversePostOrderTraversal<MachineFunction *>;
  RPOTType RPOT(&MF);
  for (RPOTType::rpo_iterator I = RPOT.begin(), E = RPOT.end(); I != E; ++I) {
    MachineBasicBlock *MBB = *I;
    for (MachineBasicBlock *succ : MBB->successors()) {
      dbgs() << blockToString(MBB) << " -> " << blockToString(succ) << "\n";
    }
  }
  dbgs() << "------------------\n";
}

bool TVMLoopInstructions::runOnMachineFunction(MachineFunction &MF) {
  bool changed = false;
  TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();
 
#if DEBUG_INITIAL_CODE
  //  for (MachineBasicBlock &MBB : MF) {
//     MBB.dump();
//  }
  printCFGEdges(MF);
  dbgs() << "\n\n";
#endif

  changed |= removeEmptyBlocks(MF);
#if DEBUG_REMOVE_EMPTY 
  printCFGEdges(MF);
  dbgs() << "\n\n";
#endif

  vector<vector<MachineBasicBlock *>> chains;
  collectMachineBlockChains(MF, chains);
#if DEBUG_MERGE
  dbgs() << "Chains to merge : " << chains.size() << "\n";
#endif

  for (vector<MachineBasicBlock *> &chain : chains) {
    mergeMachineBlockChain(chain);
  }

	changed |= !chains.empty();

#if DEBUG_MERGE
  printCFGEdges(MF);
  dbgs() << "\n\n";

  for (MachineBasicBlock &MBB : MF) {
    MBB.dump();
  }
  dbgs() << "\n\n";
#endif

  Loops = &getAnalysis<MachineLoopInfo>();
#if DEBUG_LOOP_ANALYZE
  dbgs() << "Loop Info : \n";
  Loops->dump();
#endif

  for (MachineLoop *loop : *Loops) {
    LoopKind kind = analyzeLoop(loop);
		switch (kind) { 
		case WhileDo:
      changed |= transformWhileLoop(loop);
      break;
    case DoWhile:
      changed |= transformDoLoop(loop);
      break;
    default:  break;
		}
  }

  return changed;
}

bool TVMLoopInstructions::removeEmptyBlocks(MachineFunction &MF) {
  using RPOTType = ReversePostOrderTraversal<MachineFunction *>;
  RPOTType RPOT(&MF);

  vector<MachineBasicBlock *> emptyBlocks;

  for (RPOTType::rpo_iterator I = RPOT.begin(), E = RPOT.end(); I != E; ++I) {
    MachineBasicBlock *MBB = *I;
    if (MBB->pred_size() != 1 || MBB->succ_size() != 1)
      continue;
    MachineInstr &last = MBB->instr_back();
    if (last.getOpcode() != TVM::JMPX)
      continue;
    if (MBB->size() == 1) {
      emptyBlocks.push_back(MBB);
      continue;
    } 
    else if (MBB->size() == 2) {
      MachineInstr &prelast = *(++MBB->instr_rbegin());
      if (prelast.getOpcode() != TVM::PUSHCONT_MBB)
        continue;
      emptyBlocks.push_back(MBB);
    } 
		else continue;
  }

	for (MachineBasicBlock *MBB : emptyBlocks) {
    MachineBasicBlock *pred = *MBB->pred_begin();
    pred->removeSuccessor(MBB);
    pred->transferSuccessors(MBB);
    MBB->eraseFromParent();
  }

	return !emptyBlocks.empty();
}

void TVMLoopInstructions::collectMachineBlockChains(
    MachineFunction &MF, vector<vector<MachineBasicBlock *>> &chains) {
  unordered_set<MachineBasicBlock *> visited;

  using RPOTType = ReversePostOrderTraversal<MachineFunction *>;
  RPOTType RPOT(&MF);

  for (RPOTType::rpo_iterator I = RPOT.begin(), E = RPOT.end(); I != E; ++I) {
    MachineBasicBlock *MBB = *I;
    if (visited.find(MBB) != visited.end())
      continue;
    visited.insert(MBB);
#if DEBUG_COLLECT_CHAINS
    dbgs() << "Test chain for block " << blockToString(MBB)
           << " successors : " << MBB->succ_size();
    if (MBB->succ_size())
      dbgs() << "  succesor predecessors : " << (*MBB->succ_begin())->pred_size(); 
    dbgs() << "\n";
#endif
    if (MBB->succ_size() != 1)
      continue;
    MachineBasicBlock *succ = *MBB->succ_begin();
    if (succ->pred_size() != 1)
      continue;
    vector<MachineBasicBlock *> chain;
    chain.push_back(MBB);
    for (;;) {
      visited.insert(succ);
      chain.push_back(succ);
      MBB = succ;
      if (MBB->succ_size() != 1)
        break;
      succ = *MBB->succ_begin();
      if (succ->pred_size() != 1)
        break;
    }
    chains.push_back(chain);
  }
}

void TVMLoopInstructions::mergeMachineBlockChain(
    vector<MachineBasicBlock *> const &chain) {
#if DEBUG_MERGE
  dbgs() << "Merge Machine Block Chain : " << blockListToString(chain) << "\n";
#endif

  MachineBasicBlock *header = chain[0];
  //  header->dump();
  MachineInstr &headerJump = header->instr_back();
  assert(headerJump.getOpcode() == TVM::JMPX);
  header->erase_instr(&headerJump);
  MachineInstr &headerPushcont = header->instr_back();
  if (headerPushcont.getOpcode() == TVM::PUSHCONT_MBB);
  header->erase_instr(&headerPushcont);

  vector<MachineInstr *> instrList;

  for (unsigned i = 1; i < chain.size(); i++) {
    MachineBasicBlock *mbb = chain[i];
    for (MachineInstr &instr : *mbb) {
      if (instr.getOpcode() == TVM::PUSHCONT_MBB ||
          instr.getOpcode() == TVM::JMPX) {
        if (i == chain.size() - 1)
          instrList.push_back(&instr);
      } else {
        instrList.push_back(&instr);
      }
    }
  }

  for (MachineInstr *instr : instrList) {
    instr->removeFromParent();
    // header->insertAfter(header->end(), instr);
    header->push_back(instr);
  }

  MachineBasicBlock *succ0 = *header->succ_begin();
  header->removeSuccessor(succ0);
  header->transferSuccessors(chain[chain.size() - 1]);

  for (unsigned i = 1; i < chain.size(); i++) {
    MachineBasicBlock *mbb = chain[i];
    if (mbb->succ_size() > 0) {
      MachineBasicBlock *succ = *mbb->succ_begin();
      mbb->removeSuccessor(succ);
    }
    mbb->eraseFromParent();
  }

  //  header->dump();
}

bool TVMLoopInstructions::testAcceptableLoopCondition(MachineBasicBlock &MBB) {
  MachineInstr &last = MBB.instr_back();
  return last.getOpcode() == TVM::IFELSE;
}

LoopKind TVMLoopInstructions::analyzeLoop(MachineLoop *loop) {
  MachineBasicBlock *topBlock = loop->getTopBlock();
  MachineBasicBlock *bottomBlock = loop->getBottomBlock();
  MachineBasicBlock *controlBlock = loop->findLoopControlBlock();

#if DEBUG_LOOP_ANALYZE
  dbgs() << "TopBlock : ";
  topBlock->dump();
  dbgs() << "Bottom Block : ";
  bottomBlock->dump();
  // dbgs() << "Control Block : ";
  // controlBlock->dump();
#endif

  if (topBlock->pred_size() != 2)
    return Irregular;
  if (bottomBlock->succ_size() == 1) {
    if (*bottomBlock->succ_begin() == topBlock) {
      if (testAcceptableLoopCondition(*topBlock))
        return WhileDo;
    }
  } 
	else if (bottomBlock->succ_size() == 2) {
    MachineBasicBlock *succ0 = *bottomBlock->succ_begin();
    MachineBasicBlock *succ1 = *(++bottomBlock->succ_begin());
    if (succ0 == topBlock  ||  succ1 == topBlock) {
      if (testAcceptableLoopCondition(*bottomBlock))
			  return DoWhile;
    } 
  }

  return Irregular;
}

bool TVMLoopInstructions::transformWhileLoop(MachineLoop *loop) {
#if DEBUG_LOOP_TRANSFORM
  dbgs() << "Transform while loop" << "\n";
#endif
  MachineBasicBlock *predecessor = loop->getLoopPredecessor();
#if DEBUG_LOOP_TRANSFORM
  dbgs() << "Loop predecessor : " << blockToString(predecessor) << "\n";
#endif

	// TODO
	// disable loop transormation from previous passes, which transforms "while" loop to "do" loop

	return false;
}

bool TVMLoopInstructions::transformDoLoop(MachineLoop *loop) {
#if DEBUG_LOOP_TRANSFORM
  dbgs() << "Transform do loop" << "\n";
#endif
  MachineBasicBlock *predecessor = loop->getLoopPredecessor();
  MachineBasicBlock *topBlock = loop->getTopBlock();
  MachineBasicBlock *bottomBlock = loop->getBottomBlock();
  MachineBasicBlock *successor = nullptr;
  for (MachineBasicBlock *succ : bottomBlock->successors()) {
    if (succ != topBlock) {
      successor = succ;
      break;
    }
  }
  assert(successor);

#if DEBUG_LOOP_TRANSFORM
  dbgs() << "Loop predecessor : " << blockToString(predecessor) << "\n";
  dbgs() << "Loop successor : " << blockToString(successor) << "\n";
#endif
	// Loop transformations
	// For loop predecessor:
	// change JUMXP on the end of block to loop next block
	// add UNTIL before JUMPX
	//
	// For loop body:
	// remove IFELSE from the end of bottom loop block
  MachineInstr &jmpx = predecessor->instr_back();
  if (jmpx.getOpcode() != TVM::JMPX)
    return false;
  unsigned reg = jmpx.getOperand(0).getReg();

	// try to find PUSHCONT_MBB instruction, which sets target reg for JUMPX
  bool find = false;
  for (MachineInstr &instr : *predecessor) {
    if (instr.getOpcode() == TVM::PUSHCONT_MBB) {
      unsigned targetReg = instr.getOperand(0).getReg();
      if (targetReg == reg) {
        instr.getOperand(1).setMBB(successor);
				find = true;
        break;
      }
    }
  }

	BuildMI(*predecessor, /* &jmpx */ --predecessor->instr_end(), jmpx.getDebugLoc() , TII->get(TVM::UNTIL)).addMBB(topBlock);

	MachineInstr &bodyLast = bottomBlock->instr_back();
  auto opcode = bodyLast.getOpcode();
  assert(opcode == TVM::IFELSE ||  opcode == TVM::IFJMP);
  bodyLast.eraseFromParent();
	return true;
}