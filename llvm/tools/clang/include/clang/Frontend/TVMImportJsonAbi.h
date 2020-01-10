// TVM local begin

//===-- TVMImportJsonAbi.h - TVM Json Abi import ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Defines FrontendAction for import TVM Json Abi
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TVM_IMPORT_JSON_ABI_H
#define LLVM_CLANG_TVM_IMPORT_JSON_ABI_H

#include "clang/Frontend/FrontendActions.h"

namespace clang {

class ImportJsonAbiAction : public PreprocessorFrontendAction {
  virtual void anchor();
public:
  void ExecuteAction() override;
};

}  // end namespace clang

#endif

// TVM local end
