//===-- TVMMachineFunctionInfo.h - TVM machine function info -----*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file declares TVM-specific per-machine-function information.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMMACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_TVM_TVMMACHINEFUNCTIONINFO_H

#include "MCTargetDesc/TVMMCTargetDesc.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

namespace llvm {

/// This class is derived from MachineFunctionInfo and contains private
/// TVM-specific information for each MachineFunction.
class TVMFunctionInfo final : public MachineFunctionInfo {
  MachineFunction &MF;

  std::vector<MVT> Params;
  std::vector<MVT> Results;
  std::vector<MVT> Locals;

  /// A mapping from CodeGen vreg index to TVM register number.
  std::vector<unsigned> TVMRegs;

  /// A mapping from MachineInstr to stack model commentaries
  ///  (for stack model after this instruction)
  std::map<const MachineInstr *, std::vector<std::string>> StackModelComments;

  /// A mapping from MachineInstr after stack model to list of operand registers
  /// which have been used in the instruction before stack model
  std::map<const MachineInstr *, std::vector<unsigned>> StackModelSourceRegs;

  /// A mapping from MachineBasicBlock to starting stack model comment
  ///  (incoming stack model for this block)
  std::map<const MachineBasicBlock *, std::string> StackModelBBComments;

  /// A mapping from CodeGen vreg index to a boolean value indicating whether
  /// the given register is considered to be "stackified", meaning it has been
  /// determined or made to meet the stack requirements:
  ///   - single use (per path)
  ///   - single def (per path)
  ///   - defined and used in LIFO order with other stack registers
  BitVector VRegStackified;

  /// Virtual register allocated to save c0 for function return lowering
  unsigned C0VirtReg = 0;

public:
  explicit TVMFunctionInfo(MachineFunction &MF);
  ~TVMFunctionInfo() override;

  void addParam(MVT VT) { Params.push_back(VT); }
  const std::vector<MVT> &getParams() const { return Params; }
  size_t numParams() const { return Params.size(); }

  void addResult(MVT VT) { Results.push_back(VT); }
  const std::vector<MVT> &getResults() const { return Results; }

  void clearParamsAndResults() {
    Params.clear();
    Results.clear();
  }

  void setNumLocals(size_t NumLocals) { Locals.resize(NumLocals, MVT::i64); }
  void setLocal(size_t i, MVT VT) { Locals[i] = VT; }
  void addLocal(MVT VT) { Locals.push_back(VT); }
  const std::vector<MVT> &getLocals() const { return Locals; }

  inline static constexpr unsigned UnusedReg = -1u;

  void stackifyVReg(unsigned VReg) {
    assert(MF.getRegInfo().getUniqueVRegDef(VReg));
    auto I = Register::virtReg2Index(VReg);
    if (I >= VRegStackified.size())
      VRegStackified.resize(I + 1);
    VRegStackified.set(I);
  }
  bool isVRegStackified(unsigned VReg) const {
    auto I = Register::virtReg2Index(VReg);
    if (I >= VRegStackified.size())
      return false;
    return VRegStackified.test(I);
  }

  void initTVMRegs();
  void setTVMReg(unsigned VReg, unsigned WAReg) {
    assert(WAReg != UnusedReg);
    auto I = Register::virtReg2Index(VReg);
    assert(I < TVMRegs.size());
    TVMRegs[I] = WAReg;
  }
  unsigned getTVMReg(unsigned VReg) const {
    auto I = Register::virtReg2Index(VReg);
    assert(I < TVMRegs.size());
    return TVMRegs[I];
  }

  void addStackModelComment(const MachineInstr *MI, const std::string &val) {
    StackModelComments[MI].push_back(val);
  }
  void resetStackModelComment(const MachineInstr *MI, const std::string &val) {
    StackModelComments[MI].clear();
    StackModelComments[MI].push_back(val);
  }
  const std::vector<std::string> &
  getStackModelComments(const MachineInstr *MI) const {
    auto it = StackModelComments.find(MI);
    if (it != StackModelComments.end())
      return it->second;
    it = StackModelComments.find(nullptr);
    assert(it != StackModelComments.end());
    return it->second; // empty vector
  }

  void setStackModelBBComment(const MachineBasicBlock *MBB,
                              const std::string &val) {
    StackModelBBComments[MBB] = val;
  }
  StringRef getStackModelBBComment(const MachineBasicBlock *MBB) const {
    auto it = StackModelBBComments.find(MBB);
    return (it != StackModelBBComments.end()) ? StringRef(it->second)
                                              : StringRef("");
  }

  // Add mapping of instructions before stack model
  void setStackModelSourceRegs(const MachineInstr *Target,
                               std::vector<unsigned> &&regs) {
    StackModelSourceRegs[Target] = std::move(regs);
  }
  void setStackModelSourceRegs(const MachineInstr *Target,
                               const std::vector<unsigned> &regs) {
    StackModelSourceRegs[Target] = regs;
  }
  const std::vector<unsigned> *
  getStackModelSourceRegs(const MachineInstr *Target) {
    auto It = StackModelSourceRegs.find(Target);
    if (It == StackModelSourceRegs.end())
      return nullptr;
    return &It->second;
  }

  // Clone machine instruction intermediate data
  void cloneMachineInstrIntermediateData(const MachineInstr *Source,
                                         const MachineInstr *Destination) {
    StackModelComments[Destination].clear();
    for (const auto &Comment : getStackModelComments(Source)) {
      addStackModelComment(Destination, Comment);
    }

    if (const auto *Regs = getStackModelSourceRegs(Source)) {
      setStackModelSourceRegs(Destination, *Regs);
    }
  }

  // Clear MachineInstr intermediate data
  const MachineInstrBuilder &
  clearIntermediateData(const MachineInstrBuilder &Builder) {
    clearIntermediateData(Builder.getInstr());
    return Builder;
  }

  const MachineInstr *clearIntermediateData(const MachineInstr *MI) {
    StackModelComments[MI].clear();
    StackModelSourceRegs.erase(MI);
    return MI;
  }

  // For a given stackified WAReg, return the id number to print with push/pop.
  static unsigned getTVMRegStackId(unsigned Reg) {
    assert(Reg & INT32_MIN);
    return Reg & INT32_MAX;
  }

  // Accessors to work with virtual register for c0 saving
  unsigned int getC0VirtReg() const { return C0VirtReg; }

  bool hasC0VirtReg() const { return C0VirtReg != 0; }

  void setC0VirtReg(unsigned Register) {
    assert(!hasC0VirtReg() && "C0 virtual register has been already saved");
    assert(Register && "C0 virtual register has to be non zero");
    C0VirtReg = Register;
  }
};

void ComputeLegalValueVTs(const Function &F, const TargetMachine &TM, Type *Ty,
                          SmallVectorImpl<MVT> &ValueVTs);

void ComputeSignatureVTs(const Function &F, const TargetMachine &TM,
                         SmallVectorImpl<MVT> &Params,
                         SmallVectorImpl<MVT> &Results);

} // end namespace llvm

#endif
