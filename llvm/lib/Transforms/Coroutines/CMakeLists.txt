add_llvm_library(LLVMCoroutines
  Coroutines.cpp
  CoroCleanup.cpp
  CoroEarly.cpp
  CoroElide.cpp
  CoroFrame.cpp
  CoroSplit.cpp
  CoroTVMExpand.cpp

  DEPENDS
  intrinsics_gen
  )
