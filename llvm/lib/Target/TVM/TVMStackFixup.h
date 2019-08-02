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
#include <sstream>

#include "llvm/CodeGen/MachineInstrBuilder.h"

#include "TVMStackVreg.h"

namespace llvm {

class Stack;
class TVMFunctionInfo;
class MIArgs;

/// A patch to be applied to a stack.
/// There are two clients for the fixup:
/// * Stack performs changes in the model
/// * InstructionGenerator inserts corresponding instructions to the code
class StackFixup {
public:
  /// Prepare universal diff between two stack states
  ///  (used for stack merging at jumps).
  /// * After fixup applied to \par Src, the stack must become exactly \par Dst
  ///   (all normal Vregs should be at the same positions).
  /// * Unused values ('x') in \par Dst pattern must be filled with some values.
  /// * Unused values ('x') in \par Src pattern may be relocated to fill
  ///    \par Dst unused values, or may be deleted freely.
  static StackFixup Diff(const Stack &Dst, const Stack &Src);
  /// Prepare diff for return (clean this function's stack, keep only ret val).
  /// \par Src - current stack.
  /// \par Preserved - Vreg for return value.
  static StackFixup DiffForReturn(const Stack &Src,
      std::optional<unsigned> Preserved = std::optional<unsigned>());
  /// Prepare diff for machine instruction arguments:
  ///  * Arguments must exist at their positions at stack top.
  ///  * Non-killed arguments must have their duplicates in stack deep
  ///     (outside arguments region).
  ///  * Other stack elements may be relocated in stack freely.
  ///  * Unused elements may be deleted/replaced freely.
  /// \par Src - current stack.
  /// \par Args - Argument Vregs with kill markers.
  /// \par IsCommutative - INSTR s0, s1 == INSTR s1, s0.
  static StackFixup DiffForArgs(const Stack &Src, const MIArgs &Args,
                                bool IsCommutative = false);

  void apply(Stack &stack) const;

  // Remove one copy of this elem
  void removeElem(Stack &stack, const StackVreg &vreg);
  // Remove N copies of this elem
  void removeNElem(Stack &stack, const StackVreg &vreg, size_t N);
  // Remove all copies of this elem
  void removeAllElem(Stack &stack, const StackVreg &vreg);

