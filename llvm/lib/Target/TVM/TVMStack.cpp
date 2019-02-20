//===----------- TVMStack.cpp - Model of stack used in TVM ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
/// \file
/// Define classes to manipulate with program model of TVM stack.
//
//===----------------------------------------------------------------------===//

#include "TVMStack.h"

#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "TVMUtilities.h"

namespace llvm {

bool Stack::clear(MachineInstr *InsertPoint, unsigned Preserved) {
  auto It = llvm::find(Data, Preserved);
  size_t NumDrops = 0, NumNips = 0;
  if (It == std::end(Data)) {
    NumDrops = Data.size();
  } else {
    NumDrops = std::distance(std::begin(Data), It);
    NumNips = Data.size() - NumDrops - 1;
  }
  unsigned Opc = TVM::POP;
  // DROPs
  for (size_t i = 0; i < NumDrops; ++i)
    BuildMI(InsertPoint, TII->get(Opc)).addImm(0);
  // NIPs
  for (size_t i = 0; i < NumNips; ++i)
    BuildMI(InsertPoint, TII->get(Opc)).addImm(1);
  if (It == std::end(Data))
    Data.clear();
  else
    Data = {Data[NumDrops]};
  return NumDrops && NumNips;
}

void Stack::push(MachineInstr *InsertPoint, StackElementT Elem) {
  size_t ElemSlot = position(Elem);
  assert(ElemSlot <= PushLimit && "Unimplemented");
  if (InsertPoint != nullptr)
    BuildMI(InsertPoint, TII->get(TVM::PUSH)).addImm(ElemSlot);
  Data.push_front(Data[ElemSlot]);
}

void Stack::pushNew(MachineInstr *InsertPoint, MachineBasicBlock &MBB) {
  if (InsertPoint != nullptr)
    BuildMI(InsertPoint, TII->get(TVM::PUSHCONT_MBB)).addMBB(&MBB);
  Data.push_front(&MBB);
}

bool Stack::xchg(MachineInstr *InsertPoint, StackElementT ElemFrom,
                 size_t SlotTo) {
  auto It = llvm::find_or_fail(Data, ElemFrom);
  size_t ElemFromSlot = std::distance(std::begin(Data), It);
  assert(ElemFromSlot <= XchgLimit && "Unimplemented");
  if (Data[SlotTo] == ElemFrom)
    return false;
  if (InsertPoint != nullptr)
    BuildMI(InsertPoint, TII->get(TVM::XCHG))
        .addImm(std::min(ElemFromSlot, SlotTo))
        .addImm(std::max(ElemFromSlot, SlotTo));
  std::swap(*It, Data[SlotTo]);
  return true;
}

} // namespace llvm
