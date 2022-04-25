//===-- TVMMCAsmInfo.h - TVM asm properties --------------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the TVMMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_MCTARGETDESC_TVMMCASMINFO_H
#define LLVM_LIB_TARGET_TVM_MCTARGETDESC_TVMMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class TVMMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit TVMMCAsmInfo(const Triple &TT);
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_MCTARGETDESC_TVMMCASMINFO_H
