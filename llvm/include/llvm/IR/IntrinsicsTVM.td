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

  // A.3 Tuple, list and null primitives
  def int_tvm_first :
    GCCBuiltin<"__builtin_tvm_first">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty], [IntrNoMem, Throws]>;
  def int_tvm_second :
    GCCBuiltin<"__builtin_tvm_second">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty], [IntrNoMem, Throws]>;
  def int_tvm_setfirst :
    GCCBuiltin<"__builtin_tvm_setfirst">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty], [IntrNoMem, Throws]>;
  def int_tvm_setsecond :
    GCCBuiltin<"__builtin_tvm_setsecond">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty], [IntrNoMem, Throws]>;

  // A.4 Constant and literal primitives
  def int_tvm_pushref :
    GCCBuiltin<"__builtin_tvm_pushref">,
    Intrinsic<[llvm_TVMCell_ty], []>;

  // A.6 Comparison primitives
  def int_tvm_sempty :
    GCCBuiltin<"__builtin_tvm_sempty">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty], [IntrNoMem, Throws]>;
  def int_tvm_sdempty :
    GCCBuiltin<"__builtin_tvm_sdempty">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty], [IntrNoMem, Throws]>;
  def int_tvm_srempty :
    GCCBuiltin<"__builtin_tvm_srempty">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty], [IntrNoMem, Throws]>;

  // A.7 Cell primitives
  def int_tvm_ends :
    GCCBuiltin<"__builtin_tvm_ends">,
    Intrinsic<[], [llvm_TVMSlice_ty]>;
  def int_tvm_sti :
    GCCBuiltin<"__builtin_tvm_sti">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_i257_ty, llvm_TVMBuilder_ty, llvm_i257_ty],
              [IntrNoMem, Throws]>;
  def int_tvm_stu :
    GCCBuiltin<"__builtin_tvm_stu">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_i257_ty, llvm_TVMBuilder_ty, llvm_i257_ty],
              [IntrNoMem, Throws]>;
  def int_tvm_ldu :
    GCCBuiltin<"__builtin_tvm_ldu">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty],
              [IntrNoMem, Throws]>;

  def int_tvm_ldi :
    GCCBuiltin<"__builtin_tvm_ldi">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_i257_ty, llvm_TVMSlice_ty],
              [IntrNoMem, Throws]>;

  // A.10 Dictionary manipulation primitives
  def int_tvm_newdict : GCCBuiltin<"__builtin_tvm_newdict">,
                        Intrinsic<[llvm_TVMSlice_ty /* Dictionary */], []>;
  def int_tvm_dictuget :
    GCCBuiltin<"__builtin_tvm_dictuget">,
    Intrinsic<[llvm_TVMSlice_ty /* Dictionary */, llvm_i257_ty],
              [llvm_i257_ty, llvm_TVMSlice_ty /* Dictionary */, llvm_i257_ty]>;
  def int_tvm_dictiget :
    GCCBuiltin<"__builtin_tvm_dictiget">,
    Intrinsic<[llvm_TVMSlice_ty /* Dictionary */, llvm_i257_ty],
              [llvm_i257_ty, llvm_TVMSlice_ty /* Dictionary */, llvm_i257_ty]>;
  def int_tvm_dictuset :
    GCCBuiltin<"__builtin_tvm_dictuset">,
    Intrinsic<[llvm_i257_ty],
              [llvm_i257_ty, llvm_i257_ty, llvm_TVMSlice_ty /* Dictionary */, llvm_i257_ty]>;
  def int_tvm_dictiset :
    GCCBuiltin<"__builtin_tvm_dictiset">,
    Intrinsic<[llvm_i257_ty],
              [llvm_i257_ty, llvm_i257_ty, llvm_TVMSlice_ty /* Dictionary */, llvm_i257_ty]>;
  def int_tvm_plddict :
    GCCBuiltin<"__builtin_tvm_plddict">,
    Intrinsic<[llvm_TVMCell_ty], [llvm_TVMSlice_ty /* Dictionary */], [IntrNoMem]>;
  def int_tvm_stdicts :
    GCCBuiltin<"__builtin_tvm_stdicts">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMSlice_ty, llvm_TVMBuilder_ty],
              [IntrNoMem, Throws]>;
  def int_tvm_stdict :
    GCCBuiltin<"__builtin_tvm_stdict">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMCell_ty /* Dictionary */, llvm_TVMBuilder_ty],
              [IntrNoMem, Throws]>;

  // A.11 Application-specific primitives
  def int_tvm_accept :
    GCCBuiltin<"__builtin_tvm_accept">,
    Intrinsic<[], []>;
  def int_tvm_hashcu :
    GCCBuiltin<"__builtin_tvm_hashcu">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMCell_ty],
              [IntrNoMem, Throws]>;
  def int_tvm_hashsu :
    GCCBuiltin<"__builtin_tvm_hashsu">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMCell_ty],
              [IntrNoMem, Throws]>;
  def int_tvm_chksignu :
    GCCBuiltin<"__builtin_tvm_chksignu">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_TVMSlice_ty, llvm_i257_ty],
              [IntrNoMem, Throws]>;

  // A.13 Codepage primitives
  def int_tvm_setcp :
    GCCBuiltin<"__builtin_tvm_setcp">,
    Intrinsic<[], [llvm_i257_ty]>;
  // ----------------
  def int_tvm_div :
    GCCBuiltin<"__builtin_tvm_div">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;
  def int_tvm_mod :
    GCCBuiltin<"__builtin_tvm_mod">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;
  def int_tvm_bitsize :
    GCCBuiltin<"__builtin_tvm_bitsize">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty], []>;
  def int_tvm_ubitsize :
    GCCBuiltin<"__builtin_tvm_ubitsize">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty], []>;
  def int_tvm_fitsx :
    GCCBuiltin<"__builtin_tvm_fitsx">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty], []>;
  def int_tvm_ufitsx :
    GCCBuiltin<"__builtin_tvm_ufitsx">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty], []>;
  def int_tvm_newc :
    GCCBuiltin<"__builtin_tvm_newc">,
    Intrinsic<[llvm_TVMBuilder_ty], []>;
  def int_tvm_get_persistent_data :
    GCCBuiltin<"__builtin_tvm_get_persistent_data">,
    Intrinsic<[llvm_TVMCell_ty], []>;
  def int_tvm_getreg :
    GCCBuiltin<"__builtin_tvm_getreg">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_setreg :
    GCCBuiltin<"__builtin_tvm_setreg">,
    Intrinsic<[], [llvm_i257_ty, llvm_i257_ty]>;
  def int_tvm_set_persistent_data :
    GCCBuiltin<"__builtin_tvm_set_persistent_data">,
    Intrinsic<[], [llvm_TVMCell_ty]>;
  def int_tvm_ldrefrtos :
    GCCBuiltin<"__builtin_tvm_ldrefrtos">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_TVMSlice_ty],
              [llvm_TVMSlice_ty]>;
  def int_tvm_ldslicex :
    GCCBuiltin<"__builtin_tvm_ldslicex">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_TVMSlice_ty],
              [llvm_TVMSlice_ty, llvm_i257_ty]>;
  def int_tvm_inttoslice :
    GCCBuiltin<"__builtin_tvm_inttoslice">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_i257_ty]>;
  def int_tvm_ctos :
    GCCBuiltin<"__builtin_tvm_ctos">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMCell_ty]>;
  def int_tvm_stoc :
    GCCBuiltin<"__builtin_tvm_stoc">,
    Intrinsic<[llvm_TVMCell_ty], [llvm_TVMSlice_ty]>;
  def int_tvm_stslice :
    GCCBuiltin<"__builtin_tvm_stslice">,
    Intrinsic<[llvm_TVMBuilder_ty],
              [llvm_TVMSlice_ty, llvm_TVMBuilder_ty]>;
  def int_tvm_stref :
    GCCBuiltin<"__builtin_tvm_stref">,
    Intrinsic<[llvm_TVMBuilder_ty],
              [llvm_TVMCell_ty, llvm_TVMBuilder_ty]>;
  def int_tvm_ldref :
    GCCBuiltin<"__builtin_tvm_ldref">,
    Intrinsic<[llvm_TVMCell_ty, llvm_TVMSlice_ty],
              [llvm_TVMSlice_ty]>;
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
    Intrinsic<[llvm_TVMCell_ty], [llvm_TVMBuilder_ty]>;
  def int_tvm_sendrawmsg :
    GCCBuiltin<"__builtin_tvm_sendrawmsg">,
    Intrinsic<[], [llvm_TVMCell_ty, llvm_i257_ty]>;
  def int_tvm_retrieve_signed :
    GCCBuiltin<"__builtin_tvm_retrieve_signed">,
    Intrinsic<[llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty]>;
  def int_tvm_retrieve_unsigned :
    GCCBuiltin<"__builtin_tvm_retrieve_unsigned">,
    Intrinsic<[llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty]>;
  def int_tvm_retrieve_ref :
    GCCBuiltin<"__builtin_tvm_retrieve_ref">,
    Intrinsic<[llvm_TVMCell_ty], [llvm_TVMSlice_ty]>;
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
