add_llvm_component_library(LLVMTVMAsmPrinter
  TVMInstPrinter.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  TVM
  )
