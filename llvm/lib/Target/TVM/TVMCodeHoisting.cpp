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
/// This file implements a Machine Instruction hoisting.
/// 
/// If two instructions in different blocks are equal, in some cases we can 
/// instead insert one instruciton in Machine Block,
/// which is least common dominator of blocks, containing those instructions.
/// Such instruction pairs sometimes generated on some TVM backend stage,
/// for example during "while" loops processing (and maybe in other cases).
///
///
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "TVMSubtarget.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/CodeGen/LiveIntervals.h"
#include "llvm/CodeGen/MachineBlockFrequencyInfo.h"
#include "llvm/CodeGen/MachineDominators.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfoImpls.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include <map>
#include <unordered_map>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "tvm-code-hoisting"

#define DEBUG_DEFINITIONS 1
#define DEBUG_HOIST 1

namespace {
class TVMCodeHoisting final : public MachineFunctionPass {
  StringRef getPassName() const override { return "TVM Code Hoisting"; }

 void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<LiveIntervals>();
    AU.addPreservedID(LiveVariablesID);
    AU.addRequired<MachineDominatorTree>();
    AU.addPreserved<MachineDominatorTree>();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

public:
  static char ID; // Pass identification, replacement for typeid
  TVMCodeHoisting() : MachineFunctionPass(ID) {}

private:
  bool runOnBasicBlocks(MachineFunction &MF);

	// Calculate list of instructions which defined some virtual register
  void calculateDefinitions(MachineFunction &MF, 
		unordered_map<unsigned, vector<MachineInstr *>> &defs);

	// Find all Machine Blocks on path between from and to
  static const vector<MachineBasicBlock*> &findPathBlocks(MachineBasicBlock* from, MachineBasicBlock* to, 
		map<pair<MachineBasicBlock *, MachineBasicBlock *>, vector<MachineBasicBlock*>> &pathMap);

  static void findPathBlocks(MachineBasicBlock *cur, MachineBasicBlock *to,
     vector<MachineBasicBlock *> &list, vector<MachineBasicBlock *> &path, set<MachineBasicBlock *> &visited);

	// Check if Machine Instruction is appropriate for hoisting
	static bool isAppropriate(const MachineInstr& instr);

  // Kind of equality, but we compare only register and immediate operands
  static bool conforms(const MachineInstr &ia, const MachineInstr &ib);

	// Check if instruction result and operands are not modified
	// on path from some machine block to block in which instruction contained
	static bool isUnmodified(MachineBasicBlock* MBB, MachineInstr* instr,
      unordered_map<unsigned, vector<MachineInstr *>> &defs,
      map<pair<MachineBasicBlock *, MachineBasicBlock *>, vector<MachineBasicBlock *>> &pathMap);

	// Append instructionto the end of Machine Block, but before jump if present
	static void appendToBlock(MachineBasicBlock* MBB, MachineInstr* instr);

  // Print instructions which define different registers
  void printDefinitions(unordered_map<unsigned, vector<MachineInstr *>> defs,
            raw_ostream &os, bool multipleOnly = true);

  const TargetInstrInfo *TII;
  MachineDominatorTree *MDT;
};
} // end anonymous namespace

char TVMCodeHoisting::ID = 0;
INITIALIZE_PASS(TVMCodeHoisting, DEBUG_TYPE,
                "Machine instructions hoisting", false, false)

FunctionPass *llvm::createTVMCodeHoisting() { return new TVMCodeHoisting(); }

bool TVMCodeHoisting::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(dbgs() << "********** Machine Instruction Hoisting **********\n"
                       "********** Function: "
                    << MF.getName() << '\n');

  TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();
  MDT = &getAnalysis<MachineDominatorTree>();

  bool changed = runOnBasicBlocks(MF);
  return changed;
}

bool TVMCodeHoisting::isAppropriate(const MachineInstr &instr) { 
	switch (instr.getOpcode()) { 
	case TVM::ADD:
  case TVM::ADDCONST:
  case TVM::CONST_I257:
  case TVM::CONST_U257: 
  case TVM::DIV:
  case TVM::MOD:
  case TVM::MUL:
  case TVM::SUB:
		return true;
  default: return false;
	}
}

