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
  def int_tvm_div : GCCBuiltin<"__builtin_tvm_div">,
                    Intrinsic<[llvm_i64_ty], [llvm_i64_ty, llvm_i64_ty],
                              [IntrNoMem]>;
  def int_tvm_mod : GCCBuiltin<"__builtin_tvm_mod">,
                    Intrinsic<[llvm_i64_ty], [llvm_i64_ty, llvm_i64_ty],
                              [IntrNoMem]>;
  def int_tvm_newdict : GCCBuiltin<"__builtin_tvm_newdict">,
                        Intrinsic<[llvm_i64_ty], []>;
  def int_tvm_get_persistent_data :
    GCCBuiltin<"__builtin_tvm_get_persistent_data">,
               Intrinsic<[llvm_i64_ty], [llvm_i64_ty]>;
  def int_tvm_dictuget :
    GCCBuiltin<"__builtin_tvm_dictuget">,
               Intrinsic<[llvm_i64_ty, llvm_i64_ty],
                         [llvm_i64_ty, llvm_i64_ty, llvm_i64_ty]>;
  def int_tvm_ldrefctos :
    GCCBuiltin<"__builtin_tvm_ldrefctos">,
               Intrinsic<[llvm_i64_ty, llvm_i64_ty], [llvm_i64_ty]>;
  def int_tvm_inttoslice :
    GCCBuiltin<"__builtin_tvm_inttoslice">,
               Intrinsic<[llvm_i64_ty], [llvm_i64_ty]>;
  def int_tvm_ldu :
    GCCBuiltin<"__builtin_tvm_ldu">,
               Intrinsic<[llvm_i64_ty, llvm_i64_ty], [llvm_i64_ty]>;
  // TODO: Fix inconsitsency.
  def int_tvm_ctos :
    GCCBuiltin<"__builtin_tvm_ctos">,
               Intrinsic<[llvm_i64_ty], [llvm_i64_ty]>;
  def int_tvm_stslice :
    GCCBuiltin<"__builtin_tvm_stslice">,
               Intrinsic<[llvm_i64_ty], [llvm_i64_ty, llvm_i64_ty]>;
}
