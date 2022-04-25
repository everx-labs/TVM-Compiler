//===-- TVMMCExpr.h - TVM specific MC expression classes --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TVM_TVMMVEXPR_H
#define LLVM_LIB_TARGET_TVM_TVMMVEXPR_H

#include "llvm/ADT/StringRef.h"
#include "llvm/MC/MCExpr.h"

namespace llvm {

class TVMImmStringMCExpr : public MCTargetExpr {
private:
  StringRef Data;

  explicit TVMImmStringMCExpr(const StringRef &Data) : Data(Data) {}

public:
  /// @name Construction
  /// @{

  static const TVMImmStringMCExpr *create(const StringRef &Data,
                                          MCContext &Ctx);

  /// @}
  /// @name Accessors
  /// @{

  /// getOpcode - Get the kind of this expression.
  StringRef getString() const { return Data; }
  /// @}

  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;
  bool evaluateAsRelocatableImpl(MCValue &Res, const MCAsmLayout *Layout,
                                 const MCFixup *Fixup) const override {
    return false;
  }
  void visitUsedExpr(MCStreamer &Streamer) const override{};
  MCFragment *findAssociatedFragment() const override { return nullptr; }

  // There are no TLS NVPTXMCExprs at the moment.
  void fixELFSymbolsInTLSFixups(MCAssembler &Asm) const override {}

  static bool classof(const MCExpr *E) {
    return E->getKind() == MCExpr::Target;
  }
};

} // end namespace llvm

#endif
