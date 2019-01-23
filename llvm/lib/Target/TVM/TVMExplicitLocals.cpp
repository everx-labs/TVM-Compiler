//===-- TVMExplicitLocals.cpp - Make Locals Explicit ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file converts any remaining registers into TVM locals.
///
/// After register stackification and register coloring, convert non-stackified
/// registers into locals, inserting explicit get_local and set_local
/// instructions.
///
//===----------------------------------------------------------------------===//

#include <deque>

#include "MCTargetDesc/TVMMCTargetDesc.h"
#include "TVM.h"
#include "TVMExtras.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMSubtarget.h"
#include "TVMUtilities.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/LiveIntervals.h"
#include "llvm/CodeGen/MachineBlockFrequencyInfo.h"
#include "llvm/CodeGen/MachineDominators.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include "TVMInstMappingInfo.inc"

using namespace llvm;

#define DEBUG_TYPE "tvm-explicit-locals"

namespace {

enum class StackReorderingKind { Copy, Xchg };

struct StackReordering {
  /// The register we get data from.
  unsigned RegFrom;
  /// The number of slot we put data to.
  size_t SlotTo;
  /// If we copy (Push) or move (Xchg) the data.
  StackReorderingKind ReorderingKind;
  StackReordering(unsigned RegFrom, size_t SlotTo,
                  StackReorderingKind ReorderingKind)
      : RegFrom(RegFrom), SlotTo(SlotTo), ReorderingKind(ReorderingKind) {}
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
  /// Insert POP instructions to clean up the stack, preserving the specified
  /// element of it.
  /// \par InsertPoint specify instruction to insert after.
  /// \par Preserved virtual register needs to be kept in the stack.
  bool clear(MachineInstr *InsertPoint,
             unsigned Preserved = TVMFunctionInfo::UnusedReg);
  /// PUSH the specified slot to the specified position of the stack.
  /// Do nothing if \p Reg is already in \p Slot.
  /// Precondition: Register is present in Data.
  /// TODO: Stack PUSH limitations aren't handled yet.
  /// \par InsertPoint specify instruction to insert after.
  /// \par Reg virtual register number for the data source.
  bool push(MachineInstr *InsertPoint, unsigned Reg);
  /// \par InsertPoint specify instruction to insert after.
  /// \par RegFrom register number to be exchanged in the stack.
  /// \par SlotTo slot number to be exchanged with.
  /// Precondition: Slot number for RegFrom != SlotTo.
  bool xchg(MachineInstr *InsertPoint, unsigned RegFrom, size_t SlotTo);
  /// Return position of \par Reg in the stack.
  /// Precondition: \par Reg is in the stack.
  size_t position(unsigned Reg) const {
    return std::distance(std::begin(Data), llvm::find_or_fail(Data, Reg));
  }
  /// Return register for \par Slot in the stack.
  /// Precondition: Slot < Data.size().
  unsigned reg(size_t Slot) const {
    assert(Slot < Data.size());
    return Data[Slot];
  }
  /// Fill the specified \p Slot with \p Reg. Doesn't generate any instruction.
  void set(size_t Slot, size_t Reg) {
    assert(Slot < Data.size() && "Out of range access");
    Data[Slot] = Reg;
  }
  /// Remove arguments an instruction consumed from the stack.
  /// Precondition: Stack has enough Slots to consume.
  void consumeArguments(size_t NumSlots) {
    assert(NumSlots <= Data.size());
    Data.erase(std::begin(Data), std::begin(Data) + NumSlots);
  }
  /// Pushes result of an instruction to the stack.
  void addDef(unsigned Reg) { Data.push_front(Reg); }
  /// TODO: we need to decide how to handle these limitations.
  /// They shouldn't be defined in this scope.
  /// Maximal N in a valid PUSH sN instruction.
  static inline constexpr size_t PushLimit = 255;
  /// Maximal N, M in a valid XCHG sN, sM instruction.
  static inline constexpr size_t XchgLimit = 15;

private:
  const TargetInstrInfo *TII;
  std::deque<unsigned> Data;
};

class TVMExplicitLocals final : public MachineFunctionPass {
public:
  StringRef getPassName() const override { return "TVM Explicit Locals"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<LiveIntervals>();
    AU.addPreserved<LiveIntervals>();
    AU.addPreservedID(LiveVariablesID);
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;
  /// Inserts necessary stack manipulation instructions to supply \par MI with
  /// the correct data.
  bool processInstruction(MachineInstr &MI, LiveIntervals &LIS,
                          MachineRegisterInfo &MRI, const TargetInstrInfo *TII,
                          Stack &TheStack);

  static char ID; // Pass identification, replacement for typeid
  TVMExplicitLocals() : MachineFunctionPass(ID) {}

private:
  /// If \par MO is no longer used after \par MI.
  bool IsKilled(const MachineInstr &MI, unsigned Register,
                const LiveIntervals &LIS) const;
  /// Forms vector of Pushes and Xchgs to supply an instruction with the right
  /// data.
  /// The function assumes that non-register arguments always come first.
  StackReorderings computeReorderings(MachineInstr &MI, LiveIntervals &LIS,
                                      Stack &TheStack,
                                      size_t NonStackOperands) const;
  /// Perform the specified stack manipulations and generates code for them.
  /// Insert instructions in the position specified by \par InsertPoint.
  void performReorderings(const StackReorderings &Reorderings,
                          MachineInstr *InsertPoint, Stack &TheStack);
};

// Return true if reorderings might be optimized away for a commutative
// instruction.
bool isCommutation(const StackReorderings &Reorderings, const Stack &TheStack) {
  if (Reorderings.size() != 1u)
    return false;
  const StackReordering &Reordering = Reorderings[0];
  return (Reordering.SlotTo == 1u &&
          TheStack.position(Reordering.RegFrom) == 0u) ||
         (Reordering.SlotTo == 0u &&
          TheStack.position(Reordering.RegFrom) == 1u);
}

} // end anonymous namespace

char TVMExplicitLocals::ID = 0;
INITIALIZE_PASS(TVMExplicitLocals, DEBUG_TYPE,
                "Convert registers to TVM locals", false, false)

FunctionPass *llvm::createTVMExplicitLocals() {
  return new TVMExplicitLocals();
}

/// Get the appropriate Pop opcode for the given register class.
static unsigned getPopOpcode(const TargetRegisterClass *RC) {
  if (RC == &TVM::I64RegClass)
    return TVM::POP_S;
  llvm_unreachable("Unexpected register class");
}

/// Get the appropriate Push opcode for the given register class.
static unsigned getPushOpcode(const TargetRegisterClass *RC) {
  if (RC == &TVM::I64RegClass)
    return TVM::PUSH_S;
  llvm_unreachable("Unexpected register class");
}

static unsigned getXchgOpcode(const TargetRegisterClass *RC) {
  if (RC == &TVM::I64RegClass)
    return TVM::XCHG;
  llvm_unreachable("Unexpected register class");
}

bool Stack::clear(MachineInstr *InsertPoint, unsigned Preserved) {
  auto It = llvm::find(Data, Preserved);
  size_t NumDrops = 0, NumNips = 0;
  if (It == std::end(Data)) {
    NumDrops = Data.size();
  } else {
    NumDrops = std::distance(std::begin(Data), It);
    NumNips = Data.size() - NumDrops - 1;
  }
  unsigned Opc = getPopOpcode(&TVM::I64RegClass);
  // DROPs
  for (size_t i = 0; i < NumDrops; ++i)
    BuildMI(*InsertPoint->getParent(), InsertPoint, InsertPoint->getDebugLoc(),
            TII->get(Opc))
        .addImm(0);
  // NIPs
  for (size_t i = 0; i < NumNips; ++i)
    BuildMI(*InsertPoint->getParent(), InsertPoint, InsertPoint->getDebugLoc(),
            TII->get(Opc))
        .addImm(1);
  if (It == std::end(Data))
    Data.clear();
  else
    Data = {Data[NumDrops]};
  return NumDrops && NumNips;
}

bool Stack::push(MachineInstr *InsertPoint, unsigned Reg) {
  size_t RegSlot = position(Reg);
  assert(RegSlot <= PushLimit && "Unimplemented");
  unsigned Opc = getPushOpcode(&TVM::I64RegClass);
  BuildMI(*InsertPoint->getParent(), InsertPoint, InsertPoint->getDebugLoc(),
          TII->get(Opc))
      .addImm(RegSlot);
  Data.push_front(Data[RegSlot]);
  return true;
}

bool Stack::xchg(MachineInstr *InsertPoint, unsigned RegFrom, size_t SlotTo) {
  auto It = llvm::find_or_fail(Data, RegFrom);
  size_t RegFromSlot = std::distance(std::begin(Data), It);
  assert(RegFromSlot <= XchgLimit && "Unimplemented");
  assert(RegFromSlot != SlotTo);
  unsigned Opc = getXchgOpcode(&TVM::I64RegClass);
  BuildMI(*InsertPoint->getParent(), InsertPoint, InsertPoint->getDebugLoc(),
          TII->get(Opc))
      .addImm(std::min(RegFromSlot, SlotTo))
      .addImm(std::max(RegFromSlot, SlotTo));
  std::swap(*It, Data[SlotTo]);
  return true;
}

bool TVMExplicitLocals::IsKilled(const MachineInstr &MI, unsigned Register,
                                 const LiveIntervals &LIS) const {
  const LiveInterval &LI = LIS.getInterval(Register);
  // If there is no live interval starting from the current instruction
  // for the given argument, the argument is killed.
  return !LI.getVNInfoAt(LIS.getInstructionIndex(MI).getRegSlot());
}

StackReorderings
TVMExplicitLocals::computeReorderings(MachineInstr &MI, LiveIntervals &LIS,
                                      Stack &TheStack,
                                      size_t NonStackOperands) const {
  StackReorderings Result{};
  size_t NumDefs = MI.getNumDefs();
  size_t NumOperands = MI.getNumOperands();

  llvm::SmallSet<unsigned, 2> RegUsed{};

  // The same register could be used multiple times, but the stack keeps
  // the only copy of it, so we need to produce copies for each but the last
  // usage of the register even if its killed by MI.
  llvm::DenseMap<unsigned, size_t> LastUseOperandIndex(NextPowerOf2(2 * 4 / 3));
  for (size_t ROpNo = NumDefs + NonStackOperands; ROpNo < NumOperands;
       ++ROpNo) {
    size_t OpNo = NumOperands - ROpNo;
    const auto &Operand = MI.getOperand(OpNo);
    if (!Operand.isReg())
      continue;
    RegUsed.insert(Operand.getReg());
    LastUseOperandIndex[Operand.getReg()] = OpNo;
  }

  // Instruction format: INST %defs..., %non-register args... %register args...
  for (size_t ROpNo = NumDefs + NonStackOperands; ROpNo < NumOperands;
       ++ROpNo) {
    size_t OpNo = NumOperands - ROpNo;
    const auto &Operand = MI.getOperand(OpNo);
    if (!Operand.isReg())
      continue;
    unsigned RegFrom = Operand.getReg();
    if (IsKilled(MI, Operand.getReg(), LIS) &&
        LastUseOperandIndex[Operand.getReg()] == OpNo)
      Result.emplace_back(RegFrom, ROpNo - NumDefs - NonStackOperands,
                          StackReorderingKind::Xchg);
    else
      Result.emplace_back(RegFrom, ROpNo - NumDefs - NonStackOperands,
                          StackReorderingKind::Copy);
  }

  // We need to adjust XChgs to number of non-register operands together with
  // number of Pushes followed by it.
  size_t NumPushes = 0;
  // Collect reorderings that are supposed to be removed later.
  llvm::SmallVector<const StackReordering *, 2> PseudoXchg{};
  for (auto &Reordering : Result) {
    if (Reordering.ReorderingKind == StackReorderingKind::Copy) {
      assert(Reordering.SlotTo >= NumPushes + NonStackOperands);
      Reordering.SlotTo -= NumPushes + NonStackOperands;
      ++NumPushes;
    } else {
      assert(Reordering.SlotTo >= NumPushes + NonStackOperands);
      Reordering.SlotTo -= NumPushes + NonStackOperands;
      // Collect XCHG sN, sN pseudos.
      if (Reordering.SlotTo ==
          TheStack.position(Reordering.RegFrom) + NumPushes)
        PseudoXchg.push_back(&Reordering);
      // Collect XCHG sM, sN (M > N) if XCHG sN, sM is also present and cyclic
      // dependencies of a bigger length.
      if (Reordering.SlotTo <
              TheStack.position(Reordering.RegFrom) + NumPushes &&
          TheStack.position(Reordering.RegFrom) + NumPushes <
              NumOperands - NumDefs - NonStackOperands &&
          exist(RegUsed, TheStack.reg(Reordering.SlotTo)) &&
          IsKilled(MI, TheStack.reg(Reordering.SlotTo), LIS))
        PseudoXchg.push_back(&Reordering);
    }
  }

  // Remove redundant reorderings.
  Result.erase(
      llvm::remove_if(Result,
                      [&PseudoXchg](const StackReordering &Reordering) {
                        return llvm::find(PseudoXchg, &Reordering) !=
                               std::end(PseudoXchg);
                      }),
      std::end(Result));
  return Result;
}

void TVMExplicitLocals::performReorderings(const StackReorderings &Reorderings,
                                           MachineInstr *InsertPoint,
                                           Stack &TheStack) {
  if (Reorderings.empty())
    return;
  // We need to perform reorderings in reverse order except for a sequence of
  // XCHGs. E.g. if we have XCHG1, XCHG2, PUSH, it should be executed as PUSH,
  // XCHG1, XCHG2.
  size_t NumElements = Reorderings.size();
  size_t LastPush = NumElements;
  size_t RevIdx = 0;
  while (RevIdx < NumElements) {
    size_t Idx = NumElements - RevIdx - 1;
    if (Reorderings[Idx].ReorderingKind == StackReorderingKind::Copy) {
      // If the current reordering is PUSH, execute preceding (in reverse order)
      // XCHGs
      for (size_t I = Idx + 1; I < LastPush; ++I) {
        assert(Reorderings[I].ReorderingKind == StackReorderingKind::Xchg &&
               "Unexpected reordering");
        TheStack.xchg(InsertPoint, Reorderings[I].RegFrom,
                      Reorderings[I].SlotTo);
      }
      TheStack.push(InsertPoint, Reorderings[Idx].RegFrom);
      if (Reorderings[Idx].SlotTo)
        TheStack.xchg(InsertPoint, Reorderings[Idx].RegFrom,
                      Reorderings[Idx].SlotTo);
      LastPush = Idx;
    }
    ++RevIdx;
  }
  // If reorderings start with XCHGs.
  for (size_t I = 0; I < LastPush; ++I) {
    assert(Reorderings[I].ReorderingKind == StackReorderingKind::Xchg &&
           "Unexpected reordering");
    TheStack.xchg(InsertPoint, Reorderings[I].RegFrom, Reorderings[I].SlotTo);
  }
}

bool TVMExplicitLocals::processInstruction(MachineInstr &MI, LiveIntervals &LIS,
                                           MachineRegisterInfo &MRI,
                                           const TargetInstrInfo *TII,
                                           Stack &TheStack) {
  if (MI.isImplicitDef())
    return false;
  if (MI.isReturn()) {
    assert(MI.getNumOperands() <= 2 &&
           "Multiple returns are not implemented yet");
    if (MI.getNumOperands() == 0)
      TheStack.clear(&MI);
    else
      TheStack.clear(&MI, MI.getOperand(0).getReg());
    MI.eraseFromParent();
    return true;
  }

  size_t NumDefs = MI.getNumDefs();
  size_t NumOperands = MI.getNumOperands();
  size_t NonStackOperands =
      count_if(MI.operands(), [](MachineOperand MOP) { return !MOP.isReg(); });
  bool Changed = false;
  StackReorderings Reorderings =
      computeReorderings(MI, LIS, TheStack, NonStackOperands);
  int NewOpcode = -1;
  if (isCommutation(Reorderings, TheStack)) {
    if (MI.isCommutable())
      Reorderings.clear();
    else if (MI.getOpcode() == TVM::SUB) {
      NewOpcode = TVM::SUBR_S;
      Reorderings.clear();
    }
  }
  if (NewOpcode < 0)
    if (MI.getOpcode() != TVM::CONST_I64)
      NewOpcode = TVM::RForm2SForm[MI.getOpcode()];
  performReorderings(Reorderings, &MI, TheStack);
  TheStack.consumeArguments(NumOperands - NonStackOperands - NumDefs);
  for (size_t ROpNo = 0; ROpNo < NumDefs; ++ROpNo) {
    size_t OpNo = NumDefs - ROpNo - 1;
    const auto &Operand = MI.getOperand(OpNo);
    assert(Operand.isReg() && "Def must be a register");
    TheStack.addDef(Operand.getReg());
  }
  if (NewOpcode >= 0) {
    BuildMI(*MI.getParent(), &MI, MI.getDebugLoc(), TII->get(NewOpcode));
    MI.removeFromParent();
    Changed = true;
  }
  return Changed;
}

// TODO: For now it only stackifies function arguments. Extend.
bool TVMExplicitLocals::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(dbgs() << "********** Make Locals Explicit **********\n"
                       "********** Function: "
                    << MF.getName() << '\n');

