//===-- TVMInstrInfo.cpp - TVM Instruction Information --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the TVM implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "TVMInstrInfo.h"
#include "TVM.h"
#include "TVMMachineFunctionInfo.h"
#include "TVMTargetMachine.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "TVMGenInstrInfo.inc"

// Pin the vtable to this file.
void TVMInstrInfo::anchor() {}

TVMInstrInfo::TVMInstrInfo(TVMSubtarget &)
    : TVMGenInstrInfo(TVM::ADJCALLSTACKDOWN, TVM::ADJCALLSTACKUP), RI() {}

void TVMInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                               MachineBasicBlock::iterator I,
                               const DebugLoc &DL, MCRegister DestReg,
                               MCRegister SrcReg, bool KillSrc) const {
  BuildMI(MBB, I, DL, get(TVM::REG_TO_REG_COPY), DestReg)
      .addReg(SrcReg, KillSrc ? RegState::Kill : 0);
}

unsigned TVMInstrInfo::getInstSizeInBytes(const MachineInstr &MI) const {
  const MCInstrDesc &Desc = MI.getDesc();

  switch (Desc.getOpcode()) {
  case TargetOpcode::CFI_INSTRUCTION:
  case TargetOpcode::EH_LABEL:
  case TargetOpcode::IMPLICIT_DEF:
  case TargetOpcode::KILL:
  case TargetOpcode::DBG_VALUE:
    return 0;
  case TargetOpcode::INLINEASM: {
    const MachineFunction *MF = MI.getParent()->getParent();
    const TargetInstrInfo &TII = *MF->getSubtarget().getInstrInfo();
    return TII.getInlineAsmLength(MI.getOperand(0).getSymbolName(),
                                  *MF->getTarget().getMCAsmInfo());
  }
  }

  return 8;
}

// Predication support
/// Returns true if the instruction is already predicated.
bool TVMInstrInfo::isPredicated(const MachineInstr &MI) const {
  return MI.getOpcode() == TVM::THROWIF || MI.getOpcode() == TVM::THROWIFNOT;
}

/// Convert the instruction into a predicated instruction.
/// It returns true if the operation was successful.
bool TVMInstrInfo::PredicateInstruction(MachineInstr &MI,
                                        ArrayRef<MachineOperand> Pred) const {
  // Expecting 2 vals in Pred { Inverted, CondReg }
  if (MI.getOpcode() != TVM::THROW || Pred.size() != 2) {
    return false;
  }
  assert (isPredicable(MI) && "Expected predicable instruction");
  assert (Pred[0].isImm() && "Expected immediate for 'Inverted'");
  assert (Pred[1].isReg() && "Expected register for 'Cond'");
  bool Inverted = Pred[0].getImm();
  auto ErrCode = MI.getOperand(0);
  MI.RemoveOperand(0);
  MI.setDesc(get(Inverted ? TVM::THROWIFNOT : TVM::THROWIF));
  MachineInstrBuilder(*MI.getParent()->getParent(), MI)
      .add(Pred[1])
      .add(ErrCode);
  return true;
}

/// Returns true if the first specified predicate
/// subsumes the second, e.g. GE subsumes GT.
bool TVMInstrInfo::SubsumesPredicate(ArrayRef<MachineOperand> Pred1,
                                     ArrayRef<MachineOperand> Pred2) const {
  assert(Pred1.size() == 2 && "Invalid TVM first predicate");
  assert(Pred2.size() == 2 && "Invalid TVM second predicate");
  return Pred1[0].getImm() == Pred2[0].getImm() &&
      Pred1[1].getReg() == Pred2[1].getReg();
}

/// If the specified instruction defines any predicate
/// or condition code register(s) used for predication, returns true as well
/// as the definition predicate(s) by reference.
bool TVMInstrInfo::DefinesPredicate(MachineInstr &,
                                    std::vector<MachineOperand> &) const {
  return false;
}

int TVMInstrInfo::getCondOpcode(int Opc, bool IfTrue) const {
  if (Opc == TVM::THROW) {
    return IfTrue ? TVM::THROWIF : TVM::THROWIFNOT;
  }
  llvm_unreachable("TVMInstrInfo::getCondOpcode for unsupported opcode");
}

