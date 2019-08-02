//===--- TVMStackBlockInfo.h - block details about stack model --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMSTACKBLOCKINFO_H
#define LLVM_LIB_TARGET_TVM_TVMSTACKBLOCKINFO_H

#include "TVMStack.h"

namespace llvm {

class MachineBasicBlock;

class TVMStackBlockInfo {
public:
  void setMBB(MachineBasicBlock *MBBv) { MBB = MBBv; }

  void setFixedBegin(const Stack &stack) {
    FixedBegin = stack;
  }
  void setFixedEnd(const Stack &stack) {
    FixedEnd = stack;
  }
  void setCalculatedEnd(const Stack &stack) {
    CalculatedEnd = stack;
  }
  const Stack &fixedBegin() const {
    assert(FixedBegin && "Non-fixed begin in TVMStackBlockInfo");
    return *FixedBegin;
  }
  const Stack &calculatedEnd() const {
    assert(CalculatedEnd && "Non-calculated end in TVMStackBlockInfo");
    return *CalculatedEnd;
  }
  const Stack &fixedEnd() const {
    assert(FixedEnd && "Non-fixed end in TVMStackBlockInfo");
    return *FixedEnd;
  }
  bool isFixedEnd() const {
    return FixedEnd.has_value();
  }

  unsigned roadBegin() const { return RoadBegin; }
  unsigned roadEnd() const { return RoadEnd; }

  void setRoadBegin(unsigned roadBegin) {
    RoadBegin = roadBegin;
  }
  void setRoadEnd(unsigned roadEnd) {
    RoadEnd = roadEnd;
  }
private:
  MachineBasicBlock *MBB = nullptr;
  /// Initial stack state
  std::optional<Stack> FixedBegin;
  /// Calculated end stack model (before terminator and fixups)
  std::optional<Stack> CalculatedEnd;
  /// Requred final stack state after applying fixup
  std::optional<Stack> FixedEnd;

  unsigned RoadBegin = 0;
  unsigned RoadEnd = 0;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMSTACKBLOCKINFO_H
