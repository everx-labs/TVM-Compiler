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
/// This file implements a locla (intrablock) scheduling pass.
///
///
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "TVMStack.h"
#include "TVMStackFixup.h"
#include "TVMSubtarget.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/SmallVector.h"
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

#include <iostream>
#include <unordered_map>
#include <unordered_set>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "tvm-local-scheduler"

/*
class InstructionTree {
  MachineInstr &instr;
  const TargetInstrInfo *instrInfo;
  vector<InstructionTree *> children;

public:
  InstructionTree(MachineInstr &insn, const TargetInstrInfo *TII = nullptr)
      : instr(insn), instrInfo(TII), children() {}

  ~InstructionTree() {}

  MachineInstr &getInstruction() { return instr; }

  void addChild(InstructionTree *child) { children.push_back(child); }

  string toString() {
    string str;
    raw_string_ostream OS(str);
    instr.print(OS, true, false, true, false, instrInfo);
    if (children.size() > 0) {
      OS << "(";
      bool first = true;
      for (auto tree : children) {
        if (!first)
          OS << ",";
        OS << tree->toString();
        first = false;
      }
      OS << ")";
    }
    return OS.str();
  }
};

class TreeGraphNode {
  int index;
  InstructionTree *tree;
  vector<TreeGraphNode *> predecessors;

public:
  TreeGraphNode(int idx, InstructionTree *itree)
      : index(idx), tree(itree), predecessors() {}

  int getIndex() { return index; }

  InstructionTree *getTree() { return tree; }

  void addPredecessor(TreeGraphNode *node) { predecessors.push_back(node); }

  bool hasPredecessor(TreeGraphNode *node) {
    auto it = find(predecessors.begin(), predecessors.end(), node);
    return it != predecessors.end();
  }

  int getPredecessorNum() { return predecessors.size(); }

  vector<TreeGraphNode *> &getPredecessors() { return predecessors; }

  string toString() { return tree->toString(); }
};

class TreeGraph {
  vector<TreeGraphNode *> roots;
  vector<TreeGraphNode *> leafs;
  vector<TreeGraphNode *> nodes;

public:
  TreeGraph() : roots(), leafs(), nodes() {}

  ~TreeGraph() {}

  int size() { return nodes.size(); }

  TreeGraphNode *addNode(InstructionTree *tree) {
    TreeGraphNode *node = new TreeGraphNode(nodes.size(), tree);
    nodes.push_back(node);
    return node;
  }

  void addRoot(TreeGraphNode *node) { roots.push_back(node); }

  void addLeaf(TreeGraphNode *node) { leafs.push_back(node); }

  void addEdge(TreeGraphNode *from, TreeGraphNode *to) {
    to->addPredecessor(from);
  };

  // Calculate root & leafs
  void preprocess();

  bool verify();

  void findTopologicalSorts(vector<vector<TreeGraphNode *>> &topsorts);

  static string toString(vector<TreeGraphNode *> list);

  string toString(int count[]);

private:
  void findTopologicalSorts(vector<vector<TreeGraphNode *>> &topsorts,
                            int count[], vector<TreeGraphNode *> &queue,
                            vector<TreeGraphNode *> &topsort);

  static void remove(vector<TreeGraphNode *> list, TreeGraphNode *element) {
    std::remove(list.begin(), list.end(), element);
  }
};

void TreeGraph::preprocess() {
  vector<uint8_t> hasSuccessors(size());
  for (int i = 0; i < size(); i++)
    hasSuccessors[i] = 0;
  for (auto node : nodes) {
    if (node->getPredecessorNum() == 0) {
      roots.push_back(node);
    } else {
      for (auto pred : node->getPredecessors()) {
        hasSuccessors[pred->getIndex()] = 1;
      }
    }
  }
  for (auto node : nodes) {
    if (!hasSuccessors[node->getIndex()])
      leafs.push_back(node);
  }
}

bool TreeGraph::verify() {
  bool valid = true;
  for (auto node : nodes) {
    if (node->getPredecessorNum() == 0) {
      auto it = find(leafs.begin(), leafs.end(), node);
      if (it == leafs.end())
        valid = false;
    }
  }
  return valid;
}

void TreeGraph::findTopologicalSorts(
    vector<vector<TreeGraphNode *>> &topsorts) {
  vector<TreeGraphNode *> queue(leafs);
  vector<TreeGraphNode *> topsort;
  int *count = new int[size()];
  for (int i = 0; i < size(); i++)
    count[i] = 0;
  for (TreeGraphNode *node : nodes) {
    for (auto pred : node->getPredecessors()) {
      count[pred->getIndex()]++;
    }
  }
  std::cout << "  count = " << toString(count).c_str() << endl;
  findTopologicalSorts(topsorts, count, queue, topsort);
  delete[] count;
}

string TreeGraph::toString(vector<TreeGraphNode *> list) {
  string str;
  bool first = true;
  for (auto node : list) {
    if (!first)
      str += ", ";
    str += node->toString();
    first = false;
  }
  return str;
}

string TreeGraph::toString(int count[]) {
  string str;
  char buf[10];
  for (int i = 0; i < size(); i++) {
    _itoa_s(count[i], buf, 10);
    str += buf;
    if (i < size() - 1)
      str += ", ";
  }
  return str;
}

void TreeGraph::findTopologicalSorts(vector<vector<TreeGraphNode *>> &topsorts,
                                     int count[],
                                     vector<TreeGraphNode *> &queue,
                                     vector<TreeGraphNode *> &topsort) {
  //	std::cout << "queue : " << toString(queue).c_str() << "  topsort = " <<
  //toString(topsort).c_str() << "  count = " << toString(count).c_str() <<
  //endl;
  if (!queue.empty()) {
    for (TreeGraphNode *node : queue) {
      topsort.push_back(node);
      // Create the queue without node element
      vector<TreeGraphNode *> subQueue;
      for (TreeGraphNode *snode : queue) {
        if (snode != node)
          subQueue.push_back(snode);
      }
      // save count indexes, which we decremented
      vector<int> decrements;
      for (auto pred : node->getPredecessors()) {
        decrements.push_back(pred->getIndex());
        int cnt = count[pred->getIndex()] - 1;
        count[pred->getIndex()] = cnt;
        if (cnt == 0)
          subQueue.push_back(pred);
      }
      findTopologicalSorts(topsorts, count, subQueue, topsort);
      topsort.pop_back();
      // restore count indexes
      for (auto index : decrements) {
        count[index] = count[index] + 1;
      }
    }
  } else {
    if (topsort.size() == size()) {
      //			std::cout << "topsort " <<
      //toString(topsort).c_str() << endl;
      topsorts.push_back(topsort);
    }
  }
}
*/

// Convert (virtual) register index to string
string regToString(unsigned reg) {
  char buf[8];
  string str("%");
  _itoa_s(reg & 0x7FFF, buf, 10);
  str += buf;
  return str;
}

// Convert Register list to string
string regListToString(vector<unsigned> const &list) {
  string str = "[ ";
  bool first = true;
  for (unsigned reg : list) {
    if (!first)
      str += ", ";
    str += regToString(reg);
    first = false;
  }
  str += " ]";
  return str;
}

// Convert Register unordered set to string
string regSetToString(unordered_set<unsigned> const &set) {
  string str = "[ ";
  bool first = true;
  for (unsigned reg : set) {
    if (!first)
      str += ", ";
    str += regToString(reg);
    ;
    first = false;
  }
  str += " ]";
  return str;
}

// Convert Register unordered map (for example, to def/use count) to string
string regMapToString(unordered_map<unsigned, unsigned> const &map) {
  string str = "[ ";
  bool first = true;
  for (std::pair<unsigned, unsigned> it : map) {
    if (!first)
      str += ", ";
    regToString(it.first);
    str += " = ";
    str += (it.second);
    first = false;
  }
  str += " ]";
  return str;
}

// Simple test if vector contains the register or other value
static bool contains(vector<unsigned> const &list, unsigned reg) {
  //  return std::find(list.begin(), list.end(), value) != list.end();

  for (int i = 0; i < list.size(); i++) {
    if (list[i] == reg)
      return true;
  }
  return false;
}

// Find index of the register in vector
static unsigned indexOf(vector<unsigned> const &list, unsigned reg) {
  unsigned idx;
  for (idx = 0; idx < list.size(); idx++) {
    if (list[idx] == reg)
      break;
  }
  return idx;
}