// JMPX, IFELSE, THROW, THROWIF
bool TVMInstrInfo::analyzeBranch(MachineBasicBlock &MBB,
                                 MachineBasicBlock *&TBB,
                                 MachineBasicBlock *&FBB,
                                 SmallVectorImpl<MachineOperand> &Cond,
                                 bool) const {
  TBB = nullptr;
  FBB = nullptr;
  Cond.clear();

  // If the block has no terminators, it just falls into the block after it.
  MachineBasicBlock::instr_iterator I = MBB.instr_end();
  if (I == MBB.instr_begin())
    return false;

  I = MBB.instr_end();
  --I;
  while (I->isDebugInstr()) {
    if (I == MBB.instr_begin())
      return false;
    --I;
  }
  if (!I->isTerminator())
    return false;
  const MachineRegisterInfo &MRI = MBB.getParent()->getRegInfo();
  if (I->getOpcode() == TVM::JMPX) {
    assert(I->getOperand(0).isReg() && "JMPX operand is not register");
    unsigned ContReg = I->getOperand(0).getReg();
    auto* PushMI = MRI.getVRegDef(ContReg);
    if (!PushMI)
      return true;
    assert(PushMI && PushMI->getOpcode() == TVM::PUSHCONT_MBB &&
           "JMPX op is not from PUSHCONT_MBB");
    assert(PushMI->getOperand(1).isMBB() && "PUSHCONT_MBB op is not MBB");
    TBB = PushMI->getOperand(1).getMBB();
    return false;
  }
  if (I->getOpcode() == TVM::IFELSE) {
    Cond.push_back(MachineOperand::CreateImm(0));
    Cond.push_back(I->getOperand(0));
    unsigned TrueReg = I->getOperand(1).getReg();
    unsigned FalseReg = I->getOperand(2).getReg();
    auto* TruePushMI = MRI.getVRegDef(TrueReg);
    if (!TruePushMI)
      return true;
    assert(TruePushMI->getOpcode() == TVM::PUSHCONT_MBB &&
           "IFELSE op is not from PUSHCONT_MBB");
    auto* FalsePushMI = MRI.getVRegDef(FalseReg);
    if (!FalsePushMI)
      return true;
    assert(FalsePushMI->getOpcode() == TVM::PUSHCONT_MBB &&
           "IFELSE op is not from PUSHCONT_MBB");
    TBB = TruePushMI->getOperand(1).getMBB();
    FBB = FalsePushMI->getOperand(1).getMBB();

    // BranchFolding trying to make single-block back-branch as TBB
    // We need to make this swap to prevent infinite loop
    if (FBB == &MBB) {
      std::swap(TBB, FBB);
      Cond[0].setImm(1);
    }

    return false;
  }
  if (I->getOpcode() == TVM::IFJMP || I->getOpcode() == TVM::IFNOTJMP) {
    Cond.push_back(MachineOperand::CreateImm(I->getOpcode() == TVM::IFNOTJMP));
    Cond.push_back(I->getOperand(0));
    unsigned TrueReg = I->getOperand(1).getReg();
    auto* TruePushMI = MRI.getVRegDef(TrueReg);
    if (!TruePushMI)
      return true;
    assert(TruePushMI->getOpcode() == TVM::PUSHCONT_MBB &&
           "IFJMP op is not from PUSHCONT_MBB");
    TBB = TruePushMI->getOperand(1).getMBB();
    FBB = nullptr;
    return false;
  }
  if (I->getOpcode() == TVM::THROW) {
    return true;
  }
  if (I->getOpcode() == TVM::THROWIF) {
    return true;
  }
  if (I->getOpcode() == TVM::THROWIFNOT) {
    return true;
  }
  // Otherwise, can't handle this.
  return true;
}