  struct drop {};
  struct nip {};
  struct xchgTop {
    explicit xchgTop(unsigned i) : i (i) {
      assert(i <= XchgDeepLimit && "Unimplemented");
    }
    bool operator == (const xchgTop &v) const { return i == v.i; }
    unsigned i;
  };
  struct swap : xchgTop {
    swap() : xchgTop(1) {}
  };
  struct xchg {
    xchg(unsigned i, unsigned j)
      : i(std::min(i, j))
      , j(std::max(i, j)) {
      assert(i <= XchgLimit && "Unimplemented");
      assert(j <= XchgLimit && "Unimplemented");
    }
    bool operator == (const xchg &v) const { return i == v.i && j == v.j; }
    unsigned i;
    unsigned j;
  };
  struct pushI {
    explicit pushI(unsigned i) : i (i) {
      assert(i <= PushLimit && "Unimplemented");
    }
    unsigned i;
  };
  struct dup : pushI {
    dup() : pushI(0) {}
  };
  struct pushUndef {};
  struct blkswap {
    blkswap(unsigned deepSz, unsigned topSz) : deepSz(deepSz), topSz(topSz) {
      assert(deepSz >= 1 || topSz >= 1 && "Wrong size in blkswap");
    }
    bool isImm() const { return goodFor(deepSz, topSz); }
    // If blkswap with such arguments may be prepared with immediate args
    //  (without additional argument pushes).
    static bool goodFor(unsigned deepSz, unsigned topSz) {
      assert((deepSz >= 1 || topSz >= 1) && "Wrong size in blkswap");
      return (deepSz - 1 <= BlkswapImmLimit) && (topSz - 1 <= BlkswapImmLimit);
    }
    unsigned deepSz;
    unsigned topSz;
  };
  struct roll {
    explicit roll(int i) : i(i) {
      assert(i != 0 && "Roll for top elem");
    }
    bool isImmRoll() const {
      return static_cast<unsigned>(std::abs(i)) <= RollImmLimit + 1;
    }
    int i;
  };
  struct reverse {
    reverse(unsigned deepSz, unsigned topIdx)
        : deepSz(deepSz), topIdx(topIdx) {
      assert(deepSz >= 2 && "Wrong size in reverse");
    }
    bool isImm() const {
      return deepSz - 2 <= ReverseImmLimit && topIdx <= ReverseImmLimit;
    }
    unsigned deepSz;
    unsigned topIdx;
  };
  struct blkdrop {
    explicit blkdrop(unsigned sz) : sz(sz) {
      assert(sz >= 0 && "blkdrop for null size");
    }
    bool isImm() const {
      return sz <= BlkdropImmLimit;
    }
    unsigned sz;
  };
  struct doubleChange {
    // true - push, false - xchg
    doubleChange(bool p1, bool p2, unsigned i, unsigned j)
      : p{ p1, p2 }, args{ i, j } {
      assert(i != j && "wrong doubleChange");
    }
    bool isXchg(unsigned i) const { return !p[i]; }
    bool isPush(unsigned i) const { return p[i]; }
    unsigned countXchgs() const { return llvm::count(p, false); }
    unsigned countPushes() const { return llvm::count(p, true); }
    unsigned code() const {
      return ((p[0] ? 1 : 0) << 1) |
              (p[1] ? 1 : 0);
    }
    std::string codeName() const {
      std::ostringstream Oss;
      for (bool IsPush : p)
        Oss << (IsPush ? "p" : "x");
      return Oss.str();
    }
    bool p[2];
    unsigned args[2];
  };
  struct tripleChange {
    // true - push, false - xchg
    tripleChange(bool p1, bool p2, bool p3, unsigned i, unsigned j, unsigned k)
      : p{ p1, p2, p3 }, args{ i, j, k } {
      assert(i != j && j != k && i != k && "wrong tripleChange");
    }
    bool isXchg(unsigned i) const { return !p[i]; }
    bool isPush(unsigned i) const { return p[i]; }
    unsigned countXchgs() const { return llvm::count(p, false); }
    unsigned countPushes() const { return llvm::count(p, true); }
    unsigned code() const {
      return ((p[0] ? 1 : 0) << 2) |
             ((p[1] ? 1 : 0) << 1) |
              (p[2] ? 1 : 0);
    }
    std::string codeName() const {
      std::ostringstream Oss;
      for (bool IsPush : p)
        Oss << (IsPush ? "p" : "x");
      return Oss.str();
    }
    bool p[3];
    unsigned args[3];
  };
  using Change = std::variant<drop, nip, xchgTop, xchg, pushI, pushUndef,
      blkswap, blkdrop, roll, reverse, doubleChange, tripleChange>;
  using ChangesVec = std::vector<std::pair<Change, std::string>>;

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
  void dump() const;
  void print(raw_ostream &OS) const;
  void printElem(raw_ostream &OS, const Change &change) const;
  friend raw_ostream& operator<<(raw_ostream &OS, const StackFixup &V) {
    V.print(OS);
    return OS;
  }
#endif

  /// TODO: we need to decide how to handle these limitations.
  /// They shouldn't be defined in this scope.
  /// Maximal N in a valid PUSH sN instruction.
  static inline constexpr size_t PushLimit = 255;
  /// Maximal N, M in a valid XCHG sN, sM instruction.
  static inline constexpr size_t XchgLimit = 15;
  static inline constexpr size_t XchgDeepLimit = 255;
  static inline constexpr size_t BlkdropImmLimit = 15;
  static inline constexpr size_t BlkswapImmLimit = 15;
  static inline constexpr size_t RollImmLimit = 15;
  static inline constexpr size_t ReverseImmLimit = 15;

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

  const ChangesVec &getChanges() const {
    return Changes;
  }
private:
  void optimizeEqualXchgs();
  void optimizeBlkswap();
  void optimize(bool IsCommutative = false);

  static Change makeRoll(unsigned deepElem);
  static Change makeRollRev(unsigned toDeepElem);
  static Change makeBlkSwap(unsigned deepElems, unsigned topElems);
  static Change makeReverse(unsigned Sz);
  static Change makeBlkdrop(unsigned Sz);
  static void generateXchgs(StackFixup &rv, const Stack &from, const Stack &to);
  static void generateVagonXchgs(StackFixup &rv, const Stack &from,
                                 const Stack &to);

  Change operator()(const Change &change) {
    Changes.push_back(make_pair(change, std::string()));
    return change;
  }
  void operator()(const Stack &stack);

  void setLastComment(const std::string &comment) {
    Changes.back().second = comment;
  }
  ChangesVec Changes;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMSTACKFIXUP_H
