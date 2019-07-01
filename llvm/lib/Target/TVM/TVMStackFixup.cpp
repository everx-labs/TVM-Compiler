//===------- TVMStackFixup.cpp - Difference between 2 stack models --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "TVMStackFixup.h"
#include "TVMStack.h"

#include <algorithm>

#include "TVMSubtarget.h"
#include "TVMExtras.h"
#include "TVMUtilities.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/DebugInfoMetadata.h"

namespace llvm {

StackFixup StackFixup::Diff(const Stack &to, const Stack &from) {
  StackFixup rv;

  Stack curStack(from);

  Stack::StackDeq splitFrom(from.begin(), from.end());
  auto delVregsIt = llvm::partition(splitFrom, [&to](const StackVreg &vreg) {
    return llvm::find(to, vreg) != to.end();
  });
  auto keepVregs = llvm::make_range(splitFrom.begin(), delVregsIt);
  auto delVregs = llvm::make_range(delVregsIt, splitFrom.end());

  // Sorting regs-to-delete by position from top of the stack
  llvm::sort(delVregs.begin(), delVregs.end(),
             [&from](const StackVreg &L, const StackVreg &R) {
    return from.position(L) < from.position(R);
  });

  // Generate changes to delete unused vregs
  auto removeElem = [&](const StackVreg &vreg) {
    rv.removeElem(curStack, vreg);
  };
  llvm::for_each(delVregs, removeElem);

  // Generate changes to insert copies (pushes)
  llvm::sort(keepVregs.begin(), keepVregs.end());
  auto uniqueEnd = std::unique(keepVregs.begin(), keepVregs.end());
  keepVregs = make_range(keepVregs.begin(), uniqueEnd);

  auto addElem = [&](const StackVreg &vreg) {
    auto i = curStack.position(vreg);
    if (i == 0)
      rv(curStack += rv(dup()));
    else
      rv(curStack += rv(pushI(i)));
  };

  llvm::for_each(keepVregs, [&](const StackVreg &vreg) {
    auto pushes = llvm::count(to, vreg) - llvm::count(from, vreg);
    if (!pushes)
      return;
    if (pushes > 0) {
      for (unsigned n = 0; n < pushes; ++n)
        addElem(vreg);
    } else {
      for (unsigned n = 0; n < -pushes; ++n)
        removeElem(vreg);
    }
  });

  // Generate changes to re-order
  assert(llvm::size(to) == llvm::size(curStack));

  for (unsigned n = 0; n < llvm::size(to); ++n) {
    if (to[n] != curStack[n]) {
      auto i = curStack.position(n, to[n]);
      rv(curStack += rv(xchg(i, n)));
    }
  }

  return rv;
}

StackFixup StackFixup::DiffForReturn(const Stack &from,
                                     std::optional<unsigned> Preserved) {
  StackFixup rv;
  Stack curStack(from);
  auto numPops = llvm::size(from);
  if (Preserved) {
    --numPops;
    size_t i = from.position(StackVreg(*Preserved));
    size_t j = llvm::size(from) - 1;
    if (i != j)
      rv(curStack += rv(xchg(i, j)));
  }
  for (unsigned i = 0; i < numPops; ++i)
    rv(curStack += drop());
  return rv;
}

// If one of defined register exists in stack, this value will be obsolete
//  after this instruction.
// So we need to eliminate all occurancies of this register except some,
//  required in this MI uses.
StackFixup StackFixup::RemoveUnusedDefinitions(MachineInstr &MI, Stack &stack) {
  StackFixup rv;
  for (MachineOperand &Op : MI.defs()) {
    StackVreg vreg(Op.getReg());
    size_t Uses = llvm::count_if(MI.uses(), [&](const MachineOperand &Use) {
      return Use.isReg() && Op.isReg() && Op.getReg() == Use.getReg();
    });
    if (!Uses) {
      rv.removeNElem(stack, vreg, stack.count(vreg));
    } else {
      auto existing = stack.count(vreg);
      if (existing > Uses)
        rv.removeNElem(stack, vreg, existing - Uses);
    }
  }
  return rv;
}

StackFixup StackFixup::PruneDeadDefinitions(MachineInstr &MI, Stack &stack) {
  StackFixup rv;
  for (const auto &MO : MI.defs()) {
    if (MO.isReg() && MO.isDead()) {
      rv.removeAllElem(stack, StackVreg(MO.getReg()));
    }
  }
  return rv;
}

void StackFixup::apply(Stack &stack) const {
  for (auto p : Changes)
    stack += p.first;
}

void StackFixup::removeElem(Stack &stack, const StackVreg &vreg) {
  auto &rv= (*this);
  auto i = stack.position(vreg);
  if (i == 0) {
    rv(stack += rv(drop()));
  } else if (i == 1) {
    rv(stack += rv(nip()));
  } else {
    rv(stack += rv(xchgTop(i)));
    rv(stack += rv(drop()));
  }
}

void StackFixup::removeNElem(Stack &stack, const StackVreg &vreg, size_t N) {
  for (unsigned i = 0; i < N; ++i)
    removeElem(stack, vreg);
}

void StackFixup::removeAllElem(Stack &stack, const StackVreg &vreg) {
  removeNElem(stack, vreg, stack.count(vreg));
}

void StackFixup::operator()(const Stack &stack) {
  setLastComment(stack.toString());
}

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
void StackFixup::printElem(raw_ostream &OS, const Change &change) const {
  visit(overloaded{
      [&](drop){ OS << "drop"; },
      [&](nip){ OS << "nip"; },
      [&](swap){ OS << "swap"; },
      [&](xchgTop v){ OS << "xchg s(" << v.i << ")"; },
      [&](xchg v){ OS << "xchg s(" << v.i << "), s(" << v.j << ")"; },
      [&](dup){ OS << "dup"; },
      [&](pushI v){ OS << "push s(" << v.i << ")"; }
    }, change);
}

void StackFixup::print(raw_ostream &OS) const {
  llvm::for_each(Changes, [&](const std::pair<Change, std::string> &v){
    printElem(OS, v.first);
    OS << "; " << v.second << "\n";
  });
}

void StackFixup::dump() const {
  print(dbgs());
}

#endif

MachineInstr *StackFixup::InstructionGenerator::
operator()(const std::pair<Change, std::string> &pair) const {
  MachineInstr *MI = nullptr;
  visit(overloaded{
      [&](drop){
        MI = BuildMI(*MBB, InsertPt, DL, TII->get(TVM::DROP)).getInstr();
      },
      [&](nip){
        MI = BuildMI(*MBB, InsertPt, DL, TII->get(TVM::NIP)).getInstr();
      },
      [&](swap){
        MI = BuildMI(*MBB, InsertPt, DL, TII->get(TVM::SWAP)).getInstr();
      },
      [&](xchgTop v){
        MI = BuildMI(*MBB, InsertPt, DL, TII->get(TVM::XCHG_TOP)).addImm(v.i)
                     .getInstr();
      },
      [&](xchg v){
        MI = BuildMI(*MBB, InsertPt, DL, TII->get(TVM::XCHG))
                     .addImm(v.i).addImm(v.j)
                     .getInstr();
      },
      [&](dup){
        MI = BuildMI(*MBB, InsertPt, DL, TII->get(TVM::DUP)).getInstr();
      },
      [&](pushI v){
        MI = BuildMI(*MBB, InsertPt, DL, TII->get(TVM::PUSH)).addImm(v.i)
                       .getInstr();
      }
    }, pair.first);

  assert(MI && "MI is not generated");

  MFI->addStackModelComment(MI, pair.second);
  return MI;
}

void StackFixup::InstructionGenerator::operator()(const StackFixup &v) const {
  llvm::for_each(v.getChanges(), *this);
}

} // namespace llvm
