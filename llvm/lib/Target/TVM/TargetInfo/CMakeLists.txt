add_llvm_component_library(LLVMTVMInfo
  TVMTargetInfo.cpp

  LINK_COMPONENTS
  Support

  ADD_TO_COMPONENT
  TVM
  )
