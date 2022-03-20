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

enum InductionExpressionKind { IEConstant, IERegister, IEAdd, IESub, IEMul };

// Induction expression representation
class InductionExpression {
  friend class RegisterInductionVariable;

  InductionExpressionKind kind;
  int64_t imm;
  InductionExpression *left;
  InductionExpression *right;

private:
  InductionExpression(InductionExpressionKind _kind, int64_t _imm,
                      InductionExpression * _left, InductionExpression * _right)
      : kind(_kind), imm(_imm), left(_left), right(_right) {}

public:
  InductionExpressionKind getKind() { return kind; }

	unsigned getReg() { 
		assert(kind == IERegister);
		return (unsigned) imm; 
	}

	int64_t getImmediate() { 
		assert(kind == IEConstant);
		return imm; 
	}

	InductionExpression *getLeft() { 
		assert(isExpression());
		return left;
	}

	InductionExpression *getRight() {
    assert(isExpression());
    return right;
  }

	bool isConstant() { return kind == IEConstant; }

	bool isReg() { return kind == IERegister; }

	bool isExpression() {
		return kind == IEAdd || kind == IESub || kind == IEMul;
	}

	bool isZero() { return isConstant() && getImmediate() == 0; } 

	bool isOne() { return isConstant() && getImmediate() == 1; }

	string toString();
};

// Reprsentation of (linear) induction variable.
// Induction variable value calculated by formula:
//   indVar = baseVar * factor + diff
//
// It's kind of simplifier Scalar Evolution for machine code instructions.
class RegisterInductionVariable {
  // Number of register representing induction variable
  unsigned indVar;
  // Number of register representing basic variable
  unsigned baseVar;
  InductionExpression *factor;
  InductionExpression *diff;
  // Machine Instuction which defined this induction variable
  MachineInstr *defInstr;

private:
  RegisterInductionVariable(unsigned indVarReg, unsigned baseVarReg,
         InductionExpression *fact, InductionExpression *dif, MachineInstr *defInsn)
      : indVar(indVarReg), baseVar(baseVarReg), factor(fact), diff(dif),
        defInstr(defInsn) {}

public:
  unsigned getIndVar() { return indVar; }

  unsigned getBaseVar() { return baseVar; }

  InductionExpression* getFactor() { return factor; }

  InductionExpression* getDiff() { return diff; }

  MachineInstr* getDefInstr() { return defInstr; }

  // Return true if this variable is basic (fundamantal) in loop,
  // return false if this variable is derivative from other induction variable
  bool isFundamental() {
    return (indVar == baseVar) && (factor->isOne()) && (diff->isZero());
  }

  string toString();
};

// Convert (virtual) register index to string
string regToString(unsigned reg) {
  char buf[8];
  string str("%");
  _itoa_s(reg & 0x7FFF, buf, 10);
  str += buf;
  return str;
}

string InductionExpression::toString() { 
	string str;
  char buf[20];
	switch (kind) { 
	case IEConstant:
    _itoa_s(getImmediate(), buf, 10);
		str += buf;
		break;
  case IERegister:
    str += regToString(getReg());
		break;
  case IEAdd:
    str += "(";
    str += getLeft()->toString();
    str += " + ";
    str += getRight()->toString();
		str += ")";
		break;
  case IESub:
    str += "(";
    str += getLeft()->toString();
    str += " - ";
    str += getRight()->toString();
    str += ")";
    break;
  case IEMul:
    str += "(";
    str += getLeft()->toString();
    str += " * ";
    str += getRight()->toString();
    str += ")";
    break;
	}
	return str;
}

string RegisterInductionVariable::toString() {
  string str;
  str += regToString(indVar);
  if (!factor->isOne()) {
    str += " * ";
    str += factor->toString();
  }
  str += regToString(baseVar);
  if (!diff->isZero()) {
    str += " + ";
    str += diff->toString();
  }
  return str;
}

enum LoopKind {
  WhileDo, //  "while { }" or "for (...;...;...) {}" loop
  DoWhile, // "do {} while(...)" loop
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
  bool transformUntilLoop(MachineLoop *loop);

