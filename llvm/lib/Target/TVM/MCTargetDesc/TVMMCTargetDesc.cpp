//===-- TVMMCTargetDesc.cpp - TVM Target Descriptions ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides TVM specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "TVMMCTargetDesc.h"
#include "InstPrinter/TVMInstPrinter.h"
#include "TVMMCAsmInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "TVMGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "TVMGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "TVMGenRegisterInfo.inc"

static MCInstrInfo *createTVMMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitTVMMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createTVMMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitTVMMCRegisterInfo(X, 0);
  return X;
}

static MCSubtargetInfo *createTVMMCSubtargetInfo(const Triple &TT,
                                                 StringRef CPU, StringRef FS) {
  return createTVMMCSubtargetInfoImpl(TT, CPU, /* tUNEcpu */ CPU, FS);
}

static MCInstPrinter *createTVMMCInstPrinter(const Triple &T,
                                             unsigned SyntaxVariant,
                                             const MCAsmInfo &MAI,
                                             const MCInstrInfo &MII,
                                             const MCRegisterInfo &MRI) {
  assert(SyntaxVariant == 0 && "TVM has one assembler syntax variant");
  return new TVMInstPrinter(MAI, MII, MRI);
}

static MCAsmInfo *createTVMMCAsmInfo(const MCRegisterInfo &MRI,
                                       const Triple &TT,
                                       const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new TVMMCAsmInfo(TT);

  // TODO
//  MCRegister SP = MRI.getDwarfRegNum(TVM::X2, true);
//  MCCFIInstruction Inst = MCCFIInstruction::cfiDefCfa(nullptr, SP, 0);
//  MAI->addInitialFrameState(Inst);

  return MAI;
}

extern "C" void LLVMInitializeTVMTargetMC() {
//  RegisterMCAsmInfo<TVMMCAsmInfo> X(getTheTVMTarget());
  TargetRegistry::RegisterMCAsmInfo(getTheTVMTarget(), createTVMMCAsmInfo);
  TargetRegistry::RegisterMCInstrInfo(getTheTVMTarget(), createTVMMCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(getTheTVMTarget(), createTVMMCRegisterInfo);
  TargetRegistry::RegisterMCSubtargetInfo(getTheTVMTarget(),
                                          createTVMMCSubtargetInfo);
  TargetRegistry::RegisterMCInstPrinter(getTheTVMTarget(),
                                        createTVMMCInstPrinter);
}
