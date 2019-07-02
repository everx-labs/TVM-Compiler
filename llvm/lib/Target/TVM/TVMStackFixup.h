//===--- TVMStackFixup.h - Difference between 2 stack models ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMSTACKFIXUP_H
#define LLVM_LIB_TARGET_TVM_TVMSTACKFIXUP_H

#include <deque>
#include <variant>
#include <optional>

#include "llvm/CodeGen/MachineInstrBuilder.h"

#include "TVMStackVreg.h"

namespace llvm {

class Stack;
class TVMFunctionInfo;

class StackFixup {
public:
  static StackFixup Diff(const Stack &to, const Stack &from);

  static StackFixup DiffForReturn(const Stack &from,
      std::optional<unsigned> Preserved = std::optional<unsigned>());

  static StackFixup RemoveUnusedDefinitions(MachineInstr &MI, Stack &stack);
  static StackFixup PruneDeadDefinitions(MachineInstr &MI, Stack &stack);

  void apply(Stack &stack) const;

  // Remove one copy of this elem
  void removeElem(Stack &stack, const StackVreg &vreg);
  // Remove N copies of this elem
  void removeNElem(Stack &stack, const StackVreg &vreg, size_t N);
  // Remove all copies of this elem
  void removeAllElem(Stack &stack, const StackVreg &vreg);

  struct drop {};
  struct nip {};
  struct swap {};
  struct xchgTop {
    explicit xchgTop(unsigned i) : i (i) {
      assert(i <= XchgDeepLimit && "Unimplemented");
    }
    unsigned i;
  };
  struct xchg {
    explicit xchg(unsigned i, unsigned j)
      : i(std::min(i, j))
      , j(std::max(i, j)) {
      assert(i <= XchgLimit && "Unimplemented");
      assert(j <= XchgLimit && "Unimplemented");
    }
    unsigned i;
    unsigned j;
  };
  struct dup {};
  struct pushI {
    explicit pushI(unsigned i) : i (i) {
      assert(i <= PushLimit && "Unimplemented");
    }
    unsigned i;
  };
  struct pushZero {};
  typedef std::variant<drop, nip, xchgTop, xchg, dup, pushI, pushZero> Change;

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
  void dump() const;
  void print(raw_ostream &OS) const;
  void printElem(raw_ostream &OS, const Change &change) const;
#endif

  /// TODO: we need to decide how to handle these limitations.
  /// They shouldn't be defined in this scope.
  /// Maximal N in a valid PUSH sN instruction.
  static inline constexpr size_t PushLimit = 255;
  /// Maximal N, M in a valid XCHG sN, sM instruction.
  static inline constexpr size_t XchgLimit = 15;
  static inline constexpr size_t XchgDeepLimit = 255;
  static inline constexpr size_t BlkdropImmLimit = 15;

  class InstructionGenerator {
  public:
    InstructionGenerator(const TargetInstrInfo *TII, TVMFunctionInfo *MFI,
                         MachineBasicBlock *MBB,
                         MachineBasicBlock::iterator InsertPt)
      : TII(TII), MFI(MFI), MBB(MBB), InsertPt(InsertPt) {
      if (InsertPt != MBB->end())
        DL = InsertPt->getDebugLoc();
    }
    MachineInstr *operator()(const std::pair<Change, std::string> &v) const;

    void operator()(const StackFixup &v) const;
  private:
    const TargetInstrInfo *TII;
    TVMFunctionInfo *MFI;
    MachineBasicBlock *MBB;
    MachineBasicBlock::iterator InsertPt;
    DebugLoc DL;
  };

  const std::vector<std::pair<Change, std::string>> &getChanges() const {
    return Changes;
  }
private:
  Change operator()(const Change &change) {
    Changes.push_back(make_pair(change, std::string()));
    return change;
  }
  void operator()(const Stack &stack);

  void setLastComment(const std::string &comment) {
    Changes.back().second = comment;
  }
  std::vector<std::pair<Change, std::string>> Changes;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMSTACKFIXUP_H
