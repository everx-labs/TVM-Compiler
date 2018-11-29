//===-- TVMMCAsmInfo.cpp - TVM asm properties -----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the TVMMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "TVMMCAsmInfo.h"
using namespace llvm;

void TVMMCAsmInfo::anchor() { }

TVMMCAsmInfo::TVMMCAsmInfo(const Triple &TT) {
  CommentString = ";";
}