  // Try to convert  "do {} while(...)" loop to original "while { }"  loop
  //   in order to use WHILE instruction from TVM to simplify loop structure
  // Returns true if conversion was successful
  bool convertUntilToWhileLoop(MachineLoop *loop);

  // Find all induction variables inside Machine Loop
  void findInductionVariables(MachineLoop *loop,
                              vector<RegisterInductionVariable> &indVars);

  // Get list of blocks from Machine Loop, which can be used for induction
  // variable analysis
  void getInductionBlocks(MachineLoop *loop, vector<MachineBasicBlock *> &list);

  // Since we analyze instructions not in SSA form, we need to check (maybe?),
  //  if potential induction variable defined only once in Machine Loop.
  // To implements this, we prepare a set of register variables which defined
  // only once.
  void getUniqueDefinedSet(MachineLoop *loop, unordered_set<unsigned> &regs);

  // Returns true if reg is not modified inside Machine Block
  static bool isConstant(unsigned reg, MachineBasicBlock *MBB);

  // Returns true if reg is not modified inside Machine Loop
  static bool isConstant(unsigned reg, MachineLoop *loop);

  // Return true if list of induction variable contains reg,
  //  define in Machine Instruction defInstr
  static bool containsVariable(vector<RegisterInductionVariable> &indVars,
                               unsigned reg, MachineInstr *defInstr);

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
      changed |= transformUntilLoop(loop);
      break;
    default:
      break;
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
    } else if (MBB->size() == 2) {
      MachineInstr &prelast = *(++MBB->instr_rbegin());
      if (prelast.getOpcode() != TVM::PUSHCONT_MBB)
        continue;
      emptyBlocks.push_back(MBB);
    } else
      continue;
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
      dbgs() << "  succesor predecessors : "
             << (*MBB->succ_begin())->pred_size();
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
  if (headerPushcont.getOpcode() == TVM::PUSHCONT_MBB)
    ;
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
  } else if (bottomBlock->succ_size() == 2) {
    MachineBasicBlock *succ0 = *bottomBlock->succ_begin();
    MachineBasicBlock *succ1 = *(++bottomBlock->succ_begin());
    if (succ0 == topBlock || succ1 == topBlock) {
      if (testAcceptableLoopCondition(*bottomBlock))
        return DoWhile;
    }
  }

  return Irregular;
}

bool TVMLoopInstructions::isConstant(unsigned reg, MachineBasicBlock *MBB) {
  for (MachineInstr &MI : *MBB) {
    for (const MachineOperand &op : MI.defs()) {
      if (op.isReg()) {
        unsigned defReg = op.getReg();
        if (reg == defReg)
          return false;
      }
    }
  }
  return true;
}

bool TVMLoopInstructions::isConstant(unsigned reg, MachineLoop *loop) {
  for (auto it = loop->block_begin(); it != loop->block_end(); it++) {
    MachineBasicBlock *MBB = *it;
    if (!isConstant(reg, MBB))
      return false;
  }
  return true;
}

void TVMLoopInstructions::getInductionBlocks(
    MachineLoop *loop, vector<MachineBasicBlock *> &list) {
  MachineBasicBlock *topBlock = loop->getTopBlock();
  MachineBasicBlock *bottomBlock = loop->getBottomBlock();
  list.push_back(topBlock);
  if (bottomBlock != topBlock)
    list.push_back(bottomBlock);
  // TODO we can also add sub blocks which are not inside
  //   if-else branches and other loops,
  //   but it seems quite rare cases
}

void TVMLoopInstructions::getUniqueDefinedSet(MachineLoop *loop,
                                              unordered_set<unsigned> &regs) {
  unordered_set<unsigned> multiDefs;
  for (auto it = loop->block_begin(); it != loop->block_end(); it++) {
    MachineBasicBlock *MBB = *it;
    for (MachineInstr &MI : *MBB) {
      for (const MachineOperand &op : MI.defs()) {
        if (op.isReg()) {
          unsigned defReg = op.getReg();
          if (multiDefs.find(defReg) != multiDefs.end())
            continue;
          if (regs.find(defReg) != regs.end()) {
            regs.erase(defReg);
            multiDefs.insert(defReg);
          } else
            regs.insert(defReg);
        }
      }
    }
  }
}

