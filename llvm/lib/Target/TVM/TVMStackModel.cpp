//===--------- TVMStackModel.cpp - Rewrite Reg-forms with S-forms ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Introduce explicit stack manipulation. Rewrite all register forms of
/// instructions with their stack counterparts.
///
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/TVMMCTargetDesc.h"
#include "TVM.h"
#include "TVMStack.h"
#include "TVMSubtarget.h"
#include "TVMUtilities.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/LiveIntervals.h"
#include "llvm/CodeGen/MachineBlockFrequencyInfo.h"
#include "llvm/CodeGen/MachineDominators.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/raw_ostream.h"

#include "TVMInstMappingInfo.inc"

using namespace llvm;

#define DEBUG_TYPE "tvm-stack-model"

namespace {

class TVMStackModel final : public MachineFunctionPass {
public:
  StringRef getPassName() const override { return "TVM Stack Model"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<LiveIntervals>();
    AU.addPreservedID(LiveVariablesID);
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;
  /// Inserts necessary stack manipulation instructions to supply \par MI with
  /// the correct data.
  bool processInstruction(MachineInstr &MI, LiveIntervals &LIS,
                          const TargetInstrInfo *TII, Stack &TheStack);

  static char ID; // Pass identification, replacement for typeid
  TVMStackModel() : MachineFunctionPass(ID) {}

private:
  bool runOnBasicBlocks(MachineFunction &MF, Stack &TheStack);

  /// Forms vector of Pushes and Xchgs to supply an instruction with the right
  /// data.
  /// The function assumes that non-register arguments always come first.
  StackReorderings computeReorderings(MachineInstr &MI, LiveIntervals &LIS,
                                      Stack &TheStack,
                                      size_t NonStackOperands) const;
  /// Perform the specified stack manipulations and generates code for them.
  /// Insert instructions in the position specified by \par InsertPoint.
  void performReorderings(const StackReorderings &Reorderings,
                          MachineInstr *InsertPoint, Stack &TheStack) const;
};

/// Return true if reorderings might be optimized away for a commutative
/// instruction.
bool isCommutation(const StackReorderings &Reorderings, const Stack &TheStack) {
  if (Reorderings.size() != 1u)
    return false;
  if (!Reorderings[0].isXchg())
    return false;
  size_t SlotTo = Reorderings[0].SlotTo;
  size_t SlotFrom = TheStack.position(Reorderings[0].ElemFrom);
  return (SlotTo == 1 && SlotFrom == 0) || (SlotTo == 0 && SlotFrom == 1);
}

inline bool isStackOperand(const MachineOperand &MOP) {
  assert((MOP.isReg() || MOP.isImm() || MOP.isMBB() || MOP.isGlobal()) &&
         "Unexpected operand type: reconsider the predicate.");
  // TODO: Globals should be on stack.
  return MOP.isReg() || MOP.isMBB();
}

/// If \par MO is no longer used after \par MI.
bool isKilled(const MachineInstr &MI, unsigned Register,
              const LiveIntervals &LIS) {
  const LiveInterval &LI = LIS.getInterval(Register);
  // If there is no live interval starting from the current instruction
  // for the given argument, the argument is killed.
  return !LI.getVNInfoAt(LIS.getInstructionIndex(MI).getRegSlot());
}

/// Make predecessors' stacks coherent and initialize the stack with stacks of
/// predecessors, or with \par Initializer if there is no predecessors.
Stack &initializeStack(MachineBasicBlock &MBB,
                       DenseMap<MachineBasicBlock *, Stack> &BBStack,
                       Stack &Initializer) {
  assert(BBStack.count(&MBB) == 0u && "Back edges are not supported yet");
  auto Predecessors = MBB.predecessors();
  auto NumPredecessors = MBB.pred_size();
  if (NumPredecessors == 0u) {
    auto [It, Flag] = BBStack.try_emplace(&MBB, Initializer);
    assert(Flag && "Insertion failed");
    return It->second;
  }
  if (NumPredecessors == 1u) {
    auto [It, Flag] =
        BBStack.try_emplace(&MBB, element(BBStack, *std::begin(Predecessors)));
    return It->second;
  }
  auto Begin = std::begin(Predecessors);
  auto &Stack = element(BBStack, *Begin);
  for (auto It = std::next(std::begin(Predecessors)),
            E = std::end(Predecessors);
       It != E; ++It) {
    auto *Predecessor = *It;
    Stack::join(Stack, element(BBStack, Predecessor),
                Predecessor->instr_back());
  }
  auto [It, Flag] = BBStack.insert({&MBB, Stack});
  return It->second;
}

/// Remove dead virtual registers from a non-exit BB's stack.
void pruneDeadRegisters(MachineBasicBlock &MBB, Stack &TheStack,
                        const LiveIntervals &LIS) {
  if (!MBB.succ_size())
    return;
  auto &Inst = MBB.front();
  std::vector<unsigned> DeadVR;
  for (auto &Element : TheStack) {
    if (std::holds_alternative<unsigned>(Element)) {
      auto Register = std::get<unsigned>(Element);
      if (Register == TVMFunctionInfo::UnusedReg) {
        DeadVR.push_back(Register);
        continue;
      }
      if (isKilled(Inst, Register, LIS) &&
          llvm::none_of(Inst.operands(), [Register](const auto &Operand) {
            if (!Operand.isReg())
              return false;
            return Operand.getReg() == Register;
          }))
        DeadVR.push_back(Register);
    }
  }
  for (auto Elem : DeadVR)
    TheStack.pop(Inst, Elem);
}

/// If \p MI result is not used remove it right away.
void pruneDeadDefinitions(MachineInstr &MI, LiveIntervals &LIS,
                          Stack &TheStack) {
  for (size_t I = 0, E = MI.getNumDefs(); I < E; ++I) {
    MachineOperand &MO = MI.getOperand(I);
    if (MO.isReg() && MO.isDead())
      TheStack.pop(MI, MO.getReg());
  }
}

/// Compute how to break a loop in stack reorderings for an instruction.
/// Since reorderings are computed independently there are possible XCHG loops
/// E.g. XCHG s0, s1; XCHG s1, s0. If to execute both instructions, the order
/// of elements in a stack won't change. The function compute which reordering
/// is to remove to break 2- or 3-loop.
/// \return Pointer to a reordering to remove.
const StackReordering *breakCycle(const StackReorderings &Reorderings,
                                  const std::vector<size_t> SlotsFrom) {
  assert(Reorderings.size() < 4 &&
         "Instructions with 4 or more stack arguments are not supported");
  if (Reorderings.size() < 2)
    return nullptr;

  size_t Size = Reorderings.size();

  // 2-cycles
  for (size_t I1 = 1; I1 < Size; ++I1)
    for (size_t I2 = 0; I2 < I1; ++I2)
      if (Reorderings[I1].SlotTo == SlotsFrom[I2] &&
          SlotsFrom[I1] == Reorderings[I2].SlotTo)
        return &Reorderings[I2];

  // 3-cycles
  // We excluded 2-cycles possibility, hovewer we have to check against
  // self-cycles.
  auto It = llvm::find(SlotsFrom, Reorderings[0].SlotTo);
  if (It == std::end(SlotsFrom) || It == std::begin(SlotsFrom))
    return nullptr;
  auto It2 = llvm::find(SlotsFrom, Reorderings[*It].SlotTo);
  if (It2 == std::end(SlotsFrom) || It == It2)
    return nullptr;
  auto It3 = llvm::find(SlotsFrom, Reorderings[*It2].SlotTo);
  if (It3 != std::begin(SlotsFrom))
    return nullptr;

  // A cycle with 3 reordering could be done with 2 XCHG, so break it on It2.
  return &Reorderings[*It2];
}

} // end anonymous namespace

char TVMStackModel::ID = 0;
INITIALIZE_PASS(TVMStackModel, DEBUG_TYPE, "Stackify register instructions",
                false, false)

FunctionPass *llvm::createTVMStackModel() { return new TVMStackModel(); }

StackReorderings
TVMStackModel::computeReorderings(MachineInstr &MI, LiveIntervals &LIS,
                                  Stack &TheStack,
                                  size_t NonStackOperands) const {
  StackReorderings Result{};
  size_t NumDefs = MI.getNumDefs();
  size_t NumOperands = MI.getNumOperands();

  llvm::SmallSet<unsigned, 3> RegUsed{};
  size_t NumStackOperands = NumOperands - NumDefs - NonStackOperands;

  // Expected operands order: defs, global addresses, registers or MBB,
  // constants
  size_t NumGlobals = 0;
  for (unsigned I = 0; I < NumOperands; ++I)
    if (MI.getOperand(I).isGlobal())
      NumGlobals++;

  // Let's ensure that all operands have expected type
  for (unsigned I = 0; I < NumOperands; ++I) {
    const auto &Op = MI.getOperand(I);
    if (I < NumDefs)
      assert(Op.isDef() && "Expected Def");
    else if (I < NumDefs + NumGlobals)
      assert(Op.isGlobal() && "Expected GlobalAddress");
    else if (I < NumDefs + NumGlobals + NumStackOperands)
      assert(isStackOperand(Op) && "Expected a register or a basic block");
    else
      // TODO: should be !isStackOperand(Op) once we put global address
      //       on stack.
      assert(Op.isImm() && "Expected Imm");
  }

  size_t NumImms = NumOperands - NumDefs - NumGlobals - NumStackOperands;
  assert(NumImms + NumGlobals == NonStackOperands);

  // The same register could be used multiple times, but the stack keeps
  // the only copy of it, so we need to produce copies for each but the last
  // usage of the register even if its killed by MI.
  llvm::DenseMap<unsigned, size_t> FirstUseOperandIndex(
      NextPowerOf2(2 * 4 / 3));
  for (size_t ROpNo = 0; ROpNo < NumStackOperands; ++ROpNo) {
    size_t OpNo = NumOperands - 1 - NumImms - ROpNo;
    const auto &Operand = MI.getOperand(OpNo);
    assert(isStackOperand(Operand) && "Wrong instruction format");
    // Control-flow arguments mustn't be used in an instruction more than once.
    // Thus we could omit tracking last use of it.
    if (!Operand.isReg())
      continue;
    RegUsed.insert(Operand.getReg());
    if (FirstUseOperandIndex.count(Operand.getReg()) == 0u)
      FirstUseOperandIndex[Operand.getReg()] = OpNo;
  }

  for (size_t ROpNo = 0; ROpNo < NumStackOperands; ++ROpNo) {
    size_t OpNo = NumOperands - 1 - NumImms - ROpNo;
    const auto &Operand = MI.getOperand(OpNo);
    assert(isStackOperand(Operand) && "Expected Reg or MBB");
    if (Operand.isReg()) {
      unsigned RegFrom = Operand.getReg();
      auto Kind =
          (isKilled(MI, RegFrom, LIS) && FirstUseOperandIndex[RegFrom] == OpNo)
              ? StackReorderingKind::Xchg
              : StackReorderingKind::Copy;
      Result.emplace_back(RegFrom, ROpNo, Kind);
    } else if (Operand.isMBB()) {
      Result.emplace_back(Operand.getMBB(), ROpNo, StackReorderingKind::New);
    }
  }

  // We need to adjust XChgs to number of non-register operands together with
  // number of Pushes followed by it.
  size_t NumPushes = 0;
  size_t TotalPushes = llvm::count_if(
      Result, [](const StackReordering &R) { return R.isCopy() || R.isNew(); });
  std::vector<size_t> SlotsFrom;
  for (auto &Reordering : Result) {
    // Note that we modify records in Result below
    size_t &SlotTo = Reordering.SlotTo;
    assert(SlotTo >= NumPushes);
    SlotTo -= NumPushes;
    if (Reordering.isCopy() || Reordering.isNew()) {
      ++NumPushes;
      // We don't care about pushes, so put a sentinel.
      // The maximal slot number is 255 by the specification.
      SlotsFrom.push_back(std::numeric_limits<size_t>::max());
    } else {
      size_t SlotFrom =
          TheStack.position(Reordering.ElemFrom) + TotalPushes - NumPushes;
      SlotsFrom.push_back(SlotFrom);
    }
  }

  // Break 2- and 3- cycles.
  // We don't care about self-cycles here because xchg implementation checks
  // against them.
  if (const StackReordering *BreakCycle = breakCycle(Result, SlotsFrom)) {
    Result.erase(
        llvm::remove_if(Result,
                        [BreakCycle](const StackReordering &Reordering) {
                          return &Reordering == BreakCycle;
                        }),
        std::end(Result));
  }

#ifndef NDEBUG
  // Simulate stack to ensure we have the correct operands on top
  Stack TheStack2 = TheStack;
  performReorderings(Result, nullptr, TheStack2);
  for (size_t ROpNo = 0; ROpNo < NumStackOperands; ++ROpNo) {
    size_t OpNo = NumOperands - 1 - NumImms - ROpNo;
    const auto &Op = MI.getOperand(OpNo);
    assert(isStackOperand(Op) && "Expected Reg or MBB");
    if (Op.isReg())
      assert(TheStack2.slotContains(ROpNo, Op.getReg()));
    if (Op.isMBB())
      assert(TheStack2.slotContains(ROpNo, Op.getMBB()));
  }
#endif

  return Result;
}

void TVMStackModel::performReorderings(const StackReorderings &Reorderings,
                                       MachineInstr *InsertPoint,
                                       Stack &TheStack) const {
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
    auto &Reordering = Reorderings[Idx];
    if (Reordering.isCopy() || Reordering.isNew()) {
      // If the current reordering is PUSH, execute preceding (in reverse order)
      // XCHGs
      for (size_t I = Idx + 1; I < LastPush; ++I) {
        assert(Reorderings[I].isXchg() && "Unexpected reordering");
        TheStack.xchg(InsertPoint, Reorderings[I]);
      }
      auto &Elem = Reordering.ElemFrom;
      if (Reordering.isCopy()) {
        TheStack.push(InsertPoint, Elem);
      } else {
        assert(std::holds_alternative<MachineBasicBlock *>(Elem) &&
               "Unimplemented");
        auto *MBB = std::get<MachineBasicBlock *>(Elem);
        assert(MBB);
        TheStack.pushNew(InsertPoint, *MBB);
      }
      if (Reordering.SlotTo > 0)
        TheStack.xchg(InsertPoint, Reordering);
      LastPush = Idx;
    }
    ++RevIdx;
  }
  // If reorderings start with XCHGs.
  for (size_t I = 0; I < LastPush; ++I) {
    assert(Reorderings[I].isXchg() && "Unexpected reordering");
    TheStack.xchg(InsertPoint, Reorderings[I]);
  }
}

