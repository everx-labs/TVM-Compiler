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
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"

using namespace clang::driver;
using namespace clang::driver::tools;
using namespace clang::driver::toolchains;
using namespace clang;
using namespace llvm::opt;

tvm::Linker::Linker(const ToolChain &TC) : Tool("tvm::Linker", "tvm_linker", TC) {}

bool tvm::Linker::isLinkJob() const { return true; }

bool tvm::Linker::hasIntegratedCPP() const { return false; }

const char *tvm::Linker::constructLLVMLinkCommand(
    Compilation &C, const JobAction &JA, const InputInfoList &Inputs,
    const ArgList &Args, StringRef OutputFilePrefix) const {
  ArgStringList CmdArgs;
  // Add the input bc's created by compile step.
  for (const auto &II : Inputs) {
    if (II.getType() == types::TY_LLVM_BC || II.getType() == types::TY_LLVM_IR)
      CmdArgs.push_back(II.getFilename());
  }

  // Add an intermediate output file.
  CmdArgs.push_back("-o");
  std::string TmpName =
      C.getDriver().GetTemporaryPath(OutputFilePrefix.str() + "-linked", "bc");
  const char *OutputFileName =
      C.addTempFile(C.getArgs().MakeArgString(TmpName));
  CmdArgs.push_back(OutputFileName);
  SmallString<128> ExecPath(C.getDriver().Dir);
  llvm::sys::path::append(ExecPath, "llvm-link");
  const char *Exec = Args.MakeArgString(ExecPath);
  C.addCommand(llvm::make_unique<Command>(JA, *this, Exec, CmdArgs, Inputs));
  return OutputFileName;
}

const char *tvm::Linker::constructOptCommand(
    Compilation &C, const JobAction &JA, const InputInfoList &Inputs,
    const llvm::opt::ArgList &Args,
    llvm::StringRef OutputFilePrefix, const char *InputFileName, bool Internalize) const {
  // Construct opt command.
  ArgStringList OptArgs;
  // The input to opt is the output from llvm-link.
  OptArgs.push_back(InputFileName);
  // Pass optimization arg to opt.
  if (Arg *A = Args.getLastArg(options::OPT_O_Group)) {
    StringRef OOpt = "3";
    if (A->getOption().matches(options::OPT_O4) ||
        A->getOption().matches(options::OPT_Ofast))
      OOpt = "3";
    else if (A->getOption().matches(options::OPT_O0))
      OOpt = "0";
    else if (A->getOption().matches(options::OPT_O)) {
      // -Os, -Oz, and -O(anything else) map to -O2
      OOpt = llvm::StringSwitch<const char *>(A->getValue())
                 .Case("1", "1")
                 .Case("2", "2")
                 .Case("3", "3")
                 .Case("s", "2")
                 .Case("z", "2")
                 .Default("2");
    }
    if (!Internalize)
      OptArgs.push_back(Args.MakeArgString("-O" + OOpt));
    else 
      OptArgs.push_back(Args.MakeArgString("-O0"));
  }
  if (Internalize) {
    std::vector EntryPoints { "main_external", "main_internal", "main_ticktock", "main_split", "main_merge"};
    std::string InternalizeVal = EntryPoints[0];
    for (auto EP : llvm::make_range(std::next(std::begin(EntryPoints)), std::end(EntryPoints))) {
      InternalizeVal += std::string(",") + EP;
    }
    OptArgs.push_back("-internalize");
    OptArgs.push_back(Args.MakeArgString("-internalize-public-api-list=" + InternalizeVal));
  }
  OptArgs.push_back("-mtriple=tvm");
  OptArgs.push_back("-o");
  std::string TmpFileName = C.getDriver().GetTemporaryPath(
      OutputFilePrefix.str() + "-optimized", "bc");
  const char *OutputFileName =
      C.addTempFile(C.getArgs().MakeArgString(TmpFileName));
  OptArgs.push_back(OutputFileName);
  SmallString<128> OptPath(C.getDriver().Dir);
  llvm::sys::path::append(OptPath, "opt");
  const char *OptExec = Args.MakeArgString(OptPath);
  C.addCommand(llvm::make_unique<Command>(JA, *this, OptExec, OptArgs, Inputs));
  return OutputFileName;
}

const char *tvm::Linker::constructLlcCommand(
    Compilation &C, const JobAction &JA, const InputInfoList &Inputs,
    const llvm::opt::ArgList &Args,
    llvm::StringRef OutputFilePrefix, const char *InputFileName) const {
  // Construct llc command.
  ArgStringList LlcArgs{InputFileName, "-mtriple=tvm",
                        "-filetype=asm",
                        "-o"};
  std::string LlcOutputFileName =
      C.getDriver().GetTemporaryPath(OutputFilePrefix, "s");
  const char *LlcOutputFile =
      C.addTempFile(C.getArgs().MakeArgString(LlcOutputFileName));
  LlcArgs.push_back(LlcOutputFile);
  SmallString<128> LlcPath(C.getDriver().Dir);
  llvm::sys::path::append(LlcPath, "llc");
  const char *Llc = Args.MakeArgString(LlcPath);
  C.addCommand(llvm::make_unique<Command>(JA, *this, Llc, LlcArgs, Inputs));
  return LlcOutputFile;
}

// For tvm the inputs of the linker job are bitcode and output is
// a boc file. It calls llvm-link, opt, llc, then tvm_link.
void tvm::Linker::ConstructJob(Compilation &C, const JobAction &JA,
                               const InputInfo &Output,
                               const InputInfoList &Inputs, const ArgList &Args,
                               const char *LinkingOutput) const {

  assert(getToolChain().getTriple().getArch() == llvm::Triple::tvm &&
         "Unsupported target");

  // Prefix for temporary file name.
  std::string Prefix =
      llvm::sys::path::stem(Inputs[0].getFilename()).str();

  // Each command outputs different files.
  const char *LLVMLinkCommand =
      constructLLVMLinkCommand(C, JA, Inputs, Args, Prefix);
  const char *OptIntCommand = constructOptCommand(C, JA, Inputs, Args,
                                                  Prefix, LLVMLinkCommand, true);
  const char *OptCommand = constructOptCommand(C, JA, Inputs, Args,
                                               Prefix, OptIntCommand, false);
  const char *LlcCommand =
      constructLlcCommand(C, JA, Inputs, Args, Prefix, OptCommand);

  const ToolChain &ToolChain = getToolChain();
  const char *Linker = Args.MakeArgString(ToolChain.GetLinkerPath());

  ArgStringList LinkerArgs{"compile", LlcCommand};

  for (Arg *A : Args) {
    if (A->getOption().matches(options::OPT_Wl_COMMA) ||
        A->getOption().matches(options::OPT_Xlinker))
      for (auto Val : A->getValues())
        LinkerArgs.push_back(Val);
  }

  std::string StdLib = getToolChain().GetFilePath("stdlib_cpp.tvm");
  LinkerArgs.push_back("--lib");
  LinkerArgs.push_back(Args.MakeArgString(StdLib));

  C.addCommand(llvm::make_unique<Command>(JA, *this, Linker, LinkerArgs, Inputs));
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
