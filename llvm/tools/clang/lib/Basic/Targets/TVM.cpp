//===----------- TVM.cpp - Implement TVM target feature support -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements TVM TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "Targets.h"
#include "clang/Basic/Builtins.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/TargetBuiltins.h"
#include "llvm/ADT/StringSwitch.h"

using namespace clang;
using namespace clang::targets;

const Builtin::Info TVMTargetInfo::BuiltinInfo[] = {
#define BUILTIN(ID, TYPE, ATTRS)                                               \
  {#ID, TYPE, ATTRS, nullptr, ALL_LANGUAGES, nullptr},
#define LIBBUILTIN(ID, TYPE, ATTRS, HEADER)                                    \
  {#ID, TYPE, ATTRS, HEADER, ALL_LANGUAGES, nullptr},
#include "clang/Basic/BuiltinsTVM.def"
};

static constexpr llvm::StringLiteral ValidCPUNames[] = {{""}};

bool TVMTargetInfo::hasFeature(StringRef Feature) const {
  return llvm::StringSwitch<bool>(Feature).Default(false);
}

bool TVMTargetInfo::isValidCPUName(StringRef Name) const {
  return llvm::find(ValidCPUNames, Name) != std::end(ValidCPUNames);
}

void TVMTargetInfo::fillValidCPUList(SmallVectorImpl<StringRef> &Values) const {
  Values.append(std::begin(ValidCPUNames), std::end(ValidCPUNames));
}

void TVMTargetInfo::getTargetDefines(const LangOptions &Opts,
                                     MacroBuilder &Builder) const {
  defineCPUMacros(Builder, "tvm", /*Tuning=*/false);
  Builder.defineMacro("__ELF__");
  Builder.defineMacro("_LIBCPP_OBJECT_FORMAT_ELF");
  Builder.defineMacro("_LIBCPP_NO_RTTI");
  Builder.defineMacro("_LIBCPP_HAS_NO_THREADS");
  Builder.defineMacro("_LIBCPP_NO_EXCEPTIONS");
  // TODO: Temporary, until target-specific stdlib files re-work
  Builder.defineMacro("__x86_64__");
}

bool TVMTargetInfo::handleTargetFeatures(std::vector<std::string> &Features,
                                         DiagnosticsEngine &Diags) {
  return true;
}

ArrayRef<Builtin::Info> TVMTargetInfo::getTargetBuiltins() const {
  return llvm::makeArrayRef(BuiltinInfo, clang::TVM::LastTSBuiltin -
                                             Builtin::FirstTSBuiltin);
}