  bool Changed = false;
  TVMFunctionInfo &MFI = *MF.getInfo<TVMFunctionInfo>();
  const auto *TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();
  Stack TheStack(TII, MFI.numParams());
  LiveIntervals &LIS = getAnalysis<LiveIntervals>();
  MachineRegisterInfo &MRI = MF.getRegInfo();

  size_t NumArgs = MFI.numParams();

  // Handle ARGUMENTS first to ensure that they get the designated numbers.
  for (MachineBasicBlock::iterator I = MF.begin()->begin(),
                                   E = MF.begin()->end();
       I != E;) {
    MachineInstr &MI = *I++;
    if (!TVM::isArgument(MI))
      break;
    unsigned Reg = MI.getOperand(0).getReg();
    assert(!MFI.isVRegStackified(Reg));
    unsigned ArgNo = NumArgs - MI.getOperand(1).getImm() - 1;
    TheStack.set(ArgNo, Reg);
    MI.eraseFromParent();
    Changed = true;
  }

  // Visit each instruction in the function.
  for (MachineBasicBlock &MBB : MF) {
    for (MachineBasicBlock::iterator I = MBB.begin(), E = MBB.end(); I != E;) {
      MachineInstr &MI = *I++;
      assert(!TVM::isArgument(MI));

      if (MI.isDebugInstr() || MI.isLabel())
        continue;

      // TODO: multiple defs should be handled separately.
      assert(MI.getDesc().getNumDefs() <= 1);
      Changed |= processInstruction(MI, LIS, MRI, TII, TheStack);
    }
  }

  return Changed;
}
