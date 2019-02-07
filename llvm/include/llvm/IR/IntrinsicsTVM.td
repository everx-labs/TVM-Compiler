//===- IntrinsicsTVM.td - Defines TVM intrinsics -----------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines all of the TVM-specific intrinsics.
//
//===----------------------------------------------------------------------===//

let TargetPrefix = "tvm" in {
  def int_tvm_div : Intrinsic<[llvm_i64_ty], [llvm_i64_ty, llvm_i64_ty],
                              [IntrNoMem]>;
}
