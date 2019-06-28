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

#include <algorithm>

#include "TVMSubtarget.h"
#include "TVMUtilities.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/DebugInfoMetadata.h"

namespace llvm {

Stack::Stack(MachineFunction &MF, size_t Size)
    : TII(MF.getSubtarget<TVMSubtarget>().getInstrInfo()),
      TRI(MF.getSubtarget<TVMSubtarget>().getRegisterInfo()),
      MRI(&MF.getRegInfo()), MFI(MF.getInfo<TVMFunctionInfo>()),
      Data(Size, StackVreg(TVMFunctionInfo::UnusedReg)) {}

bool Stack::clear(MachineInstr *InsertPoint, unsigned Preserved) {
  auto It = llvm::find(Data, StackVreg(Preserved));
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

void Stack::addDef(unsigned Reg, const DILocalVariable *DbgVar) {
  Data.push_front(StackVreg(Reg, DbgVar));
}

void Stack::push(MachineInstr *InsertPoint, StackElementT Elem) {
  size_t ElemSlot = position(Elem);
  assert(ElemSlot <= PushLimit && "Unimplemented");
  Data.push_front(Data[ElemSlot]);
  if (InsertPoint != nullptr) {
    auto *MI =
        BuildMI(InsertPoint, TII->get(TVM::PUSH)).addImm(ElemSlot).getInstr();
    MFI->addStackModelComment(MI, toString());
  }
}

bool Stack::xchg(MachineInstr *InsertPoint, StackElementT ElemFrom,
                 size_t SlotTo) {
  auto It = llvm::find_or_fail(Data, ElemFrom);
  size_t ElemFromSlot = std::distance(std::begin(Data), It);
  assert(ElemFromSlot <= XchgLimit && "Unimplemented");
  if (Data[SlotTo] == ElemFrom)
    return false;
  std::swap(*It, Data[SlotTo]);
  if (InsertPoint != nullptr) {
    auto *MI = BuildMI(InsertPoint, TII->get(TVM::XCHG))
                   .addImm(std::min(ElemFromSlot, SlotTo))
                   .addImm(std::max(ElemFromSlot, SlotTo))
                   .getInstr();
    MFI->addStackModelComment(MI, toString());
  }
  return true;
}

void Stack::pop(MachineInstr &InsertPoint, const StackElementT &Elem) {
  auto It = llvm::find_or_fail(Data, Elem);
  size_t Slot = std::distance(std::begin(Data), It);
  Data.erase(It);
  auto *MI = BuildMI(&InsertPoint, TII->get(TVM::POP)).addImm(Slot).getInstr();
  MFI->addStackModelComment(MI, toString());
}

void Stack::join(Stack &S1, Stack &S2, MachineInstr &InsertPoint) {
  size_t Slot = 0;
  auto &Data1 = S1.Data, Data2 = S2.Data;
  size_t Size = Data1.size();
  assert(&InsertPoint == &InsertPoint.getParent()->back() &&
         "Stack manipulations are supposed to be inserted at the end of a "
         "basic block");
  assert(Size == Data2.size() && "Not implemented");
  while (Slot < Size) {
    S2.xchg(&InsertPoint, Data1[Slot], Slot);
    ++Slot;
  }
}

void Stack::print(raw_ostream &OS) const {
  OS << "{ ";
  if (!Data.empty()) {
    printElement(OS, *Data.begin());
    llvm::for_each(drop_begin(Data, 1), [&OS, this](const StackElementT &Elem) {
      OS << " | ";
      printElement(OS, Elem);
    });
  }
  OS << " }";
}

std::string Stack::toString() const {
  std::string buf;
  raw_string_ostream os(buf);
  print(os);
  os.flush();
  return buf;
}

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
/// Allow easy printing of the stack from the debugger.
void Stack::dump() {
  print(dbgs());
  dbgs() << "\n";
  dbgs().flush();
}
#endif

void Stack::printElement(raw_ostream &OS, const StackElementT &Elem) const {
  std::visit(overloaded{[this, &OS](const StackVreg &Vreg) {
                          OS << printReg(Vreg.VirtReg, TRI, 0, MRI);
                          if (Vreg.DbgVar) {
                            OS << "(" << Vreg.DbgVar->getName() << ")";
                          }
                        },
                        [&OS](MachineBasicBlock *mbb) {
                          OS << "bb." << mbb->getNumber();
                          if (const auto *BB = mbb->getBasicBlock())
                            if (BB->hasName())
                              OS << "." << BB->getName();
                        }},
             Elem);
}

} // namespace llvm
