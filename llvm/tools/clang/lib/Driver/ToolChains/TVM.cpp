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

// TODO: Copied from WebAssembly. We need to figure out what to do with linker.
tvm::Linker::Linker(const ToolChain &TC) : GnuTool("tvm::Linker", "lld", TC) {}

bool tvm::Linker::isLinkJob() const { return true; }

bool tvm::Linker::hasIntegratedCPP() const { return false; }

void tvm::Linker::ConstructJob(Compilation &C, const JobAction &JA,
                               const InputInfo &Output,
                               const InputInfoList &Inputs, const ArgList &Args,
                               const char *LinkingOutput) const {

  const ToolChain &ToolChain = getToolChain();
  const char *Linker = Args.MakeArgString(ToolChain.GetLinkerPath());
  ArgStringList CmdArgs;
  CmdArgs.push_back("-flavor");
  CmdArgs.push_back("tvm");

  if (Args.hasArg(options::OPT_s))
    CmdArgs.push_back("--strip-all");

  Args.AddAllArgs(CmdArgs, options::OPT_L);
  Args.AddAllArgs(CmdArgs, options::OPT_u);
  ToolChain.AddFilePathLibArgs(Args, CmdArgs);

  if (!Args.hasArg(options::OPT_nostdlib, options::OPT_nostartfiles))
    CmdArgs.push_back(Args.MakeArgString(ToolChain.GetFilePath("crt1.o")));

  AddLinkerInputs(ToolChain, Inputs, Args, CmdArgs, JA);

  if (!Args.hasArg(options::OPT_nostdlib, options::OPT_nodefaultlibs)) {
    if (ToolChain.ShouldLinkCXXStdlib(Args))
      ToolChain.AddCXXStdlibLibArgs(Args, CmdArgs);

    if (Args.hasArg(options::OPT_pthread))
      CmdArgs.push_back("-lpthread");

    CmdArgs.push_back("-lc");
    AddRunTimeLibs(ToolChain, ToolChain.getDriver(), CmdArgs, Args);
  }

  CmdArgs.push_back("-o");
  CmdArgs.push_back(Output.getFilename());

  C.addCommand(llvm::make_unique<Command>(JA, *this, Linker, CmdArgs, Inputs));
}

TVM::TVM(const Driver &D, const llvm::Triple &Triple,
         const llvm::opt::ArgList &Args)
    : ToolChain(D, Triple, Args) {

  assert(Triple.isArch32Bit() != Triple.isArch64Bit());

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

// TODO: Support profiling.
bool TVM::SupportsProfiling() const { return false; }

bool TVM::HasNativeLLVMSupport() const { return true; }

void TVM::addClangTargetOptions(const ArgList &DriverArgs,
                                ArgStringList &CC1Args,
                                Action::OffloadKind) const {
  if (DriverArgs.hasFlag(clang::driver::options::OPT_fuse_init_array,
                         options::OPT_fno_use_init_array, true))
    CC1Args.push_back("-fuse-init-array");
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
    addSystemInclude(DriverArgs, CC1Args,
                     getDriver().SysRoot + "/usr/include/cpp-sdk/std/target");
    addSystemInclude(DriverArgs, CC1Args,
                     getDriver().SysRoot + "/cpp-sdk/std/target");
  }
}

void TVM::AddClangCXXStdlibIncludeArgs(const ArgList &DriverArgs,
                                       ArgStringList &CC1Args) const {
  if (!DriverArgs.hasArg(options::OPT_nostdlibinc) &&
      !DriverArgs.hasArg(options::OPT_nostdincxx)) {
    addSystemInclude(DriverArgs, CC1Args,
                     getDriver().SysRoot + "/usr/include/cpp-sdk");
    addSystemInclude(DriverArgs, CC1Args,
                     getDriver().SysRoot + "/usr/include/cpp-sdk/std");
    addSystemInclude(DriverArgs, CC1Args,
                     getDriver().SysRoot + "/cpp-sdk");
    addSystemInclude(DriverArgs, CC1Args,
                     getDriver().SysRoot + "/cpp-sdk/std");
  }
}

void TVM::AddCXXStdlibLibArgs(const llvm::opt::ArgList &Args,
                              llvm::opt::ArgStringList &CmdArgs) const {

  switch (GetCXXStdlibType(Args)) {
  case ToolChain::CST_Libcxx:
    CmdArgs.push_back("-lc++");
    CmdArgs.push_back("-lc++abi");
    break;
  case ToolChain::CST_Libstdcxx:
    llvm_unreachable("invalid stdlib name");
  }
}

std::string TVM::getThreadModel() const {
  // TVM does not support threads.
  return "single";
}

Tool *TVM::buildLinker() const { return new tools::tvm::Linker(*this); }
