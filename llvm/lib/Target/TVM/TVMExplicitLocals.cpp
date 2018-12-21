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

#include "MCTargetDesc/TVMMCTargetDesc.h"
#include "TVM.h"
#include "TVMExtras.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMSubtarget.h"
#include "TVMUtilities.h"
#include "llvm/CodeGen/MachineBlockFrequencyInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include <deque>
using namespace llvm;

#define DEBUG_TYPE "tvm-explicit-locals"

namespace {

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
  /// \par Slot number of stack slot (i.e. N in sN).
  bool push(MachineInstr *InsertPoint, unsigned Reg);
  /// Fill the specified \p Slot with \p Reg. Doesn't generate any instruction.
  void set(size_t Slot, unsigned Reg) {
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
  static inline constexpr size_t PushLimit = 256;

private:
  const TargetInstrInfo *TII;
  std::deque<unsigned> Data;
};

class TVMExplicitLocals final : public MachineFunctionPass {
public:
  StringRef getPassName() const override { return "TVM Explicit Locals"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    AU.addPreserved<MachineBlockFrequencyInfo>();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;
  bool processInstruction(MachineInstr &MI, Stack &TheStack);

  static char ID; // Pass identification, replacement for typeid
  TVMExplicitLocals() : MachineFunctionPass(ID) {}
};
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
  auto It = llvm::find_or_fail(Data, Reg);
  size_t RegSlot = std::distance(std::begin(Data), It);
  assert(RegSlot <= PushLimit && "Unimplemented");
  unsigned Opc = getPushOpcode(&TVM::I64RegClass);
  BuildMI(*InsertPoint->getParent(), InsertPoint, InsertPoint->getDebugLoc(),
          TII->get(Opc))
      .addImm(RegSlot);
  Data.push_front(*It);
  return true;
}

bool TVMExplicitLocals::processInstruction(MachineInstr &MI, Stack &TheStack) {
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
  size_t NonStackOperands = 0;
  bool Changed = false;
  // Instruction format: INST %defs..., other operands...
  for (size_t ROpNo = NumDefs; ROpNo < NumOperands; ++ROpNo) {
    size_t OpNo = NumOperands - ROpNo;
    const auto& Operand = MI.getOperand(OpNo);
    if (!Operand.isReg()) {
      ++NonStackOperands;
      continue;
    }
    // For now we PUSH all arguments to the right place.
    // TODO: If the instruction kills the argument it could be XCHG'ed instead.
    // TODO: Commutative and Reversible (i.e. having R form e.g. SUBR) need
    // special optimization.
    Changed |= TheStack.push(&MI, Operand.getReg());
  }
  TheStack.consumeArguments(NumOperands - NonStackOperands - NumDefs);
  for (size_t ROpNo = 0; ROpNo < NumDefs; ++ROpNo) {
    size_t OpNo = NumDefs - ROpNo - 1;
    const auto& Operand = MI.getOperand(OpNo);
    assert(Operand.isReg() && "Def must be a register");
    TheStack.addDef(Operand.getReg());
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

  // Handle ARGUMENTS first to ensure that they get the designated numbers.
  for (MachineBasicBlock::iterator I = MF.begin()->begin(),
                                   E = MF.begin()->end();
       I != E;) {
    MachineInstr &MI = *I++;
    if (!TVM::isArgument(MI))
      break;
    unsigned Reg = MI.getOperand(0).getReg();
    assert(!MFI.isVRegStackified(Reg));
    unsigned ArgNo = MI.getOperand(1).getImm();
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
      Changed |= processInstruction(MI, TheStack);
    }
  }

  return Changed;
}