bool TVMLoopInstructions::containsVariable(
    vector<RegisterInductionVariable> &indVars, unsigned reg,
    MachineInstr *defInstr) {
  for (RegisterInductionVariable &vr : indVars) {
    if (vr.getIndVar() == reg && vr.getDefInstr() == defInstr)
      return true;
  }
  return false;
}

void TVMLoopInstructions::findInductionVariables(
    MachineLoop *loop, vector<RegisterInductionVariable> &indVars) {
  vector<MachineBasicBlock *> blocks;
  getInductionBlocks(loop, blocks);
  unordered_set<unsigned> uniqueDef;
  getUniqueDefinedSet(loop, uniqueDef);

  // Calculate fundamental (basic) induction variables
  for (MachineBasicBlock *MBB : blocks) {
    for (MachineInstr &MI : *MBB) {
      MachineOperand &def = MI.getOperand(0);
      if (!def.isReg())
        continue;
      unsigned reg = def.getReg();
      // Maybe excessive test, but now instructions are already not in SSA form
      if (uniqueDef.find(reg) == uniqueDef.end())
        continue;
      switch (MI.getOpcode()) {
      case TVM::ADD:
      case TVM::SUB: {
        MachineOperand &op1 = MI.getOperand(1);
        MachineOperand &op2 = MI.getOperand(2);
        if (op1.isReg() && op2.isReg()) {
          if (op1.getReg() == reg) {
            if (!isConstant(op2.getReg(), loop))
              continue;
          } else if (op2.getReg() == reg) {
            if (!isConstant(op1.getReg(), loop))
              continue;
          } else
            continue;
        } else
          continue;
      } break;
      case TVM::ADDCONST:
        break;
      case TVM::INC:
        break;
      case TVM::DEC:
        break;
      default:
        continue;
      }
			// TODO
//      indVars.push_back(RegisterInductionVariable(reg, reg, 1, 0, &MI));
    }

    // Calculate derivative induction variables
    bool change = false;
    do {
      for (MachineBasicBlock *MBB : blocks) {
        for (MachineInstr &MI : *MBB) {
          MachineOperand &def = MI.getOperand(0);
          if (!def.isReg())
            continue;
          unsigned reg = def.getReg();
          if (containsVariable(indVars, reg, &MI))
            continue;
        }
      }
    } while (change);
  }
}

bool TVMLoopInstructions::transformWhileLoop(MachineLoop *loop) {
#if DEBUG_LOOP_TRANSFORM
  dbgs() << "Transform while loop"
         << "\n";
#endif
  MachineBasicBlock *predecessor = loop->getLoopPredecessor();
#if DEBUG_LOOP_TRANSFORM
  dbgs() << "Loop predecessor : " << blockToString(predecessor) << "\n";
#endif

  // TODO
  // disable loop transormation from previous passes, which transforms "while"
  // loop to "do" loop

  return false;
}

bool TVMLoopInstructions::transformUntilLoop(MachineLoop *loop) {
#if DEBUG_LOOP_TRANSFORM
  dbgs() << "Transform do loop"
         << "\n";
#endif
  if (convertUntilToWhileLoop(loop))
    return true;

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

  BuildMI(*predecessor, /* &jmpx */ --predecessor->instr_end(),
          jmpx.getDebugLoc(), TII->get(TVM::UNTIL))
      .addMBB(topBlock);

  MachineInstr &bodyLast = bottomBlock->instr_back();
  auto opcode = bodyLast.getOpcode();
  assert(opcode == TVM::IFELSE || opcode == TVM::IFJMP);
  bodyLast.eraseFromParent();
  return true;
}

// Returns true if operation code corresponds to compare with register
bool isCompareReg(unsigned opcode) {
  switch (opcode) {
  case TVM::EQ:
  case TVM::NE:
  case TVM::SLT:
  case TVM::SGT:
  case TVM::SLE:
  case TVM::SGE:
    return true;
  default:
    return false;
  }
}