/// Remove the branching code at the end of the specific MBB.
/// This is only invoked in cases where AnalyzeBranch returns success. It
/// returns the number of instructions that were removed.
unsigned TVMInstrInfo::removeBranch(MachineBasicBlock &MBB,
                                    int *BytesRemoved) const {
  MachineBasicBlock::iterator I = MBB.getLastNonDebugInstr();
  if (I == MBB.end())
    return 0;
  if (I->getOpcode() != TVM::JMPX &&
      I->getOpcode() != TVM::IFELSE &&
      I->getOpcode() != TVM::IFJMP &&
      I->getOpcode() != TVM::IFNOTJMP)
    return 0;
  if (I->getOpcode() == TVM::JMPX) {
    if (BytesRemoved)
      *BytesRemoved = 8;
    I->eraseFromParent();
    return 1;
  }
  if (I->getOpcode() == TVM::IFELSE) {
    if (BytesRemoved)
      *BytesRemoved = 8;
    I->eraseFromParent();
    return 1;
  }
  if (I->getOpcode() == TVM::IFJMP || I->getOpcode() == TVM::IFNOTJMP) {
    if (BytesRemoved)
      *BytesRemoved = 8;
    I->eraseFromParent();
    return 1;
  }
  return 0;
}

/// Insert branch code into the end of the specified MachineBasicBlock.
/// The operands to this method are the same as those
/// returned by AnalyzeBranch.  This is only invoked in cases where
/// AnalyzeBranch returns success. It returns the number of instructions
/// inserted.
///
/// It is also invoked by tail merging to add unconditional branches in
/// cases where AnalyzeBranch doesn't apply because there was no original
/// branch to analyze.  At least this much must be implemented, else tail
/// merging needs to be disabled.
unsigned TVMInstrInfo::
  insertBranch(MachineBasicBlock &MBB, MachineBasicBlock *TBB,
               MachineBasicBlock *FBB, ArrayRef<MachineOperand> Cond,
               const DebugLoc &DL, int *BytesAdded) const {
  // Shouldn't be a fall through.
  assert(TBB && "insertBranch must not be told to insert a fallthrough");
  MachineRegisterInfo &MRI = MBB.getParent()->getRegInfo();
  if (TBB && FBB) {
    assert(Cond.size() == 2 && "Wrong Conds size");
    assert(MBB.getFirstTerminator() == MBB.end() && "Block already has terminator");
    bool Inverted = Cond[0].getImm();
    auto Pred = Cond[1];

    // TBB and FBB may be in vregs defined by PUSHCONT_MBB with dominated def
    //  to this place.
    // Or may be not. And then we need to insert PUSHCONT_MBBs before IFELSE.
    // And better run TVMContinuationsHoist after to optimize PUSHCONTs.

    unsigned TrueReg = MRI.createVirtualRegister(&TVM::I257RegClass);
    BuildMI(&MBB, DL, get(TVM::PUSHCONT_MBB), TrueReg).addMBB(TBB)
        .addImm(0);
    unsigned FalseReg = MRI.createVirtualRegister(&TVM::I257RegClass);
    BuildMI(&MBB, DL, get(TVM::PUSHCONT_MBB), FalseReg).addMBB(FBB)
        .addImm(0);
    if (Inverted)
      std::swap(TrueReg, FalseReg);

    BuildMI(&MBB, DL, get(TVM::IFELSE))
        .add(Pred)
        .addReg(TrueReg)
        .addReg(FalseReg);
    if (BytesAdded)
      *BytesAdded = 8 * 3;
    return 3;
  }
  if (Cond.empty()) {
    assert(!FBB && "FBB in uncond branch");
    auto IfJmp = MBB.getFirstTerminator();
    if (IfJmp != MBB.end()) {
      // Inserting uncond branch after existing cond branch -
      //  converting to IFELSE
      assert((IfJmp->getOpcode() == TVM::IFJMP ||
              IfJmp->getOpcode() == TVM::IFNOTJMP) &&
             "We can only insert uncond branch after cond branch");
      auto Cond = IfJmp->getOperand(0);
      unsigned TrueReg = IfJmp->getOperand(1).getReg();
      bool Inverted = IfJmp->getOpcode() == TVM::IFNOTJMP;
      IfJmp->eraseFromParent();
      unsigned FalseReg = MRI.createVirtualRegister(&TVM::I257RegClass);
      BuildMI(&MBB, DL, get(TVM::PUSHCONT_MBB), FalseReg).addMBB(TBB)
          .addImm(0);
      if (Inverted)
        std::swap(TrueReg, FalseReg);
      BuildMI(&MBB, DL, get(TVM::IFELSE))
          .add(Cond)
          .addReg(TrueReg)
          .addReg(FalseReg);
      if (BytesAdded)
        *BytesAdded = 8;
      return 1;
    } else {
      unsigned TrueReg = MRI.createVirtualRegister(&TVM::I257RegClass);
      BuildMI(&MBB, DL, get(TVM::PUSHCONT_MBB), TrueReg).addMBB(TBB)
          .addImm(0);
      BuildMI(&MBB, DL, get(TVM::JMPX)).addReg(TrueReg);
      if (BytesAdded)
        *BytesAdded = 8 * 2;
      return 2;
    }
  } else {
    assert(Cond.size() == 2 && "Wrong Conds size");
    assert(MBB.getFirstTerminator() == MBB.end() &&
           "Block already has terminator");

    bool Inverted = Cond[0].getImm();
    auto Pred = Cond[1];

    unsigned TrueReg = MRI.createVirtualRegister(&TVM::I257RegClass);
    BuildMI(&MBB, DL, get(TVM::PUSHCONT_MBB), TrueReg).addMBB(TBB)
        .addImm(0);
    BuildMI(&MBB, DL, get(Inverted ? TVM::IFNOTJMP : TVM::IFJMP))
        .add(Pred)
        .addReg(TrueReg);
    if (BytesAdded)
      *BytesAdded = 8 * 2;
    return 2;
  }
}

