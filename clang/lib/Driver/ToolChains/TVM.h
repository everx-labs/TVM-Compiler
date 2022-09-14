//===----------- TVM.h - TVM ToolChain Implementations ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_TVM_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_TVM_H

#include "Gnu.h"
#include "clang/Driver/Tool.h"
#include "clang/Driver/ToolChain.h"

namespace clang {
namespace driver {
namespace tools {
namespace tvm {

class LLVM_LIBRARY_VISIBILITY Linker : public Tool {
public:
  explicit Linker(const ToolChain &TC);
  bool isLinkJob() const override;
  bool hasIntegratedCPP() const override;
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;

private:
  /// \return llvm-link output file name.
  const char *constructLLVMLinkCommand(Compilation &C, const JobAction &JA,
                                       const InputInfoList &Inputs,
                                       const llvm::opt::ArgList &Args,
                                       llvm::StringRef OutputFilePrefix) const;

  /// \return abi-json output file name.
  const char *constructClangCommand(Compilation &C, const JobAction &JA,
                                    const InputInfoList &Inputs,
                                    const llvm::opt::ArgList &Args,
                                    llvm::StringRef OutputFilePrefix,
                                    const char *InputFileName) const;
  /// \return opt output file name.
  const char *constructOptCommand(Compilation &C, const JobAction &JA,
                                  const InputInfoList &Inputs,
                                  const llvm::opt::ArgList &Args,
                                  llvm::StringRef OutputFilePrefix,
                                  const char *InputFileName,
                                  bool Internalize) const;

  /// \return llc output file name.
  const char *constructLlcCommand(Compilation &C, const JobAction &JA,
                                  const InputInfoList &Inputs,
                                  const llvm::opt::ArgList &Args,
                                  llvm::StringRef SubArchName,
                                  const char *InputFileName,
                                  bool PrefixIsName = false) const;
};

} // end namespace tvm
} // end namespace tools

namespace toolchains {

class LLVM_LIBRARY_VISIBILITY TVM final : public ToolChain {
public:
  TVM(const Driver &D, const llvm::Triple &Triple,
      const llvm::opt::ArgList &Args);

private:
  bool IsMathErrnoDefault() const override;
  bool IsObjCNonFragileABIDefault() const override;
  bool UseObjCMixedDispatch() const override;
  bool isPICDefault() const override;
  bool isPIEDefault() const override;
  bool isPICDefaultForced() const override;
  bool IsIntegratedAssemblerDefault() const override;
  bool hasBlocksRuntime() const override;
  bool SupportsProfiling() const override;
  bool HasNativeLLVMSupport() const override;
  void
  addClangTargetOptions(const llvm::opt::ArgList &DriverArgs,
                        llvm::opt::ArgStringList &CC1Args,
                        Action::OffloadKind DeviceOffloadKind) const override;
  RuntimeLibType GetDefaultRuntimeLibType() const override;
  CXXStdlibType GetCXXStdlibType(const llvm::opt::ArgList &Args) const override;
  void
  AddClangSystemIncludeArgs(const llvm::opt::ArgList &DriverArgs,
                            llvm::opt::ArgStringList &CC1Args) const override;
  void AddClangCXXStdlibIncludeArgs(
      const llvm::opt::ArgList &DriverArgs,
      llvm::opt::ArgStringList &CC1Args) const override;
  void AddCXXStdlibLibArgs(const llvm::opt::ArgList &Args,
                           llvm::opt::ArgStringList &CmdArgs) const override;
  std::string getThreadModel() const override;

  const char *getDefaultLinker() const override { return "tvm_linker"; }

  Tool *buildLinker() const override;
  std::string tvmRoot() const;
};

} // end namespace toolchains
} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_TVM_H