static string blockToString(MachineBasicBlock *MBB) {
  if (!MBB)
    return string("null");
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

// Print edges of MachineFunction CFG
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

// Representing Machine Block as data queue to disguise an original stack model
class BlockQueueInfo {
public:
  // Preferred Input Queue (without register repetitions, will be duplicated
  // inside block if needed)
  vector<unsigned> preferredInputQueue;
  // Preferred Output Queue
  vector<unsigned> preferredOutputQueue;
  // Number of uses of register inside the block
  unordered_map<unsigned, unsigned> numUses;
  // Actual Input Queue, must be converted on the start of block to convenient
  // for block calculations
  vector<unsigned> actualInputQueue;
  // Actual Output Queue, must be converted from Preferred Output Queue during
  // queue fixup on join CFG or DJ-Graph edges
  vector<unsigned> actualOutputQueue;

  BlockQueueInfo()
      : preferredInputQueue(), preferredOutputQueue(), numUses(),
        actualInputQueue(), actualOutputQueue() {}

  void dump();

  // Calculate fixup on block input
  StackFixup calculateInputFixup(MachineFunction &MF) {
    return calculateFixup(MF, preferredInputQueue, actualInputQueue);
  }

  // Calculate fixup on block output
  StackFixup calculateOutputFixup(MachineFunction &MF) {
    return calculateFixup(MF, actualOutputQueue, preferredOutputQueue);
  }

  // Calculate optimized queue from several different queues
  // Needs for fork/join queue fixups
  static void calculateOptimizedMerge(vector<vector<unsigned>> const &queues,
                                      vector<unsigned> const &queueWeights,
                                      vector<unsigned> merge);

  static void calculateOptimizedMerge(vector<vector<unsigned>> const &queues,
                                      vector<unsigned> merge);

  // Calculate fixup from different queue (stack) values
  static StackFixup calculateFixup(MachineFunction &MF,
                                   vector<unsigned> const &dst,
                                   vector<unsigned> const &src);

	// Calculate maximum length of each queue registers with coincident indexes.
  // Use for simple fixup operations.
  static unsigned calculateCommonTailLength(vector<vector<unsigned>> const &queues);
};

class SchedulerUtils {
public:
  // Gather "pseodo-global" registers, i.e. such that can be reified
  // (pushed)
  //  from constant values (like block addresses) or fixed places on stack,
  //  like parameters
  // Other values will be received from block input "queue"
  static void gatherPseudoGlobal(MachineFunction &MF, unordered_map<unsigned, MachineInstr *> &pseudoMap);

  // Similar as from TVMStackModel, but with different parameters
  static void gatherBlockLiveIns(MachineBasicBlock &MBB, MachineRegisterInfo *MRI,
                     LiveIntervals * LIS, unordered_set<unsigned> &vregs);

  // Similar as from TVMStackModel, but with different parameters
  static void gatherBlockLiveOuts(MachineBasicBlock &MBB,
                     MachineRegisterInfo *MRI, LiveIntervals *LIS, unordered_set<unsigned> &vregs);

  // Gather information about input & output block data queue
  static void gatherBlockQueueInfo(MachineBasicBlock &MBB, MachineRegisterInfo *MRI,
                    LiveIntervals *LIS, unordered_map<unsigned, MachineInstr *> const &pseudoMap,
                    BlockQueueInfo &queueInfo);
};

void SchedulerUtils::gatherBlockLiveIns(MachineBasicBlock &MBB,
    MachineRegisterInfo *MRI, LiveIntervals *LIS,
          unordered_set<unsigned> &vregs) {
  for (unsigned i = 0, e = MRI->getNumVirtRegs(); i != e; ++i) {
    unsigned Reg = TargetRegisterInfo::index2VirtReg(i);
    if (LIS->hasInterval(Reg)) {
      if (LIS->isLiveInToMBB(LIS->getInterval(Reg), &MBB)) {
        vregs.insert(Reg);
      }
    }
  }
}

void SchedulerUtils::gatherBlockLiveOuts(MachineBasicBlock &MBB,
   MachineRegisterInfo *MRI, LiveIntervals *LIS, unordered_set<unsigned> &vregs) {
  for (unsigned i = 0, e = MRI->getNumVirtRegs(); i != e; ++i) {
    unsigned Reg = TargetRegisterInfo::index2VirtReg(i);
    if (LIS->hasInterval(Reg)) {
      if (LIS->isLiveOutOfMBB(LIS->getInterval(Reg), &MBB)) {
        vregs.insert(Reg);
      }
    }
  }
}

void SchedulerUtils::gatherBlockQueueInfo(MachineBasicBlock &MBB, 
	  MachineRegisterInfo *MRI, LiveIntervals *LIS,
    unordered_map<unsigned, MachineInstr *> const &pseudoMap,
    BlockQueueInfo &queueInfo) {
  unordered_set<unsigned> liveIns;
  gatherBlockLiveIns(MBB, MRI, LIS, liveIns);
  unordered_set<unsigned> liveOuts;
  gatherBlockLiveOuts(MBB, MRI, LIS, liveOuts);

  for (MachineInstr &MI : MBB) {
    for (const MachineOperand &op : MI.uses()) {
      if (op.isReg()) {
        unsigned reg = op.getReg();
        unsigned numUses = 1;
        auto search = queueInfo.numUses.find(reg);
        if (search != queueInfo.numUses.end()) {
          numUses = search->second + 1;
          queueInfo.numUses.insert_or_assign(reg, numUses);
        }

        if (pseudoMap.find(reg) == pseudoMap.end() &&
            liveIns.find(reg) != liveIns.end()) {
          if (numUses == 1)
            queueInfo.preferredInputQueue.push_back(reg);
        }
      }
    }
    for (const MachineOperand &op : MI.defs()) {
      if (op.isReg()) {
        unsigned reg = op.getReg();
        if (liveOuts.find(reg) != liveOuts.end()) {
          queueInfo.preferredOutputQueue.push_back(reg);
        }
      }
    }
  }
}

void SchedulerUtils::gatherPseudoGlobal(MachineFunction &MF, 
    unordered_map<unsigned, MachineInstr *> &pseudoMap) {
  unordered_set<unsigned> defined;
  for (MachineBasicBlock &MBB : MF) {
    for (MachineInstr &MI : MBB) {
      //      MI.dump();
      unsigned code = MI.getOpcode();
      unsigned reg = 0;
      switch (code) {
      case TVM::ARGUMENT: {
        const MachineOperand &result = MI.getOperand(0);
        reg = result.getReg();
      } break;
      case TVM::PUSHCONT_MBB: {
        const MachineOperand &result = MI.getOperand(0);
        reg = result.getReg();
      } break;
      default:
        break;
      }
      if (reg != 0) {
        if (defined.find(reg) == defined.end()) {
          // dbgs() << "Reg %" << (reg & 0x7FFF) << "\n";
          pseudoMap.insert(pair<unsigned, MachineInstr *>(reg, &MI));
        }
      }
      for (const MachineOperand &op : MI.defs()) {
        if (op.isReg()) {
          unsigned reg = op.getReg();
          if (defined.find(reg) != defined.end()) {
            pseudoMap.erase(reg);
          }
          defined.insert(reg);
        }
      }
    }
  }
}

enum LayeredInstructionDAGNodeKind { LINSource, LINSink, LINInstruction };

class LayeredInstructionDAGNode {
  friend class LayeredInstructionDAG;

  LayeredInstructionDAGNodeKind kind;
  MachineInstr *instr;
  unsigned reg;
  vector<LayeredInstructionDAGNode *> Predecessors;
  vector<LayeredInstructionDAGNode *> Successors;
  unsigned level;
  unsigned orderNum;

private:
  LayeredInstructionDAGNode(MachineInstr *insn)
      : kind(LINInstruction), instr(insn), reg(0), Predecessors(), Successors(),
        level(0) {}

  LayeredInstructionDAGNode(LayeredInstructionDAGNodeKind kn, unsigned rg)
      : kind(kn), instr(nullptr), reg(rg), Predecessors(), Successors(),
        level(0) {}

public:
  LayeredInstructionDAGNodeKind getKind() { return kind; }

  MachineInstr *getInstruction() { return instr; }

  unsigned getReg() { return reg; }

  unsigned getLevel() { return level; }

  void setLevel(unsigned lvl) { level = lvl; }

  // return order number of node in scheduling sequence
  // the more the number - the later is the node instruction
  unsigned getOrderNum() { return orderNum; }

  void setOrderNum(unsigned num) { orderNum = num; }

  // TODO
  const TargetInstrInfo *getInstructionInfo() { return nullptr; }

  string toString();

  // Layered Instruction Graph iterators
  using pred_iterator = vector<LayeredInstructionDAGNode *>::iterator;
  using const_pred_iterator =
      vector<LayeredInstructionDAGNode *>::const_iterator;
  using succ_iterator = vector<LayeredInstructionDAGNode *>::iterator;
  using const_succ_iterator =
      vector<LayeredInstructionDAGNode *>::const_iterator;

  pred_iterator pred_begin() { return Predecessors.begin(); }

  const_pred_iterator pred_begin() const { return Predecessors.begin(); }

  pred_iterator pred_end() { return Predecessors.end(); }

  const_pred_iterator pred_end() const { return Predecessors.end(); }

  unsigned pred_size() const { return (unsigned)Predecessors.size(); }

  bool pred_empty() const { return Predecessors.empty(); }

  succ_iterator succ_begin() { return Successors.begin(); }

  const_succ_iterator succ_begin() const { return Successors.begin(); }

  succ_iterator succ_end() { return Successors.end(); }

  const_succ_iterator succ_end() const { return Successors.end(); }

  unsigned succ_size() const { return (unsigned)Successors.size(); }

  bool succ_empty() const { return Successors.empty(); }

  inline iterator_range<pred_iterator> predecessors() {
    return make_range(pred_begin(), pred_end());
  }

  inline iterator_range<const_pred_iterator> predecessors() const {
    return make_range(pred_begin(), pred_end());
  }

  inline iterator_range<succ_iterator> successors() {
    return make_range(succ_begin(), succ_end());
  }

  inline iterator_range<const_succ_iterator> successors() const {
    return make_range(succ_begin(), succ_end());
  }

private:
  void addPredecessor(LayeredInstructionDAGNode *node) {
    Predecessors.push_back(node);
  }

  bool hasPredecessor(LayeredInstructionDAGNode *node) {
    auto it = find(Predecessors.begin(), Predecessors.end(), node);
    return it != Predecessors.end();
  }

  void addSuccessor(LayeredInstructionDAGNode *node) {
    Successors.push_back(node);
  }

  bool hasSuccessor(LayeredInstructionDAGNode *node) {
    auto it = find(Successors.begin(), Successors.end(), node);
    return it != Successors.end();
  }
};

string LayeredInstructionDAGNode::toString() {
  string str;
  char buf[8];
  switch (kind) {
  case LINSource:
    str += "source(";
    str += regToString(reg);
    str += ")";
    break;
  case LINSink:
    str += "sink(";
    str += regToString(reg);
    str += ")";
    break;
  case LINInstruction: {
    raw_string_ostream OS(str);
    MachineInstr *instr = getInstruction();
    instr->print(OS, true, false, true, false, getInstructionInfo());
  } break;
  }
  return str;
}

class LayeredInstructionDAG {
  MachineBasicBlock *MBB;
  vector<vector<LayeredInstructionDAGNode *>> levels;

  // Pool-allocate DJGraph-lifetime
  BumpPtrAllocator Allocator;

  // Allocation management for DJ Nodes in graph
  Recycler<LayeredInstructionDAGNode> LayeredInstructionDAGNodeRecycler;

public:
  LayeredInstructionDAG(MachineBasicBlock *mbb,
                        unordered_set<unsigned> const &pseudoSet,
                        unordered_set<unsigned> liveOut)
      : MBB(mbb), levels() {
    constructFromMBB(pseudoSet, liveOut);
  }

  MachineBasicBlock *getMBB() { return MBB; }

  unsigned getNumLevels() { return levels.size(); }

  vector<vector<LayeredInstructionDAGNode *>> const &getLevels() {
    return levels;
  }

  // scheduling nodes computation order
  void scheduleNodes(vector<LayeredInstructionDAGNode *> &order);

  void dump();

private:
  LayeredInstructionDAGNode *createNode(MachineInstr *instr);

  LayeredInstructionDAGNode *createNode(LayeredInstructionDAGNodeKind kind,
                                        unsigned reg);

  void LayeredInstructionDAG::addEdge(LayeredInstructionDAGNode *source,
                                      LayeredInstructionDAGNode *target);

  void constructFromMBB(unordered_set<unsigned> const &pseudoSet,
                        unordered_set<unsigned> liveOut);
};

LayeredInstructionDAGNode *
LayeredInstructionDAG::createNode(MachineInstr *instr) {
  return new (
      LayeredInstructionDAGNodeRecycler.Allocate<LayeredInstructionDAGNode>(
          Allocator)) LayeredInstructionDAGNode(instr);
}

LayeredInstructionDAGNode *
LayeredInstructionDAG::createNode(LayeredInstructionDAGNodeKind kind,
                                  unsigned reg) {
  return new (
      LayeredInstructionDAGNodeRecycler.Allocate<LayeredInstructionDAGNode>(
          Allocator)) LayeredInstructionDAGNode(kind, reg);
}

void LayeredInstructionDAG::addEdge(LayeredInstructionDAGNode *source,
                                    LayeredInstructionDAGNode *target) {
  source->addSuccessor(target);
  target->addPredecessor(source);
}

void LayeredInstructionDAG::scheduleNodes(
    vector<LayeredInstructionDAGNode *> &order) {
  // simple algorithm used node levels order
  // may be enhanced in future
  unsigned orderNum = 0;
  for (unsigned level = 0; level < levels.size(); level++) {
    vector<LayeredInstructionDAGNode *> const &lv = levels[level];
    for (unsigned pos = 0; pos < lv.size(); pos++) {
      lv[pos]->setOrderNum(orderNum++);
    }
  }
}

void LayeredInstructionDAG::constructFromMBB(
    unordered_set<unsigned> const &pseudoSet, unordered_set<unsigned> liveOut) {
  unordered_map<unsigned, LayeredInstructionDAGNode *> defMap; // reg -> node
  vector<pair<unsigned, LayeredInstructionDAGNode *>> sinkCandidates;
  vector<LayeredInstructionDAGNode *> sources;
  levels.push_back(sources);
  levels.push_back(vector<LayeredInstructionDAGNode *>());
  unsigned curLevel = 1;
  for (MachineInstr &MI : *MBB) {
    LayeredInstructionDAGNode *insnNode = createNode(&MI);
    for (const MachineOperand &op : MI.uses()) {
      if (op.isReg()) {
        unsigned reg = op.getReg();
        if (pseudoSet.find(reg) == pseudoSet.end())
          continue;
        LayeredInstructionDAGNode *def;
        auto it = defMap.find(reg);
        if (it == defMap.end()) {
          def = createNode(LINSource, reg);
          def->setLevel(0);
          defMap.insert_or_assign(reg, def);
        } else
          def = it->second;
        if (def->getLevel() == curLevel) {
          levels.push_back(vector<LayeredInstructionDAGNode *>());
          curLevel++;
        }
        addEdge(def, insnNode);
      }
    }
    insnNode->setLevel(curLevel);
    levels[curLevel].push_back(insnNode);

    for (const MachineOperand &op : MI.defs()) {
      if (op.isReg()) {
        unsigned reg = op.getReg();
        defMap.insert_or_assign(reg, insnNode);
        if (liveOut.find(reg) != liveOut.end()) {
          for (auto it = sinkCandidates.begin(); it != sinkCandidates.end();
               it++) {
            unsigned sinkReg = it->first;
            LayeredInstructionDAGNode *sink = it->second;
            if (sinkReg == reg) {
              sinkCandidates.erase(it);
              break;
            }
          }
          sinkCandidates.push_back(make_pair(reg, insnNode));
        }
      }
    }
  }

  vector<LayeredInstructionDAGNode *> sinks;
  curLevel++;
  for (auto it = sinkCandidates.begin(); it != sinkCandidates.end(); it++) {
    unsigned sinkReg = it->first;
    LayeredInstructionDAGNode *sinkInstr = it->second;
    LayeredInstructionDAGNode *sink = createNode(LINSink, sinkReg);
    addEdge(sinkInstr, sink);
    sinks.push_back(sink);
  }
  levels.push_back(sinks);
}

void LayeredInstructionDAG::dump() {
  dbgs() << "LayeredInstructionDAG: "
         << "\n";
  for (unsigned i = 0; i < getNumLevels(); i++) {
    dbgs() << "Level " << i << ": ";
    bool first = true;
    for (LayeredInstructionDAGNode *node : levels[i]) {
      assert(node->getLevel() == i);
      if (!first)
        dbgs() << ", ";
      dbgs() << node->toString();
      first = false;
    }
  }
  dbgs() << "Edges:\n";
  for (unsigned i = 0; i < getNumLevels(); i++) {
    for (LayeredInstructionDAGNode *source : levels[i]) {
      for (LayeredInstructionDAGNode *target : source->successors()) {
        dbgs() << source->toString();
        dbgs() << " -> ";
        dbgs() << target->toString();
        dbgs() << "\n";
      }
    }
  }
}

struct OperandQueueSlot {
  // Target Node
  LayeredInstructionDAGNode *target;
  // Number of input port in target node, corresponding to this slot
  unsigned targetInPort;
  // Number of uses of this slot data remained
  // if useRemain > 1, we need to copy this slot for next use instructions
  unsigned usesRemain;
};

// Model of Machine Block local stack by means of operand queue.
// The head of queue corresponds to the top of stack
class OperandQueue {
  LayeredInstructionDAG *DAG;
  vector<OperandQueueSlot> queue;

public:
  OperandQueue(LayeredInstructionDAG *dag) : DAG(dag), queue() {}

  OperandQueueSlot const &getSlot(unsigned slotNum) { return queue[slotNum]; }

  // Insert new slot into queue
  // Return insertion position of slot in queue
  unsigned enqueue(LayeredInstructionDAGNode *target, unsigned targetInPort,
                   unsigned useRem = 1);

  // Insert new slot into queue which corresponds to next instruction with the
  // same definition
  unsigned enqueueNext(OperandQueueSlot const &slot);

  // Remove num elements from queue head
  void dequeue(unsigned num);

  void dump();

private:
  inline static unsigned order(LayeredInstructionDAGNode *target,
                               unsigned targetInPort) {
    return (target->getOrderNum() << 8) + targetInPort;
  }
};

unsigned OperandQueue::enqueue(LayeredInstructionDAGNode *targ,
                               unsigned targInPort, unsigned useRem) {
  OperandQueueSlot slot;
  slot.target = targ;
  slot.targetInPort = targInPort;
  slot.usesRemain = useRem;
  unsigned ord = order(targ, targInPort);
  unsigned pos;
  for (pos = 0; pos < queue.size(); pos++) {
    OperandQueueSlot const &posSlot = getSlot(pos);
    if (order(posSlot.target, posSlot.targetInPort) > ord)
      break;
  }
  // TODO
  return pos;
}

unsigned OperandQueue::enqueueNext(OperandQueueSlot const &slot) {
  // TODO
  return 0;
}

void OperandQueue::dequeue(unsigned num) {
  // TODO
}

void OperandQueue::dump() {
  // TODO
}

enum DJEdgeKind { Dominance, Join };

class DJNode {
  friend class DJGraph;

  MachineBasicBlock *MBB;
  int level;
  vector<pair<DJNode *, DJEdgeKind>> Predecessors;
  vector<pair<DJNode *, DJEdgeKind>> Successors;
  BlockQueueInfo queueInfo;

private:
  DJNode(MachineBasicBlock *mbb)
      : MBB(mbb), level(-1), Predecessors(), Successors(), queueInfo() {}

public:
  MachineBasicBlock &getMBB() { return *MBB; }

  int getLevel() { return level; }

  void setLevel(int lev) { level = lev; }

  BlockQueueInfo &getBlockQueueInfo() { return queueInfo; }

  bool hasPredecessor(DJNode *node, DJEdgeKind kind);

  bool hasSuccessor(DJNode *node, DJEdgeKind kind);

  bool isImmediatelyDominates(DJNode *node) {
    return hasSuccessor(node, Dominance);
  }

  bool isDominates(DJNode *node);

  // Calculate fixup on block input
  StackFixup calculateInputFixup(MachineFunction &MF) {
    return getBlockQueueInfo().calculateInputFixup(MF);
  }

  // Calculate fixup on block output
  StackFixup calculateOutputFixup(MachineFunction &MF) {
    return getBlockQueueInfo().calculateOutputFixup(MF);
  }

  string toString();

  // Machine-DJ Graph iterators
  using pred_iterator = vector<pair<DJNode *, DJEdgeKind>>::iterator;
  using const_pred_iterator =
      vector<pair<DJNode *, DJEdgeKind>>::const_iterator;
  using succ_iterator = vector<pair<DJNode *, DJEdgeKind>>::iterator;
  using const_succ_iterator =
      vector<pair<DJNode *, DJEdgeKind>>::const_iterator;

  pred_iterator pred_begin() { return Predecessors.begin(); }

  const_pred_iterator pred_begin() const { return Predecessors.begin(); }

  pred_iterator pred_end() { return Predecessors.end(); }

  const_pred_iterator pred_end() const { return Predecessors.end(); }

  unsigned pred_size() const { return (unsigned)Predecessors.size(); }

  bool pred_empty() const { return Predecessors.empty(); }

  succ_iterator succ_begin() { return Successors.begin(); }

  const_succ_iterator succ_begin() const { return Successors.begin(); }

  succ_iterator succ_end() { return Successors.end(); }

  const_succ_iterator succ_end() const { return Successors.end(); }

  unsigned succ_size() const { return (unsigned)Successors.size(); }

  bool succ_empty() const { return Successors.empty(); }

  inline iterator_range<pred_iterator> predecessors() {
    return make_range(pred_begin(), pred_end());
  }

  inline iterator_range<const_pred_iterator> predecessors() const {
    return make_range(pred_begin(), pred_end());
  }

  inline iterator_range<succ_iterator> successors() {
    return make_range(succ_begin(), succ_end());
  }

  inline iterator_range<const_succ_iterator> successors() const {
    return make_range(succ_begin(), succ_end());
  }

private:
  void addPredecessor(DJNode *pred, DJEdgeKind kind) {
    Predecessors.push_back(make_pair(pred, kind));
  }

  void addSuccessor(DJNode *pred, DJEdgeKind kind) {
    Successors.push_back(make_pair(pred, kind));
  }
};

bool DJNode::hasPredecessor(DJNode *node, DJEdgeKind kind) {
  for (pair<DJNode *, DJEdgeKind> pred : predecessors()) {
    if (pred.first == this && pred.second == kind)
      return true;
  }
  return false;
}

bool DJNode::hasSuccessor(DJNode *node, DJEdgeKind kind) {
  for (pair<DJNode *, DJEdgeKind> succ : successors()) {
    if (succ.first == node && succ.second == kind)
      return true;
  }
  return false;
}

bool DJNode::isDominates(DJNode *node) {
  if (isImmediatelyDominates(node))
    return true;
  for (pair<DJNode *, DJEdgeKind> succ : successors()) {
    if (succ.second == Dominance && succ.first->isDominates(node))
      return true;
  }
  return false;
}

string DJNode::toString() {
  //  MCSymbol *sym = getMBB().getSymbol();
  //  return sym->getName().str();
  return blockToString(&getMBB());
}

class DJGraph {
  // Dominator levels
  vector<vector<DJNode *>> levels;
  map<const MachineBasicBlock *, DJNode *> nodeMap;
  vector<DJNode *> nodes;
  DJNode *root;

  // Pool-allocate DJGraph-lifetime
  BumpPtrAllocator Allocator;

  // Allocation management for DJ Nodes in graph
  Recycler<DJNode> DJNodeRecycler;

public:
  DJGraph(MachineFunction &MF, MachineDominatorTree &MDT) : levels() {
    init(MF, MDT);
  }

  DJNode *getRoot() { return root; }

  unsigned getNumLevels() { return levels.size(); }

  vector<vector<DJNode *>> const &getLevels() { return levels; }

  // Lookup DJ Node corresponding to Machine Block MBB
  DJNode *lookupNode(const MachineBasicBlock *MBB) {
    auto it = nodeMap.find(MBB);
    if (it == nodeMap.end())
      return nullptr;
    return it->second;
  }

  void dump();

  using iterator = vector<DJNode *>::iterator;
  using const_iterator = vector<DJNode *>::const_iterator;

  iterator begin() { return nodes.begin(); }

  const_iterator begin() const { return nodes.begin(); }

  iterator end() { return nodes.end(); }

  const_iterator end() const { return nodes.end(); }

  iterator level_begin(unsigned level) { return levels[level].begin(); }

  const_iterator level_begin(unsigned level) const {
    return levels[level].begin();
  }

  iterator level_end(unsigned level) { return levels[level].end(); }

  const_iterator level_end(unsigned level) const { return levels[level].end(); }

private:
  void init(MachineFunction &MF, MachineDominatorTree &MDT);

  DJNode *createDJNode(MachineBasicBlock *MBB);

  void addEdge(DJNode *source, DJNode *target, DJEdgeKind kind);

  void walkDomTree(MachineDomTreeNode *domTreeNode, int level);
};

void DJGraph::init(MachineFunction &MF, MachineDominatorTree &MDT) {
  for (MachineBasicBlock &MBB : MF) {
    DJNode *djNode = createDJNode(&MBB);
    nodes.push_back(djNode);
    nodeMap[&MBB] = djNode;
  }
  walkDomTree(MDT.getRootNode(), 0);

  for (MachineBasicBlock &source : MF) {
    for (MachineBasicBlock *target : source.successors()) {
      DJNode *sourceNode = nodeMap[&source];
      DJNode *targetNode = nodeMap[target];
      //      dbgs() << sourceNode->toString() << " -> " <<
      //      targetNode->toString() << "\n";
      if (sourceNode->isDominates(targetNode))
        continue;
      addEdge(sourceNode, targetNode, Join);
    }
  }
}

DJNode *DJGraph::createDJNode(MachineBasicBlock *MBB) {
  return new (DJNodeRecycler.Allocate<DJNode>(Allocator)) DJNode(MBB);
}

void DJGraph::walkDomTree(MachineDomTreeNode *domTreeNode, int level) {
  if (levels.size() <= level)
    levels.push_back(vector<DJNode *>());
  const MachineBasicBlock *MBB = domTreeNode->getBlock();
  DJNode *node = lookupNode(MBB);
  assert(node);
  node->setLevel(level);
  levels[level].push_back(node);
  for (MachineDomTreeNode *child : domTreeNode->getChildren()) {
    DJNode *target = nodeMap.find(child->getBlock())->second;
    addEdge(node, target, Dominance);
    walkDomTree(child, level + 1);
  }
}

void DJGraph::addEdge(DJNode *source, DJNode *target, DJEdgeKind kind) {
  source->addSuccessor(target, kind);
  target->addPredecessor(source, kind);
}

void DJGraph::dump() {
  dbgs() << "DJGraph: "
         << "\n";
  for (unsigned i = 0; i < getNumLevels(); i++) {
    dbgs() << "Level " << i << ": ";
    bool first = true;
    for (DJNode *node : levels[i]) {
      assert(node->getLevel() == i);
      if (!first)
        dbgs() << ", ";
      dbgs() << node->toString();
      first = false;
    }
    dbgs() << "\n";
  }
  dbgs() << "Dominant Edges:\n";
  for (unsigned i = 0; i < getNumLevels(); i++) {
    for (DJNode *source : levels[i]) {
      for (pair<DJNode *, DJEdgeKind> target : source->successors()) {
        if (target.second == Dominance) {
          dbgs() << source->toString();
          dbgs() << " -> ";
          dbgs() << target.first->toString();
          dbgs() << "\n";
        }
      }
    }
  }
  dbgs() << "Join Edges:\n";
  for (unsigned i = 0; i < getNumLevels(); i++) {
    for (DJNode *source : levels[i]) {
      for (pair<DJNode *, DJEdgeKind> target : source->successors()) {
        if (target.second == Join) {
          dbgs() << source->toString();
          dbgs() << " -> ";
          dbgs() << target.first->toString();
          dbgs() << "\n";
        }
      }
    }
  }
}

class StackTreeNode {
	friend class StackTree;

	StackTreeNode *parent;
	vector<StackTreeNode *> children;
	vector<unsigned> registers;
	MachineBasicBlock *MBB;

private:
  StackTreeNode() : parent(nullptr), children() {}

	void setParent(StackTreeNode *parent_) {
		parent = parent;
    parent->children.push_back(this);
	}

public:
  StackTreeNode *getParent() { return parent; }

	MachineBasicBlock *getMBB() { return MBB; }

	unsigned size() { 
		unsigned sz = registers.size();
    if (parent)
      sz += parent->size();
		return sz;
	}

	unsigned localSize() { return registers.size(); }

	// Get path from root node to this node
	void getPath(vector<StackTreeNode *> &path) { 
		if (parent)
      parent->getPath(path);
    path.push_back(this);
	}

	// Get stack registers from root node to this node
	void getContents(vector<unsigned> &regs) { 
		if (parent)
      parent->getContents(regs);
    for (unsigned reg : registers) {
       regs.push_back(reg);
    }
	}

	// Lookup register in current node or parent (recursively).
	// Return register offset from the top of stack or -1
	// if register is not in stack
	int lookupRegisterOffset(unsigned reg);

	string toString();

	void dump(string margin = "");

  // StackTreeNode children iterator
  using child_iterator = vector<StackTreeNode *>::iterator;
  using const_child_iterator = vector<StackTreeNode *>::const_iterator;

  child_iterator children_begin() { return children.begin(); }

  const_child_iterator children_begin() const { return children.begin(); }

  child_iterator children_end() { return children.end(); }

	const_child_iterator children_end() const { return children.end(); }
};

int StackTreeNode::lookupRegisterOffset(unsigned reg) {
  for (int index = registers.size() - 1; index >= 0; index--) {
    if (registers[index] == reg)
      return registers.size() - 1 - index;
	}
  if (!parent)
		return -1;
  int offset = parent->lookupRegisterOffset(reg);
  if (offset >= 0)
    return registers.size() + offset;
  return -1; 
}

string StackTreeNode::toString() {
	string str;
  if (parent)
    str += parent->toString();
  for (unsigned reg : registers) {
    str += regToString(reg);
    str += " ";
  }
	return str;
}

void StackTreeNode::dump(string margin) { 
	dbgs() << margin;
  dbgs() << blockToString(getMBB()) << " : ";
  dbgs() << "{ " << toString() << "}";
  for (StackTreeNode *child : children) {
    dump(margin + " ");
  }
}

// Flexible Stack Model.
// Stack Model structure corresponds to function's Region Structure.
// Path from root to some node corresponds to this node's stack state.
class StackTree {
	StackTreeNode *root;
	map<MachineBasicBlock *, StackTreeNode *> nodeMap;

  // Pool-allocate StackableTreeNode-lifetime
  BumpPtrAllocator Allocator;

  // Allocation management for StackableRegion in graph
  Recycler<StackTreeNode> StackTreeNodeRecycler;

public:
  StackTree() {}

	StackTreeNode *getRoot() { return root; }

	StackTreeNode *lookupNode(MachineBasicBlock *MBB) { return nodeMap[MBB]; }

	void dump() {
    if (getRoot())
      getRoot()->dump();
    else dbgs() << "null";
  }

	StackTreeNode *createNode() {
    return new (StackTreeNodeRecycler.Allocate<StackTreeNode>(
      Allocator)) StackTreeNode();
	}
};

enum StackableRegionKind {
  SRLeaf,        // Leaf region (contains single Machine Block)
  SRSequence,    // Sequence of regions
  SRIfThen,      // if (...) { ... }
  SRIfThenElse,  // if (...)  { ... } else { ... }
  SRSelfLoop,    // do { ... } while (...)
  SRWhileLoop,   // while (...) { ... }
  SRNaturalLoop, // TODO can't be generated by current backend passes (?)
  SRProper,      // TODO can't be generated from structural C/C++ programs
  SRImproper     // TODO can't be generated from structural C/C++ programs
};

// Constants used in approximation of region weights
#define LOOP_REGION_WEIGHT 8
#define LEVEL_REGION_WEIGHT 1

// Class StackableRegion represents different elements of program's control
// structure. Resembles regions of program structure analysis (if/else, while,
// etc.) but with concerning to stack calculations.
class StackableRegion {
  friend class StackableRegionStructure;

  StackableRegionKind kind;
  MachineBasicBlock *MBB;
  vector<StackableRegion *> subregions;
  BlockQueueInfo queueInfo;
	uint64_t weight;

private:
  StackableRegion(StackableRegionKind kind_)
      : kind(kind_), subregions(), queueInfo(), weight(1) {}

  StackableRegion(StackableRegionKind kind_, MachineBasicBlock *MBB_)
      : kind(kind_), MBB(MBB_), subregions(), queueInfo(), weight(1) {}

  StackableRegion(StackableRegionKind kind_, StackableRegion *child1)
      : kind(kind_), MBB(nullptr), subregions(), queueInfo(), weight(1) {
    subregions.push_back(child1);
  }

  StackableRegion(StackableRegionKind kind_, StackableRegion *child1,
                  StackableRegion *child2)
      : kind(kind_), MBB(nullptr), subregions(), queueInfo(), weight(1) {
    subregions.push_back(child1);
    subregions.push_back(child2);
  }

  StackableRegion(StackableRegionKind kind_, StackableRegion *child1,
                  StackableRegion *child2, StackableRegion *child3)
      : kind(kind_), MBB(nullptr), subregions(), queueInfo(), weight(1) {
    subregions.push_back(child1);
    subregions.push_back(child2);
    subregions.push_back(child3);
  }

  StackableRegion(StackableRegionKind kind_,
                  vector<StackableRegion *> &children)
      : kind(kind_), MBB(nullptr), subregions(children), queueInfo(),
        weight(1) {}

public:
  StackableRegionKind getKind() { return kind; }

	uint64_t getWeight() { return weight; }

  MachineBasicBlock *getMBB() {
    assert(kind == SRLeaf);
    return MBB;
  }

	StackableRegion *getCondition() {
		assert(kind == SRIfThen  ||  kind == SRIfThenElse || kind == SRWhileLoop || kind == SRNaturalLoop);
    return subregions[0];
	}

	StackableRegion *getThen() {
    assert(kind == SRIfThen || kind == SRIfThenElse);
    return subregions[1];
  }

  StackableRegion *getElse() {
    assert(kind == SRIfThenElse);
    return subregions[2];
  }

  StackableRegion *getBody() {
    assert(kind == SRSelfLoop || kind == SRWhileLoop || kind == SRNaturalLoop);
    if (kind == SRSelfLoop)
      return subregions[0];
    else return subregions[1];
  }

  BlockQueueInfo &getQueueInfo() { return queueInfo; }

	bool isLoop() {
		return kind == SRSelfLoop || kind == SRWhileLoop || kind == SRNaturalLoop;
	}

	// Returns head block - Machine Block on the entrance of region
	MachineBasicBlock *getHeadBlock();

	// Returns tail block - Machine Block on the exit of region.
	// If there is no single tail block, returns nullptr
	MachineBasicBlock *getTailBlock();

  // StackableRegion children iterator
  using child_iterator = vector<StackableRegion *>::iterator;
  using const_child_iterator = vector<StackableRegion *>::const_iterator;

  child_iterator begin() { return subregions.begin(); }

  const_child_iterator begin() const { return subregions.begin(); }

  child_iterator end() { return subregions.end(); }

  const_child_iterator end() const { return subregions.end(); }

  static string toString(StackableRegionKind kind);

  string toString();

  void dump(string margin);

private:
  // Recursively calculate (approximately) region and its children weights
  void calculateWeights(uint64_t weight_ = 1);

  // Recursively calculate region's BlockQueueInfo
  void calculateQueueInfo(MachineRegisterInfo *MRI, LiveIntervals *LIS,
                     unordered_map<unsigned, MachineInstr *> const &pseudoMap);
};

// Structure of Stackable Regions.
// Contains region allocation and pointer to root region.
class StackableRegionStructure {
  DJGraph djGraph;
  StackableRegion *root;

  // Pool-allocate StackableRegionStructure-lifetime
  BumpPtrAllocator Allocator;

  // Allocation management for StackableRegion in graph
  Recycler<StackableRegion> StackableRegionRecycler;

public:
  StackableRegionStructure(MachineFunction &MF, MachineDominatorTree &MDT)
      : djGraph(MF, MDT) {
    init(MF);
  }

  StackableRegion &getRoot() { return *root; }

  DJGraph &getDJGraph() { return djGraph; }

  // Recursively calculate region's BlockQueueInfo
  void calculateQueueInfo(MachineRegisterInfo *MRI, LiveIntervals *LIS,
                     unordered_map<unsigned, MachineInstr *> const &pseudoMap);

  void dump();

private:
  StackableRegion *createLeaf(MachineBasicBlock *MBB) {
    return new (StackableRegionRecycler.Allocate<StackableRegion>(Allocator))
        StackableRegion(SRLeaf, MBB);
  }

  StackableRegion *createSequence(vector<StackableRegion *> &children) {
    return new (StackableRegionRecycler.Allocate<StackableRegion>(Allocator))
        StackableRegion(SRSequence, children);
  }

  StackableRegion *createSequence(StackableRegion *child1,
                                  StackableRegion *child2);

  StackableRegion *createIfThen(StackableRegion *condition,
                                StackableRegion *ifRegion) {
    return new (StackableRegionRecycler.Allocate<StackableRegion>(Allocator))
        StackableRegion(SRIfThen, condition, ifRegion);
  }

  StackableRegion *createIfThenElse(StackableRegion *condition,
                                    StackableRegion *ifRegion,
                                    StackableRegion *elseRegion) {
    return new (StackableRegionRecycler.Allocate<StackableRegion>(Allocator))
        StackableRegion(SRIfThenElse, condition, ifRegion, elseRegion);
  }

  StackableRegion *createSelfLoop(StackableRegion *body) {
    return new (StackableRegionRecycler.Allocate<StackableRegion>(Allocator))
        StackableRegion(SRSelfLoop, body);
  }

  StackableRegion *createWhileLoop(StackableRegion *condition,
                                   StackableRegion *body) {
    return new (StackableRegionRecycler.Allocate<StackableRegion>(Allocator))
        StackableRegion(SRWhileLoop, condition, body);
  }

  void init(MachineFunction &MF);

  StackableRegion *processNode(DJNode *node,
                               map<DJNode *, StackableRegion *> &nodeMap,
                               map<DJNode *, vector<DJNode *>> &succJoins);

  StackableRegion *matchSelfLoop(DJNode *node,
                                 map<DJNode *, StackableRegion *> &nodeMap,
                                 map<DJNode *, vector<DJNode *>> &succJoins);

  StackableRegion *matchSequence(DJNode *node,
                                 map<DJNode *, StackableRegion *> &nodeMap,
                                 map<DJNode *, vector<DJNode *>> &succJoins);

  void addJoinEdges(DJNode *node, map<DJNode *, vector<DJNode *>> &succJoins);
};

StackableRegion *StackableRegionStructure::createSequence(StackableRegion *child1,
                                         StackableRegion *child2) {
  if (child1->getKind() != SRSequence && child2->getKind() != SRSequence) {
    return new (StackableRegionRecycler.Allocate<StackableRegion>(Allocator))
        StackableRegion(SRSequence, child1, child2);
  } else {
    vector<StackableRegion *> children;
    if (child1->getKind() == SRSequence) {
      for (StackableRegion *child : *child1) {
        children.push_back(child);
      }
    } else
      children.push_back(child1);
    if (child2->getKind() == SRSequence) {
      for (StackableRegion *child : *child2) {
        children.push_back(child);
      }
    } else
      children.push_back(child1);
    return new (StackableRegionRecycler.Allocate<StackableRegion>(Allocator))
        StackableRegion(SRSequence, children);
  }
}

void StackableRegionStructure::init(MachineFunction &MF) {
  printCFGEdges(MF);
  dbgs() << "\n";
  djGraph.dump();
  dbgs() << "\n";

  map<DJNode *, StackableRegion *> nodeMap;
  // Create Leaf regions and add them to nodeMap
  for (DJNode *node : djGraph) {
    StackableRegion *leaf = createLeaf(&node->getMBB());
    nodeMap[node] = leaf;
  }

  // Create joint edges for initial leaf regions
  map<DJNode *, vector<DJNode *>> succJoins;
  for (DJNode *node : djGraph) {
    vector<DJNode *> joinList;
    for (auto it = node->succ_begin(); it != node->succ_end(); it++) {
      pair<DJNode *, DJEdgeKind> p = *it;
      if (p.second != Join)
        continue;
      DJNode *source = p.first;
      joinList.push_back(source);
    }
    succJoins[node] = joinList;
  }

  for (int level = djGraph.getNumLevels() - 2; level >= 0; level--) {
    for (auto it = djGraph.level_begin(level); it != djGraph.level_end(level);
         it++) {
      DJNode *node = *it;
      processNode(node, nodeMap, succJoins);
    }
  }
  root = nodeMap[*djGraph.level_begin(0)];
}

StackableRegion *StackableRegionStructure::processNode(
    DJNode *node, map<DJNode *, StackableRegion *> &nodeMap,
    map<DJNode *, vector<DJNode *>> &succJoins) {
  dbgs() << "Node " << node->toString() << "\n";

  // CFG Exit node
  if (node->succ_begin() == node->succ_end())
    return nullptr;

  StackableRegion *newRegion = nullptr;
  for (auto it = node->succ_begin(); it != node->succ_end(); it++) {
    pair<DJNode *, DJEdgeKind> p = *it;
    if (p.second != Dominance)
      continue;
    DJNode *node2 = p.first;
    matchSelfLoop(node2, nodeMap, succJoins);
  }
  matchSelfLoop(node, nodeMap, succJoins);

  newRegion = matchSequence(node, nodeMap, succJoins);
  if (newRegion)
    return newRegion;

  vector<DJNode *> joinSources;
  DJNode *join = nullptr;

  // Detect fork regions (if-then / if-then-else / switch)
  for (auto it = node->succ_begin(); it != node->succ_end(); it++) {
    pair<DJNode *, DJEdgeKind> p = *it;
    if (p.second != Dominance)
      continue;
    DJNode *node2 = p.first;
    vector<DJNode *> &joinTargets = succJoins[node2];
    int targNum = 0;
    for (DJNode *target : joinTargets) {
      //      dbgs() << node2->toString() << " -> " << target->toString() <<
      //      "\n";
      if (target == node2) {
        joinSources.clear();
        break;
      }
      if (target->getLevel() > node2->getLevel())
        continue;
      if (++targNum > 1) {
        joinSources.clear();
        break;
      }
      joinSources.push_back(node2);
      if (join && join != target) {
        joinSources.clear();
        break;
      }
      join = target;
    }
  }
  if (joinSources.size() > 0) {
    StackableRegion *condRegion = nullptr;
    if (joinSources.size() == 1)
      condRegion = createIfThen(nodeMap[node], nodeMap[joinSources[0]]);
    else if (joinSources.size() == 2)
      condRegion = createIfThenElse(nodeMap[node], nodeMap[joinSources[0]],
                                    nodeMap[joinSources[1]]);
    else {
      // TODO create switch
    }
    assert(condRegion);
    if (join->getLevel() > node->getLevel())
      newRegion = createSequence(condRegion, nodeMap[join]);
    else
      newRegion = condRegion;
  }

  if (!newRegion) {
  }

  assert(newRegion);
  nodeMap[node] = newRegion;
  // Add join edges for new region
  addJoinEdges(node, succJoins);
  return newRegion;
}

StackableRegion *StackableRegionStructure::matchSequence(
    DJNode *node, map<DJNode *, StackableRegion *> &nodeMap,
    map<DJNode *, vector<DJNode *>> &succJoins) {
  // If node has one child without any join edges, create Sequence
  DJNode *seqNode = nullptr;
  for (auto it = node->succ_begin(); it != node->succ_end(); it++) {
    pair<DJNode *, DJEdgeKind> p = *it;
    if (p.second != Dominance)
      continue;
    if (seqNode)
      return nullptr;
    seqNode = p.first;
    vector<DJNode *> &joinTargets = succJoins[seqNode];
    for (DJNode *target : joinTargets) {
      if (target->getLevel() < seqNode->getLevel())
        return nullptr;
    }
  }
  if (seqNode) {
    StackableRegion *seq = createSequence(nodeMap[node], nodeMap[seqNode]);
    nodeMap[node] = seq;
    return seq;
  }
  return nullptr;
}

StackableRegion *StackableRegionStructure::matchSelfLoop(
    DJNode *node, map<DJNode *, StackableRegion *> &nodeMap,
    map<DJNode *, vector<DJNode *>> &succJoins) {
  StackableRegion *region = nodeMap[node];
  if (region->getKind() == SRSelfLoop)
    return region;
  // At first detect self-loops on level under node
  bool selfLoop = false;
  vector<DJNode *> &joins = succJoins[node];
  for (DJNode *join : joins) {
    if (join == node)
      selfLoop = true;
  }
  if (selfLoop) {
    StackableRegion *selfLoop = createSelfLoop(nodeMap[node]);
    nodeMap[node] = selfLoop;
    joins.erase(find(joins.begin(), joins.end(), node));
    return selfLoop;
  }
  return nullptr;
}

void StackableRegionStructure::addJoinEdges(
    DJNode *node, map<DJNode *, vector<DJNode *>> &succJoins) {
  vector<DJNode *> &nodeJoins = succJoins[node];
  for (auto it = node->succ_begin(); it != node->succ_end(); it++) {
    pair<DJNode *, DJEdgeKind> p = *it;
    if (p.second != Dominance)
      continue;
    DJNode *node2 = p.first;
    vector<DJNode *> &joinTargets = succJoins[node2];
    for (DJNode *target : joinTargets) {
      if (target->getLevel() <= node->getLevel() &&
          find(nodeJoins.begin(), nodeJoins.end(), target) == nodeJoins.end()) {
        nodeJoins.push_back(target);
      }
    }
  }
}

void StackableRegionStructure::calculateQueueInfo(MachineRegisterInfo *MRI, 
	  LiveIntervals *LIS,  unordered_map<unsigned, MachineInstr *> const &pseudoMap) {
  getRoot().calculateWeights();
  getRoot().calculateQueueInfo(MRI, LIS, pseudoMap);
}

void StackableRegionStructure::dump() {
  dbgs() << "Region Tree:\n";
  getRoot().dump("");
}

MachineBasicBlock *StackableRegion::getHeadBlock() { 
	if (kind == SRLeaf) 
		return getMBB();
  return subregions[0]->getHeadBlock();
}

MachineBasicBlock *StackableRegion::getTailBlock() { 
	switch (kind) {
  case SRLeaf:
    return getMBB();
  case SRSelfLoop:
    return getBody()->getTailBlock();
  case SRWhileLoop:
    return getCondition()->getTailBlock();
  default: return nullptr;
	}
}

void StackableRegion::calculateWeights(uint64_t weight_) {
  switch (kind) {
  case SRLeaf:
		weight = weight_;
		break;
  case SRSequence:
		weight = weight_;
    for (StackableRegion *child : *this) {
      calculateWeights(weight);
		}
		break;
  case SRIfThen:
  case SRIfThenElse:
    weight = weight_;
    getCondition()->calculateWeights(weight);
    getThen()->calculateWeights(weight * LEVEL_REGION_WEIGHT);
    if (kind == SRIfThenElse)
      getElse()->calculateWeights(weight * LEVEL_REGION_WEIGHT);
		break;
  case SRSelfLoop:
  case SRWhileLoop:
  case SRNaturalLoop:
		weight = weight_ * LOOP_REGION_WEIGHT;
    for (StackableRegion *child : *this) {
      calculateWeights(weight);
    }
		break;
  case SRProper:
  case SRImproper:
  default:
    assert(false);
	}
}

void StackableRegion::calculateQueueInfo(MachineRegisterInfo *MRI, LiveIntervals *LIS,
	  unordered_map<unsigned, MachineInstr *> const &pseudoMap) {
  for (StackableRegion *child : *this) {
    child->calculateQueueInfo(MRI, LIS, pseudoMap);
	}
  switch (kind) {
  case SRLeaf:
    SchedulerUtils::gatherBlockQueueInfo(*getMBB(), MRI, LIS, pseudoMap, queueInfo);
    break;
  case SRSequence:
 
		break;
  case SRIfThen:

		break;
  case SRIfThenElse:

    break;
  case SRSelfLoop:

		break;
  case SRWhileLoop:

		break;
  case SRNaturalLoop:

    break;
  case SRProper:
  case SRImproper:
  default:
    assert(false);
  }
}

string StackableRegion::toString(StackableRegionKind kind) {
  const char *str;
  switch (kind) {
  case SRLeaf:
    str = "Leaf";
    break;
  case SRSequence:
    str = "Sequence";
    break;
  case SRIfThen:
    str = "If Then";
    break;
  case SRIfThenElse:
    str = "If Then Else";
    break;
  case SRSelfLoop:
    str = "Self Loop";
    break;
  case SRWhileLoop:
    str = "While Loop";
    break;
  case SRNaturalLoop:
    str = "Natural Loop";
    break;
  case SRProper:
    str = "Proper";
    break;
  case SRImproper:
    str = "Improper";
    break;
  default:
    str = "???";
  }
  return string(str);
}

string StackableRegion::toString() {
  string str = toString(getKind());
  switch (kind) {
  case SRLeaf:
    str += " ";
    str += getMBB()->getName();
    break;
  case SRSequence:
  case SRIfThen:
  case SRIfThenElse:
  case SRSelfLoop:
  case SRWhileLoop:
  case SRNaturalLoop:
  case SRProper:
  case SRImproper:
  default:
    break;
  }
  return str;
}

void StackableRegion::dump(string margin) {
  dbgs() << margin << toString() << "\n";
  for (StackableRegion *child : *this) {
    child->dump(margin + "  ");
  }
}

namespace {
class TVMLocalScheduler final : public MachineFunctionPass {
  StringRef getPassName() const override { return "TVM Local Scheduler"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<LiveIntervals>();
    AU.addPreservedID(LiveVariablesID);
    AU.addRequired<MachineLoopInfo>();
    AU.addPreserved<MachineLoopInfo>();
    AU.addRequired<MachineDominatorTree>();
    AU.addPreserved<MachineDominatorTree>();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

public:
  static char ID; // Pass identification, replacement for typeid
  TVMLocalScheduler() : MachineFunctionPass(ID) {}

private:
  bool runOnBasicBlocks(MachineFunction &MF);

  //  void createTreeGraph(MachineBasicBlock &MBB);

  MachineRegisterInfo *MRI;
  LiveIntervals *LIS;
  MachineDominatorTree *MDT;
};
} // end anonymous namespace

char TVMLocalScheduler::ID = 0;
INITIALIZE_PASS(TVMLocalScheduler, DEBUG_TYPE,
                "Schedule instructions within a blocks", false, false)

FunctionPass *llvm::createTVMLocalScheduler() {
  return new TVMLocalScheduler();
}

bool TVMLocalScheduler::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(dbgs() << "********** Local Scheduler **********\n"
                       "********** Function: "
                    << MF.getName() << '\n');

  bool changed = false;

  MRI = &MF.getRegInfo();
  LIS = &getAnalysis<LiveIntervals>();
  MDT = &getAnalysis<MachineDominatorTree>();

  changed |= runOnBasicBlocks(MF);

  return changed;
}

bool TVMLocalScheduler::runOnBasicBlocks(MachineFunction &MF) {
  bool changed = false;

  unordered_map<unsigned, MachineInstr *> pseudoMap;
  SchedulerUtils::gatherPseudoGlobal(MF, pseudoMap);
  /*
  dbgs() << "Pseudo Globals : ";
  for (auto it = pseudoMap.begin(); it != pseudoMap.end(); it++) {
    dbgs() << "%" << (it->first & 0x7FFF) << " ";
        }
  dbgs() << "\n";
  */

  //  DJGraph djGraph(MF, *MDT);
  //  djGraph.dump();

  StackableRegionStructure SRS(MF, *MDT);
  // SRS.getDJGraph().dump();
  // dbgs() << "\n";
  SRS.dump();
  dbgs() << "\n";

  for (DJNode *node : SRS.getDJGraph()) {
    SchedulerUtils::gatherBlockQueueInfo(node->getMBB(), MRI, LIS, pseudoMap,
                                         node->getBlockQueueInfo());
  }

  /*
auto &firstBB = MF.front();

using RPOTType = ReversePostOrderTraversal<MachineFunction *>;
RPOTType RPOT(&MF);

for (RPOTType::rpo_iterator I = RPOT.begin(), E = RPOT.end(); I != E;
  ++I) {
MachineBasicBlock *MBB = *I;

createTreeGraph(*MBB);
}
  */
  return changed;
}

/*
void TVMLocalScheduler::createTreeGraph(MachineBasicBlock & MBB) {
  int size = MBB.size();
  if (size == 0)
    return;
  cout << "Block size = " << size << endl;

  for (MachineInstr &MI : MBB) {

    //			assert(!TVM::isArgument(MI));

    if (MI.isDebugInstr() || MI.isLabel())
      continue;

    MI.dump();
  }
}
*/

void BlockQueueInfo::dump() {
  dbgs() << "Preferred Input Queue: "
         << regListToString(preferredInputQueue).c_str() << "\n"
         << "Preferred Output Queue: "
         << regListToString(preferredOutputQueue).c_str() << "\n"
         << "Actual Input Queue: " << regListToString(actualInputQueue).c_str()
         << "\n"
         << "Actual Output Queue: " << regListToString(actualInputQueue).c_str()
         << "\n"
         << "Number Of Uses :" << regMapToString(numUses);

  dbgs() << "\n";
}

// Suboptimal algorithm
// Set register position using minimum weighted sum with some parameters:
// k1 * length(queue)
// k2 * abs(dist(queuePos, mergePos))
// k3 - if there is no such register in queue
void BlockQueueInfo::calculateOptimizedMerge(
    vector<vector<unsigned>> const &queues,
    vector<unsigned> const &queueWeights, vector<unsigned> merge) {
  unsigned k1 = 2;
  unsigned k2 = 2;
  unsigned k3 = 1;
  unordered_set<unsigned> allRegs;
  for (vector<unsigned> const &it1 : queues) {
    for (unsigned reg : it1) {
      if (allRegs.find(reg) == allRegs.end())
        allRegs.insert(reg);
    }
  }

  while (allRegs.size() > 1) {
    unsigned idx = merge.size();
    unsigned optimalReg = 0;
    unsigned minimumWeight = 0x7FFFFFFF;
    for (unsigned reg : allRegs) {
      for (unsigned qidx = 0; qidx < queues.size(); qidx++) {
        vector<unsigned> const &queue = queues[qidx];
        unsigned queueWeight = queueWeights[qidx];
        unsigned queueIdx = indexOf(queue, reg);
        unsigned weight = k1 * queue.size();
        if (queueIdx < queue.size()) {
          if (queueIdx > idx)
            weight += k2 * (queueIdx - idx);
          else
            weight += k2 * (idx - queueIdx);
        } else
          queueWeight += k3;
        weight *= queueWeight;
        if (weight < minimumWeight) {
          optimalReg = reg;
          minimumWeight = weight;
        }
      }
    }
    merge.push_back(optimalReg);
    allRegs.erase(optimalReg);
  }
  merge.push_back((unsigned)*allRegs.begin());
}

void BlockQueueInfo::calculateOptimizedMerge(
    vector<vector<unsigned>> const &queues, vector<unsigned> merge) {
  vector<unsigned> queueWeights;
  for (unsigned i = 0; i < queues.size(); i++)
    queueWeights.push_back(1);
  calculateOptimizedMerge(queues, queueWeights, merge);
}

StackFixup BlockQueueInfo::calculateFixup(MachineFunction &MF,
                                          vector<unsigned> const &dst,
                                          vector<unsigned> const &src) {
  unsigned size = max(src.size(), dst.size());
  // We create two dummy stacks and than find a fixup using
  // function from StackModel
  Stack srcStack(MF, size);
  Stack dstStack(MF, size);
  for (unsigned i = 0; i < size; i++) {
    if (i < src.size())
      srcStack.set(i, StackVreg(src[i]));
    else // fill with dummy registers
      srcStack.set(i, StackVreg(0));
  }
  for (unsigned i = 0; i < size; i++) {
    if (i < dst.size())
      dstStack.set(i, StackVreg(dst[i]));
    else // fill with dummy registers
      dstStack.set(i, StackVreg(0));
  }
  return StackFixup::Diff(dstStack, srcStack);
}

unsigned BlockQueueInfo::calculateCommonTailLength(vector<vector<unsigned>> const &queues) {
	unsigned cnt = 0;
  for (;;) {
    for (vector<unsigned> const  &queue : queues) {
			unsigned val = (unsigned) -1;
      if (queue.size() == cnt)
				return cnt;
      unsigned queueVal = queue[queue.size() - 1 - cnt];
      if (val == (unsigned) -1)
        val = queueVal;
      else if (queueVal != val)
				return cnt;
    }
	}
	return cnt;
}
