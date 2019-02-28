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

#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/Support/Debug.h"

#include "TVMExtras.h"
#include "TVMMachineFunctionInfo.h"

namespace llvm {

enum class StackReorderingKind { Copy, Xchg, New };
using StackElementT = std::variant<unsigned, MachineBasicBlock *>;

struct StackReordering {
  /// The register or the basic block we get data from.
  StackElementT ElemFrom;
  /// The number of slot we put data to.
  size_t SlotTo;
  /// If we copy (Push), move (Xchg) or create new element (PushCont, etc).
  StackReorderingKind ReorderingKind;
  /// Check if the reordering copies an existing element to SlotTo.
  bool isCopy() const { return ReorderingKind == StackReorderingKind::Copy; }
  /// Check if the reordering creates a new element at SlotTo.
  bool isNew() const { return ReorderingKind == StackReorderingKind::New; }
  /// Check if the reordering exchanges elements in a stack.
  bool isXchg() const { return ReorderingKind == StackReorderingKind::Xchg; }
  StackReordering(StackElementT ElemFrom, size_t SlotTo,
                  StackReorderingKind ReorderingKind)
      : ElemFrom(ElemFrom), SlotTo(SlotTo), ReorderingKind(ReorderingKind) {}
#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
  /// Allow easy printing of a stack reordering from the debugger.
  void dump() {
    // TODO: implement support for MBB when needed.
    assert(std::holds_alternative<unsigned>(ElemFrom));
    auto tag =
        isCopy() ? "Copy" : (isXchg() ? "Xchg" : (isNew() ? "New" : "Unknown"));
    dbgs() << tag << " SlotTo = " << SlotTo;
    // TODO: Implement proper register printing.
    dbgs() << ", reg = " << std::get<unsigned>(ElemFrom) << "\n";
  }
#endif
};

// Most of the instructions have at most 2 arguments.
using StackReorderings = SmallVector<StackReordering, 2>;

/// Implement the programming model of the hardware stack and keep it in sync
/// with the emitted code.
/// Provide interfaces to track positions of local variables and mutate the
/// stack.
class Stack {
public:
  Stack(const TargetInstrInfo *TII, size_t Size)
      : TII(TII), Data(Size, TVMFunctionInfo::UnusedReg) {}
  Stack(const TargetInstrInfo *TII) : TII(TII) {}
  auto begin() { return Data.begin(); }
  auto begin() const { return Data.begin(); }
  auto end() { return Data.end(); }
  auto end() const { return Data.end(); }
  /// Insert POP instructions to clean up the stack, preserving the specified
  /// element of it.
  /// \par InsertPoint specify instruction to insert after.
  /// \par Preserved virtual register needs to be kept in the stack.
  bool clear(MachineInstr *InsertPoint,
             unsigned Preserved = TVMFunctionInfo::UnusedReg);
  /// PUSH the specified slot to the specified position of the stack.
  /// Precondition: Elem is present in Data.
  /// TODO: Stack PUSH limitations aren't handled yet.
  /// \par InsertPoint specify instruction to insert after.
  /// \par Elem virtual register or basic block.
  void push(MachineInstr *InsertPoint, StackElementT Elem);
  /// Put a continuation (represented as a pointer to a basic block) on top of
  /// the stack. This operation doesn't depend on whether the element is already
  /// in stack and it never results in generation of PUSH instruction. Instead,
  /// it uses PUSHCONT.
  void pushNew(MachineInstr *InsertPoint, MachineBasicBlock &MBB);
  /// \par InsertPoint specify instruction to insert after.
  /// \par ElemFrom register or BB to be exchanged in the stack.
  /// \par SlotTo slot number to be exchanged with.
  /// Precondition: Slot number for ElemFrom != SlotTo.
  bool xchg(MachineInstr *InsertPoint, StackElementT ElemFrom, size_t SlotTo);
  /// A helper function for general xchg()
  bool xchg(MachineInstr *InsertPoint, const StackReordering &Reordering) {
    return xchg(InsertPoint, Reordering.ElemFrom, Reordering.SlotTo);
  }
  /// Remove \par Elem from the stack. Insert corresponding POP instruction
  /// after \par InsertPoint.
  /// Precondition: Elem is in the stack.
  void pop(MachineInstr &InsertPoint, const StackElementT &Elem);
  /// Return position of \par Elem in the stack.
  /// Precondition: \par Elem is in the stack.
  size_t position(StackElementT Elem) const {
    return std::distance(std::begin(Data), llvm::find_or_fail(Data, Elem));
  }
  /// Return register for \par Slot in the stack.
  /// Precondition: Slot < Data.size() && Data[Slot] is a register.
  unsigned reg(size_t Slot) const {
    assert(Slot < Data.size() && "Out of range access");
    assert(std::holds_alternative<unsigned>(Data[Slot]) &&
           "Stack doesn't contain a register at Slot");
    return std::get<unsigned>(Data[Slot]);
  }
  /// Checks if element at \par Slot is a register.
  /// Precondition: Slot < Data.size()
  unsigned isReg(size_t Slot) const {
    assert(Slot < Data.size() && "Out of range access");
    return std::holds_alternative<unsigned>(Data[Slot]);
  }
  /// Fill the specified \p Slot with \p Elem. Doesn't generate any instruction.
  void set(size_t Slot, const StackElementT &Elem) {
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
  void addDef(unsigned Reg) { Data.push_front(Reg); }
  /// Checks if specified \p Slot contains specified \p Elem.
  bool slotContains(size_t Slot, const StackElementT &Elem) const {
    assert(Slot < Data.size() && "Out of range access");
    return Data[Slot] == Elem;
  }
#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
  /// Allow easy printing of the stack from the debugger.
  void dump() {
    // TODO: Align with conventional dump methods.
    // LLVM has rules on dump(), most of the framework follows. Under debugger,
    // a user could expect to call dump on a significant part of LLVM objects.
    // See https://llvm.org/doxygen/AsmWriter_8cpp_source.html#l04297 for
    // reference.
    // TODO: add support for other than register types later
    for (const auto &elem : Data)
      dbgs() << " " << std::get<unsigned>(elem);
    dbgs() << "\n";
  }
#endif
  /// Modify \par S1 and \par S2 so that identical values are in identical
  /// positions.
  /// Insert necessary stack manipulation instructions to the end of basic
  /// blocks \par S2 came from. It should be designated by \par InsertPoint.
  static void join(Stack &S1, Stack &S2, MachineInstr &InsertPoint);
  /// TODO: we need to decide how to handle these limitations.
  /// They shouldn't be defined in this scope.
  /// Maximal N in a valid PUSH sN instruction.
  static inline constexpr size_t PushLimit = 255;
  /// Maximal N, M in a valid XCHG sN, sM instruction.
  static inline constexpr size_t XchgLimit = 15;

private:
  const TargetInstrInfo *TII;
  std::deque<StackElementT> Data;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMSTACK_H
