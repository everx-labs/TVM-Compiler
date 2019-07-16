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
#include "llvm/CodeGen/LiveIntervals.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/DebugInfoMetadata.h"

namespace llvm {

Stack::Stack(MachineFunction &MF, size_t Size)
    : TRI(MF.getSubtarget<TVMSubtarget>().getRegisterInfo()),
      MRI(&MF.getRegInfo()),
      Data(Size, StackVreg(TVMFunctionInfo::UnusedReg)) {}

/// If \par MO is no longer used after \par MI.
static bool isKilled(const MachineInstr &MI, unsigned Register,
                     const LiveIntervals &LIS) {
  const LiveInterval &LI = LIS.getInterval(Register);
  // If there is no live interval starting from the current instruction
  // for the given argument, the argument is killed.
  if (!LI.getVNInfoAt(LIS.getInstructionIndex(MI).getRegSlot()))
    return true;
  for (size_t I = 0, E = MI.getNumDefs(); I < E; ++I)
    if (MI.getOperand(I).isReg() && MI.getOperand(I).getReg() == Register)
      return true;
  return false;
}

MIArgs::MIArgs(MachineInstr &MI, const LiveIntervals &LIS) {
  auto regUses = llvm::make_filter_range(MI.uses(),
                   [](const MachineOperand &Op) { return Op.isReg(); });
  for (auto Arg : regUses) {
    Args.emplace_back(StackVreg(Arg.getReg()),
                      isKilled(MI, Arg.getReg(), LIS));
  }
}

Stack Stack::reqArgs(const MIArgs &Args) const {
  Stack rv(*this);
  SmallVector<StackVreg, 4> Ops;
  for (auto Arg : Args.getArgs()) {
    auto it = llvm::find(rv.Data, Arg.Vreg);
    if (it != rv.Data.end()) {
      Ops.push_back(*it);
      if (Arg.IsKilled)
        rv.Data.erase(it);
    } else {
      auto dupIt = llvm::find(Ops, Arg.Vreg);
      assert(dupIt != Ops.end() && "No required argument in stack");
      Ops.push_back(*dupIt);
    }
  }
  auto reverted = llvm::reverse(Ops);
  rv.Data.insert(rv.Data.begin(), reverted.begin(), reverted.end());
  return rv;
}

Stack Stack::addArgs(const MIArgs &Args) const {
  Stack rv(*this);
  for (auto Arg : Args.getArgs())
    rv.Data.push_front(Arg.Vreg);
  return rv;
}

Stack Stack::filteredByLiveIns(MachineBasicBlock &MBB,
                               const LiveIntervals &LIS) const {
  Stack rv(*this);
  for (StackVreg &vreg : rv.Data) {
    if (!LIS.hasInterval(vreg.VirtReg) ||
        !LIS.isLiveInToMBB(LIS.getInterval(vreg.VirtReg), &MBB)) {
      vreg = StackVreg(TVMFunctionInfo::UnusedReg);
    }
  }
  return rv;
}

Stack Stack::filteredByLiveOuts(MachineBasicBlock &MBB,
                                const LiveIntervals &LIS) const {
  Stack rv(*this);
  for (StackVreg &vreg : rv.Data) {
    if (!LIS.hasInterval(vreg.VirtReg) ||
        !LIS.isLiveOutOfMBB(LIS.getInterval(vreg.VirtReg), &MBB)) {
      vreg = StackVreg(TVMFunctionInfo::UnusedReg);
    }
  }
  return rv;
}

// Replace existing regs, defined in this MI (and not used) by UnusedReg
Stack Stack::filteredByMIdefs(const MachineInstr &MI) const {
  // If one of defined register exists in stack, this value will be obsolete
  //  after this instruction.
  // So we need to eliminate (replace by UnusedReg) this register in stack,
  //  if it is not required in this MI uses.
  Stack rv(*this);
  for (const MachineOperand &Op : MI.defs()) {
    if (Op.isReg())
      continue;
    StackVreg Vreg(Op.getReg());
    auto it = llvm::find_if(MI.uses(), [&](const MachineOperand &Use) {
      return Use.isReg() && Op.getReg() == Use.getReg();
    });
    bool Used = (it != MI.uses().end());
    if (!Used)
      rv.Data[rv.position(Vreg)] = StackVreg(TVMFunctionInfo::UnusedReg);
  }
  return rv;
}

void Stack::filterByDeadDefs(MachineInstr &MI) {
  for (const auto &MO : MI.defs()) {
    if (MO.isReg() && MO.isDead()) {
      for (auto &Vreg : Data) {
        if (Vreg.VirtReg == MO.getReg())
          Vreg.VirtReg = TVMFunctionInfo::UnusedReg;
      }
    }
  }
}

void Stack::fillUnusedRegs(SmallVector<StackVreg, 16> &Regs) {
  for (StackVreg &VR : Data) {
    if (Regs.empty())
        return;
    if (VR.VirtReg == TVMFunctionInfo::UnusedReg) {
      VR = Regs.pop_back_val();
    }
  }
}

void Stack::addDef(unsigned Reg, const DILocalVariable *DbgVar) {
  Data.push_front(StackVreg(Reg, DbgVar));
}

Stack& Stack::operator += (const StackFixup::Change &change) {
  auto multiChange = [&](const auto &v){
    unsigned ArgsSize = sizeof(v.args) / sizeof(*v.args);
    // First we do xchgs to avoid indices correction
    unsigned xchgNum = 0;
    auto xchgs = v.countXchgs();
    for (unsigned i = 0; i < ArgsSize; ++i) {
      if (v.isXchg(i)) {
        std::swap(Data[v.args[i]], Data[xchgs - 1 - xchgNum]);
        ++xchgNum;
      }
    }
    // Now insert pushes
    xchgNum = 0;
    unsigned pushNum = 0;
    for (unsigned i = 0; i < ArgsSize; ++i) {
      if (v.isPush(i)) {
        Data.insert(Data.begin() + (xchgs - xchgNum),
                    Data[v.args[i] + pushNum]);
        ++pushNum;
      } else {
        ++xchgNum;
      }
    }
  };
  std::visit(overloaded{
      [this](StackFixup::drop){
        Data.pop_front();
      },
      [this](StackFixup::nip) {
        Data.erase(std::next(Data.begin()));
      },
      [this](StackFixup::xchgTop v) {
        std::swap(Data[0], Data[v.i]);
      },
      [this](StackFixup::xchg v) {
        std::swap(Data[v.i], Data[v.j]);
      },
      [this](StackFixup::pushI v){ Data.push_front(Data[v.i]); },
      [this](StackFixup::pushUndef){
        Data.push_front(StackVreg(TVMFunctionInfo::UnusedReg));
      },
      [this](StackFixup::blkswap v) {
        assert(v.deepSz + v.topSz <= Data.size() && "Wrong blkswap");
        decltype(Data) topCopy(Data.begin(), Data.begin() + v.topSz);
        Data.erase(Data.begin(), Data.begin() + v.topSz);
        Data.insert(Data.begin() + v.deepSz, topCopy.begin(), topCopy.end());
      },
      [this](StackFixup::roll v) {
        assert((unsigned)std::abs(v.i) < Data.size() && "Wrong blkswap");
        if (v.i > 0) {
          auto deepElem = Data[v.i];
          Data.erase(Data.begin() + v.i);
          Data.push_front(deepElem);
        } else {
          auto topElem = Data.front();
          Data.pop_front();
          Data.insert(Data.begin() + (-v.i), topElem);
        }
      },
      [this](StackFixup::reverse v) {
        assert(v.topIdx + v.deepSz <= Data.size() && "Wrong reverse");
        std::reverse(Data.begin() + v.topIdx,
                 Data.begin() + v.topIdx + v.deepSz);
      },
      [this](StackFixup::blkdrop v) {
        assert(v.sz <= Data.size() && "Wrong blkdrop");
        Data.erase(Data.begin(), Data.begin() + v.sz);
      },
      [&](const StackFixup::doubleChange &v) {
        for (unsigned Arg : v.args)
          assert(Arg < Data.size() && "Bad doubleChange");
        multiChange(v);
      },
      [&](const StackFixup::tripleChange &v) {
        for (unsigned Arg : v.args)
          assert(Arg < Data.size() && "Bad tripleChange");
        multiChange(v);
      }
    }, change);
  return *this;
}

Stack Stack::operator + (const StackFixup &fixup) const {
  Stack rv(*this);
  fixup.apply(rv);
  return rv;
}

void Stack::print(raw_ostream &OS) const {
  OS << "{ ";
  if (!Data.empty()) {
    llvm::for_each(reverse(Data), [&OS, this](const StackVreg &Elem) {
      printElement(OS, Elem);
      OS << " | ";
    });
  }
  OS << "- }";
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
void Stack::dump() const {
  print(dbgs());
  dbgs() << "\n";
  dbgs().flush();
}
#endif

void Stack::printElement(raw_ostream &OS, const StackVreg &Vreg) const {
  if (Vreg.VirtReg == TVMFunctionInfo::UnusedReg) {
    OS << "x";
    return;
  }
  OS << printReg(Vreg.VirtReg, TRI, 0, MRI);
  if (Vreg.DbgVar) {
    OS << "(" << Vreg.DbgVar->getName() << ")";
  }
}

StackFixup Stack::operator-(const Stack &v) const {
  return StackFixup::Diff(*this, v);
}

} // namespace llvm
