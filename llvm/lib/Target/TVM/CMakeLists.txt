set(LLVM_TARGET_DEFINITIONS TVM.td)

tablegen(LLVM TVMGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM TVMGenCallingConv.inc -gen-callingconv)
tablegen(LLVM TVMGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM TVMGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM TVMGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM TVMGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM TVMInstMappingInfo.inc -gen-tvm-instr-mapping-info)

add_public_tablegen_target(TVMTableGen)

add_llvm_target(TVMCodeGen
  TVMArgumentMove.cpp
  TVMControlFlowPrepare.cpp
  TVMSubtarget.cpp
  TVMTargetMachine.cpp
  TVMISelLowering.cpp
  TVMInstrInfo.cpp
  TVMFrameLowering.cpp
  TVMRegisterInfo.cpp
  TVMISelDAGToDAG.cpp
  TVMMachineFunctionInfo.cpp
  TVMAsmPrinter.cpp
  TVMLoopPrepare.cpp
  TVMLoopInstructions.cpp
  TVMMCExpr.cpp
  TVMMCInstLower.cpp
  TVMPrepareForLiveIntervals.cpp
  TVMRematerialize.cpp
  TVMReplacePhysRegs.cpp
  TVMRegStackify.cpp
  TVMRegNumbering.cpp
  TVMPeephole.cpp
  TVMStack.cpp
  TVMStackBlockInfo.cpp
  TVMStackFixup.cpp
  TVMStackPatterns.cpp
  TVMStackModel.cpp
  TVMStoreCombine.cpp
  TVMUtilities.cpp
  TVMContinuationsHoist.cpp
  TVMLoadStoreReplace.cpp
  TVMMoveMaterializable.cpp
  TVMIfConversionTerm.cpp
  TVMLowerIntrinsics.cpp
)

add_subdirectory(InstPrinter)
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
