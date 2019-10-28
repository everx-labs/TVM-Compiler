//===--------- TVMStack.h - Model of stack used in TVM ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
/// \file
/// Declare interfaces to manipulate with program model of TVM stack.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMSTACK_H
#define LLVM_LIB_TARGET_TVM_TVMSTACK_H

#include <deque>
#include <variant>

#include "llvm/CodeGen/LiveIntervals.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/Support/Debug.h"

#include "TVMExtras.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMStackVreg.h"
#include "TVMStackFixup.h"

namespace llvm {

/// Hold arguments of a machine instruction.
struct MIArg {
  MIArg(StackVreg Vreg, bool IsKilled) : Vreg(Vreg), IsKilled(IsKilled) {}
  bool operator == (const MIArg &v) const {
    return Vreg == v.Vreg && IsKilled == v.IsKilled;
  }
  StackVreg Vreg;
  bool IsKilled;
};
class MIArgs {
public:
  MIArgs() = default;
  MIArgs(MachineInstr &MI, const LiveIntervals &LIS, SlotIndex LIIndex);
  size_t size() const { return Args.size(); }
  const SmallVector<MIArg, 4> &getArgs() const { return Args; }
private:
  SmallVector<MIArg, 4> Args;
};

/// Implement the programming model of the hardware stack and keep it in sync
/// with the emitted code.
/// Provide interfaces to track positions of local variables and mutate the
/// stack.
class Stack {
public:
  typedef std::deque<StackVreg> StackDeq;

  Stack(MachineFunction &MF, size_t Size);

  Stack &operator += (const StackFixup::Change &change);

  Stack operator + (const StackFixup &fixup) const;

  auto size() const { return Data.size(); }

  bool operator == (const Stack &v) const { return Data == v.Data; }
  bool operator != (const Stack &v) const { return Data != v.Data; }

  const StackVreg &operator[](unsigned i) const { return Data[i]; }

  /// Return a copy of stack with \par Args added on top.
  /// When fixup is calculated we need to compare the stack after final
  /// non-terminator instruction and the pre-calculated final stack with
  /// terminator args on top of that.
  Stack withArgs(const MIArgs &Args) const;

  /// Return a copy of stack with all registers but \par MBB live-is.
  /// replaced by the unused register.
  Stack filteredByLiveIns(MachineBasicBlock &MBB,
                          const LiveIntervals &LIS) const;
  /// Return a copy of stack with all registers but \par MBB live-outs.
  /// replaced by the unused register.
  Stack filteredByLiveOuts(MachineBasicBlock &MBB,
                           const LiveIntervals &LIS) const;
  // Replace existing regs, defined in this MI (and not used) by UnusedReg
  Stack filteredByMIdefs(const MachineInstr &MI) const;

  void filterByDeadDefs(MachineInstr &MI);

  /// Replace regs, absent in TheStack, by UnusedReg (for undefs)
  void filterByImpDefs(const Stack &TheStack);

  /// Occupy unused space in stack (filled with unused registers) with \par
  /// Regs.
  void fillUnusedRegs(SmallVector<StackVreg, 16> &Regs);

  auto begin() { return Data.begin(); }
  auto begin() const { return Data.begin(); }
  auto end() { return Data.end(); }
  auto end() const { return Data.end(); }

  /// Return position of \par Elem in the stack.
  /// Precondition: \par Elem is in the stack.
  size_t position(const StackVreg& Elem) const {
    return std::distance(std::begin(Data), llvm::find_or_fail(Data, Elem));
  }
  size_t position(size_t From, const StackVreg& Elem) const {
    return std::distance(std::begin(Data),
                         find_or_fail(drop_begin(Data, From), Elem));
  }
  size_t position(unsigned Reg) const {
    return std::distance(std::begin(Data), llvm::find_or_fail(Data, Reg));
  }
  /// Return position of \par N occurrence of \par Elem from stack deep
  /// \par N - starts from 0
  size_t positionNrev(const StackVreg &Elem, int N) const {
    auto it = std::find_if(Data.rbegin(), Data.rend(),
                           [&](const StackVreg &CurElem) {
      return (Elem == CurElem) && (N-- == 0);
    });
    assert(it != Data.rend());
    return Data.size() - 1 - (it - Data.rbegin());
  }
  /// Return register for \par Slot in the stack.
  /// Precondition: Slot < Data.size() && Data[Slot] is a register.
  unsigned reg(size_t Slot) const {
    assert(Slot < Data.size() && "Out of range access");
    return Data[Slot].VirtReg;
  }
  /// Fill the specified \p Slot with \p Elem.
  void set(size_t Slot, const StackVreg &Elem) {
    assert(Slot < Data.size() && "Out of range access");
    Data[Slot] = Elem;
  }
  /// Remove arguments an instruction consumed from the stack.
  /// Precondition: Stack has enough Slots to consume.
  void consumeArguments(size_t NumSlots) {
    assert(NumSlots <= Data.size());
    Data.erase(std::begin(Data), std::begin(Data) + NumSlots);
  }
  /// Pushes result of an instruction to the stack.
  void addDef(unsigned Reg, const DILocalVariable *DbgVar);
  /// Checks if specified \p Slot contains specified \p Elem.
  bool slotContains(size_t Slot, const StackVreg &Elem) const {
    assert(Slot < Data.size() && "Out of range access");
    return Data[Slot] == Elem;
  }
  /// Checks if specified \p Elem present.
  bool exist(const StackVreg &Elem) const {
    return llvm::exist(Data, Elem);
  }
  size_t count(const StackVreg &Elem) const {
    return llvm::count(Data, Elem);
  }
  void print(raw_ostream &OS) const;
  void printElement(raw_ostream &OS, const StackVreg &Vreg) const;
  std::string toString() const;
#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
  /// Allow easy printing of the stack from the debugger.
  void dump() const;
#endif

  StackFixup operator-(const Stack &v) const;
private:
  friend class StackFixup;
  const TargetRegisterInfo *TRI;
  const MachineRegisterInfo *MRI;
  StackDeq Data;
};

//===----------------------------------------------------------------------===//
// Debugging Support

inline raw_ostream& operator<<(raw_ostream &OS, const Stack &V) {
  V.print(OS);
  return OS;
}

} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMSTACK_H
