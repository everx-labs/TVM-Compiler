add_llvm_component_library(LLVMTVMDesc
  TVMMCTargetDesc.cpp
  TVMMCAsmInfo.cpp

  LINK_COMPONENTS
  MC
  TVMAsmPrinter 
  TVMInfo 
  Support

  ADD_TO_COMPONENT
  TVM
  )
