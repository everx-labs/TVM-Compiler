//===-- TVMStackPatterns.h - optimized stack exchange patterns --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "TVMStackFixup.h"

#ifndef LLVM_LIB_TARGET_TVM_TVMSTACKPATTERNS_H
#define LLVM_LIB_TARGET_TVM_TVMSTACKPATTERNS_H

namespace llvm {

namespace StackPatterns {

void pattern_x(StackFixup &Rv, unsigned i, unsigned Dst0);

void pattern2_pp(StackFixup &Rv, unsigned i, unsigned j);
void pattern2_xp(StackFixup &Rv, unsigned i, unsigned j);
void pattern2_px(StackFixup &Rv, unsigned i, unsigned j);
void pattern2_xx(StackFixup &Rv, unsigned i, unsigned j);
void pattern2_xx(StackFixup &Rv, unsigned i, unsigned j, unsigned Dst0,
                 unsigned Dst1);

void pattern3_xxx(StackFixup &Rv, unsigned i, unsigned j, unsigned k);
void pattern3_xxp(StackFixup &Rv, unsigned i, unsigned j, unsigned k);
void pattern3_xpx(StackFixup &Rv, unsigned i, unsigned j, unsigned k);
void pattern3_xpp(StackFixup &Rv, unsigned i, unsigned j, unsigned k);
void pattern3_pxx(StackFixup &Rv, unsigned i, unsigned j, unsigned k);
void pattern3_pxp(StackFixup &Rv, unsigned i, unsigned j, unsigned k);
void pattern3_ppx(StackFixup &Rv, unsigned i, unsigned j, unsigned k);
void pattern3_ppp(StackFixup &Rv, unsigned i, unsigned j, unsigned k);

} // namespace StackPatterns

} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMSTACKPATTERNS_H
