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
                    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty],
                              [IntrNoMem]>;
  def int_tvm_mod : GCCBuiltin<"__builtin_tvm_mod">,
                    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty],
                              [IntrNoMem]>;
  def int_tvm_bitsize : GCCBuiltin<"__builtin_tvm_bitsize">,
                    Intrinsic<[llvm_i257_ty], [llvm_i257_ty],
                              []>;
  def int_tvm_ubitsize : GCCBuiltin<"__builtin_tvm_ubitsize">,
                    Intrinsic<[llvm_i257_ty], [llvm_i257_ty],
                              []>;
  def int_tvm_fitsx : GCCBuiltin<"__builtin_tvm_fitsx">,
                    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty],
                              []>;
  def int_tvm_ufitsx : GCCBuiltin<"__builtin_tvm_ufitsx">,
                    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty],
                              []>;
  def int_tvm_newdict : GCCBuiltin<"__builtin_tvm_newdict">,
                        Intrinsic<[llvm_i257_ty], []>;
  def int_tvm_newc : GCCBuiltin<"__builtin_tvm_newc">,
                        Intrinsic<[llvm_i257_ty], []>;
  def int_tvm_get_persistent_data :
    GCCBuiltin<"__builtin_tvm_get_persistent_data">,
               Intrinsic<[llvm_i257_ty], []>;
  def int_tvm_getreg :
    GCCBuiltin<"__builtin_tvm_getreg">,
               Intrinsic<[llvm_i257_ty], [llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_setreg :
    GCCBuiltin<"__builtin_tvm_setreg">,
               Intrinsic<[], [llvm_i257_ty, llvm_i257_ty]>;
  def int_tvm_set_persistent_data :
    GCCBuiltin<"__builtin_tvm_set_persistent_data">,
               Intrinsic<[], [llvm_i257_ty]>;
  def int_tvm_dictuget :
    GCCBuiltin<"__builtin_tvm_dictuget">,
               Intrinsic<[llvm_i257_ty, llvm_i257_ty],
                         [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty]>;
  def int_tvm_dictiget :
    GCCBuiltin<"__builtin_tvm_dictiget">,
               Intrinsic<[llvm_i257_ty, llvm_i257_ty],
                         [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty]>;
  def int_tvm_dictuset :
    GCCBuiltin<"__builtin_tvm_dictuset">,
               Intrinsic<[llvm_i257_ty],
                         [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty,
                          llvm_i257_ty]>;
  def int_tvm_dictiset :
    GCCBuiltin<"__builtin_tvm_dictiset">,
               Intrinsic<[llvm_i257_ty],
                         [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty,
                          llvm_i257_ty]>;
  def int_tvm_ldrefrtos :
    GCCBuiltin<"__builtin_tvm_ldrefrtos">,
               Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_i257_ty]>;
  def int_tvm_ldslicex :
    GCCBuiltin<"__builtin_tvm_ldslicex">,
               Intrinsic<[llvm_i257_ty, llvm_i257_ty],
                         [llvm_i257_ty, llvm_i257_ty]>;
  def int_tvm_inttoslice :
    GCCBuiltin<"__builtin_tvm_inttoslice">,
               Intrinsic<[llvm_i257_ty], [llvm_i257_ty]>;
  def int_tvm_ldu :
    GCCBuiltin<"__builtin_tvm_ldu">,
               Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_i257_ty]>;
  // TODO: Fix inconsitsency.
  def int_tvm_ctos :
    GCCBuiltin<"__builtin_tvm_ctos">,
               Intrinsic<[llvm_i257_ty], [llvm_i257_ty]>;
  def int_tvm_stoc :
    GCCBuiltin<"__builtin_tvm_stoc">,
               Intrinsic<[llvm_i257_ty], [llvm_i257_ty]>;
  def int_tvm_stslice :
    GCCBuiltin<"__builtin_tvm_stslice">,
               Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty]>;
  def int_tvm_stref :
    GCCBuiltin<"__builtin_tvm_stref">,
               Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty]>;
  def int_tvm_ldref :
    GCCBuiltin<"__builtin_tvm_ldref">,
               Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_i257_ty]>;
  def int_tvm_throw :
    GCCBuiltin<"__builtin_tvm_throw">,
               Intrinsic<[], [llvm_i257_ty]>;
  def int_tvm_throwif :
    GCCBuiltin<"__builtin_tvm_throwif">,
               Intrinsic<[], [llvm_i257_ty, llvm_i257_ty]>;
  def int_tvm_pdload :
    GCCBuiltin<"__builtin_tvm_pdload">,
               Intrinsic<[llvm_i257_ty], [llvm_i257_ty]>;
  def int_tvm_pdstore :
    GCCBuiltin<"__builtin_tvm_pdstore">,
               Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty]>;
  def int_tvm_endc :
    GCCBuiltin<"__builtin_tvm_endc">,
               Intrinsic<[llvm_i257_ty], [llvm_i257_ty]>;
  def int_tvm_stu :
    GCCBuiltin<"__builtin_tvm_stu">,
               Intrinsic<[llvm_i257_ty],
                         [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty]>;
  def int_tvm_sendrawmsg :
    GCCBuiltin<"__builtin_tvm_sendrawmsg">,
               Intrinsic<[], [llvm_i257_ty, llvm_i257_ty]>;
  def int_tvm_retrieve_signed :
    GCCBuiltin<"__builtin_tvm_retrieve_signed">,
               Intrinsic<[llvm_i257_ty],
                         [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty]>;
  def int_tvm_retrieve_unsigned :
    GCCBuiltin<"__builtin_tvm_retrieve_unsigned">,
               Intrinsic<[llvm_i257_ty],
                         [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty]>;
  def int_tvm_retrieve_ref :
    GCCBuiltin<"__builtin_tvm_retrieve_ref">,
               Intrinsic<[llvm_i257_ty], [llvm_i257_ty]>;
  def int_tvm_nop :
    GCCBuiltin<"__builtin_tvm_nop">,
               Intrinsic<[], []>;
  def int_tvm_dumpstk :
    GCCBuiltin<"__builtin_tvm_dumpstk">,
               Intrinsic<[], []>;
  def int_tvm_dumpstktop :
    GCCBuiltin<"__builtin_tvm_dumpstktop">,
               Intrinsic<[], [llvm_i257_ty]>;
  def int_tvm_dump :
    GCCBuiltin<"__builtin_tvm_dump">,
               Intrinsic<[], [llvm_i257_ty]>;
  def int_tvm_print :
    GCCBuiltin<"__builtin_tvm_print">,
               Intrinsic<[], [llvm_i257_ty]>;
  def int_tvm_dump_value :
    GCCBuiltin<"__builtin_tvm_dump_value">,
               Intrinsic<[], [llvm_i257_ty]>;
  def int_tvm_print_value :
    GCCBuiltin<"__builtin_tvm_print_value">,
               Intrinsic<[], [llvm_i257_ty]>;
  def int_tvm_logstr :
    GCCBuiltin<"__builtin_tvm_logstr">,
               Intrinsic<[], [llvm_ptr_ty]>;
  def int_tvm_printstr :
    GCCBuiltin<"__builtin_tvm_printstr">,
               Intrinsic<[], [llvm_ptr_ty]>;
  def int_tvm_logflush :
    GCCBuiltin<"__builtin_tvm_logflush">,
               Intrinsic<[], []>;
}
