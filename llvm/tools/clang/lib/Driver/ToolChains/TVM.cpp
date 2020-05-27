//===---------- TVM.cpp - TVM ToolChain Implementation ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "TVM.h"
#include "CommonArgs.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/DriverDiagnostic.h"
#include "clang/Driver/Options.h"
#include "llvm/Option/ArgList.h"

using namespace clang::driver;
using namespace clang::driver::tools;
using namespace clang::driver::toolchains;
using namespace clang;
using namespace llvm::opt;

tvm::Linker::Linker(const ToolChain &TC) : Tool("tvm::Linker", "tvm_linker", TC) {}

bool tvm::Linker::isLinkJob() const { return true; }

bool tvm::Linker::hasIntegratedCPP() const { return false; }

void tvm::Linker::ConstructJob(Compilation &C, const JobAction &JA,
                               const InputInfo &Output,
                               const InputInfoList &Inputs, const ArgList &Args,
                               const char *LinkingOutput) const {

  const ToolChain &ToolChain = getToolChain();
  const char *Linker = Args.MakeArgString(ToolChain.GetLinkerPath());
  ArgStringList CmdArgs;

  AddLinkerInputs(ToolChain, Inputs, Args, CmdArgs, JA);

  CmdArgs.push_back("-o");
  CmdArgs.push_back(Output.getFilename());

  C.addCommand(llvm::make_unique<Command>(JA, *this, Linker, CmdArgs, Inputs));
}

TVM::TVM(const Driver &D, const llvm::Triple &Triple,
         const llvm::opt::ArgList &Args)
    : ToolChain(D, Triple, Args) {

  getProgramPaths().push_back(getDriver().getInstalledDir());

  getFilePaths().push_back(getDriver().SysRoot + "/lib");
}

bool TVM::IsMathErrnoDefault() const { return false; }

bool TVM::IsObjCNonFragileABIDefault() const { return true; }

bool TVM::UseObjCMixedDispatch() const { return true; }

bool TVM::isPICDefault() const { return false; }

bool TVM::isPIEDefault() const { return false; }

bool TVM::isPICDefaultForced() const { return false; }

bool TVM::IsIntegratedAssemblerDefault() const { return false; }

bool TVM::hasBlocksRuntime() const { return false; }

bool TVM::SupportsProfiling() const { return false; }

bool TVM::HasNativeLLVMSupport() const { return false; }

void TVM::addClangTargetOptions(const ArgList &DriverArgs,
                                ArgStringList &CC1Args,
                                Action::OffloadKind) const {
}

ToolChain::RuntimeLibType TVM::GetDefaultRuntimeLibType() const {
  return ToolChain::RLT_CompilerRT;
}

ToolChain::CXXStdlibType TVM::GetCXXStdlibType(const ArgList &) const {
  return ToolChain::CST_Libcxx;
}

void TVM::AddClangSystemIncludeArgs(const ArgList &DriverArgs,
                                    ArgStringList &CC1Args) const {
  if (!DriverArgs.hasArg(options::OPT_nostdinc)) {
    // Distribution include paths.
    addSystemInclude(DriverArgs, CC1Args,
                     getDriver().SysRoot + "/include/");
    // Build include paths for tests.
    addSystemInclude(DriverArgs, CC1Args,
                     getDriver().SysRoot);
  }
}

void TVM::AddClangCXXStdlibIncludeArgs(const ArgList &DriverArgs,
                                       ArgStringList &CC1Args) const {
  if (!DriverArgs.hasArg(options::OPT_nostdlibinc) &&
      !DriverArgs.hasArg(options::OPT_nostdincxx)) {
    // Distribution include paths.
    addSystemInclude(DriverArgs, CC1Args,
                     getDriver().SysRoot + "/include/std");
    addSystemInclude(DriverArgs, CC1Args,
                     getDriver().SysRoot + "/include/std/target");
    // Build include paths for tests.
    addSystemInclude(DriverArgs, CC1Args,
                     getDriver().SysRoot + "/std");
    addSystemInclude(DriverArgs, CC1Args,
                     getDriver().SysRoot + "/std/target");
    // FIXME: CI does the installation via manual copying.
    addSystemInclude(DriverArgs, CC1Args, "/usr/include/cpp-sdk/std");
    addSystemInclude(DriverArgs, CC1Args, "/usr/include/cpp-sdk");
    addSystemInclude(DriverArgs, CC1Args, "/usr/include/cpp-sdk/std/target");
  }
}

void TVM::AddCXXStdlibLibArgs(const llvm::opt::ArgList &Args,
                              llvm::opt::ArgStringList &CmdArgs) const {
}

std::string TVM::getThreadModel() const {
  // TVM does not support threads.
  return "single";
}

Tool *TVM::buildLinker() const { return new tools::tvm::Linker(*this); }
