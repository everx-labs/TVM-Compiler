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

tvm::Linker::Linker(const ToolChain &TC)
    : Tool("tvm::Linker", "tvm_linker", TC) {}

bool tvm::Linker::isLinkJob() const { return true; }

bool tvm::Linker::hasIntegratedCPP() const { return false; }

static const char *getOptimizationLevel(const llvm::opt::ArgList &Args) {
  StringRef OOpt = "3";
  if (Arg *A = Args.getLastArg(options::OPT_O_Group)) {
    if (A->getOption().matches(options::OPT_O4) ||
        A->getOption().matches(options::OPT_Ofast))
      OOpt = "3";
    else if (A->getOption().matches(options::OPT_O0))
      OOpt = "0";
    else if (A->getOption().matches(options::OPT_O)) {
      // -Os, -Oz, and -O(anything else) map to -O3
      OOpt = llvm::StringSwitch<const char *>(A->getValue())
                 .Case("1", "1")
                 .Case("2", "2")
                 .Case("3", "3")
                 .Case("s", "3")
                 .Case("z", "3")
                 .Default("3");
    }
  }
  return Args.MakeArgString("-O" + OOpt);
}

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
  C.addCommand(std::make_unique<Command>(
      JA, *this, ResponseFileSupport::AtFileCurCP(), Exec, CmdArgs, Inputs));
  return OutputFileName;
}

const char *tvm::Linker::constructClangCommand(
    Compilation &C, const JobAction &JA, const InputInfoList &Inputs,
    const llvm::opt::ArgList &Args, llvm::StringRef OutputFilePrefix,
    const char *InputFileName) const {
  // Construct clang command.
  ArgStringList ClangArgs{InputFileName, "-export-json-abi", "-o"};
  const char *ClangOutputFile =
      C.getArgs().MakeArgString(OutputFilePrefix + ".abi");
  ClangArgs.push_back(ClangOutputFile);
  SmallString<128> ClangPath(C.getDriver().Dir);
  llvm::sys::path::append(ClangPath, "clang");
  const char *Clang = Args.MakeArgString(ClangPath);
  C.addCommand(std::make_unique<Command>(
      JA, *this, ResponseFileSupport::AtFileCurCP(),
    Clang, ClangArgs, Inputs));
  return ClangOutputFile;
}

