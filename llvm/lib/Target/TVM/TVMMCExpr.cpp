//===-- TVMMCExpr.cpp - TVM specific MC expression classes ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "TVMMCExpr.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
using namespace llvm;

#define DEBUG_TYPE "tvm-mcexpr"

const TVMImmStringMCExpr *TVMImmStringMCExpr::create(const StringRef &Data,
                                                     MCContext &Ctx) {
  return new (Ctx) TVMImmStringMCExpr(Data);
}

void TVMImmStringMCExpr::printImpl(raw_ostream &OS,
                                   const MCAsmInfo *MAI) const {
  OS << Data;
}