// Returns true if operation code corresponds to compare with immediate
// operation
bool isCompareImm(unsigned opcode) {
  switch (opcode) {
  case TVM::EQIMM:
  case TVM::NEIMM:
  case TVM::STLIMM:
  case TVM::SGTIMM:
  case TVM::ULTIMM:
  case TVM::UGTIMM:
    return true;
  default:
    return false;
  }
}

// Returns operation code for reverse comparion relative to opcode
unsigned getReverseCompareOpcode(unsigned opcode) {
  switch (opcode) {
  case TVM::EQ:
    return TVM::NE;
  case TVM::NE:
    return TVM::EQ;
  case TVM::SLT:
    return TVM::SGE;
  case TVM::SGT:
    return TVM::SLE;
  case TVM::SLE:
    return TVM::SGT;
  case TVM::SGE:
    return TVM::SLT;
  case TVM::EQIMM:
    return TVM::NEIMM;
  case TVM::NEIMM:
    return TVM::EQIMM;
  case TVM::STLIMM:
  case TVM::SGTIMM:
  case TVM::ULTIMM:
  case TVM::UGTIMM:
  default:
    return 0;
  }
}

bool TVMLoopInstructions::convertUntilToWhileLoop(MachineLoop *loop) {
  // Search for diamond pattern:
  // Root with IF condition at end to check if we can enter to first iteration
  // of loop Loop Preheader Loop Body Loop Cleanup - assigng variables if we
  // don't enter to loop body due to precondition Loop Postblock
  //
  // Edges:
  // Root -> Loop Preheader, Root -> Loop Cleanup
  // Loop Preheader -> Loop Body (Top Block)
  // Loop Body (Bottom Block) -> Loop Postblock
  // Loop Cleanup -> Loop Postblock

  MachineBasicBlock *preheader = loop->getLoopPredecessor();
  MachineBasicBlock *topBlock = loop->getTopBlock();
  MachineBasicBlock *bottomBlock = loop->getBottomBlock();

  if (preheader->pred_size() != 1 || preheader->succ_size() != 1)
    return false;
  MachineBasicBlock *root = *preheader->pred_begin();
  if (root->succ_size() != 2)
    return false;
  MachineBasicBlock *condCleanup;
  if (preheader != *root->succ_begin())
    condCleanup = *root->succ_begin();
  else
    condCleanup = *(++root->succ_begin());
  if (condCleanup->pred_size() != 1 || condCleanup->succ_size() != 1)
    return false;
  MachineBasicBlock *postBlock = *condCleanup->succ_begin();
  if (find(bottomBlock->succ_begin(), bottomBlock->succ_end(), postBlock) ==
      bottomBlock->succ_end())
    return false;

  MachineInstr &ifFirst = *root->instr_rbegin();
  MachineInstr &ifNext = *bottomBlock->instr_rbegin();

  //	ifFirst.dump();
  //  ifNext.dump();

  unsigned firstCond = ifFirst.getOpcode();
  unsigned nextCond = ifNext.getOpcode();

  if (firstCond != TVM::IFJMP && firstCond != TVM::IFNOTJMP)
    return false;
  if (nextCond != TVM::IFELSE)
    return false;
  unsigned firstCondReg = ifFirst.getOperand(0).getReg();
  unsigned mainCondReg = ifNext.getOperand(0).getReg();

  // Find comparasion instruction from loop body
  MachineInstr *mainCompare = nullptr;
  for (auto it = bottomBlock->instr_rbegin(); it != bottomBlock->rend(); it++) {
    MachineInstr &instr = *it;
    bool cmpReg = isCompareReg(instr.getOpcode());
    bool cmpImm = isCompareImm(instr.getOpcode());
    if (cmpReg || cmpImm) {
      unsigned destReg = instr.getOperand(0).getReg();
      if (destReg == mainCondReg) {
        unsigned srcReg1 = instr.getOperand(0).getReg();
        // TODO check if source register in comparison is inductive register
        // variable
        /*
        if (srcReg1 != mainInductReg) {
          if (cmpReg) {
            unsigned srcReg2 = instr.getOperand(1).getReg();
            if (srcReg2 != mainInductReg)
              continue;
          }
        }
                                */
        mainCompare = &instr;
        break;
      }
    }
  }
  if (!mainCompare)
    return false;
  //  mainCompare->dump();

  // Now we must check if the first loop condition appropriately derivates
  // from main induction register variable
  MachineInstr *firstCompare = nullptr;
  for (auto it = root->instr_rbegin(); it != root->instr_rend(); it++) {
    MachineInstr &instr = *it;
    bool cmpReg = isCompareReg(instr.getOpcode());
    bool cmpImm = isCompareImm(instr.getOpcode());
    if (cmpReg || cmpImm) {
      unsigned destReg = instr.getOperand(0).getReg();
      if (destReg == firstCondReg) {
        unsigned srcReg1 = instr.getOperand(0).getReg();
        if (srcReg1 != mainCondReg) {
          if (cmpReg) {
            unsigned srcReg2 = instr.getOperand(1).getReg();
            if (srcReg2 != mainCondReg)
              continue;
          }
        }
        firstCompare = &instr;
        break;
      }
    }
  }

  if (!firstCompare)
    return false;
  //  firstCompare->dump();

  // All initialization instruction from condition cleanup block
  // must have their counterparts in loop preheader
  auto cit = condCleanup->instr_rbegin();
  cit++; // skip the last JMPX
  while (cit != condCleanup->instr_rend()) {
    MachineInstr &instr = *cit++;
    bool match = false;
    for (MachineInstr &instr2 : *preheader) {
      // TODO maybe we need more complex instruction matching
      if (instr.getOpcode() == instr2.getOpcode() &&
          instr.getOperand(0).getReg() == instr2.getOperand(0).getReg()) {
        match = true;
        break;
      }
    }
    if (!match)
      return false;
  }

  // Remove conditional jump from the end of root
  firstCompare->eraseFromParent();
  ifFirst.eraseFromParent();

  MachineInstr *postLoopCont = nullptr;
  MachineInstr *loopBodyCont = nullptr;
  MachineInstr *condCleanupCont = nullptr;

  // Find continuations from root
  for (MachineInstr &instr : *root) {
    if (instr.getOpcode() == TVM::PUSHCONT_MBB) {
      MachineBasicBlock *block = instr.getOperand(1).getMBB();
      instr.dump();
      if (block == postBlock)
        postLoopCont = &instr;
      else if (block == condCleanup)
        condCleanupCont = &instr;
    }
  }

  // Transfer all variable initializations from preheader to root
  vector<MachineInstr *> insns;
  for (MachineInstr &instr : *preheader) {
    unsigned opcode = instr.getOpcode();
    if (opcode == TVM::JMPX)
      break;
    else if (opcode == TVM::PUSHCONT_MBB) {
      loopBodyCont = &instr;
      continue;
    }
    insns.push_back(&instr);
  }
  for (MachineInstr *instr : insns) {
    instr->removeFromParent();
    root->push_back(instr);
  }

  root->dump();

  assert(postLoopCont);
  assert(loopBodyCont);
  assert(condCleanupCont);

  postLoopCont->removeFromParent();
  loopBodyCont->removeFromParent();

  // Now the preheader will be condition continuation block for WHILE
  // instruction
  preheader->clear();
  mainCompare->removeFromParent();
  preheader->push_back(mainCompare);
  //	preheader->dump();

  // Remove conditional jump from the end of loop's bottom block
  ifNext.removeFromParent();

  // condition cleanup block is not longer needed and we delete it and
  // corresponding continuation push
  condCleanupCont->eraseFromParent();
  condCleanup->eraseFromParent();

  // Add WHILE instruction + continuations
  /*
  postLoopCont->getOperand(1).setMBB(preheader);
  const DebugLoc &dbgLoc = root->instr_back().getDebugLoc();
  BuildMI(*postBlock, postBlock->instr_begin(), dbgLoc, TII->get(TVM::WHILE))
      .addReg(postLoopCont->getOperand(0).getReg())
      .addReg(loopBodyCont->getOperand(0).getReg());
  postBlock->insert(postBlock->instr_begin(), loopBodyCont);
  postBlock->insert(postBlock->instr_begin(), postLoopCont);
	*/

  preheader->getParent()->dump();

  return true;
}