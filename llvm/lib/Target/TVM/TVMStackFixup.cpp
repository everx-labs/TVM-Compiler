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

  SmallVector<StackVreg, 16> fromRegs(from.begin(), from.end());
  SmallVector<StackVreg, 16> toRegs(to.begin(), to.end());

  llvm::sort(fromRegs.begin(), fromRegs.end());
  llvm::erase_if(fromRegs, [](const StackVreg &vreg){
    return vreg.VirtReg == TVMFunctionInfo::UnusedReg; });
  llvm::sort(toRegs.begin(), toRegs.end());
  llvm::erase_if(toRegs, [](const StackVreg &vreg){
    return vreg.VirtReg == TVMFunctionInfo::UnusedReg; });

  SmallVector<StackVreg, 16> delVregs;
  std::set_difference(fromRegs.begin(), fromRegs.end(),
                      toRegs.begin(), toRegs.end(),
                      std::back_inserter(delVregs));
  Stack unmaskedTo(to);
  unmaskedTo.fillUnusedRegs(delVregs);
  auto removeElem = [&](const StackVreg &vreg) {
    rv.removeElem(curStack, vreg);
  };
  if (!delVregs.empty()) {
    // Sorting regs-to-delete by position from top of the stack
    llvm::sort(delVregs.begin(), delVregs.end(),
               [&from](const StackVreg &L, const StackVreg &R) {
      return from.position(L) < from.position(R);
    });

    // Generate changes to delete unused vregs
    llvm::for_each(delVregs, removeElem);
  }

  fromRegs = SmallVector<StackVreg, 16>(curStack.begin(), curStack.end());
  toRegs = SmallVector<StackVreg, 16>(unmaskedTo.begin(), unmaskedTo.end());

  llvm::sort(fromRegs.begin(), fromRegs.end());
  llvm::sort(toRegs.begin(), toRegs.end());

  delVregs.clear();
  std::set_difference(fromRegs.begin(), fromRegs.end(),
                      toRegs.begin(), toRegs.end(),
                      std::back_inserter(delVregs));
  llvm::for_each(delVregs, removeElem);

  fromRegs = SmallVector<StackVreg, 16>(curStack.begin(), curStack.end());
  toRegs = SmallVector<StackVreg, 16>(unmaskedTo.begin(), unmaskedTo.end());

  llvm::sort(fromRegs.begin(), fromRegs.end());
  llvm::sort(toRegs.begin(), toRegs.end());

  SmallVector<StackVreg, 16> addVregs;
  std::set_difference(toRegs.begin(), toRegs.end(),
                      fromRegs.begin(), fromRegs.end(),
                      std::back_inserter(addVregs));

  // Generate changes to insert copies (pushes)
  auto addElem = [&](const StackVreg &vreg) {
    if (vreg.VirtReg == TVMFunctionInfo::UnusedReg) {
      rv(curStack += rv(pushZero()));
      return;
    }
    auto i = curStack.position(vreg);
    if (i == 0)
      rv(curStack += rv(dup()));
    else
      rv(curStack += rv(pushI(i)));
  };
  llvm::for_each(addVregs, addElem);

  // Generate changes to re-order
  assert(llvm::size(unmaskedTo) == llvm::size(curStack));

  size_t sz = llvm::size(unmaskedTo);
  assert(sz <= XchgDeepLimit && "Too deep stack");

  for (unsigned n = 0; n < sz; ++n) {
    if (unmaskedTo[n] != curStack[n]) {
      auto i = curStack.position(n, unmaskedTo[n]);

      if (i == 0)
        rv(curStack += rv(xchgTop(n)));
      else if (n == 0)
        rv(curStack += rv(xchgTop(i)));
      else if (i <= XchgLimit && n <= XchgLimit) {
        rv(curStack += rv(xchg(i, n)));
      } else {
        // 3 top xchanges for deep reorderings (xchgTop supports deep index)
        // xchg i,n = { xchg i,0; xchg 0,n; xchg i,0; }
        rv(curStack += rv(xchgTop(i)));
        rv(curStack += rv(xchgTop(n)));
        rv(curStack += rv(xchgTop(i)));
      }
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
    if (i != j) {
      if (i == 0) {
        rv(curStack += rv(xchgTop(j)));
      } else if (j == 0) {
        rv(curStack += rv(xchgTop(i)));
      } else if (i <= XchgLimit && j <= XchgLimit) {
        rv(curStack += rv(xchg(i, j)));
      } else {
        rv(curStack += rv(xchgTop(i)));
        rv(curStack += rv(xchgTop(j)));
        rv(curStack += rv(xchgTop(i)));
      }
    }
  }
  for (unsigned i = 0; i < numPops; ++i)
    rv(curStack += rv(drop()));
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
      [&](pushI v){ OS << "push s(" << v.i << ")"; },
      [&](pushZero){ OS << "zero"; }
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
        if (v.i <= XchgLimit) {
          MI = BuildMI(*MBB, InsertPt, DL, TII->get(TVM::XCHG_TOP)).addImm(v.i)
                       .getInstr();
        } else if (v.i <= XchgDeepLimit) {
          MI = BuildMI(*MBB, InsertPt, DL, TII->get(TVM::XCHG_TOP_DEEP))
                       .addImm(v.i)
                       .getInstr();
        }
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
      },
      [&](pushZero){
        MI = BuildMI(*MBB, InsertPt, DL, TII->get(TVM::ZERO)).getInstr();
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