bool TVMStackModel::processInstruction(MachineInstr &MI, LiveIntervals &LIS,
                                       const TargetInstrInfo *TII,
                                       Stack &TheStack) {
  if (MI.isImplicitDef())
    return false;

  size_t NumDefs = MI.getNumDefs();
  size_t NumOperands = MI.getNumOperands();

  if (MI.isReturn()) {
    assert(NumOperands <= 2 && "Multiple returns are not implemented yet");
    if (NumOperands == 0)
      TheStack.clear(&MI);
    else
      TheStack.clear(&MI, MI.getOperand(0).getReg());
    return true;
  }

  size_t NonStackOperands =
      count_if(MI.operands(),
               [](const MachineOperand &MOP) { return !isStackOperand(MOP); });

  size_t NumGlobals = 0, NumImms = 0;
  for (const MachineOperand &Op : MI.operands()) {
    if (Op.isGlobal())
      NumGlobals++;
    if (Op.isImm())
      NumImms++;
  }
  assert(NonStackOperands == NumGlobals + NumImms);
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
    NewOpcode = TVM::RegForm2SForm[MI.getOpcode()];
  performReorderings(Reorderings, &MI, TheStack);
  unsigned NumToConsume = NumOperands - NonStackOperands - NumDefs;
#ifndef NDEBUG
  // Let's ensure that consumed registers are used in instruction
  // TODO: Doesn't cover numerous corner cases. Covering them would require to
  // reimplement consumption under NDEBUG or extending consumption interface.
  for (unsigned I = 0; I < NumToConsume; I++)
    if (TheStack.isReg(I))
      assert(llvm::count_if(MI.operands(),
                            [&](const MachineOperand &Op) {
                              return Op.isReg() &&
                                     Op.getReg() == TheStack.reg(I);
                            }) &&
             "Consuming register not used in instruction");
#endif
  TheStack.consumeArguments(NumToConsume);
  for (size_t ROpNo = 0; ROpNo < NumDefs; ++ROpNo) {
    const auto &Operand = MI.getOperand(NumDefs - ROpNo - 1);
    assert(Operand.isReg() && "Def must be a register");
    TheStack.addDef(Operand.getReg());
  }
  if (NewOpcode >= 0) {
    // add global addresses before the command
    // TODO: continuation must be modelled in the stack then.
    for (unsigned I = 0; I < NumGlobals; I++) {
      const auto &Op = MI.getOperand(NumDefs + I);
      assert(Op.isGlobal() && "Expected GlobalAddress");
      BuildMI(&MI, TII->get(TVM::PUSHCONT_LABEL))
          .addGlobalAddress(Op.getGlobal(), Op.getOffset());
    }
    MachineInstrBuilder MIB = BuildMI(&MI, TII->get(NewOpcode));
    for (unsigned I = 0; I < NumImms; I++) {
      const auto &Op = MI.getOperand(NumOperands - NumImms + I);
      assert(Op.isImm() && "Expected Imm");
      MIB.addImm(Op.getImm());
    }
    pruneDeadDefinitions(MI, LIS, TheStack);
    MI.removeFromParent();
    Changed = true;
  }
  return Changed;
}

