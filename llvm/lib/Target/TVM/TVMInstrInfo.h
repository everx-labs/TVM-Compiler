//===-- TVMInstrInfo.h - TVM Instruction Information ------------*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_TVM_TVMINSTRINFO_H
#define LLVM_LIB_TARGET_TVM_TVMINSTRINFO_H

#include "TVMRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "TVMGenInstrInfo.inc"

namespace llvm {

class TVMSubtarget;

class TVMInstrInfo : public TVMGenInstrInfo {
public:
  explicit TVMInstrInfo(TVMSubtarget &STI);

  /// getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  /// such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  ///
  const TargetRegisterInfo &getRegisterInfo() const { return RI; }

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
                   const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc) const override;

  unsigned getInstSizeInBytes(const MachineInstr &MI) const override;

  int64_t getFramePoppedByCallee(const MachineInstr &I) const {
    assert(isFrameInstr(I) && "Not a frame instruction");
    assert(I.getOperand(1).getImm() >= 0 && "Size must not be negative");
    return I.getOperand(1).getImm();
  }

  // Predication support
  /// Returns true if the instruction is already predicated.
  bool isPredicated(const MachineInstr &MI) const override;

  /// Convert the instruction into a predicated instruction.
  /// It returns true if the operation was successful.
  bool PredicateInstruction(MachineInstr &MI,
                            ArrayRef<MachineOperand> Pred) const override;

  /// Returns true if the first specified predicate
  /// subsumes the second, e.g. GE subsumes GT.
  bool SubsumesPredicate(ArrayRef<MachineOperand> Pred1,
                         ArrayRef<MachineOperand> Pred2) const override;

  /// If the specified instruction defines any predicate
  /// or condition code register(s) used for predication, returns true as well
  /// as the definition predicate(s) by reference.
  bool DefinesPredicate(MachineInstr &MI,
                        std::vector<MachineOperand> &Pred) const /* override */;

  int getCondOpcode(int Opc, bool IfTrue) const;

  /// Analyze the branching code at the end of MBB, returning
  /// true if it cannot be understood (e.g. it's a switch dispatch or isn't
  /// implemented for a target).  Upon success, this returns false and returns
  /// with the following information in various cases:
  ///
  /// 1. If this block ends with no branches (it just falls through to its succ)
  ///    just return false, leaving TBB/FBB null.
  /// 2. If this block ends with only an unconditional branch, it sets TBB to be
  ///    the destination block.
  /// 3. If this block ends with a conditional branch and it falls through to a
  ///    successor block, it sets TBB to be the branch destination block and a
  ///    list of operands that evaluate the condition. These operands can be
  ///    passed to other TargetInstrInfo methods to create new branches.
  /// 4. If this block ends with a conditional branch followed by an
  ///    unconditional branch, it returns the 'true' destination in TBB, the
  ///    'false' destination in FBB, and a list of operands that evaluate the
  ///    condition.  These operands can be passed to other TargetInstrInfo
  ///    methods to create new branches.
  ///
  /// Note that removeBranch and insertBranch must be implemented to support
  /// cases where this method returns success.
  ///
  /// If AllowModify is true, then this routine is allowed to modify the basic
  /// block (e.g. delete instructions after the unconditional branch).
  bool analyzeBranch(MachineBasicBlock &MBB, MachineBasicBlock *&TBB,
                     MachineBasicBlock *&FBB,
                     SmallVectorImpl<MachineOperand> &Cond,
                     bool AllowModify) const override;

  /// Remove the branching code at the end of the specific MBB.
  /// This is only invoked in cases where AnalyzeBranch returns success. It
  /// returns the number of instructions that were removed.
  unsigned removeBranch(MachineBasicBlock &MBB,
                        int *BytesRemoved = nullptr) const override;

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
  unsigned insertBranch(MachineBasicBlock &MBB, MachineBasicBlock *TBB,
                        MachineBasicBlock *FBB, ArrayRef<MachineOperand> Cond,
                        const DebugLoc &DL,
                        int *BytesAdded = nullptr) const override;

  // TODO override
  bool reverseBranchCondition(SmallVectorImpl<MachineOperand> &Cond) const override;

  // TODO override
  void ReplaceUsesOfBlockWith(MachineBasicBlock *Pred,
                              MachineBasicBlock *Old,
                              MachineBasicBlock *New) const /* override */;
  bool canFallthrough(MachineBasicBlock &From, MachineBasicBlock &To) const /* override */;

private:
  const TVMRegisterInfo RI;
  virtual void anchor();
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMINSTRINFO_H