// Kind of equality, but we compare only register and immediate operands
bool TVMCodeHoisting::conforms(const MachineInstr &ia, const MachineInstr &ib) { 
	if (ia.getOpcode() != ib.getOpcode())
		return false;
  for (unsigned i = 0; i < ia.getNumOperands(); i++) {
    MachineOperand &opera = ((MachineInstr &) ia).getOperand(i);
    MachineOperand &operb = ((MachineInstr &) ib).getOperand(i);
    if (opera.isReg()) {
      if (opera.getReg() != operb.getReg())
				return false;
		} 
		else if (opera.isImm()) {
      if (opera.getImm() != operb.getImm())
				return false;
		}
	}
	return true;
}

void TVMCodeHoisting::calculateDefinitions(MachineFunction &MF,
    unordered_map<unsigned, vector<MachineInstr *>> &defs) {
  for (MachineBasicBlock &MBB : MF) {
    for (MachineInstr &instr : MBB) {
      for (MachineOperand &def : instr.defs()) {
        if (def.isReg()) {
          unsigned reg = def.getReg();
          auto it = defs.find(reg);
          if (it != defs.end()) {
            vector<MachineInstr *> &list = it->second;
            list.push_back(&instr);
          } 
					else {
            vector<MachineInstr *> list;
            list.push_back(&instr);
            defs.insert(pair<unsigned, vector<MachineInstr *>>(reg, list));
					}
				}
			}
		}
	}
}

const vector<MachineBasicBlock *> &TVMCodeHoisting::findPathBlocks(
    MachineBasicBlock *from, MachineBasicBlock *to,
	  map<pair<MachineBasicBlock *, MachineBasicBlock *>, vector<MachineBasicBlock *>> &pathMap) {
  pair<MachineBasicBlock *, MachineBasicBlock *> p(from, to);
  auto it = pathMap.find(p);
  if (it != pathMap.end())
		return it->second;
  vector<MachineBasicBlock *> list;
  vector<MachineBasicBlock *> path;
	set<MachineBasicBlock *> visited;
  findPathBlocks(from, to, list, path, visited);
	pathMap.insert(make_pair(p, list));
  return pathMap.find(p)->second;
}

void TVMCodeHoisting::findPathBlocks(MachineBasicBlock *cur, MachineBasicBlock *to,
    vector<MachineBasicBlock *> &list,
	  vector<MachineBasicBlock *> &path, set<MachineBasicBlock *> &visited) {
  if (find(visited.begin(), visited.end(), cur) != visited.end())
		return;
	// Mark the current node as visited and store it in path
  visited.insert(cur);
  path.push_back(cur);
  if (cur == to) {
    for (MachineBasicBlock *MBB : path) {
      if (find(list.begin(), list.end(), MBB) == list.end())
        list.push_back(MBB);
    }
  } 
	else {
    for (MachineBasicBlock *succ : cur->successors()) {
      findPathBlocks(succ, to, list, path, visited);
	  }
  }
  visited.erase(cur);
  path.pop_back();
}

bool TVMCodeHoisting::isUnmodified(MachineBasicBlock *MBB, MachineInstr *instr,
    unordered_map<unsigned, vector<MachineInstr *>> &defs,
	  map<pair<MachineBasicBlock *, MachineBasicBlock *>, vector<MachineBasicBlock *>> &pathMap) {
  MachineBasicBlock *MBBi = instr->getParent(); 
//	dbgs() << "From MBB : " << MBB->getName() << " To MBB : " << MBBi->getName() << " Instruction : ";  instr->dump();
	const vector<MachineBasicBlock *> &paths = findPathBlocks(MBB, MBBi, pathMap);
	unsigned defReg = instr->getOperand(0).getReg();
  vector<MachineInstr *> &defList = defs[defReg];
  for (MachineBasicBlock *MBBp : paths) {
    if (MBBp == MBB)
		  continue;
    for (MachineInstr *insp : defList) {
      if (insp == instr)
			  continue;
      if (insp->getParent() == MBBp)
				return false;
		}
		// Check if some instruction operand is modified in block
    for (MachineOperand &use : instr->uses()) {
      if (!use.isReg())
				continue;
      unsigned useReg = use.getReg();
      vector<MachineInstr *> &defUseList = defs[useReg];
      for (MachineInstr *insu : defUseList) {
        if (insu->getParent() == MBBp)
					return false;
			}
    }
 	}
	return true;
}