// Traverse all basic blocks in machine function and rewrite all instructions
// from R-form to S-form. All function arguments are already in stack.
// Topological order visitation. Back edges are not yet supported.
bool TVMStackModel::runOnBasicBlocks(MachineFunction &MF, Stack &TheStack) {
  bool Changed = false;

  LiveIntervals &LIS = getAnalysis<LiveIntervals>();
  DenseMap<MachineBasicBlock *, Stack> BBStack;
  const auto *TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();

  while (BBStack.size() < MF.size()) {
    size_t Size = BBStack.size();
    for (MachineBasicBlock &MBB : MF) {
      if (BBStack.count(&MBB) != 0u)
        continue;
      bool PredecessorsProcessed = true;
      for (auto *Predecessor : MBB.predecessors())
        if (BBStack.count(Predecessor) == 0u) {
          PredecessorsProcessed = false;
          break;
        }
      if (!PredecessorsProcessed)
        continue;
      Stack &CurrentStack = initializeStack(MBB, BBStack, TheStack);
      pruneDeadRegisters(MBB, CurrentStack, LIS);
      for (MachineBasicBlock::iterator I = MBB.begin(), E = MBB.end();
           I != E;) {
        MachineInstr &MI = *I++;
        assert(!TVM::isArgument(MI));

        if (MI.isDebugInstr() || MI.isLabel())
          continue;

        Changed |= processInstruction(MI, LIS, TII, CurrentStack);
      }
    }
    assert(BBStack.size() > Size &&
           "Not Implemented: there is a back edge in CFG.");
  }

  return Changed;
}

