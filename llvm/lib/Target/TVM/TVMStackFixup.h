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
#include <optional>
#include <sstream>
#include <variant>

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
  static StackFixup
  DiffForReturn(const Stack &Src,
                std::optional<unsigned> Preserved = std::optional<unsigned>());
  /// Prepare diff for return with many values
  ///  (clean this function's stack, keep only return values).
  /// \par Src - current stack.
  /// \par RetRegs - Vregs for return values.
  static StackFixup DiffForReturnMulti(const Stack &Src,
                                       ArrayRef<unsigned> RetRegs);
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

  static StackFixup DiffForHiddenStack(const Stack &Src, size_t Element,
                                       unsigned OutRegister);

  void apply(Stack &stack) const;

  // Remove one copy of this elem
  void removeElem(Stack &stack, const StackVreg &vreg);
  // Remove N copies of this elem
  void removeNElem(Stack &stack, const StackVreg &vreg, size_t N);
  // Remove all copies of this elem
  void removeAllElem(Stack &stack, const StackVreg &vreg);

  struct pop {
    pop(unsigned i) : i(i) {}
    unsigned i;
  };
  struct xchgTop {
    explicit xchgTop(unsigned i, bool checkLimits = true) : i(i) {
      if (checkLimits) {
        assert(i <= XchgDeepLimit && "Unimplemented");
      }
    }
    bool operator==(const xchgTop &v) const { return i == v.i; }
    unsigned i;
  };
  struct swap : xchgTop {
    swap() : xchgTop(1) {}
  };
  struct xchg {
    xchg(unsigned i, unsigned j, bool checkLimits = true)
        : i(std::min(i, j)), j(std::max(i, j)) {
      if (checkLimits) {
        assert(i <= XchgLimit && "Unimplemented");
        assert(j <= XchgLimit && "Unimplemented");
      }
    }
    bool operator==(const xchg &v) const { return i == v.i && j == v.j; }
    unsigned i;
    unsigned j;
  };
  struct pushI {
    explicit pushI(unsigned i, bool checkLimits = true) : i(i) {
      if (checkLimits) {
        assert(i <= PushLimit && "Unimplemented");
      }
    }
    bool isImm() const { return i <= PushLimit; }
    unsigned i;
  };
  struct pushHidden : pushI {
    explicit pushHidden(unsigned i, unsigned reg, bool checkLimits = true)
        : pushI(i, checkLimits), reg(reg) {}
    unsigned i;
    unsigned reg;
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
  struct rot : blkswap {
    rot() : blkswap(1, 2) {}
  };
  struct roll {
    explicit roll(int i) : i(i) { assert(i != 0 && "Roll for top elem"); }
    bool isImmRoll() const {
      return static_cast<unsigned>(std::abs(i)) <= RollImmLimit + 1;
    }
    int i;
  };
  struct reverse {
    reverse(unsigned deepSz, unsigned topIdx) : deepSz(deepSz), topIdx(topIdx) {
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
    bool isImm() const { return sz <= BlkdropImmLimit; }
    unsigned sz;
  };
  struct doubleChange {
    // true - push, false - xchg
    doubleChange(bool p1, bool p2, unsigned i, unsigned j)
        : p{p1, p2}, args{i, j} {}
    bool isXchg(unsigned i) const { return !p[i]; }
    bool isPush(unsigned i) const { return p[i]; }
    unsigned countXchgs() const { return llvm::count(p, false); }
    unsigned countPushes() const { return llvm::count(p, true); }
    unsigned code() const { return ((p[0] ? 1 : 0) << 1) | (p[1] ? 1 : 0); }
    std::string codeName() const {
      std::ostringstream Oss;
      for (bool IsPush : p)
        Oss << (IsPush ? "p" : "x");
      return Oss.str();
    }
    bool p[2];
    unsigned args[2];
  };
  struct xchg2 : doubleChange {
    xchg2(unsigned i, unsigned j) : doubleChange(false, false, i, j) {}
  };
  struct push2 : doubleChange {
    push2(unsigned i, unsigned j) : doubleChange(true, true, i, j) {}
  };
  struct xcpu : doubleChange {
    xcpu(unsigned i, unsigned j) : doubleChange(false, true, i, j) {}
  };
  struct puxc : doubleChange {
    puxc(unsigned i, unsigned j) : doubleChange(true, false, i, j) {}
  };
  struct tripleChange {
    // true - push, false - xchg
    tripleChange(bool p1, bool p2, bool p3, unsigned i, unsigned j, unsigned k)
        : p{p1, p2, p3}, args{i, j, k} {}
    bool isXchg(unsigned i) const { return !p[i]; }
    bool isPush(unsigned i) const { return p[i]; }
    unsigned countXchgs() const { return llvm::count(p, false); }
    unsigned countPushes() const { return llvm::count(p, true); }
    unsigned code() const {
      return ((p[0] ? 1 : 0) << 2) | ((p[1] ? 1 : 0) << 1) | (p[2] ? 1 : 0);
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
  // xxx
  struct xchg3 : tripleChange {
    xchg3(unsigned i, unsigned j, unsigned k)
        : tripleChange(false, false, false, i, j, k) {}
  };
  // ppp
  struct push3 : tripleChange {
    push3(unsigned i, unsigned j, unsigned k)
        : tripleChange(true, true, true, i, j, k) {}
  };
  // xxp
  struct xc2pu : tripleChange {
    xc2pu(unsigned i, unsigned j, unsigned k)
        : tripleChange(false, false, true, i, j, k) {}
  };
  // xpx
  struct xcpuxc : tripleChange {
    xcpuxc(unsigned i, unsigned j, unsigned k)
        : tripleChange(false, true, false, i, j, k) {}
  };
  // xpp
  struct xcpu2 : tripleChange {
    xcpu2(unsigned i, unsigned j, unsigned k)
        : tripleChange(false, true, true, i, j, k) {}
  };
  // pxx
  struct puxc2 : tripleChange {
    puxc2(unsigned i, unsigned j, unsigned k)
        : tripleChange(true, false, false, i, j, k) {}
  };
  // pxp
  struct puxcpu : tripleChange {
    puxcpu(unsigned i, unsigned j, unsigned k)
        : tripleChange(true, false, true, i, j, k) {}
  };
  // ppx
  struct pu2xc : tripleChange {
    pu2xc(unsigned i, unsigned j, unsigned k)
        : tripleChange(true, true, false, i, j, k) {}
  };
  using Change =
      std::variant<pop, xchgTop, xchg, pushI, pushHidden, pushUndef, blkswap,
                   blkdrop, roll, reverse, doubleChange, tripleChange>;
  using ChangesVec = std::vector<std::pair<Change, std::string>>;

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
  void dump() const;
  void print(raw_ostream &OS) const;
  void printElem(raw_ostream &OS, const Change &change) const;
  friend raw_ostream &operator<<(raw_ostream &OS, const StackFixup &V) {
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

  const ChangesVec &getChanges() const { return Changes; }
  Change operator()(const Change &change) {
    Changes.push_back(make_pair(change, std::string()));
    return change;
  }
  void operator()(const Stack &stack);

  void annotate(const Stack &stack);

  static Change makeRoll(unsigned deepElem);
  static Change makeRollRev(unsigned toDeepElem);
  static Change makeBlkSwap(unsigned deepElems, unsigned topElems);
  static Change makeReverse(unsigned Sz);
  static Change makeBlkdrop(unsigned Sz);

private:
  void optimizeEqualXchgs();
  void optimize(bool IsCommutative = false);

  static void generateXchgs(StackFixup &rv, const Stack &from, const Stack &to);
  static void generateVagonXchgs(StackFixup &rv, const Stack &from,
                                 const Stack &to);

  void setLastComment(const std::string &comment) {
    Changes.back().second = comment;
  }
  ChangesVec Changes;
}; // namespace llvm

} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMSTACKFIXUP_H
