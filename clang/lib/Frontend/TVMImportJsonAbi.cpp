// TVM local begin

#include "clang/Frontend/TVMImportJsonAbi.h"

#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/Path.h"

using namespace clang;

void ImportJsonAbiAction::anchor() { }

namespace {
llvm::Optional<std::string> CppTypeForJsonAbiType(StringRef JsonAbiName) {
  if (JsonAbiName == "address")
    return std::string("MsgAddress");
  if (JsonAbiName == "bool")
    return std::string("bool_t");
  if (JsonAbiName.consume_front("uint")) {
    unsigned BitSize;
    if (!JsonAbiName.getAsInteger(10, BitSize))
      return std::string("uint_t<" + std::to_string(BitSize) + ">");
    return {};
  }
  if (JsonAbiName.consume_front("int")) {
    unsigned BitSize;
    if (!JsonAbiName.getAsInteger(10, BitSize))
      return std::string("int_t<" + std::to_string(BitSize) + ">");
    return {};
  }
  return {};
}
}

void ImportJsonAbiAction::ExecuteAction() {
#ifdef JSON_ABI_LOCAL_ASSERT
#error "JSON_ABI_LOCAL_ASSERT macros override"
#endif
#define JSON_ABI_LOCAL_ASSERT(Cond, Err) \
  if (!(Cond)) { \
    unsigned DiagID = \
        CI.getDiagnostics().getCustomDiagID(DiagnosticsEngine::Error, "%0"); \
    CI.getDiagnostics().Report(DiagID) << Err; \
    return; \
  }

  CompilerInstance &CI = getCompilerInstance();
  auto Buffer = CI.getFileManager().getBufferForFile(getCurrentFile());
  if (!Buffer) {
      CI.getDiagnostics().Report(diag::err_cannot_open_file)
          << getCurrentFile() << Buffer.getError().message();
      return;
  }
  std::unique_ptr<raw_pwrite_stream> OS =
      CI.createDefaultOutputFile(true, getCurrentFile(), "hpp");
  if (!OS)
    return;
  if (auto E = llvm::json::parse(Buffer.get()->getBuffer())) {
    const auto *Abi = E->getAsObject();
    JSON_ABI_LOCAL_ASSERT(Abi, "Wrong abi")
    auto AbiVer = Abi->getInteger("ABI version");
    JSON_ABI_LOCAL_ASSERT(AbiVer && *AbiVer == 1, "Wrong abi version")

    *OS << "#pragma once\n\n";
    *OS << "#include <tvm/schema/message.hpp>\n\n";
    *OS << "namespace tvm { namespace schema {\n\n";

    auto InterfaceName = CI.getFrontendOpts().TVMJsonAbiStructName;
    if (InterfaceName.empty()) {
      StringRef fname = llvm::sys::path::filename(getCurrentFile());
      size_t pos = fname.find_first_of('.');
      if (pos != StringRef::npos)
        fname = fname.substr(0, pos);
      InterfaceName = ("I" + fname).str();
    }

    *OS << "struct " << InterfaceName << " {\n";

    const auto *Functions = Abi->getArray("functions");
    for (const auto &Func : *Functions) {
      auto *FunObj = Func.getAsObject();
      JSON_ABI_LOCAL_ASSERT(FunObj && FunObj->getString("name"),
                            "Wrong function")
      const auto *Outputs = FunObj->getArray("outputs");
      *OS << "  ";
      if (Outputs->size() == 1) {
        auto *OutputObj = Outputs->front().getAsObject();
        JSON_ABI_LOCAL_ASSERT(OutputObj, "Wrong output")
        auto RvType = OutputObj->getString("type");
        auto CppType = CppTypeForJsonAbiType(*RvType);
        *OS << *CppType;
      } else {
        *OS << "void";
      }

      *OS << " " << *FunObj->getString("name") << "(";
      const auto *Inputs = FunObj->getArray("inputs");
      if (Inputs) {
        bool IsFirst = true;
        for (const auto &Input : *Inputs) {
          if (auto *InputObj = Input.getAsObject()) {
            auto ParamName = InputObj->getString("name");
            auto ParamType = InputObj->getString("type");
            JSON_ABI_LOCAL_ASSERT(ParamName && ParamType,
                                  "Wrong function input")
            auto CppType = CppTypeForJsonAbiType(*ParamType);
            JSON_ABI_LOCAL_ASSERT(CppType, "Unsupported input type")
            if (!IsFirst) *OS << ", ";
            IsFirst = false;
            *OS << *CppType << " " << *ParamName;
          }
        }
      }
      *OS << ")";
      auto FuncId = FunObj->getInteger("id");
      if (FuncId)
        *OS << " = " << *FuncId;
      *OS << ";\n";
    }

    *OS << "};\n\n";
    *OS << "}} // namespace tvm::schema\n";
  } else {
    handleAllErrors(E.takeError(), [&](const llvm::ErrorInfoBase &E) {
      unsigned DiagID =
          CI.getDiagnostics().getCustomDiagID(DiagnosticsEngine::Error, "%0");
      CI.getDiagnostics().Report(DiagID) << E.message();
    });
  }
  #undef JSON_ABI_LOCAL_ASSERT
}
// TVM local end