// TODO: For now it only stackifies function arguments. Extend.
bool TVMStackModel::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(
      dbgs()
      << "********** Rewrite Instructions in Reg-form to S-form **********\n"
         "********** Function: "
      << MF.getName() << '\n');

  bool Changed = false;
  TVMFunctionInfo &MFI = *MF.getInfo<TVMFunctionInfo>();
  const auto *TII = MF.getSubtarget<TVMSubtarget>().getInstrInfo();

  MachineBasicBlock &MF_first_block = *(MF.begin());
  assert(!MF_first_block.empty());

  auto &ANI = *(MF_first_block.begin());

  // Process ARGUMENT_NUM instruction to adjust arguments number on stack.
  if (TVM::isArgumentNum(ANI)) {
    int args = ANI.getOperand(0).getImm();
    for (int i = 0; i < args; i++)
      MFI.addParam(MVT::i64);
    ANI.eraseFromParent();
    Changed = true;
  }

  size_t NumArgs = MFI.numParams();
  Stack TheStack(TII, NumArgs);

  // Handle ARGUMENTS first to ensure that they get the designated numbers.
  for (MachineBasicBlock::iterator I = MF_first_block.begin(),
                                   E = MF_first_block.end();
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

  if (runOnBasicBlocks(MF, TheStack))
    Changed = true;

  return Changed;
}
