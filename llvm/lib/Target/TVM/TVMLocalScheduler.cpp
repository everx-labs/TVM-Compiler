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
#include "TVMUtilities.h"
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

//#include "TVMInstMappingInfo.inc"
namespace llvm {
namespace TVM {
extern int RegForm2SForm[]; // defined in StackModel.cpp
} // end namespace TVM
} // namespace llvm

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

// Simple vector copy
static void copyList(const vector<unsigned> &src, vector<unsigned> dst) {
  //  std::copy(src.begin(), src.end(), dst.begin());
  for (unsigned i = 0; i < src.size(); i++)
    dst.push_back(src[i]);
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

  bool isEmpty() {
    return preferredInputQueue.empty() && preferredOutputQueue.empty() &&
           actualInputQueue.empty() && actualOutputQueue.empty();
  }

  void dump(string margin = "");

  // Calculate fixup on block input
  StackFixup calculateInputFixup(MachineFunction &MF) {
    return calculateFixup(MF, preferredInputQueue, actualInputQueue);
  }

  // Calculate fixup on block output
  StackFixup calculateOutputFixup(MachineFunction &MF) {
    return calculateFixup(MF, actualOutputQueue, preferredOutputQueue);
  }

  // Calculate common chain of two queues with maximum length.
  // Result contains values with order changes relatively to input queues
  // no more than maxPermutations.
  static void calculateMaximumCommonChain(vector<unsigned> const &queue1,
                                          vector<unsigned> const &queue2,
                                          vector<unsigned> &result,
                                          unsigned maxPermutations = 0xFFFF);

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
  static unsigned
  calculateCommonTailLength(vector<vector<unsigned>> const &queues);

  // Some test for BlockQueue functions
  static bool test(MachineFunction &MF);
};

class SchedulerUtils {
public:
  // Gather "pseodo-global" registers, i.e. such that can be reified
  // (pushed)
  //  from constant values (like block addresses) or fixed places on stack,
  //  like parameters
  // Other values will be received from block input "queue"
  static void
  gatherPseudoGlobal(MachineFunction &MF,
                     unordered_map<unsigned, MachineInstr *> &pseudoMap);

  // Similar as from TVMStackModel, but with different parameters
  static void gatherBlockLiveIns(MachineBasicBlock &MBB,
                                 MachineRegisterInfo *MRI, LiveIntervals *LIS,
                                 unordered_set<unsigned> &vregs);

  // Similar as from TVMStackModel, but with different parameters
  static void gatherBlockLiveOuts(MachineBasicBlock &MBB,
                                  MachineRegisterInfo *MRI, LiveIntervals *LIS,
                                  unordered_set<unsigned> &vregs);

  // Gather information about input & output block data queue
  static void
  gatherBlockQueueInfo(MachineBasicBlock &MBB, MachineRegisterInfo *MRI,
                       LiveIntervals *LIS,
                       unordered_map<unsigned, MachineInstr *> const &pseudoMap,
                       BlockQueueInfo &queueInfo);

  // Convert an instruction in Reg-form to S-form.
  // Similar to function from StackModel, adapted to this
  // flexible stack tree model.
  // If reverse = true, generate reverse instruction (a.k.a SUBR_S instedad of
  // SUB_S).
  static MachineInstr *convertToSForm(const MachineInstr &MI,
                                      const TargetInstrInfo *TII,
                                      bool reverse = false);

  // Return operation code for reverse instruction.
  // If ther is no reverse instruction, returns -1.
  static int getReverseOpcode(unsigned opcode);
};

void SchedulerUtils::gatherBlockLiveIns(MachineBasicBlock &MBB,
                                        MachineRegisterInfo *MRI,
                                        LiveIntervals *LIS,
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
                                         MachineRegisterInfo *MRI,
                                         LiveIntervals *LIS,
                                         unordered_set<unsigned> &vregs) {
  for (unsigned i = 0, e = MRI->getNumVirtRegs(); i != e; ++i) {
    unsigned Reg = TargetRegisterInfo::index2VirtReg(i);
    if (LIS->hasInterval(Reg)) {
      if (LIS->isLiveOutOfMBB(LIS->getInterval(Reg), &MBB)) {
        vregs.insert(Reg);
      }
    }
  }
}