bool TVMInstrInfo::reverseBranchCondition(
    SmallVectorImpl<MachineOperand> &Cond) const {
  assert(Cond.size() == 2 && "Wrong Cond args size");
  Cond[0].setImm(!Cond[0].getImm());
  return false;
}

namespace {
template<typename Func>
// TODO: Maybe implement separate register class for MBB operands
void enumMBBoperands(MachineInstr &Term, Func func) {
  switch (Term.getOpcode()) {
  case TVM::IF:    case TVM::IFNOT:
  case TVM::IFJMP: case TVM::IFNOTJMP:
    func(Term.getOperand(1));
    break;
  case TVM::IFELSE:
    func(Term.getOperand(1));
    func(Term.getOperand(2));
    break;
  case TVM::JMPX:
    func(Term.getOperand(0));
    break;
  default:
    llvm_unreachable("Unsupported terminator in enumMBBoperands");
    break;
  }
}
}

void TVMInstrInfo::ReplaceUsesOfBlockWith(MachineBasicBlock *Pred,
                                          MachineBasicBlock *Old,
                                          MachineBasicBlock *New) const {
  MachineRegisterInfo &MRI = Pred->getParent()->getRegInfo();
  for (MachineInstr &Term : Pred->terminators()) {
    enumMBBoperands(Term, [&](MachineOperand &Op) {
      if (!Op.isReg())
        return;
      unsigned MbbReg = Op.getReg();
      auto* PushMI = MRI.getVRegDef(MbbReg);
      if (PushMI->getOpcode() != TVM::PUSHCONT_MBB)
       return;
      assert(PushMI->getOperand(1).isMBB() && "PUSHCONT_MBB op is not MBB");

      if (PushMI->getOperand(1).getMBB() == Old) {
        if (MRI.hasOneNonDBGUse(MbbReg)) {
          // If we have only one non-debug use, we can override existing mbb
          PushMI->getOperand(1).setMBB(New);
        } else {
          // We need to find existing PUSHCONT_MBB with New mbb,
          //  dominating this terminator,
          // Or insert new PUSHCONT_MBB right before terminator
          //  or before label decl (for back-branches)

          auto it = Pred->getFirstTerminator();
          auto DL = it->getDebugLoc();
          MbbReg = MRI.createVirtualRegister(&TVM::I257RegClass);
          BuildMI(*Pred, it, DL, get(TVM::PUSHCONT_MBB), MbbReg).addMBB(New)
              .addImm(0);
          Op.setReg(MbbReg);
        }
      }
    });
  }
  // Update the successor information.
  Pred->replaceSuccessor(Old, New);
}

bool TVMInstrInfo::canFallthrough(MachineBasicBlock &,
                                  MachineBasicBlock &To) const {
  // We can fallthrough only to block with single predecessor.
  return To.pred_size() == 1;
}
