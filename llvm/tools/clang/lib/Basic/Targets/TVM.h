//===-------- TVM.h - Declare TVM target feature support --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares TVM TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_BASIC_TARGETS_TVM_H
#define LLVM_CLANG_LIB_BASIC_TARGETS_TVM_H

#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/Compiler.h"

namespace clang {
namespace targets {

class LLVM_LIBRARY_VISIBILITY TVMTargetInfo : public TargetInfo {
private:
  static const Builtin::Info BuiltinInfo[];

public:
  explicit TVMTargetInfo(const llvm::Triple &T, const TargetOptions &)
      : TargetInfo(T) {
    NoAsmVariants = true;
    SuitableAlign = 128;
    LargeArrayMinWidth = 128;
    LargeArrayAlign = 128;
    SimdDefaultAlign = 128;
    SigAtomicType = SignedLong;
    LongDoubleWidth = LongDoubleAlign = 128;
    LongDoubleFormat = &llvm::APFloat::IEEEquad();

    MaxAtomicPromoteWidth = MaxAtomicInlineWidth = 64;
    PointerWidth = PointerAlign = 64;
    BoolWidth = BoolAlign = 64;
    IntWidth = IntAlign = 64;
    LongWidth = LongAlign = 64;
    LongLongWidth = LongLongAlign = 64;

    SizeType = UnsignedLong;
    PtrDiffType = SignedLong;
    IntPtrType = SignedLong;
    resetDataLayout("E-S1024-i256:256:256");
  }

private:
  bool
  initFeatureMap(llvm::StringMap<bool> &Features, DiagnosticsEngine &Diags,
                 StringRef CPU,
                 const std::vector<std::string> &FeaturesVec) const override {
    return TargetInfo::initFeatureMap(Features, Diags, CPU, FeaturesVec);
  }

  bool hasFeature(StringRef Feature) const final;

  bool handleTargetFeatures(std::vector<std::string> &Features,
                            DiagnosticsEngine &Diags) final;

  bool isValidCPUName(StringRef Name) const final;
  void fillValidCPUList(SmallVectorImpl<StringRef> &Values) const final;

  bool setCPU(const std::string &Name) final { return isValidCPUName(Name); }

  ArrayRef<Builtin::Info> getTargetBuiltins() const final;

  BuiltinVaListKind getBuiltinVaListKind() const final {
    return VoidPtrBuiltinVaList;
  }

  ArrayRef<const char *> getGCCRegNames() const final { return None; }

  ArrayRef<TargetInfo::GCCRegAlias> getGCCRegAliases() const final {
    return None;
  }

  bool validateAsmConstraint(const char *&Name,
                             TargetInfo::ConstraintInfo &Info) const final {
    return false;
  }

  const char *getClobbers() const final { return ""; }

  bool isCLZForZeroUndef() const final { return false; }

  bool hasInt128Type() const final { return true; }

  IntType getIntTypeByWidth(unsigned BitWidth, bool IsSigned) const final {
    return BitWidth == IsSigned ? SignedLongLong : UnsignedLongLong;
  }

  IntType getLeastIntTypeByWidth(unsigned BitWidth, bool IsSigned) const final {
    return IsSigned ? SignedLongLong : UnsignedLongLong;
  }

protected:
  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override;
};

} // namespace targets
} // namespace clang
#endif // LLVM_CLANG_LIB_BASIC_TARGETS_TVM_H