void SchedulerUtils::gatherBlockQueueInfo(
    MachineBasicBlock &MBB, MachineRegisterInfo *MRI, LiveIntervals *LIS,
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

void SchedulerUtils::gatherPseudoGlobal(
    MachineFunction &MF, unordered_map<unsigned, MachineInstr *> &pseudoMap) {
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

int SchedulerUtils::getReverseOpcode(unsigned opcode) {
  switch (opcode) {
  case TVM::SUB:
    return TVM::SUBR;
  case TVM::SUB_S:
    return TVM::SUBR_S;
  case TVM::DIV:
    return TVM::DIV;
  case TVM::DIV_S:
    return TVM::DIV_S;
  case TVM::STI:
    return TVM::STIR;
  case TVM::STI_S:
    return TVM::STIR_S;
  case TVM::STU:
    return TVM::STUR;
  case TVM::STU_S:
    return TVM::STUR_S;
  case TVM::STIX:
    return TVM::STIXR;
  case TVM::STIX_S:
    return TVM::STIXR_S;
  case TVM::STUX:
    return TVM::STUXR;
  case TVM::STUX_S:
    return TVM::STUXR_S;
  case TVM::SGT:
    return TVM::SLT;
  case TVM::SGT_S:
    return TVM::SLT_S;
  case TVM::SGE:
    return TVM::SLE;
  case TVM::SGE_S:
    return TVM::SLE_S;
  case TVM::SLT:
    return TVM::SGT;
  case TVM::SLT_S:
    return TVM::SGT_S;
  case TVM::SLE:
    return TVM::SGE;
  case TVM::SLE_S:
    return TVM::SGE_S;
  default:
    return -1;
  }
}

enum LayeredInstructionDAGNodeKind { LINSource, LINSink, LINInstruction };

class LayeredInstructionDAGNode {
  friend class LayeredInstructionDAG;

public:
  class Edge {
    LayeredInstructionDAGNode *node;
    unsigned outPortIndex;
    unsigned inPortIndex;
    unsigned reg;

  public:
    explicit Edge(LayeredInstructionDAGNode *node_, unsigned outPortIndex_,
                  unsigned inPortIndex_, unsigned reg_)
        : node(node_), outPortIndex(outPortIndex_), inPortIndex(inPortIndex_),
          reg(reg_) {}

    LayeredInstructionDAGNode *getNode() const { return node; }

    unsigned getOutPortIndex() const { return outPortIndex; }

    unsigned getInPortIndex() const { return inPortIndex; }

    unsigned getReg() const { return reg; }
  };

private:
  LayeredInstructionDAG &DAG;
  LayeredInstructionDAGNodeKind kind;
  MachineInstr *instr;
  unsigned reg;
  vector<Edge> Predecessors;
  vector<Edge> Successors;
  unsigned level;
  unsigned orderNum;

private:
  LayeredInstructionDAGNode(LayeredInstructionDAG &DAG_, MachineInstr *insn)
      : DAG(DAG_), kind(LINInstruction), instr(insn), reg(0), Predecessors(),
        Successors(), level(0) {}

  LayeredInstructionDAGNode(LayeredInstructionDAG &DAG_,
                            LayeredInstructionDAGNodeKind kn, unsigned rg)
      : DAG(DAG_), kind(kn), instr(nullptr), reg(rg), Predecessors(),
        Successors(), level(0) {}

public:
  LayeredInstructionDAGNodeKind getKind() const { return kind; }

  LayeredInstructionDAG &getDAG() const { return DAG; }

  MachineInstr *getInstruction() const { return instr; }

  unsigned getReg() const { return reg; }

  unsigned getLevel() const { return level; }

  void setLevel(unsigned lvl) { level = lvl; }

  // return order number of node in scheduling sequence
  // the more the number - the later is the node instruction
  unsigned getOrderNum() const { return orderNum; }

  void setOrderNum(unsigned num) { orderNum = num; }

  const TargetInstrInfo *getInstructionInfo();

  // Return number of successors for register reg
  unsigned getNumSuccessors(unsigned reg) const {
    unsigned num = 0;
    for (Edge edge : successors()) {
      if (edge.getReg() == reg)
        num++;
    }
    return num;
  }

  // Get all succesors of this node using register reg
  void getSuccessors(unsigned reg,
                     vector<LayeredInstructionDAGNode *> result) const {
    for (Edge edge : successors()) {
      if (edge.getReg() == reg)
        result.push_back(edge.getNode());
    }
  }

  // Returns output port index for register reg.
  // If there is no such port, returns -1.
  int getOutPortIndex(unsigned reg) {
    const MachineInstr &instr = *getInstruction();
    int num = 0;
    for (const MachineOperand &op : instr.defs()) {
      if (op.isReg() && op.getReg() == reg)
        return num;
      num++;
    }
    return -1;
  }

  string toString();

  // Layered Instruction Graph iterators
  using pred_iterator = vector<Edge>::iterator;
  using const_pred_iterator = vector<Edge>::const_iterator;
  using succ_iterator = vector<Edge>::iterator;
  using const_succ_iterator = vector<Edge>::const_iterator;

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
  void addPredecessor(LayeredInstructionDAGNode *node, unsigned outPortIndex,
                      unsigned inPortIndex, unsigned reg) {
    Predecessors.push_back(Edge(node, outPortIndex, inPortIndex, reg));
  }

  bool hasPredecessor(LayeredInstructionDAGNode *node) {
    for (Edge edge : predecessors()) {
      if (edge.getNode() == node)
        return true;
    }
    return false;
  }

  void addSuccessor(LayeredInstructionDAGNode *node, unsigned outPortIndex,
                    unsigned inPortIndex, unsigned reg) {
    Successors.push_back(Edge(node, outPortIndex, inPortIndex, reg));
  }

  bool hasSuccessor(LayeredInstructionDAGNode *node) {
    for (Edge edge : successors()) {
      if (edge.getNode() == node)
        return true;
    }
    return false;
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
  LayeredInstructionDAG(MachineBasicBlock *MBB_,
                        unordered_set<unsigned> const &pseudoSet,
                        unordered_set<unsigned> liveOut)
      : MBB(MBB_), levels() {
    constructFromMBB(pseudoSet, liveOut);
  }

  MachineBasicBlock *getMBB() const { return MBB; }

  const TargetInstrInfo *getInstructionInfo() {
    return getMBB()->getParent()->getSubtarget<TVMSubtarget>().getInstrInfo();
  }

  unsigned getNumLevels() const { return levels.size(); }

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
                                      LayeredInstructionDAGNode *target,
                                      unsigned outPortIndex,
                                      unsigned inPortIndex, unsigned reg);

  void constructFromMBB(unordered_set<unsigned> const &pseudoSet,
                        unordered_set<unsigned> liveOut);
};

LayeredInstructionDAGNode *
LayeredInstructionDAG::createNode(MachineInstr *instr) {
  return new (
      LayeredInstructionDAGNodeRecycler.Allocate<LayeredInstructionDAGNode>(
          Allocator)) LayeredInstructionDAGNode(*this, instr);
}

LayeredInstructionDAGNode *
LayeredInstructionDAG::createNode(LayeredInstructionDAGNodeKind kind,
                                  unsigned reg) {
  return new (
      LayeredInstructionDAGNodeRecycler.Allocate<LayeredInstructionDAGNode>(
          Allocator)) LayeredInstructionDAGNode(*this, kind, reg);
}

const TargetInstrInfo *LayeredInstructionDAGNode::getInstructionInfo() {
  return getDAG().getInstructionInfo();
}

void LayeredInstructionDAG::addEdge(LayeredInstructionDAGNode *source,
                                    LayeredInstructionDAGNode *target,
                                    unsigned outPortIndex, unsigned inPortIndex,
                                    unsigned reg) {
  source->addSuccessor(target, outPortIndex, inPortIndex, reg);
  target->addPredecessor(source, outPortIndex, inPortIndex, reg);
}

void LayeredInstructionDAG::scheduleNodes(
    vector<LayeredInstructionDAGNode *> &order) {
  // simple algorithm used node levels order
  // may be enhanced in future
  unsigned orderNum = 0;
  for (unsigned level = 0; level < levels.size(); level++) {
    vector<LayeredInstructionDAGNode *> const &lv = levels[level];
    for (unsigned pos = 0; pos < lv.size(); pos++) {
      LayeredInstructionDAGNode *node = lv[pos];
      node->setOrderNum(orderNum++);
      order.push_back(node);
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
    // MI.dump();
    if (MI.getOpcode() == TVM::ARGUMENT &&
        pseudoSet.find(MI.getOperand(0).getReg()) != pseudoSet.end())
      continue;
    LayeredInstructionDAGNode *insnNode = createNode(&MI);
    unsigned inPortIndex = 0;
    for (const MachineOperand &op : MI.uses()) {
      if (op.isReg()) {
        unsigned reg = op.getReg();
        if (pseudoSet.find(reg) != pseudoSet.end())
          continue;
        unsigned outPortIndex = 0;
        LayeredInstructionDAGNode *def;
        auto it = defMap.find(reg);
        if (it == defMap.end()) {
          def = createNode(LINSource, reg);
          def->setLevel(0);
          defMap.insert_or_assign(reg, def);
        } else {
          def = it->second;
          int idx = def->getOutPortIndex(reg);
          assert(idx >= 0);
          outPortIndex = idx;
        }
        if (def->getLevel() == curLevel) {
          levels.push_back(vector<LayeredInstructionDAGNode *>());
          curLevel++;
        }
        addEdge(def, insnNode, outPortIndex, inPortIndex, reg);
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
    int outPortIndex = sinkInstr->getOutPortIndex(sinkReg);
    assert(outPortIndex >= 0);
    addEdge(sinkInstr, sink, outPortIndex, 0, sinkReg);
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
    dbgs() << "\n";
  }
  dbgs() << "Edges:\n";
  for (unsigned i = 0; i < getNumLevels(); i++) {
    for (LayeredInstructionDAGNode *source : levels[i]) {
      for (LayeredInstructionDAGNode::Edge target : source->successors()) {
        dbgs() << "[" << regToString(target.getReg()) << "] ";
        dbgs() << source->toString();
        dbgs() << " -> ";
        dbgs() << target.getNode()->toString();
        dbgs() << "\n";
      }
    }
    dbgs() << "\n";
  }
}

// Model of Machine Block local stack by means of operand queue.
// The head of queue corresponds to the top of stack
class OperandQueue {
public:
  class Slot {
    // Target Node
    LayeredInstructionDAGNode *target;
    // Number of input port in target node, corresponding to this slot
    unsigned targetInPortIndex;
    // Number of uses of this slot data remained
    // if useRemain > 1, we need to copy this slot for next use instructions
    unsigned usesRemain;
    // Register used by this slot
    unsigned reg;

  public:
    explicit Slot(LayeredInstructionDAGNode *target_,
                  unsigned targetInPortIndex_, unsigned reg_,
                  unsigned usesRemain_ = 1)
        : target(target_), targetInPortIndex(targetInPortIndex_), reg(reg_),
          usesRemain(usesRemain_) {}

    LayeredInstructionDAGNode *getTarget() const { return target; }

    unsigned getInPortIndex() const { return targetInPortIndex; }

    unsigned getReg() const { return reg; }

    unsigned getRemainUses() const { return usesRemain; }

    unsigned decrementUses() {
      assert(usesRemain > 0);
      return --usesRemain;
    }

    string toString();
  };

private:
  LayeredInstructionDAG *DAG;
  vector<Slot> queue;

public:
  OperandQueue(LayeredInstructionDAG *dag) : DAG(dag), queue() {}

  unsigned size() const { return queue.size(); }

  // Return slot for index slotIndex.
  // Index 0 - head of the queue, index (size() -1) - tail of the queue.
  Slot &getSlot(unsigned slotIndex) {
    return queue[queue.size() - 1 - slotIndex];
  }

  const Slot &getSlot(unsigned slotIndex) const {
    return queue[queue.size() - 1 - slotIndex];
  }

  // Insert new slot into queue
  // Return insertion position of slot in queue
  unsigned enqueue(LayeredInstructionDAGNode *target, unsigned targetInPort,
                   unsigned reg, unsigned useRem = 1);

  // Insert auxiliary slot into queue (uses for instruction arguments & results)
  unsigned enqueue(unsigned reg);

  // Remove num elements from queue head
  void dequeue(unsigned num = 1);

  // Return true if this queue contains register reg, false otherwise.
  bool contains(unsigned reg) const { return lookupIndex(reg) >= 0; }

  // Lookup slot index contains register reg.
  // If there is no such slot, returns -1.
  int lookupIndex(unsigned reg) const;

  string toString();

  void dump();

  inline static unsigned order(const LayeredInstructionDAGNode *target,
                               unsigned targetInPort) {
    return ((target ? target->getOrderNum() : 0) << 8) + targetInPort;
  }

  inline static unsigned order(const LayeredInstructionDAGNode::Edge &edge) {
    return order(edge.getNode(), edge.getInPortIndex());
  }
};

int OperandQueue::lookupIndex(unsigned reg) const {
  for (int idx = 0; idx < queue.size(); idx++) {
    const OperandQueue::Slot &slot = getSlot(idx);
    if (!slot.getTarget())
      continue;
    if (getSlot(idx).getReg() == reg)
      return idx;
  }
  return -1;
}

unsigned OperandQueue::enqueue(LayeredInstructionDAGNode *targ,
                               unsigned targInPortIndex, unsigned reg,
                               unsigned usesRemain) {
  Slot slot(targ, targInPortIndex, reg, usesRemain);
  unsigned ord = order(targ, targInPortIndex);
  unsigned pos = 0;
  for (pos = 0; pos < size(); pos++) {
    Slot const &posSlot = getSlot(pos);
    if (order(posSlot.getTarget(), posSlot.getInPortIndex()) > ord)
      break;
  }
  if (pos != 0) {
    auto it = queue.begin() + (queue.size() - pos);
    queue.insert(it, slot);
  } else
    queue.push_back(slot);
  return pos;
}

unsigned OperandQueue::enqueue(unsigned reg) {
  Slot slot(nullptr, 0, reg, 1);
  queue.push_back(slot);
  return queue.size() - 1;
}

void OperandQueue::dequeue(unsigned num) {
  assert(num <= size());
  while (num > 0) {
    queue.pop_back();
    num--;
  }
}

string OperandQueue::Slot::toString() {
  string str;
  str += "Reg : ";
  str += regToString(getReg());
  str += ", Target : ";
  str += getTarget() ? getTarget()->toString() : "none";
  str += ", InPortIndex : ";
  char buf[8];
  _itoa_s(getInPortIndex(), buf, 10);
  str += buf;
  str += ", Uses remain : ";
  _itoa_s(getRemainUses(), buf, 10);
  str += buf;
  return str;
}

string OperandQueue::toString() {
  string str;
  for (int i = 0; i < size(); i++) {
    if (i != 0)
      str += " ";
    str += regToString(queue[i].getReg());
  }
  return str;
}

void OperandQueue::dump() {
  dbgs() << "Operand Queue : \n";
  for (int i = 0; i < size(); i++) {
    dbgs() << "  " << (size() - 1 - i) << " : " << queue[i].toString() << "\n";
  }
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

  StackTree *tree;
  StackTreeNode *parent;
  vector<StackTreeNode *> children;
  vector<unsigned> registers;
  MachineBasicBlock *MBB;

private:
  StackTreeNode(StackTree *tree_, StackTreeNode *parent_ = nullptr)
      : tree(tree_), children(), registers(), MBB(nullptr) {
    setParent(parent_);
  }

  StackTreeNode(StackTree *tree_, const vector<unsigned> &registers_,
                StackTreeNode *parent_ = nullptr)
      : tree(tree_), children(), registers(registers_), MBB(nullptr) {
    setParent(parent_);
  }

  void setParent(StackTreeNode *parent_) {
    parent = parent_;
    if (parent)
      parent->children.push_back(this);
  }

  void setMBB(MachineBasicBlock *MBB_) { MBB = MBB_; }

public:
  StackTree *getTree() const { return tree; }

  StackTreeNode *getParent() const { return parent; }

  MachineBasicBlock *getMBB() const { return MBB; }

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

  // Get stack registers from root node to this node
  void getContents(unordered_set<unsigned> &regs) {
    if (parent)
      parent->getContents(regs);
    for (unsigned reg : registers) {
      regs.insert(reg);
    }
  }

  // Lookup register in current node or parent (recursively).
  // Return register offset from the top of stack or -1
  // if register is not in stack
  int lookupRegisterOffset(unsigned reg);

  // Return true if stack contains register reg
  bool contains(unsigned reg) { return lookupRegisterOffset(reg) >= 0; }

  // Replace register reg to register replaceReg
  void replaceReg(unsigned reg, unsigned replaceReg);

  string toString();

  // Similar to function from Stack/StackModel, adapted
  void filterByDeadDefs(MachineInstr &MI);

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

void StackTreeNode::replaceReg(unsigned reg, unsigned replaceReg) {
  for (unsigned idx = 0; idx < registers.size(); idx++) {
    if (registers[idx] == reg)
      registers[idx] = replaceReg;
  }
  if (parent)
    parent->replaceReg(reg, replaceReg);
}

void StackTreeNode::filterByDeadDefs(MachineInstr &MI) {
  for (const MachineOperand &MO : MI.defs()) {
    if (MO.isReg() && MO.isDead()) {
      replaceReg(MO.getReg(), TVMFunctionInfo::UnusedReg);
    }
  }
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
  if (getMBB())
    dbgs() << blockToString(getMBB()) << " : ";
  else
    dbgs() << "Intermediate ";
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
  MachineFunction &MF;
  TVMFunctionInfo *MFI;
  const TargetInstrInfo *TII;
  // Map from register index to possible local variable
  unordered_map<unsigned, const DILocalVariable *> debugMap;

  // Pool-allocate StackableTreeNode-lifetime
  BumpPtrAllocator Allocator;

  // Allocation management for StackableRegion in graph
  Recycler<StackTreeNode> StackTreeNodeRecycler;

public:
  StackTree(MachineFunction &MF_) : MF(MF_) {
    MFI = MF.getInfo<TVMFunctionInfo>();
    TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();
    gatherDebugInfo();
  }

  StackTreeNode *getRoot() { return root; }

  void setRoot(StackTreeNode *root_) { root = root_; }

  void addNodeMap(MachineBasicBlock *MBB, StackTreeNode *stack) {
    stack->setMBB(MBB);
    nodeMap[MBB] = stack;
  }

  StackTreeNode *lookupNode(MachineBasicBlock *MBB) { return nodeMap[MBB]; }

  // Lookup local variable debug info for register reg
  const DILocalVariable *lookupLocalVariable(unsigned reg) {
    return debugMap[reg];
  }

  void dump() {
    if (getRoot())
      getRoot()->dump();
    else
      dbgs() << "null";
  }

  StackTreeNode *createNode(StackTreeNode *parent = nullptr) {
    return new (StackTreeNodeRecycler.Allocate<StackTreeNode>(Allocator))
        StackTreeNode(this, parent);
  }

  StackTreeNode *createNode(const vector<unsigned> &registers,
                            StackTreeNode *parent = nullptr) {
    return new (StackTreeNodeRecycler.Allocate<StackTreeNode>(Allocator))
        StackTreeNode(this, registers, parent);
  }

  // Rewrite all instrtuction from machine function to S-Form
  void rewriteToSForm();

private:
  // Rewrite an instruction from Reg-form to S-form.
  // Similar to function from StackModel, adapted to this
  // flexible stack tree model
  void rewriteToSForm(MachineInstr &MI, StackTreeNode &stack);

  // Gather debug information for registers in machine function
  void gatherDebugInfo();
};

void StackTree::gatherDebugInfo() {
  for (MachineBasicBlock &MBB : MF) {
    for (MachineInstr &MI : MBB) {
      if (MI.isDebugValue() && MI.getOperand(0).isReg())
        debugMap[MI.getOperand(0).getReg()] = MI.getDebugVariable();
    }
  }
}

void StackTree::rewriteToSForm() {
  for (MachineBasicBlock &MBB : MF) {
    StackTreeNode *stack = lookupNode(&MBB);
    assert(stack);
    for (MachineInstr &MI : MBB) {
      rewriteToSForm(MI, *stack);
    }
  }
}

void StackTree::rewriteToSForm(MachineInstr &MI, StackTreeNode &stack) {
  MachineInstr *SMI = SchedulerUtils::convertToSForm(MI, TII);
  if (SMI) {
    MI.getParent()->insertAfter(MachineBasicBlock::instr_iterator(MI), &MI);
    std::vector<unsigned> SourceRegs;
    for (unsigned I = 0; I < MI.getNumOperands(); I++) {
      const auto &Op = MI.getOperand(I);
      if (Op.isReg()) {
        SourceRegs.push_back(Op.getReg());
      }
    }
    MFI->setStackModelSourceRegs(SMI, SourceRegs);

    // TODO Maybe don't needed in this model, as opposed to StackModel
    // stack.filterByDeadDefs(MI);
  }
  MI.removeFromParent();
}

enum StackableRegionKind {
  SRLeaf,        // Leaf region (contains single Machine Block)
  SRSequence,    // Sequence of regions
  SRIfThen,      // if (...) { ... }
  SRIfThenElse,  // if (...)  { ... } else { ... }
  SRSwitch,      // switch (...) { ... }
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
  bool hasDeflt;

  // Registers defined in region
  unordered_set<unsigned> defs;

  // Registers used in region
  unordered_set<unsigned> uses;

  // Live variables (registers) on the inputs of region
  unordered_set<unsigned> liveIns;

  // Live variables (registers) on the output of region
  unordered_set<unsigned> liveOuts;

  // Local stack variables (registers)
  vector<unsigned> localStack;

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
    assert(kind == SRIfThen || kind == SRIfThenElse || kind == SRSwitch ||
           kind == SRWhileLoop || kind == SRNaturalLoop);
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

  bool hasDefault() const {
    assert(kind == SRSwitch);
    return hasDeflt;
  }

  unsigned getSwitchSize() const {
    hasDefault() ? subregions.size() - 2 : subregions.size() - 1;
  }

  StackableRegion *getDefault() const {
    assert(kind == SRSwitch);
    return hasDefault() ? subregions[subregions.size() - 1] : nullptr;
  }

  StackableRegion *getFirst() const {
    assert(kind == SRSequence);
    return subregions[0];
  }

  StackableRegion *getLast() const {
    assert(kind == SRSequence);
    return subregions[subregions.size() - 1];
  }

  StackableRegion *getBody() {
    assert(kind == SRSelfLoop || kind == SRWhileLoop || kind == SRNaturalLoop);
    if (kind == SRSelfLoop)
      return subregions[0];
    else
      return subregions[1];
  }

  BlockQueueInfo &getQueueInfo() { return queueInfo; }

  bool isLoop() {
    return kind == SRSelfLoop || kind == SRWhileLoop || kind == SRNaturalLoop;
  }

  // Returns head region - child region on entrance of this region
  StackableRegion *getHeadRegion();

  // Calculate the list of tail regions - child regions on the exit of this
  // region.
  void getTailRegions(vector<StackableRegion *> &tails);

  // Returns head block - Machine Block on the entrance of region
  MachineBasicBlock *getHeadBlock();

  // Returns tail block - Machine Block on the exit of region.
  // If there are multiple tail blocks, returns nullptr
  MachineBasicBlock *getTailBlock();

  // Calculate the list of tail blocks - Machine Block on the exit of region.
  // If there is no single tail block, returns nullptr
  void getTailBlocks(vector<MachineBasicBlock *> &blocks);

  const unordered_set<unsigned> &getDefs() const { return defs; }

  const unordered_set<unsigned> &getUses() const { return uses; }

  const unordered_set<unsigned> &getLiveIns() const { return liveIns; }

  const unordered_set<unsigned> &getLiveOuts() const { return liveOuts; }

  const vector<unsigned> &getLocalStack() const { return localStack; }

  // StackableRegion children iterator
  using child_iterator = vector<StackableRegion *>::iterator;
  using const_child_iterator = vector<StackableRegion *>::const_iterator;

  child_iterator begin() { return subregions.begin(); }

  const_child_iterator begin() const { return subregions.begin(); }

  child_iterator end() { return subregions.end(); }

  const_child_iterator end() const { return subregions.end(); }

  child_iterator case_begin() { return ++subregions.begin(); }

  const_child_iterator case_begin() const { return ++subregions.begin(); }

  child_iterator case_end() {
    return hasDefault() ? --subregions.end() : subregions.end();
  }

  const_child_iterator case_end() const {
    return hasDefault() ? --subregions.end() : subregions.end();
  }

  static string toString(StackableRegionKind kind);

  string toString();

  void dump(string margin);

  void dumpVariables(string margin);

private:
  // Recursively calculate different region variable (register) sets - defs,
  // uses, liveIns, liveOuts, ...
  void calculateVariables(MachineRegisterInfo *MRI, LiveIntervals *LIS);

  // Recursively calculate (approximately) region and its children weights
  void calculateWeights(uint64_t weight_ = 1);

  // Recursively calculate region's BlockQueueInfo
  void
  calculateQueueInfo(MachineRegisterInfo *MRI, LiveIntervals *LIS,
                     unordered_map<unsigned, MachineInstr *> const &pseudoMap);

  // Calculate stack & active out for conditional regions (ifs & switches)
  void calculateConditionStack();

  // Create StackTree structure components
  void createStackTree(StackTreeNode &stack);
};

// Structure of Stackable Regions.
// Contains region allocation and pointer to root region.
class StackableRegionStructure {
  MachineFunction &MF;
  DJGraph djGraph;
  StackableRegion *root;
  unordered_map<MachineBasicBlock *, StackableRegion *> leafMap;

  // Pool-allocate StackableRegionStructure-lifetime
  BumpPtrAllocator Allocator;

  // Allocation management for StackableRegion in graph
  Recycler<StackableRegion> StackableRegionRecycler;

public:
  StackableRegionStructure(MachineFunction &MF_, MachineDominatorTree &MDT)
      : MF(MF_), djGraph(MF_, MDT) {
    init(MF_);
  }

  StackableRegion &getRoot() const { return *root; }

  const DJGraph &getDJGraph() const { return djGraph; }

  MachineFunction &getMF() const { return MF; }

  StackableRegion *lookupLeafRegion(MachineBasicBlock *MBB) const {
    auto search = leafMap.find(MBB);
    if (search != leafMap.end())
      return search->second;
    return nullptr;
  }

  // Calculate different region variable (register) sets - defs, uses, liveIns,
  // liveOuts, ...
  void calculateVariables(MachineRegisterInfo *MRI, LiveIntervals *LIS) {
    getRoot().calculateVariables(MRI, LIS);
  }

  // Recursively calculate region's BlockQueueInfo
  void
  calculateQueueInfo(MachineRegisterInfo *MRI, LiveIntervals *LIS,
                     unordered_map<unsigned, MachineInstr *> const &pseudoMap);

  // Create StackTree structure components
  void createStackTree(StackTree &tree);

  void dump();

private:
  StackableRegion *createLeaf(MachineBasicBlock *MBB) {
    StackableRegion *rgn =
        new (StackableRegionRecycler.Allocate<StackableRegion>(Allocator))
            StackableRegion(SRLeaf, MBB);
    leafMap[MBB] = rgn;
    return rgn;
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

StackableRegion *
StackableRegionStructure::createSequence(StackableRegion *child1,
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

void StackableRegion::calculateVariables(MachineRegisterInfo *MRI,
                                         LiveIntervals *LIS) {
  for (StackableRegion *child : *this) {
    child->calculateVariables(MRI, LIS);
    defs.insert(child->defs.begin(), child->defs.end());
    uses.insert(child->uses.begin(), child->uses.end());
  }
  if (kind == SRLeaf) {
    for (MachineInstr &MI : *getMBB()) {
      for (MachineOperand &MO : MI.defs()) {
        if (MO.isReg())
          defs.insert(MO.getReg());
      }
      for (MachineOperand &MO : MI.uses()) {
        if (MO.isReg())
          uses.insert(MO.getReg());
      }
    }
    SchedulerUtils::gatherBlockLiveIns(*getMBB(), MRI, LIS, liveIns);
    SchedulerUtils::gatherBlockLiveOuts(*getMBB(), MRI, LIS, liveOuts);
  } else {
    StackableRegion *head = getHeadRegion();
    liveIns.insert(head->liveIns.begin(), head->liveIns.end());
    vector<StackableRegion *> tails;
    getTailRegions(tails);
    for (StackableRegion *tail : tails) {
      liveOuts.insert(tail->liveOuts.begin(), tail->liveOuts.end());
    }
  }
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
    // TODO improper regions ??
    assert(false);
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

void StackableRegionStructure::calculateQueueInfo(
    MachineRegisterInfo *MRI, LiveIntervals *LIS,
    unordered_map<unsigned, MachineInstr *> const &pseudoMap) {
  // TODO  getRoot().calculateWeights();
  getRoot().calculateQueueInfo(MRI, LIS, pseudoMap);
}

void StackableRegionStructure::createStackTree(StackTree &tree) {
  MachineFunction &MF = getMF();
  TVMFunctionInfo *MFI = MF.getInfo<TVMFunctionInfo>();
  MachineBasicBlock &firstMBB = MF.front();
  if (!firstMBB.empty()) {
    MachineInstr &instr = firstMBB.front();
    if (TVM::isArgumentNum(instr)) {
      int args = instr.getOperand(0).getImm();
      for (int i = 0; i < args; i++)
        MFI->addParam(MVT::i64);
      // TODO  instr.eraseFromParent();
    }
  }

  unsigned numParams = MFI->numParams();
  vector<unsigned> params(numParams);

  // Fill initial stack position with arguments.
  for (MachineInstr &MI : firstMBB) {
    if (!TVM::isArgument(MI))
      break;
    MI.dump();
    unsigned reg = MI.getOperand(0).getReg();
    assert(!MFI->isVRegStackified(reg));
    unsigned argNo = MI.getOperand(1).getCImm()->getZExtValue(); // numParams - MI.getOperand(1).getCImm()->getZExtValue() - 1;
    params[argNo] = reg;
    // TODO MI.eraseFromParent();
  }

  StackTreeNode *root = tree.createNode(params);
  tree.setRoot(root);
  tree.addNodeMap(&firstMBB, root);
  getRoot().createStackTree(*root);
}

void StackableRegion::createStackTree(StackTreeNode &stack) {
  switch (kind) {
  case SRLeaf:
    stack.getTree()->addNodeMap(getMBB(), &stack);
    break;
  case SRIfThen:
    getCondition()->createStackTree(*stack.getTree()->createNode(&stack));
    getThen()->createStackTree(
        *stack.getTree()->createNode(getLocalStack(), &stack));
    break;
  case SRIfThenElse:
    getCondition()->createStackTree(*stack.getTree()->createNode(&stack));
    getThen()->createStackTree(
        *stack.getTree()->createNode(getLocalStack(), &stack));
    getElse()->createStackTree(
        *stack.getTree()->createNode(getLocalStack(), &stack));
    break;
  case SRSwitch:
    getCondition()->createStackTree(*stack.getTree()->createNode(&stack));
    for (auto it = case_begin(); it != case_end(); it++) {
      StackableRegion *caseRegion = *it;
      caseRegion->createStackTree(
          *stack.getTree()->createNode(getLocalStack(), &stack));
    }
    if (hasDefault())
      getDefault()->createStackTree(
          *stack.getTree()->createNode(getLocalStack(), &stack));
    break;
  case SRSequence:
    break;
  case SRSelfLoop:
  case SRNaturalLoop:
  case SRWhileLoop:
  default:
    break;
  }
}

void StackableRegionStructure::dump() {
  dbgs() << "Region Tree:\n";
  getRoot().dump("");
}

StackableRegion *StackableRegion::getHeadRegion() {
  switch (kind) {
  case SRLeaf:
    return this;
  case SRIfThen:
  case SRIfThenElse:
  case SRSwitch:
    return getCondition();
  case SRSequence:
    return getFirst();
  case SRSelfLoop:
    return getBody();
  case SRNaturalLoop:
  case SRWhileLoop:
    return getCondition();
  default:
    return nullptr;
  }
}

void StackableRegion::getTailRegions(vector<StackableRegion *> &tails) {
  switch (kind) {
  case SRLeaf:
    tails.push_back(this);
    break;
  case SRIfThen:
    getThen()->getTailRegions(tails);
    break;
  case SRIfThenElse:
    getThen()->getTailRegions(tails);
    getElse()->getTailRegions(tails);
    break;
  case SRSwitch:
    for (auto it = case_begin(); it != case_end(); it++) {
      StackableRegion *caseRegion = *it;
      caseRegion->getTailRegions(tails);
    }
    if (hasDefault())
      getDefault()->getTailRegions(tails);
    break;
  case SRSequence:
    getLast()->getTailRegions(tails);
    break;
  case SRSelfLoop:
    getBody()->getTailRegions(tails);
    break;
  case SRWhileLoop:
    getCondition()->getTailRegions(tails);
    break;
  case SRNaturalLoop:
    getCondition()->getTailRegions(tails);
    getBody()->getTailRegions(tails);
    break;
  default:
    break;
  }
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
  case SRSequence:
    return getLast()->getTailBlock();
  case SRSelfLoop:
    return getBody()->getTailBlock();
  case SRWhileLoop:
    return getCondition()->getTailBlock();
  default:
    return nullptr;
  }
}

void StackableRegion::getTailBlocks(vector<MachineBasicBlock *> &blocks) {
  switch (kind) {
  case SRLeaf:
    blocks.push_back(getMBB());
    break;
  case SRIfThen:
    getThen()->getTailBlocks(blocks);
    break;
  case SRIfThenElse:
    getThen()->getTailBlocks(blocks);
    getElse()->getTailBlocks(blocks);
    break;
  case SRSwitch:
    for (auto it = case_begin(); it != case_end(); it++) {
      StackableRegion *caseRegion = *it;
      caseRegion->getTailBlocks(blocks);
    }
    if (hasDefault())
      getDefault()->getTailBlocks(blocks);
    break;
  case SRSequence:
    getLast()->getTailBlocks(blocks);
    break;
  case SRSelfLoop:
    getBody()->getTailBlocks(blocks);
    break;
  case SRWhileLoop:
    getCondition()->getTailBlocks(blocks);
    break;
  default:
    // TODO
    assert(false);
    return;
  }
}

// TODO maybe we also can use weights from MachineBlockFrequencyInfo
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
  case SRSwitch:
    weight = weight_;
    getCondition()->calculateWeights(weight);
    for (auto it = case_begin(); it != case_end(); it++) {
      StackableRegion *caseRegion = *it;
      caseRegion->calculateWeights(weight * LEVEL_REGION_WEIGHT);
      if (hasDefault())
        getDefault()->calculateWeights(weight * LEVEL_REGION_WEIGHT);
    }
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

void StackableRegion::calculateConditionStack() {
  StackableRegion *cond = getCondition();
  for (unsigned idx = 0; idx < cond->queueInfo.preferredOutputQueue.size();
       idx++) {
    unsigned reg = cond->queueInfo.preferredOutputQueue[idx];
    if (find(liveIns.begin(), liveIns.end(), reg) == liveIns.end() &&
        find(liveOuts.begin(), liveOuts.end(), reg) != liveOuts.end()) {
      localStack.push_back(reg);
      queueInfo.actualOutputQueue.push_back(reg);
    } else
      cond->queueInfo.actualOutputQueue.push_back(reg);
  }
}

void StackableRegion::calculateQueueInfo(
    MachineRegisterInfo *MRI, LiveIntervals *LIS,
    unordered_map<unsigned, MachineInstr *> const &pseudoMap) {
  for (StackableRegion *child : *this) {
    child->calculateQueueInfo(MRI, LIS, pseudoMap);
  }
  switch (kind) {
  case SRLeaf:
    SchedulerUtils::gatherBlockQueueInfo(*getMBB(), MRI, LIS, pseudoMap,
                                         queueInfo);
    break;
  case SRSequence:

    break;
  case SRIfThen:
    calculateConditionStack();
    copyList(getCondition()->queueInfo.actualOutputQueue,
             getThen()->queueInfo.actualInputQueue);
    break;
  case SRIfThenElse:
    calculateConditionStack();
    copyList(getCondition()->queueInfo.actualOutputQueue,
             getThen()->queueInfo.actualInputQueue);
    copyList(getCondition()->queueInfo.actualOutputQueue,
             getElse()->queueInfo.actualInputQueue);
    break;
  case SRSwitch:
    calculateConditionStack();

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
  case SRSwitch:
    str = "Switch";
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
  case SRSwitch:
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
  dumpVariables(margin + "  ");
  if (!getQueueInfo().isEmpty())
    getQueueInfo().dump(margin + "  ");
  for (StackableRegion *child : *this) {
    child->dump(margin + "  ");
  }
}

void StackableRegion::dumpVariables(string margin) {
  dbgs() << margin << "Defs : " << regSetToString(getDefs()) << "\n";
  dbgs() << margin << "Uses : " << regSetToString(getUses()) << "\n";
  dbgs() << margin << "Live Ins : " << regSetToString(getLiveIns()) << "\n";
  dbgs() << margin << "Live Outs : " << regSetToString(getLiveOuts()) << "\n";
  if (!getLocalStack().empty())
    dbgs() << margin << "Local Stack : " << regListToString(getLocalStack())
           << "\n";
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

  // Main code emit function
  void emitScheduledCode(const StackableRegionStructure &SRS,
                         StackTree &stackTree);

  // Queue instruction scheduler
  // Input - list of prescheduled instructions in Reg-Form nodes,
  //   Stack stack.
  // Output - list of scheduled instructions in S-Form
  void schedule(const vector<LayeredInstructionDAGNode *> nodes,
                StackTreeNode &stack, MachineBasicBlock &outMBB);

  void insertInputOperands(const LayeredInstructionDAGNode &node,
                           StackTreeNode &stack, OperandQueue &queue,
                           MachineBasicBlock &outMBB);

  void distributeOutputResults(const LayeredInstructionDAGNode &node,
                               StackTreeNode &stack, OperandQueue &queue,
                               MachineBasicBlock &outMBB);

  MachineRegisterInfo *MRI;
  LiveIntervals *LIS;
  MachineDominatorTree *MDT;
  const TargetInstrInfo *TII;
  TVMFunctionInfo *MFI;
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
  TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();
  MFI = MF.getInfo<TVMFunctionInfo>();

  changed |= runOnBasicBlocks(MF);

  return changed;
}

bool TVMLocalScheduler::runOnBasicBlocks(MachineFunction &MF) {
  bool changed = false;

  //  BlockQueueInfo::test(MF);

  unordered_map<unsigned, MachineInstr *> pseudoMap;
  //  SchedulerUtils::gatherPseudoGlobal(MF, pseudoMap);

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
  SRS.calculateVariables(MRI, LIS);
  SRS.calculateQueueInfo(MRI, LIS, pseudoMap);
  SRS.dump();
  dbgs() << "\n";

  StackTree stackTree(MF);
  SRS.createStackTree(stackTree);
  stackTree.dump();
  dbgs() << "\n";

  emitScheduledCode(SRS, stackTree);

  changed = true;

  //  for (DJNode *node : SRS.getDJGraph()) {
  //    SchedulerUtils::gatherBlockQueueInfo(node->getMBB(), MRI, LIS,
  //    pseudoMap,
  //                                         node->getBlockQueueInfo());
  //  }

  /*
auto &firstBB = MF.front();


  */
  return changed;
}

void TVMLocalScheduler::emitScheduledCode(const StackableRegionStructure &SRS,
                                          StackTree &stackTree) {
  MachineFunction &MF = SRS.getMF();
  using RPOTType = ReversePostOrderTraversal<MachineFunction *>;
  RPOTType RPOT(&MF);

  for (RPOTType::rpo_iterator I = RPOT.begin(), E = RPOT.end(); I != E; ++I) {
    MachineBasicBlock *MBB = *I;
    StackableRegion *leaf = SRS.lookupLeafRegion(MBB);
    assert(leaf);
    StackTreeNode *stack = stackTree.lookupNode(MBB);
    assert(stack);
    unordered_set<unsigned> stackSet;
    stack->getContents(stackSet);
    //    dbgs() << "Live Outs : " << regSetToString(leaf->getLiveOuts()) <<
    //    "\n"; dbgs() << "Stack Set : " << regSetToString(stackSet) << "\n";
    LayeredInstructionDAG DAG(MBB, stackSet, leaf->getLiveOuts());
    DAG.dump();
    vector<LayeredInstructionDAGNode *> order;
    DAG.scheduleNodes(order);
    dbgs() << "Instruction DAG order : \n";
    for (LayeredInstructionDAGNode *node : order) {
      dbgs() << node->toString() << "\n";
    }
    dbgs() << "\n";
    // This is temporary Machine Block, container for new instructions
    MachineBasicBlock *newMBB = MF.CreateMachineBasicBlock();
    schedule(order, *stack, *newMBB);
    newMBB->dump();
    // Transfer new instructions to old Machine Basic Block
    MBB->clear();
    vector<MachineInstr *> instrs;
    for (MachineInstr &MI : *newMBB) {
      instrs.push_back(&MI);
    }
    for (MachineInstr *MI : instrs) {
      MI->removeFromParent();
      MBB->push_back(MI);
    }
  }
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

void TVMLocalScheduler::insertInputOperands(
    const LayeredInstructionDAGNode &node, StackTreeNode &stack,
    OperandQueue &queue, MachineBasicBlock &outMBB) {
  MachineInstr *MI = node.getInstruction();
  assert(MI);
#if 1
	dbgs() << "Stack : " << stack.toString() << "\n";
  dbgs() << "Queue :\n";
  queue.dump();
  dbgs() << "\n";
#endif
  const DebugLoc &DL = MI->getDebugLoc();
  MachineFunction &MF = *MI->getParent()->getParent();
  vector<unsigned> params;
  vector<unsigned> stackArgs;
	vector<unsigned> stackSlots;
  vector<unsigned> queueArgs;

  for (const MachineOperand &op : MI->uses()) {
    if (op.isReg()) {
      unsigned reg = op.getReg();
      params.insert(params.begin(), reg);
      int queueSlotIndex = queue.lookupIndex(reg);
      if (queueSlotIndex >= 0) {
        OperandQueue::Slot &slot = queue.getSlot(queueSlotIndex);
        if (slot.getRemainUses() > 1) {
          slot.decrementUses();
          stackArgs.insert(stackArgs.begin(), reg);
          stackSlots.push_back(queueSlotIndex);
          queue.enqueue(reg);
        } else {
          // Operand register is in queue, already in place
          queueArgs.insert(queueArgs.begin(), reg);
        }
      } else {
        // Operand register is in stack tree, use PUSH instruction
        int stackOffset = stack.lookupRegisterOffset(reg);
        assert(stackOffset >= 0);
        stackSlots.push_back(stackOffset + queue.size());
        stackArgs.insert(stackArgs.begin(), reg);
        queue.enqueue(reg);
      }
    }
  }

#if 1
  dbgs() << "Stack args: " << regListToString(stackArgs) << "\n";
  dbgs() << "Queue args: " << regListToString(queueArgs) << "\n";
  dbgs() << "Queue :\n";
  queue.dump();
  dbgs() << "\n";
#endif

  for (int idx = 0; idx < stackSlots.size(); idx++) {
    unsigned i = stackSlots[idx];
    unsigned j = idx + 1 < stackSlots.size() ? stackSlots[idx + 1] : 0;
    unsigned k = idx + 2 < stackSlots.size() ? stackSlots[idx + 2] : 0;
    if (idx + 2 < stackSlots.size()  &&  i <= 15  &&  j <= 15  &&  k <= 15) {
      MachineInstr *push3 = MF.CreateMachineInstr(TII->get(TVM::PUSH3), DL);
      push3->addOperand(MF, MachineOperand::CreateImm(i));
      push3->addOperand(MF, MachineOperand::CreateImm(j - 1));
      push3->addOperand(MF, MachineOperand::CreateImm(k - 2));
      outMBB.push_back(push3);
			idx += 2;
    } 
		else if (idx + 1 < stackSlots.size()  &&  i <= 15  &&  j <= 15) {
      MachineInstr *push2 = MF.CreateMachineInstr(TII->get(TVM::PUSH2), DL);
      push2->addOperand(MF, MachineOperand::CreateImm(i));
      push2->addOperand(MF, MachineOperand::CreateImm(j - 1));
      outMBB.push_back(push2);
			idx++;
    } 
		else {
      MachineInstr *push = MF.CreateMachineInstr(TII->get(TVM::PUSH), DL);
      push->addOperand(MF, MachineOperand::CreateImm(i));
      outMBB.push_back(push);
    }
	}

  for (unsigned idx = 0; idx < queueArgs.size(); idx++) {
    const OperandQueue::Slot &slot = queue.getSlot(idx);
    //   assert(queueArgs[idx] == slot.getReg());
    stackArgs.insert(stackArgs.begin(), queueArgs[idx]);
  }
  //  queue.dequeue(queueArgs.size());

  bool skip = false;
  if (params.size() == 2 && params[0] == stackArgs[1] &&
      params[1] == stackArgs[0]) {
    // Try to optimize commutative instructions
    if (MI->isCommutable())
      skip = true;
  }

  if (!skip && params != stackArgs) {
    // We need sun fixup to adjust mix of stack and queue arguments
    StackFixup fixup = BlockQueueInfo::calculateFixup(MF, params, stackArgs);
    StackFixup::InstructionGenerator instrGen(TII, MFI, &outMBB, outMBB.end());
    instrGen(fixup);
#if 1
    dbgs() << "Parameters: " << regListToString(params) << "\n";
    dbgs() << "Stack args: " << regListToString(stackArgs) << "\n";
    dbgs() << "Args Fixup : ";
    fixup.dump();
    dbgs() << "\n\n";
#endif
  }
  outMBB.dump();
}

void TVMLocalScheduler::distributeOutputResults(
    const LayeredInstructionDAGNode &node, StackTreeNode &stack,
    OperandQueue &queue, MachineBasicBlock &outMBB) {
  MachineInstr *MI = node.getInstruction();
  assert(MI);
  StackFixup fixup;
  for (const MachineOperand &op : MI->defs()) {
    assert(op.isReg());
    unsigned reg = op.getReg();
    unsigned succNum = node.getNumSuccessors(reg);
    queue.dequeue();
    if (succNum > 0) {
      /*
      // Duplicate result if there are several consumers
      for (unsigned i = 1; i < succNum; i++) {
        fixup(StackFixup::pushI(0)); // DUP
      }
                        */
      LayeredInstructionDAGNode *target = nullptr;
      unsigned targetPortIndex;
      unsigned maxOrder = 0;
      // Find the most distant (in time scale) target node
      for (LayeredInstructionDAGNode::Edge edge : node.successors()) {
        unsigned edgeOrder = OperandQueue::order(edge);
        if (edge.getReg() == reg) {
          if (!target || edgeOrder > maxOrder) {
            target = edge.getNode();
            targetPortIndex = edge.getInPortIndex();
            maxOrder = edgeOrder;
          }
        }
      }
      unsigned slot = queue.enqueue(target, targetPortIndex, reg, succNum);
      if (slot > 0)
        fixup(StackFixup::makeRoll(slot));
    } else {
      // There are no consumers for this result, drop it
      fixup(StackFixup::pop(0)); // DROP
    }
  }

  if (fixup.getChanges().size() > 0) {
    StackFixup::InstructionGenerator instrGen(TII, MFI, &outMBB, outMBB.end());
    instrGen(fixup);
  }

#if 1
  dbgs() << "Result Fixups :\n";
  fixup.dump();
  dbgs() << "\n";
  dbgs() << "Result Queue : \n";
  queue.dump();
  dbgs() << "\n";
  outMBB.dump();
#endif
}

void TVMLocalScheduler::schedule(
    const vector<LayeredInstructionDAGNode *> nodes, StackTreeNode &stack,
    MachineBasicBlock &outMBB) {
  if (nodes.empty())
    return;
  OperandQueue queue(&nodes[0]->getDAG());
  for (LayeredInstructionDAGNode *node : nodes) {
    MachineInstr *MI = node->getInstruction();
    //   MI->dump();
    MachineInstr *SMI = SchedulerUtils::convertToSForm(*MI, TII);
    if (SMI) {
      insertInputOperands(*node, stack, queue, outMBB);
      if (!outMBB.empty()) {
        auto it = --outMBB.end();
        // Check if we can omit SWAP instruction for arguments by using reverse
        // instruction
        bool swap = false;
        MachineInstr &lastMI = *it;
        if (lastMI.getOpcode() == TVM::XCHG_TOP) {
          if (lastMI.getOperand(0).getImm() == 1)
            swap = true;
        }
        if (swap && it != outMBB.begin()) {
          // Check if we have to consecutive SWAP at the end of Machine Block
          MachineInstr &preLastMI = *(--it);
          if (preLastMI.getOpcode() == TVM::XCHG_TOP) {
            if (preLastMI.getOperand(0).getImm() == 1) {
              lastMI.removeFromParent();
              preLastMI.removeFromParent();
              swap = false;
            }
          }
        }

        // Maybe we can optimize this place not to generate to versions of SMI
        if (swap && SchedulerUtils::getReverseOpcode(SMI->getOpcode()) >= 0) {
          outMBB.dump();
          lastMI.removeFromParent();
          SMI = SchedulerUtils::convertToSForm(*MI, TII, true);
          outMBB.dump();
        }
      }
      outMBB.push_back(SMI);
      //  dbgs() << "Queue before instruction execution :  " << queue.toString()
      //  << "\n";
      for (MachineOperand &MO : MI->uses()) {
        queue.dequeue();
      }
      for (MachineOperand &MO : MI->defs()) {
        if (MO.isReg())
          queue.enqueue(MO.getReg());
      }
      //  dbgs() << "Queue after instruction execution :  " << queue.toString()
      //  << "\n";
      distributeOutputResults(*node, stack, queue, outMBB);
    }
  }
}

MachineInstr *SchedulerUtils::convertToSForm(const MachineInstr &MI,
                                             const TargetInstrInfo *TII,
                                             bool reverse) {
  size_t NumDefs = MI.getNumDefs();
  size_t NumOperands = MI.getNumOperands();
  int NewOpcode = TVM::RegForm2SForm[MI.getOpcode()];
  if (reverse) {
    NewOpcode = SchedulerUtils::getReverseOpcode(NewOpcode);
    assert(NewOpcode >= 0);
  }

  size_t NumGlobals = llvm::count_if(MI.uses(), [](const MachineOperand &MO) {
    return MO.isGlobal() || MO.isSymbol();
  });
  size_t NumImms = llvm::count_if(MI.uses(), [](const MachineOperand &MO) {
    return MO.isImm() || MO.isCImm();
  });

  MachineFunction &MF = *(MachineFunction *)MI.getParent()->getParent();
  const DebugLoc &DL = MI.getDebugLoc();
  MachineInstr *SMI = nullptr;

  if (NewOpcode >= 0) {
    for (unsigned I = 0; I < NumGlobals; I++) {
      const auto &Op = MI.getOperand(NumDefs + I);
      assert((Op.isGlobal() || Op.isSymbol()) &&
             "Expected GlobalAddress/ExternalSymbol");
      if (NewOpcode == TVM::PUSH_GLOBAL_ADDRESS_S) {
        if (Op.isGlobal()) {
          SMI = MF.CreateMachineInstr(TII->get(TVM::PUSHCONT_LABEL), DL);
          SMI->addOperand(
              MF, MachineOperand::CreateGA(Op.getGlobal(), Op.getOffset()));
        } else {
          SMI = MF.CreateMachineInstr(TII->get(TVM::PUSHCONT_LABEL), DL);
          SMI->addOperand(MF, MachineOperand::CreateES(Op.getSymbolName()));
        }
      }
    }

    SMI = MF.CreateMachineInstr(TII->get(NewOpcode), DL);

    if (NewOpcode != TVM::PUSH_GLOBAL_ADDRESS_S) {
      for (unsigned I = 0; I < NumGlobals; I++) {
        const auto &Op = MI.getOperand(NumDefs + I);
        SMI->addOperand(Op);
      }
    }

    if (MI.getOpcode() == TVM::PUSHCONT_MBB) {
      SMI->addOperand(MI.getOperand(1));
    } else {
      for (unsigned I = 0; I < NumImms; I++) {
        const auto &Op = MI.getOperand(NumOperands - NumImms + I);
        assert(Op.isImm() || Op.isCImm() && "Expected Imm or CImm");
        if (Op.isImm())
          SMI->addOperand(MF, MachineOperand::CreateImm(Op.getImm()));
        else
          SMI->addOperand(MF, MachineOperand::CreateImm(Op.getImm()));
      }
    }
  }
  return SMI;
}

void BlockQueueInfo::dump(string margin) {
  dbgs() << margin << "Preferred Input Queue : "
         << regListToString(preferredInputQueue).c_str() << "\n"
         << margin << "Preferred Output Queue : "
         << regListToString(preferredOutputQueue).c_str() << "\n"
         << margin
         << "Actual Input Queue : " << regListToString(actualInputQueue).c_str()
         << "\n"
         << margin << "Actual Output Queue : "
         << regListToString(actualInputQueue).c_str() << "\n"
         << margin << "Number Of Uses : " << regMapToString(numUses);
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
#if 0
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
#else
  Stack srcStack(MF, src.size());
  Stack dstStack(MF, dst.size());
  for (unsigned i = 0; i < src.size(); i++) {
    srcStack.set(i, StackVreg(src[i]));
  }
  for (unsigned i = 0; i < dst.size(); i++) {
    dstStack.set(i, StackVreg(dst[i]));
  }
  return StackFixup::Diff(dstStack, srcStack);
#endif
}

static unsigned
calculatePermutations(vector<pair<unsigned, int>> const &disorder,
                      int startIndex) {
  int permutCnt = 0;
  for (unsigned idx = startIndex; idx < disorder.size(); idx++) {
    permutCnt += disorder[idx].second;
  }
  return permutCnt / 2;
}

void BlockQueueInfo::calculateMaximumCommonChain(vector<unsigned> const &queue1,
                                                 vector<unsigned> const &queue2,
                                                 vector<unsigned> &result,
                                                 unsigned maxPermutations) {
  vector<unsigned> commons;
  vector<int> commonIndexes2;
  for (unsigned idx1 = 0; idx1 < queue1.size(); idx1++) {
    unsigned val1 = queue1[idx1];
    for (unsigned idx2 = 0; idx2 < queue2.size(); idx2++) {
      if (queue2[idx2] == val1) {
        commons.push_back(val1);
        commonIndexes2.push_back(idx2);
        break;
      }
    }
  }
  unsigned permutCnt = 0;
  vector<pair<unsigned, int>> disorder;
  vector<vector<unsigned>> conflictList;
  for (int idx = 0; idx < commons.size(); idx++) {
    vector<unsigned> conflicts;
    unsigned val = commons[idx];
    int disorderCnt = 0;
    int comIdx = commonIndexes2[idx];
    for (int idx2 = 0; idx2 < commons.size(); idx2++) {
      if (idx == idx2)
        continue;
      unsigned val2 = commons[idx2];
      int comIdx2 = commonIndexes2[idx2];
      if ((idx < idx2) != (comIdx < comIdx2)) {
        disorderCnt++;
        conflicts.push_back(val2);
      }
    }
    if (disorderCnt)
      permutCnt++;
    disorder.push_back(make_pair(val, disorderCnt));
    conflictList.push_back(conflicts);
  }
  if (permutCnt == 0) {
    copyList(commons, result);
    return;
  }
  std::sort(disorder.begin(), disorder.end(),
            [](pair<unsigned, int> a, pair<unsigned, int> b) {
              return a.second > b.second;
            });
  unsigned startIndex = 0;
  for (;;) {
    permutCnt = calculatePermutations(disorder, startIndex);
    if (permutCnt == 0)
      break;
    vector<unsigned> &conflicts = conflictList[disorder[startIndex].first];
    for (unsigned idx = startIndex + 1; idx < disorder.size(); idx++) {
      unsigned val = disorder[idx].first;
      if (find(conflicts.begin(), conflicts.end(), val) != conflicts.end()) {
        disorder[idx].second--;
        disorder[startIndex].second--;
      }
    }
    assert(disorder[startIndex].second == 0);
    startIndex++;
  }
  for (unsigned idx = startIndex; idx < disorder.size(); idx++)
    result.push_back(disorder[idx].first);
}

unsigned BlockQueueInfo::calculateCommonTailLength(
    vector<vector<unsigned>> const &queues) {
  unsigned cnt = 0;
  for (;;) {
    for (vector<unsigned> const &queue : queues) {
      unsigned val = (unsigned)-1;
      if (queue.size() == cnt)
        return cnt;
      unsigned queueVal = queue[queue.size() - 1 - cnt];
      if (val == (unsigned)-1)
        val = queueVal;
      else if (queueVal != val)
        return cnt;
    }
  }
  return cnt;
}

bool BlockQueueInfo::test(MachineFunction &MF) {
  vector<unsigned> queue1{0, 1, 2, 3};
  vector<unsigned> queue2{1, 0, 2, 3};
  vector<unsigned> queue3{5, 3, 0, 1, 2, 4};
  vector<unsigned> result;
  BlockQueueInfo::calculateMaximumCommonChain(queue1, queue2, result, 1);
  dbgs() << "Result : " << regListToString(result) << "\n";

  result.clear();
  BlockQueueInfo::calculateMaximumCommonChain(queue1, queue3, result, 1);
  dbgs() << "Result : " << regListToString(result) << "\n";

  vector<unsigned> fqueue1{0, 1, 2, 3};
  vector<unsigned> fqueue2{3, 1, 2, 0};
  vector<unsigned> fqueue3{3, 2, 1, 0};
  vector<unsigned> fqueue4{2, 3, 1, 0};
  vector<unsigned> fqueue5{6, 5, 4, 3, 2, 1, 0};
  vector<unsigned> fqueue6{0};
  vector<unsigned> fqueue7{4, 5, 6};
  vector<unsigned> fqueue8{4, 5, 6, 4, 5};
  vector<unsigned> fqueue9{4, 5, 6, 4, 5, 6};

  StackFixup fixup1 = BlockQueueInfo::calculateFixup(MF, fqueue1, fqueue2);
  dbgs() << "\n";
  fixup1.dump();
  dbgs() << "\n";

  StackFixup fixup2 = BlockQueueInfo::calculateFixup(MF, fqueue1, fqueue3);
  dbgs() << "\n";
  fixup2.dump();
  dbgs() << "\n";

  StackFixup fixup3 = BlockQueueInfo::calculateFixup(MF, fqueue1, fqueue4);
  dbgs() << "\n";
  fixup3.dump();
  dbgs() << "\n";

  StackFixup fixup4 = BlockQueueInfo::calculateFixup(MF, fqueue6, fqueue5);
  dbgs() << "\n";
  fixup4.dump();
  dbgs() << "\n";

  StackFixup fixup5 = BlockQueueInfo::calculateFixup(MF, fqueue8, fqueue7);
  dbgs() << "\n";
  fixup5.dump();
  dbgs() << "\n";

  StackFixup fixup6 = BlockQueueInfo::calculateFixup(MF, fqueue9, fqueue7);
  dbgs() << "\n";
  fixup6.dump();
  dbgs() << "\n";

  return true;
}