const char *tvm::Linker::constructOptCommand(
    Compilation &C, const JobAction &JA, const InputInfoList &Inputs,
    const llvm::opt::ArgList &Args, llvm::StringRef OutputFilePrefix,
    const char *InputFileName, bool Internalize) const {
  // Construct opt command.
  ArgStringList OptArgs;
  // The input to opt is the output from llvm-link.
  OptArgs.push_back(InputFileName);

  // Pass optimization arg to opt.
  if (!Internalize)
    OptArgs.push_back(getOptimizationLevel(Args));
  else
    OptArgs.push_back(Args.MakeArgString("-O0"));

  if (Internalize) {
    std::vector EntryPoints{"main_external", "main_internal", "main_ticktock",
                            "main_split", "main_merge"};
    std::string InternalizeVal = EntryPoints[0];
    for (auto EP : llvm::make_range(std::next(std::begin(EntryPoints)),
                                    std::end(EntryPoints))) {
      InternalizeVal += std::string(",") + EP;
    }
    OptArgs.push_back("-internalize");
    OptArgs.push_back(
        Args.MakeArgString("-internalize-public-api-list=" + InternalizeVal));
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
  C.addCommand(std::make_unique<Command>(
      JA, *this, ResponseFileSupport::AtFileCurCP(),
    OptExec, OptArgs, Inputs));
  return OutputFileName;
}

const char *tvm::Linker::constructLlcCommand(
    Compilation &C, const JobAction &JA, const InputInfoList &Inputs,
    const llvm::opt::ArgList &Args, llvm::StringRef OutputFilePrefix,
    const char *InputFileName, bool PrefixIsName) const {
  // Construct llc command.
  ArgStringList LlcArgs{InputFileName, "-mtriple=tvm", "-filetype=asm", "-o"};
  std::string LlcOutputFileName =
      PrefixIsName ? std::string(OutputFilePrefix.data())
                   : C.getDriver().GetTemporaryPath(OutputFilePrefix, "s");
  const char *LlcOutputFile = C.getArgs().MakeArgString(LlcOutputFileName);
  if (!PrefixIsName)
    LlcOutputFile = C.addTempFile(LlcOutputFile);
  LlcArgs.push_back(LlcOutputFile);
  LlcArgs.push_back(getOptimizationLevel(Args));
  if (Args.hasArg(options::OPT_tvm_refunc))
    LlcArgs.push_back("-tvm-re-func");
  SmallString<128> LlcPath(C.getDriver().Dir);
  llvm::sys::path::append(LlcPath, "llc");
  const char *Llc = Args.MakeArgString(LlcPath);
  C.addCommand(std::make_unique<Command>(
      JA, *this, ResponseFileSupport::AtFileCurCP(), Llc, LlcArgs, Inputs));
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
  std::string Prefix = llvm::sys::path::stem(Inputs[0].getFilename()).str();
  // Prefix for output file name.
  std::string OutPrefix = llvm::sys::path::stem(Output.getFilename()).str();

  const char *LLVMLinkCommand =
      constructLLVMLinkCommand(C, JA, Inputs, Args, Prefix);
  const char *OptIntCommand =
      constructOptCommand(C, JA, Inputs, Args, Prefix, LLVMLinkCommand, true);
  const char *OptCommand =
      constructOptCommand(C, JA, Inputs, Args, Prefix, OptIntCommand, false);
  const char *LlcCommand =
      Args.hasArg(options::OPT_lto_S)
          ? constructLlcCommand(C, JA, Inputs, Args, Output.getFilename(),
                                OptCommand, true)
          : constructLlcCommand(C, JA, Inputs, Args, Prefix, OptCommand);

  if (Args.hasArg(options::OPT_lto_S))
    return;

  // TODO: We now reinvoke the driver to produce the ABI output.
  // Constant printing pass can't be invoked from opt or llc, it's better to
  // fix it one day by moving ABI generation to llc.
  const char *JsExport =
      constructClangCommand(C, JA, Inputs, Args, OutPrefix, LLVMLinkCommand);

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
  LinkerArgs.push_back("--abi-json");
  LinkerArgs.push_back(JsExport);
  LinkerArgs.push_back("-o");
  LinkerArgs.push_back(Output.getFilename());

  C.addCommand(std::make_unique<Command>(JA, *this,
                                         ResponseFileSupport::AtFileCurCP(),
                                         Linker, LinkerArgs, Inputs));
}

TVM::TVM(const Driver &D, const llvm::Triple &Triple,
         const llvm::opt::ArgList &Args)
    : ToolChain(D, Triple, Args) {
  getProgramPaths().push_back(getDriver().getInstalledDir());
  llvm::SmallString<256> SysRootPath(getDriver().SysRoot);
  llvm::sys::path::append(SysRootPath, "lib");
  getFilePaths().push_back(SysRootPath.c_str());
  llvm::SmallString<256> FilePath(tvmRoot());
  llvm::sys::path::append(FilePath, "lib");
  getFilePaths().push_back(FilePath.c_str());
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
                                Action::OffloadKind) const {}

ToolChain::RuntimeLibType TVM::GetDefaultRuntimeLibType() const {
  return ToolChain::RLT_CompilerRT;
}

ToolChain::CXXStdlibType TVM::GetCXXStdlibType(const ArgList &) const {
  return ToolChain::CST_Libcxx;
}

void TVM::AddClangSystemIncludeArgs(const ArgList &DriverArgs,
                                    ArgStringList &CC1Args) const {
  if (!DriverArgs.hasArg(options::OPT_nostdinc)) {
    // Sysroot based include paths.
    if (!getDriver().SysRoot.empty()) {
      addSystemInclude(DriverArgs, CC1Args, getDriver().SysRoot + "/include/");
      // Include paths for tests.
      addSystemInclude(DriverArgs, CC1Args, getDriver().SysRoot);
    }
    // Distribution include paths.
    llvm::SmallString<256> Root(tvmRoot());
    llvm::sys::path::append(Root, "include");
    addSystemInclude(DriverArgs, CC1Args, Root);
  }
}

void TVM::AddClangCXXStdlibIncludeArgs(const ArgList &DriverArgs,
                                       ArgStringList &CC1Args) const {
  if (!DriverArgs.hasArg(options::OPT_nostdlibinc) &&
      !DriverArgs.hasArg(options::OPT_nostdincxx)) {
    // Sysroot based include paths.
    llvm::SmallString<256> SysRoot(getDriver().SysRoot);
    llvm::sys::path::append(SysRoot, "std");
    addSystemInclude(DriverArgs, CC1Args, SysRoot);
    llvm::sys::path::append(SysRoot, "target");
    addSystemInclude(DriverArgs, CC1Args, SysRoot);
    SysRoot = getDriver().SysRoot;
    llvm::sys::path::append(SysRoot, "include");
    llvm::sys::path::append(SysRoot, "std");
    addSystemInclude(DriverArgs, CC1Args, SysRoot);
    llvm::sys::path::append(SysRoot, "target");
    addSystemInclude(DriverArgs, CC1Args, SysRoot);
    llvm::SmallString<256> Root(tvmRoot());
    llvm::sys::path::append(Root, "include");
    addSystemInclude(DriverArgs, CC1Args, Root);
    llvm::sys::path::append(Root, "std");
    // Distribution include paths.
    addSystemInclude(DriverArgs, CC1Args, Root);
    llvm::sys::path::append(Root, "target");
    addSystemInclude(DriverArgs, CC1Args, Root);
  }
}

void TVM::AddCXXStdlibLibArgs(const llvm::opt::ArgList &Args,
                              llvm::opt::ArgStringList &CmdArgs) const {}

std::string TVM::getThreadModel() const {
  // TVM does not support threads.
  return "single";
}

Tool *TVM::buildLinker() const { return new tools::tvm::Linker(*this); }

std::string TVM::tvmRoot() const {
  std::string Root = getDriver().Dir;
  if (!llvm::sys::fs::exists(Root))
    return {};
  Root = llvm::sys::path::parent_path(Root);
  return Root;
}