// Returns true if instruction is jump on the end of Machine Block
static bool isJump(MachineInstr &instr) { 
	switch (instr.getOpcode()) { 
	case TVM::JMPX:
  case TVM::IFJMP:
  case TVM::IFELSE:
  case TVM::IFNOT:
  case TVM::IFNOTJMP:
		return true;
  default: return false;
	}
}

// Print instructions which define different registers
void TVMCodeHoisting::printDefinitions(
    unordered_map<unsigned, vector<MachineInstr *>> defs,
                 raw_ostream &os, bool multipleOnly) {
  os << "Register definitions :" << "\n";
  for (auto it : defs) {
    vector<MachineInstr *> &defList = it.second; 
		if (defList.size() < 2  &&  multipleOnly)
			continue;
		bool first = true;
		for (MachineInstr *instr : defList) {
      if (!first)
        os << " , ";
      instr->print(os, false, false, false, false, TII);
			first = false;
    }
    os << "\n";
	}
}

void TVMCodeHoisting::appendToBlock(MachineBasicBlock *MBB,
	  MachineInstr *instr) {
  if (MBB->size() == 0) {
    MBB->push_back(instr);
	}
  auto it = --MBB->instr_end();
  if (isJump(*it))
    MBB->insert(it, instr);
  else MBB->push_back(instr);
}

bool TVMCodeHoisting::runOnBasicBlocks(MachineFunction &MF) {
  bool changed = false;

  unordered_map<unsigned, vector<MachineInstr *>> defs;
  calculateDefinitions(MF, defs);
#if DEBUG_DEFINITIONS
  printDefinitions(defs, dbgs());
#endif
  map<pair<MachineBasicBlock *, MachineBasicBlock *>, vector<MachineBasicBlock *>> pathMap;

	for (auto it : defs) {
    vector<MachineInstr *> &defList = it.second; 
		vector<unsigned> hoistList;
		if (defList.size() < 2)
			continue;
		bool hoisted;
    do {
			hoisted = false;
      for (int i = 0; i < defList.size() - 1  &&  !hoisted; i++) {
        if (find(hoistList.begin(), hoistList.end(), i) != hoistList.end())
					continue;
		    MachineInstr* ini = defList[i];
        if (!isAppropriate(*ini))
				  continue;
        MachineBasicBlock *MBBi = ini->getParent();
        for (unsigned j = i + 1; j < defList.size(); j++) {
          if (find(hoistList.begin(), hoistList.end(), j) != hoistList.end())
            continue;
          MachineInstr *inj = defList[j];
          if (!isAppropriate(*inj))
            continue;
          if (!conforms(*ini, *inj))
					  continue;
          MachineBasicBlock *MBBj = inj->getParent();
				  // Now we have a pair of conforming instructions.
				  // Find the least common dominator for hoisting candidates
          MachineBasicBlock *MBBdom = MDT->findNearestCommonDominator(MBBi, MBBj);
          if (!MBBdom)
					  continue;

				  // Check if instructions defs and uses are modified on some path from dominated block
				  // to block in which instruction contained
          if (!isUnmodified(MBBdom, ini, defs, pathMap))
					  continue;
          if (!isUnmodified(MBBdom, inj, defs, pathMap))
            continue;

				  // Transfer first instruction to the end of common dominator
				  // and erase the second
          ini->eraseFromParent();
          inj->removeFromParent();
          appendToBlock(MBBdom, inj);
#if DEBUG_HOIST
          dbgs() << "Hoisting instructions ";
          inj->dump();
          dbgs() << "From machine blocks "
                 << MBBi->getName() << ", "
                 << MBBj->getName()
                 << " To machine block " 
						     << MBBdom->getName()
                 << "\n";
#endif

          hoistList.push_back(i);
          hoistList.push_back(j);
          if (MBBi->size() == 0)

					hoisted = true;
					break;
        }
      }
    } 
		while (hoisted);
	}

	return changed;
}

