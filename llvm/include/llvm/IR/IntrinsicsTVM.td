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
  // A.4 Constant and literal primitives
  def int_tvm_pushref :
    GCCBuiltin<"__builtin_tvm_pushref">,
    Intrinsic<[llvm_TVMCell_ty], []>;

  // A.6 Comparison primitives
  def int_tvm_sempty :
    GCCBuiltin<"__builtin_tvm_sempty">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_sdempty :
    GCCBuiltin<"__builtin_tvm_sdempty">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_srempty :
    GCCBuiltin<"__builtin_tvm_srempty">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_sdfirst :
    GCCBuiltin<"__builtin_tvm_sdfirst">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_sdlexcmp :
    GCCBuiltin<"__builtin_tvm_sdlexcmp">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty, llvm_TVMSlice_ty],
              [IntrNoMem]>;
  def int_tvm_sdeq :
    GCCBuiltin<"__builtin_tvm_sdeq">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty, llvm_TVMSlice_ty],
              [IntrNoMem]>;
  def int_tvm_sdpfx :
    GCCBuiltin<"__builtin_tvm_sdpfx">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty, llvm_TVMSlice_ty],
              [IntrNoMem]>;
  def int_tvm_sdpfxrev :
    GCCBuiltin<"__builtin_tvm_sdpfxrev">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty, llvm_TVMSlice_ty],
              [IntrNoMem]>;
  def int_tvm_sdppfx :
    GCCBuiltin<"__builtin_tvm_sdppfx">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty, llvm_TVMSlice_ty],
              [IntrNoMem]>;
  def int_tvm_sdppfxrev :
    GCCBuiltin<"__builtin_tvm_sdppfxrev">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty, llvm_TVMSlice_ty],
              [IntrNoMem]>;
  def int_tvm_sdsfx :
    GCCBuiltin<"__builtin_tvm_sdsfx">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty, llvm_TVMSlice_ty],
              [IntrNoMem]>;
  def int_tvm_sdsfxrev :
    GCCBuiltin<"__builtin_tvm_sdsfxrev">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty, llvm_TVMSlice_ty],
              [IntrNoMem]>;
  def int_tvm_sdpsfx :
    GCCBuiltin<"__builtin_tvm_sdpsfx">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty, llvm_TVMSlice_ty],
              [IntrNoMem]>;
  def int_tvm_sdpsfxrev :
    GCCBuiltin<"__builtin_tvm_sdpsfxrev">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty, llvm_TVMSlice_ty],
              [IntrNoMem]>;
  def int_tvm_sdcntlead0 :
    GCCBuiltin<"__builtin_tvm_sdcntlead0">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_sdcntlead1 :
    GCCBuiltin<"__builtin_tvm_sdcntlead1">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_sdcnttrail0 :
    GCCBuiltin<"__builtin_tvm_sdcnttrail0">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_sdcnttrail1 :
    GCCBuiltin<"__builtin_tvm_sdcnttrail1">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty], [IntrNoMem]>;

  // A.7 Cell primitives
  def int_tvm_ends :
    GCCBuiltin<"__builtin_tvm_ends">,
    Intrinsic<[], [llvm_TVMSlice_ty]>;
  def int_tvm_sti :
    GCCBuiltin<"__builtin_tvm_sti">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_i257_ty, llvm_TVMBuilder_ty, llvm_i257_ty],
              [IntrNoMem]>;
  def int_tvm_stu :
    GCCBuiltin<"__builtin_tvm_stu">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_i257_ty, llvm_TVMBuilder_ty, llvm_i257_ty],
              [IntrNoMem]>;
  def int_tvm_ldu :
    GCCBuiltin<"__builtin_tvm_ldu">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty],
              [IntrNoMem]>;
  def int_tvm_pldu :
    GCCBuiltin<"__builtin_tvm_pldu">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty, llvm_i257_ty],
              [IntrNoMem]>;
  def int_tvm_lduq :
    GCCBuiltin<"__builtin_tvm_lduq">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_i257_ty],
              [IntrNoMem]>;

  def int_tvm_ldi :
    GCCBuiltin<"__builtin_tvm_ldi">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty],
              [IntrNoMem]>;
  def int_tvm_pldi :
    GCCBuiltin<"__builtin_tvm_pldi">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty, llvm_i257_ty],
              [IntrNoMem]>;

  def int_tvm_ldvaruint16 :
    GCCBuiltin<"__builtin_tvm_ldvaruint16">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty],
              [IntrNoMem]>;

  def int_tvm_ldvaruint32 :
    GCCBuiltin<"__builtin_tvm_ldvaruint32">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty],
              [IntrNoMem]>;

  def int_tvm_ldvarint16 :
    GCCBuiltin<"__builtin_tvm_ldvarint16">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty],
              [IntrNoMem]>;

  def int_tvm_ldvarint32 :
    GCCBuiltin<"__builtin_tvm_ldvarint32">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty],
              [IntrNoMem]>;

  def int_tvm_stvaruint16 :
    GCCBuiltin<"__builtin_tvm_stvaruint16">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMBuilder_ty, llvm_i257_ty],
              [IntrNoMem]>;

  def int_tvm_stvarint16 :
    GCCBuiltin<"__builtin_tvm_stvarint16">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMBuilder_ty, llvm_i257_ty],
              [IntrNoMem]>;

  def int_tvm_stvaruint32 :
    GCCBuiltin<"__builtin_tvm_stvaruint32">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMBuilder_ty, llvm_i257_ty],
              [IntrNoMem]>;

  def int_tvm_stvarint32 :
    GCCBuiltin<"__builtin_tvm_stvarint32">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMBuilder_ty, llvm_i257_ty],
              [IntrNoMem]>;

  // A.10 Dictionary manipulation primitives
  def int_tvm_newdict : GCCBuiltin<"__builtin_tvm_newdict">,
                        Intrinsic<[llvm_TVMCell_ty /* Dict */], []>;
  def int_tvm_dictget :
    GCCBuiltin<"__builtin_tvm_dictget">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictgetref :
    GCCBuiltin<"__builtin_tvm_dictgetref">,
    Intrinsic<[llvm_TVMCell_ty, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictiget :
    GCCBuiltin<"__builtin_tvm_dictiget">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_i257_ty],
              [llvm_i257_ty, llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictigetref :
    GCCBuiltin<"__builtin_tvm_dictigetref">,
    Intrinsic<[llvm_TVMCell_ty, llvm_i257_ty],
              [llvm_i257_ty, llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictuget :
    GCCBuiltin<"__builtin_tvm_dictuget">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_i257_ty],
              [llvm_i257_ty, llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictugetref :
    GCCBuiltin<"__builtin_tvm_dictugetref">,
    Intrinsic<[llvm_TVMCell_ty, llvm_i257_ty],
              [llvm_i257_ty, llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;

  def int_tvm_dictgetnext :
    GCCBuiltin<"__builtin_tvm_dictgetnext">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_TVMSlice_ty, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictmin :
    GCCBuiltin<"__builtin_tvm_dictmin">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_TVMSlice_ty, llvm_i257_ty],
              [llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;

  def int_tvm_dictugetnext :
    GCCBuiltin<"__builtin_tvm_dictugetnext">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty],
              [llvm_i257_ty, llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictumin :
    GCCBuiltin<"__builtin_tvm_dictumin">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty],
              [llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictuminref :
    GCCBuiltin<"__builtin_tvm_dictuminref">,
    Intrinsic<[llvm_TVMCell_ty, llvm_i257_ty, llvm_i257_ty],
              [llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictugetprev :
    GCCBuiltin<"__builtin_tvm_dictugetprev">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty],
              [llvm_i257_ty, llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictumax :
    GCCBuiltin<"__builtin_tvm_dictumax">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty],
              [llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictumaxref :
    GCCBuiltin<"__builtin_tvm_dictumaxref">,
    Intrinsic<[llvm_TVMCell_ty, llvm_i257_ty, llvm_i257_ty],
              [llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dicturemmin :
    GCCBuiltin<"__builtin_tvm_dicturemmin">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_TVMSlice_ty,
               llvm_i257_ty, llvm_i257_ty],
              [llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dicturemminref :
    GCCBuiltin<"__builtin_tvm_dicturemminref">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_TVMCell_ty,
               llvm_i257_ty, llvm_i257_ty],
              [llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dicturemmax :
    GCCBuiltin<"__builtin_tvm_dicturemmax">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_TVMSlice_ty,
               llvm_i257_ty, llvm_i257_ty],
              [llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dicturemmaxref :
    GCCBuiltin<"__builtin_tvm_dicturemmaxref">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_TVMCell_ty,
               llvm_i257_ty, llvm_i257_ty],
              [llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictset :
    GCCBuiltin<"__builtin_tvm_dictset">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */],
              [llvm_TVMSlice_ty, llvm_TVMSlice_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictsetref :
    GCCBuiltin<"__builtin_tvm_dictsetref">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */],
              [llvm_TVMCell_ty, llvm_TVMSlice_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictiset :
    GCCBuiltin<"__builtin_tvm_dictiset">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */],
              [llvm_TVMSlice_ty, llvm_i257_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictisetref :
    GCCBuiltin<"__builtin_tvm_dictisetref">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */],
              [llvm_TVMCell_ty, llvm_i257_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictuset :
    GCCBuiltin<"__builtin_tvm_dictuset">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */],
              [llvm_TVMSlice_ty, llvm_i257_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictusetref :
    GCCBuiltin<"__builtin_tvm_dictusetref">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */],
              [llvm_TVMCell_ty, llvm_i257_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictusetget :
    GCCBuiltin<"__builtin_tvm_dictusetget">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_TVMSlice_ty, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_i257_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictusetgetref :
    GCCBuiltin<"__builtin_tvm_dictusetgetref">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_TVMCell_ty, llvm_i257_ty],
              [llvm_TVMCell_ty, llvm_i257_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictsetget :
    GCCBuiltin<"__builtin_tvm_dictsetget">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_TVMSlice_ty, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_TVMSlice_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictsetgetref :
    GCCBuiltin<"__builtin_tvm_dictsetgetref">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_TVMCell_ty, llvm_i257_ty],
              [llvm_TVMCell_ty, llvm_TVMSlice_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictreplace :
    GCCBuiltin<"__builtin_tvm_dictreplace">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_TVMSlice_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictreplaceref :
    GCCBuiltin<"__builtin_tvm_dictreplaceref">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_i257_ty],
              [llvm_TVMCell_ty, llvm_TVMSlice_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictireplace :
    GCCBuiltin<"__builtin_tvm_dictireplace">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_i257_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictireplaceref :
    GCCBuiltin<"__builtin_tvm_dictireplaceref">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_i257_ty],
              [llvm_TVMCell_ty, llvm_i257_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictureplace :
    GCCBuiltin<"__builtin_tvm_dictureplace">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_i257_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictureplaceref :
    GCCBuiltin<"__builtin_tvm_dictureplaceref">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_i257_ty],
              [llvm_TVMCell_ty, llvm_i257_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;

  def int_tvm_dictadd :
    GCCBuiltin<"__builtin_tvm_dictadd">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_TVMSlice_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictaddref :
    GCCBuiltin<"__builtin_tvm_dictaddref">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_i257_ty],
              [llvm_TVMCell_ty, llvm_TVMSlice_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictiadd :
    GCCBuiltin<"__builtin_tvm_dictiadd">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_i257_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictiaddref :
    GCCBuiltin<"__builtin_tvm_dictiaddref">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_i257_ty],
              [llvm_TVMCell_ty, llvm_i257_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictuadd :
    GCCBuiltin<"__builtin_tvm_dictuadd">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_i257_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictuaddref :
    GCCBuiltin<"__builtin_tvm_dictuaddref">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_i257_ty],
              [llvm_TVMCell_ty, llvm_i257_ty,
               llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictdelget :
    GCCBuiltin<"__builtin_tvm_dictdelget">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_TVMSlice_ty, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictdelgetref :
    GCCBuiltin<"__builtin_tvm_dictdelgetref">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_TVMCell_ty, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictudel :
    GCCBuiltin<"__builtin_tvm_dictudel">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_i257_ty],
              [llvm_i257_ty, llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_dictdel :
    GCCBuiltin<"__builtin_tvm_dictdel">,
    Intrinsic<[llvm_TVMCell_ty /* Dict */, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_TVMCell_ty /* Dict */, llvm_i257_ty]>;
  def int_tvm_lddict :
    GCCBuiltin<"__builtin_tvm_lddict">,
    Intrinsic<[llvm_TVMCell_ty/* Dictionary */, llvm_TVMSlice_ty],
              [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_plddict :
    GCCBuiltin<"__builtin_tvm_plddict">,
    Intrinsic<[llvm_TVMCell_ty], [llvm_TVMSlice_ty /* Dictionary-as-Slice */],
              [IntrNoMem]>;
  def int_tvm_stdicts :
    GCCBuiltin<"__builtin_tvm_stdicts">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMSlice_ty /* Dictionary-as-Slice */,
              llvm_TVMBuilder_ty], [IntrNoMem]>;
  def int_tvm_stdict :
    GCCBuiltin<"__builtin_tvm_stdict">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMCell_ty /* Dictionary */,
              llvm_TVMBuilder_ty], [IntrNoMem]>;

  // A.11 Application-specific primitives
  def int_tvm_hashcu :
    GCCBuiltin<"__builtin_tvm_hashcu">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMCell_ty],
              [IntrNoMem]>;
  def int_tvm_hashsu :
    GCCBuiltin<"__builtin_tvm_hashsu">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty],
              [IntrNoMem]>;
  def int_tvm_chksignu :
    GCCBuiltin<"__builtin_tvm_chksignu">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_TVMSlice_ty, llvm_i257_ty],
              [IntrNoMem]>;

  // A.11.2. Gas-related primitives.
  def int_tvm_accept :
    GCCBuiltin<"__builtin_tvm_accept">,
    Intrinsic<[], []>;
  def int_tvm_setgaslimit :
   GCCBuiltin<"__builtin_tvm_setgaslimit">,
    Intrinsic<[], [llvm_i257_ty]>;
  def int_tvm_buygas :
    GCCBuiltin<"__builtin_tvm_buygas">,
    Intrinsic<[], [llvm_i257_ty]>;
  def int_tvm_gramtogas :
    GCCBuiltin<"__builtin_tvm_gramtogas">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty]>;
  def int_tvm_gastogram :
    GCCBuiltin<"__builtin_tvm_gastogram">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty]>;
  def int_tvm_commit :
    GCCBuiltin<"__builtin_tvm_commit">,
    Intrinsic<[], []>;

  // A.11.4. Configuration primitives.
  def int_tvm_getparam :
    GCCBuiltin<"__builtin_tvm_getparam">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty],
              [IntrNoMem]>;
  def int_tvm_now :
    GCCBuiltin<"__builtin_tvm_now">,
    Intrinsic<[llvm_i257_ty], [], [IntrNoMem]>;
  def int_tvm_blocklt :
    GCCBuiltin<"__builtin_tvm_blocklt">,
    Intrinsic<[llvm_i257_ty], [], [IntrNoMem]>;
  def int_tvm_ltime :
    GCCBuiltin<"__builtin_tvm_ltime">,
    Intrinsic<[llvm_i257_ty], [], [IntrNoMem]>;
  def int_tvm_myaddr :
    GCCBuiltin<"__builtin_tvm_myaddr">,
    Intrinsic<[llvm_TVMSlice_ty], [], [IntrNoMem]>;
  def int_tvm_configroot :
    GCCBuiltin<"__builtin_tvm_configroot">,
    Intrinsic<[llvm_TVMCell_ty], [], [IntrNoMem]>;
  def int_tvm_configdict :
    GCCBuiltin<"__builtin_tvm_configdict">,
    Intrinsic<[llvm_TVMCell_ty, llvm_i257_ty], [], [IntrNoMem]>;
  def int_tvm_configparam :
    GCCBuiltin<"__builtin_tvm_configparam">,
    Intrinsic<[llvm_TVMCell_ty, llvm_i257_ty], [llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_configoptparam :
    GCCBuiltin<"__builtin_tvm_configoptparam">,
    Intrinsic<[llvm_TVMCell_ty], [llvm_i257_ty], [IntrNoMem]>;

  // A.11.5. Global variable primitives.
  def int_tvm_getglobal :
    GCCBuiltin<"__builtin_tvm_getglobal">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty], [IntrInaccessibleMemOnly, IntrReadMem]>;
  def int_tvm_setglobal :
    GCCBuiltin<"__builtin_tvm_setglobal">,
    Intrinsic<[], [llvm_i257_ty, llvm_i257_ty], [IntrInaccessibleMemOnly]>;

  // A.11.7. Application specific misc primitives.
  def int_tvm_cdatasize :
    GCCBuiltin<"__builtin_tvm_cdatasize">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty, llvm_i257_ty], [llvm_TVMCell_ty, llvm_i257_ty],
              [IntrNoMem]>;

  // A.11.8 Message and address manipulation primitives.
  def int_tvm_ldmsgaddr :
    GCCBuiltin<"__builtin_tvm_ldmsgaddr">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty],
              [IntrNoMem]>;
  def int_tvm_ldmsgaddrq :
    GCCBuiltin<"__builtin_tvm_ldmsgaddrq">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_TVMSlice_ty, llvm_i257_ty],
              [llvm_TVMSlice_ty],
              [IntrNoMem]>;

  def int_tvm_parsemsgaddr :
    GCCBuiltin<"__builtin_tvm_parsemsgaddr">,
    Intrinsic<[llvm_TVMTuple_ty], [llvm_TVMSlice_ty],
              [IntrNoMem]>;
  def int_tvm_parsemsgaddrq :
    GCCBuiltin<"__builtin_tvm_parsemsgaddrq">,
    Intrinsic<[llvm_TVMTuple_ty, llvm_i257_ty],
              [llvm_TVMSlice_ty],
              [IntrNoMem]>;

  def int_tvm_rewritestdaddr :
    GCCBuiltin<"__builtin_tvm_rewritestdaddr">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_TVMSlice_ty],
              [IntrNoMem]>;
  def int_tvm_rewritestdaddrq :
    GCCBuiltin<"__builtin_tvm_rewritestdaddrq">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty, llvm_i257_ty],
              [llvm_TVMSlice_ty],
              [IntrNoMem]>;

  def int_tvm_rewritevaraddr :
    GCCBuiltin<"__builtin_tvm_rewritevaraddr">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty],
              [IntrNoMem]>;
  def int_tvm_rewritevaraddrq :
    GCCBuiltin<"__builtin_tvm_rewritevaraddrq">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty, llvm_i257_ty],
              [llvm_TVMSlice_ty],
              [IntrNoMem]>;

  // A.11.10. Outbound message and output action primitives.
  def int_tvm_rawreserve :
    GCCBuiltin<"__builtin_tvm_rawreserve">,
    Intrinsic<[], [llvm_i257_ty, llvm_i257_ty], []>;
  def int_tvm_setcode :
    GCCBuiltin<"__builtin_tvm_setcode">,
    Intrinsic<[], [llvm_TVMCell_ty], []>;

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

  def int_tvm_muldivr :
    GCCBuiltin<"__builtin_tvm_muldivr">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

  def int_tvm_bitsize :
    GCCBuiltin<"__builtin_tvm_bitsize">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_ubitsize :
    GCCBuiltin<"__builtin_tvm_ubitsize">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_fitsx :
    GCCBuiltin<"__builtin_tvm_fitsx">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_ufitsx :
    GCCBuiltin<"__builtin_tvm_ufitsx">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty], [IntrNoMem]>;

  // IntrHasSideEffects to avoid empty builder keep
  //  through coroutine suspension points
  def int_tvm_newc :
    GCCBuiltin<"__builtin_tvm_newc">,
    Intrinsic<[llvm_TVMBuilder_ty], [], [IntrInaccessibleMemOnly, IntrReadMem]>;
  def int_tvm_ldref :
    GCCBuiltin<"__builtin_tvm_ldref">,
    Intrinsic<[llvm_TVMCell_ty, llvm_TVMSlice_ty],
              [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_ldrefrtos :
    GCCBuiltin<"__builtin_tvm_ldrefrtos">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_TVMSlice_ty],
              [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_ldslice :
    GCCBuiltin<"__builtin_tvm_ldslice">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_TVMSlice_ty],
              [llvm_TVMSlice_ty, llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_sbits :
    GCCBuiltin<"__builtin_tvm_sbits">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_srefs :
    GCCBuiltin<"__builtin_tvm_srefs">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_sbitrefs :
    GCCBuiltin<"__builtin_tvm_sbitrefs">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_brembits :
    GCCBuiltin<"__builtin_tvm_brembits">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMBuilder_ty], [IntrNoMem]>;
  def int_tvm_bremrefs :
    GCCBuiltin<"__builtin_tvm_bremrefs">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMBuilder_ty], [IntrNoMem]>;
  def int_tvm_inttoslice :
    GCCBuiltin<"__builtin_tvm_inttoslice">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_ctos :
    GCCBuiltin<"__builtin_tvm_ctos">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMCell_ty], [IntrNoMem]>;
  def int_tvm_stoc :
    GCCBuiltin<"__builtin_tvm_stoc">,
    Intrinsic<[llvm_TVMCell_ty], [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_stslice :
    GCCBuiltin<"__builtin_tvm_stslice">,
    Intrinsic<[llvm_TVMBuilder_ty],
              [llvm_TVMSlice_ty, llvm_TVMBuilder_ty], [IntrNoMem]>;
  def int_tvm_stref :
    GCCBuiltin<"__builtin_tvm_stref">,
    Intrinsic<[llvm_TVMBuilder_ty],
              [llvm_TVMCell_ty, llvm_TVMBuilder_ty], [IntrNoMem]>;
  def int_tvm_throw :
    GCCBuiltin<"__builtin_tvm_throw">,
    Intrinsic<[], [llvm_i257_ty], [IntrNoReturn]>;
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
    Intrinsic<[llvm_TVMCell_ty], [llvm_TVMBuilder_ty], [IntrNoMem]>;
  def int_tvm_sendrawmsg :
    GCCBuiltin<"__builtin_tvm_sendrawmsg">,
    Intrinsic<[], [llvm_TVMCell_ty, llvm_i257_ty], [IntrInaccessibleMemOnly]>;
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
  def int_tvm_bbits :
    GCCBuiltin<"__builtin_tvm_bbits">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMBuilder_ty]>;
  def int_tvm_brefs :
    GCCBuiltin<"__builtin_tvm_brefs">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMBuilder_ty]>;

  // A.3 Tuple, list and null primitives
  def int_tvm_index :
    GCCBuiltin<"__builtin_tvm_index">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty, llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_indexq :
    GCCBuiltin<"__builtin_tvm_indexq">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty, llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_setindex :
    GCCBuiltin<"__builtin_tvm_setindex">,
    Intrinsic<[llvm_TVMTuple_ty],
              [llvm_TVMTuple_ty, llvm_i257_ty, llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_setindexq :
    GCCBuiltin<"__builtin_tvm_setindexq">,
    Intrinsic<[llvm_TVMTuple_ty],
              [llvm_TVMTuple_ty, llvm_i257_ty, llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_chktuple :
    GCCBuiltin<"__builtin_tvm_chktuple">,
    Intrinsic<[], [llvm_TVMTuple_ty], []>;
  def int_tvm_tlen:
    GCCBuiltin<"__builtin_tvm_tlen">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;
  def int_tvm_qtlen:
    GCCBuiltin<"__builtin_tvm_qtlen">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;
  def int_tvm_istuple:
    GCCBuiltin<"__builtin_tvm_istuple">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;
  def int_tvm_last:
    GCCBuiltin<"__builtin_tvm_last">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;
  def int_tvm_tpush:
    GCCBuiltin<"__builtin_tvm_tpush">,
    Intrinsic<[llvm_TVMTuple_ty], [llvm_TVMTuple_ty, llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_tpop:
    GCCBuiltin<"__builtin_tvm_tpop">,
    Intrinsic<[llvm_TVMTuple_ty, llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;

  def int_tvm_index2 :
    GCCBuiltin<"__builtin_tvm_index2">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty, llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;
  def int_tvm_index3 :
    GCCBuiltin<"__builtin_tvm_index3">,
    Intrinsic<[llvm_i257_ty],
              [llvm_TVMTuple_ty, llvm_i257_ty, llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;
  def int_tvm_cadr :
    GCCBuiltin<"__builtin_tvm_cadr">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;
  def int_tvm_cddr :
    GCCBuiltin<"__builtin_tvm_cddr">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;
  def int_tvm_caddr :
    GCCBuiltin<"__builtin_tvm_caddr">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;
  def int_tvm_cdddr :
    GCCBuiltin<"__builtin_tvm_cdddr">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;
  def int_tvm_isnull:
    GCCBuiltin<"__builtin_tvm_isnull">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_isnull_tuple:
    GCCBuiltin<"__builtin_tvm_isnull_tuple">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;
  def int_tvm_isnull_slice:
    GCCBuiltin<"__builtin_tvm_isnull_slice">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_isnull_builder:
    GCCBuiltin<"__builtin_tvm_isnull_builder">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMBuilder_ty], [IntrNoMem]>;
  def int_tvm_isnull_cell:
    GCCBuiltin<"__builtin_tvm_isnull_cell">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMCell_ty], [IntrNoMem]>;

  // Control registers
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
  def int_tvm_get_temporary_data :
    GCCBuiltin<"__builtin_tvm_get_temporary_data">,
    Intrinsic<[llvm_TVMTuple_ty], [], [IntrNoMem]>;

  // Cast intrinsics
  // We need them for receiving slice/builder/cell/tuple from tuple element
  def int_tvm_cast_to_tuple :
    GCCBuiltin<"__builtin_tvm_cast_to_tuple">,
    Intrinsic<[llvm_TVMTuple_ty], [llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_cast_to_slice :
    GCCBuiltin<"__builtin_tvm_cast_to_slice">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_cast_to_builder :
    GCCBuiltin<"__builtin_tvm_cast_to_builder">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_cast_to_cell :
    GCCBuiltin<"__builtin_tvm_cast_to_cell">,
    Intrinsic<[llvm_TVMCell_ty], [llvm_i257_ty], [IntrNoMem]>;
  def int_tvm_cast_from_tuple :
    GCCBuiltin<"__builtin_tvm_cast_from_tuple">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;
  def int_tvm_cast_from_slice :
    GCCBuiltin<"__builtin_tvm_cast_from_slice">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty], [IntrNoMem]>;
  def int_tvm_cast_from_builder :
    GCCBuiltin<"__builtin_tvm_cast_from_builder">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMBuilder_ty], [IntrNoMem]>;
  def int_tvm_cast_from_cell :
    GCCBuiltin<"__builtin_tvm_cast_from_cell">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMCell_ty], [IntrNoMem]>;

  def int_tvm_pushnull : GCCBuiltin<"__builtin_tvm_pushnull">,
    Intrinsic<[llvm_i257_ty], [], [IntrNoMem]>;

  def int_tvm_pushslice_empty : GCCBuiltin<"__builtin_tvm_pushslice_empty">,
    Intrinsic<[llvm_TVMSlice_ty], [], [IntrNoMem]>;

  def int_tvm_bless : GCCBuiltin<"__builtin_tvm_bless">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty], [IntrNoMem]>;

  def int_tvm_tuple : GCCBuiltin<"__builtin_tvm_tuple">,
    Intrinsic<[llvm_TVMTuple_ty], [llvm_vararg_ty], [IntrNoMem]>;

  foreach i = 1-255 in
  def int_tvm_untuple#i : GCCBuiltin<"__builtin_tvm_untuple"#i>,
    Intrinsic<!listsplat(llvm_i257_ty, i), [llvm_TVMTuple_ty], [IntrNoMem]>;

  foreach i = 1-255 in
  def int_tvm_unpackfirst#i : GCCBuiltin<"__builtin_tvm_unpackfirst"#i>,
    Intrinsic<!listsplat(llvm_i257_ty, i), [llvm_TVMTuple_ty], [IntrNoMem]>;

  def int_tvm_hiddenstack :
    GCCBuiltin<"__builtin_tvm_hiddenstack">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty], [IntrNoMem]>;

// ***********************************************************************/

// ******************************************* 
//
// A.3  Tuple, List, and Null primitives
//
// *******************************************/

  // NIL ( – t), pushes the only Tuple t = () of length zero
//  def int_tvm_nil :
//    GCCBuiltin<"__builtin_tvm_nil">,
//    Intrinsic<[llvm_TVMTuple_ty], [], [IntrNoMem]>;

  // SINGLE (x – t), creates a singleton t := (x), i.e., a Tuple of length one
  def int_tvm_single :
    GCCBuiltin<"__builtin_tvm_single">,
    Intrinsic<[llvm_TVMTuple_ty], [llvm_i257_ty], [IntrNoMem]>;

  // PAIR or CONS (x y – t), creates pair t := (x, y)
  def int_tvm_pair :
    GCCBuiltin<"__builtin_tvm_pair">,
    Intrinsic<[llvm_TVMTuple_ty], [llvm_i257_ty, llvm_i257_ty], [IntrNoMem]>;

  // TRIPLE (x y z – t), creates triple t := (x, y, z)
  def int_tvm_triple :
    GCCBuiltin<"__builtin_tvm_triple">,
    Intrinsic<[llvm_TVMTuple_ty], [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty], [IntrNoMem]>;

  // FIRST or CAR (t – x), returns the fist element of a Tuple
  def int_tvm_first :
    GCCBuiltin<"__builtin_tvm_first">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;

  // SECOND or CDR (t – y), returns the second element of a Tuple
  def int_tvm_second :
    GCCBuiltin<"__builtin_tvm_second">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;

  // THIRD (t – z), returns the third element of a Tuple
  def int_tvm_third :
    GCCBuiltin<"__builtin_tvm_third">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;  

  // UNSINGLE (t – x), unpacks a singleton t = (x)
  def int_tvm_unsingle :
    GCCBuiltin<"__builtin_tvm_unsingle">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>; 

  // UNPAIR or UNCONS (t – x y), unpacks a pair t = (x, y)
  def int_tvm_unpair :
    GCCBuiltin<"__builtin_tvm_unpair">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;

  // UNTRIPLE (t – x y z), unpacks a triple t = (x, y, z)
  def int_tvm_untriple :
    GCCBuiltin<"__builtin_tvm_untriple">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty, llvm_i257_ty], [llvm_TVMTuple_ty], [IntrNoMem]>;

  // EXPLODE n (t – x1. . . xmm), unpacks a Tuple t = (x1, . . . , xm) and returns its length m, 
  //  but only if m <= n <= 15. Otherwise throws a type check exception
  // TODO

  // SETFIRST (t x – t0), sets the first component of Tuple t to x and returns the resulting Tuple t0
  def int_tvm_setfirst :
    GCCBuiltin<"__builtin_tvm_setfirst">,
    Intrinsic<[llvm_TVMTuple_ty], [llvm_TVMTuple_ty, llvm_i257_ty], [IntrNoMem]>;

  // SETSECOND (t x – t0), sets the second component of Tuple t to x and returns the resulting Tuple t0
  def int_tvm_setsecond :
    GCCBuiltin<"__builtin_tvm_setsecond">,
    Intrinsic<[llvm_TVMTuple_ty], [llvm_TVMTuple_ty, llvm_i257_ty], [IntrNoMem]>;

  // SETTHIRD (t x – t0), sets the third component of Tuple t to x and returns the resulting Tuple t0
  def int_tvm_setthird :
    GCCBuiltin<"__builtin_tvm_setthird">,
    Intrinsic<[llvm_TVMTuple_ty], [llvm_TVMTuple_ty, llvm_i257_ty], [IntrNoMem]>;

  // TUPLEVAR (x1. . . xnn – t), creates a new Tuple t of length n similarly to TUPLE, but with 0 <= n <= 255 taken from the stack
  // TODO

  // UNTUPLEVAR (t n – x1. . . xn), similar to UNTUPLE n, but with 0 <= n <= 255 taken from the stack
  // TODO

  // UNPACKFIRSTVAR (t n – x1. . . xn), similar to UNPACKFIRST n, but with 0 <= n <= 255 taken from the stack
  // TODO

  // EXPLODEVAR (t n – x1. . . xmm), similar to EXPLODE n, but with 0 <= n <= 255 taken from the stack
  // TODO

  // INDEXVARQ (t k – x), similar to INDEXQ n, but with 0 <= k <= 254 taken from the stack
  def int_tvm_indexvarq :
    GCCBuiltin<"__builtin_tvm_indexvarq">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMTuple_ty, llvm_i257_ty], [IntrNoMem]>;

  // SETINDEXVARQ (t x k – t0), similar to SETINDEXQ	k, but with 0 <= k <= 254 taken from the stack
  def int_tvm_setindexvarq :
    GCCBuiltin<"__builtin_tvm_setindexvarq">,
    Intrinsic<[llvm_TVMTuple_ty], [llvm_TVMTuple_ty, llvm_i257_ty, llvm_i257_ty], [IntrNoMem]>;


// ******************************************* 
//
// A.5.2. Division
//
// *******************************************/

// DIVR (x y – q0:= bx/y + 1/2c)
  def int_tvm_divr :
    GCCBuiltin<"__builtin_tvm_divr">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

// DIVC (x y – q00:= dx/ye)
  def int_tvm_divc :
    GCCBuiltin<"__builtin_tvm_divc">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

// DIVMOD (x y - q r), where q := bx/yc, r := x - yq
  def int_tvm_divmod :
    GCCBuiltin<"__builtin_tvm_divmod">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty], [IntrNoMem]>; 

// DIVMODR (x y – q0 r0), where q0:= bx/y + 1/2c, r0:= x - yq0
  def int_tvm_divmodr :
    GCCBuiltin<"__builtin_tvm_divmodr">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty], [IntrNoMem]>; 

// DIVMODC	(x y – q00 r00), where q00:= dx/ye, r00:= x - yq00
  def int_tvm_divmodc :
    GCCBuiltin<"__builtin_tvm_divmodc">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty], [IntrNoMem]>; 

// MODPOW2 tt + 1: (x – x mod 2tt+1)
  def int_tvm_modpow2 :
    GCCBuiltin<"__builtin_tvm_modpow2">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

// MULDIVMOD (x y z – q r), where q := bx · y/zc, r := x · y mod z (same as */MOD in Forth)
  def int_tvm_muldivmod :
    GCCBuiltin<"__builtin_tvm_muldivmod">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty], [IntrNoMem]>;

// MULRSHIFT (x y z – bxy * 2-zc) for 0 <= z <= 256
  def int_tvm_mulrshift :
    GCCBuiltin<"__builtin_tvm_mulrshift">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

// MULRSHIFTR (x y z – bxy * 2-z+ 1/2c) for 0 <= z <= 256
  def int_tvm_mulrshiftr :
    GCCBuiltin<"__builtin_tvm_mulrshiftr">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

// MULRSHIFT tt + 1 (x y - bxy · 2-tt-1c)
  def int_tvm_mulrshift_tt :
    GCCBuiltin<"__builtin_tvm_mulrshift_tt">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

// MULRSHIFTR	tt + 1 (x y - bxy · 2-tt-1+ 1/2c)
  def int_tvm_mulrshiftr_tt :
    GCCBuiltin<"__builtin_tvm_mulrshiftr_tt">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

// LSHIFTDIV (x y z - b2zx/yc) for 0 <= z <= 256
  def int_tvm_lshiftdiv :
    GCCBuiltin<"__builtin_tvm_lshiftdiv">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

// LSHIFTDIVR (x y z – b2zx/y + 1/2c) for 0 <= z <= 256
  def int_tvm_lshiftdivr :
    GCCBuiltin<"__builtin_tvm_lshiftdivr">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

// LSHIFTDIV tt + 1 (x y – b2tt+1x/yc)
  def int_tvm_lshiftdiv_tt :
    GCCBuiltin<"__builtin_tvm_lshiftdiv_tt">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

// LSHIFTDIVR tt + 1 (x y – b2tt+1x/y+ 1/2c)
  def int_tvm_lshiftdivr_tt :
    GCCBuiltin<"__builtin_tvm_lshiftdivr_tt">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

// ******************************************* 
//
// A.5.3. Shifts, logical operations
//
// *******************************************/ 

  // POW2 (y – 2**y), 0 <= y <= 1023, equivalent to ONE; SWAP; LSHIFT
  def int_tvm_pow2 :
    GCCBuiltin<"__builtin_tvm_pow2">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty], [IntrNoMem]>;

   // MINMAX or INTSORT2 (x y – x y or y x), sorts two integer
  def int_tvm_minmax :
    GCCBuiltin<"__builtin_tvm_minmax">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty], [IntrNoMem]>; 

// ******************************************* 
//
// A.5.4. Quiet arithmetic primitives
//
// *******************************************/ 

  // QADD (x y – x + y), always works if x and y are	Integer s, but returns a NaN if the addition cannot be performed
  def int_tvm_qadd :
    GCCBuiltin<"__builtin_tvm_qadd">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

  // QDIV (x y – bx/yc), returns a NaN if y = 0, or if y = -1 and
  //   x = -2256, or if either of x or y is a NaN
  def int_tvm_qdiv :
    GCCBuiltin<"__builtin_tvm_qdiv">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

  // QAND (x y – x&y), bitwise “and” (similar to AND), but returns a NaN if either x or y is a NaN
  def int_tvm_qand :
    GCCBuiltin<"__builtin_tvm_qand">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

  // QOR (x y – x|y), bitwise “or”. If  x = -1 or y = -1, the result is always -1,
  //  even if the other argument is a NaN
  def int_tvm_qor :
    GCCBuiltin<"__builtin_tvm_qor">,
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

// ******************************************* 
//
// A.6	Comparison primitives
//
// *******************************************/ 

  // SGN (x – sgn(x)), computes the sign of an integer x:	-1 if x < 0, 0 if x = 0, 1 if x > 0
  def int_tvm_sgn :
      GCCBuiltin<"__builtin_tvm_sgn">,
      Intrinsic<[llvm_i257_ty], [llvm_i257_ty], [IntrNoMem]>;

  // ISNAN (x – x = NaN), checks whether x is a NaN
  def int_tvm_isnan :
      GCCBuiltin<"__builtin_tvm_isnan">,
      Intrinsic<[llvm_i257_ty], [llvm_i257_ty], [IntrNoMem]>;

  // CHKNAN (x - x), throws an arithmetic overflow exception if x is a NaN
  def int_tvm_chknan :
      GCCBuiltin<"__builtin_tvm_chknan">,
      Intrinsic<[llvm_i257_ty], [llvm_i257_ty], [IntrNoMem]>;

// *******************************************        
//
//       A.7        Cell primitives        
//
// *******************************************/ 

  // STBREFR or ENDCST (b b00 – b), equivalent to ENDC; SWAP; STREF  
  def int_tvm_endcst :
    GCCBuiltin<"__builtin_tvm_endcst">,
    Intrinsic<[llvm_TVMBuilder_ty],
              [llvm_TVMBuilder_ty, llvm_TVMBuilder_ty], [IntrNoMem]>;

  // TODO
  // STIXR  (b x l – b0),  similar to STIX, but with arguments in a different order
  //def int_tvm_stixr :
  //  GCCBuiltin<"__builtin_tvm_stixr">,
  //  Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMBuilder_ty, llvm_i257_ty, llvm_i257_ty],
  //            [IntrNoMem]>;

  // STUXR  (b x l – b0),  similar to STUX, but with arguments in a different order
  //def int_tvm_stuxr :
  //  GCCBuiltin<"__builtin_tvm_stuxr">,
  //  Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMBuilder_ty, llvm_i257_ty, llvm_i257_ty],
  //            [IntrNoMem]>;  

  // STIR cc + 1 (b x – b0), equivalent to SWAP; STI cc + 1
  def int_tvm_stir :
    GCCBuiltin<"__builtin_tvm_stir">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMBuilder_ty, llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

  // STUR cc + 1 (b x – b0), equivalent to SWAP; STU cc + 1
  def int_tvm_stur :
    GCCBuiltin<"__builtin_tvm_stur">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMBuilder_ty, llvm_i257_ty, llvm_i257_ty],
              [IntrNoMem]>;

  // STBREF (b0 b – b00), equivalent to SWAP; STBREFREV
  def int_tvm_stbref :
    GCCBuiltin<"__builtin_tvm_stbref">,
    Intrinsic<[llvm_TVMBuilder_ty],
              [llvm_TVMBuilder_ty, llvm_TVMBuilder_ty], [IntrNoMem]>;

  // STB (b0 b – b00), appends all data from Builder b0 to Builder b
  def int_tvm_stb :
    GCCBuiltin<"__builtin_tvm_stb">,
    Intrinsic<[llvm_TVMBuilder_ty],
              [llvm_TVMBuilder_ty, llvm_TVMBuilder_ty], [IntrNoMem]>;

  // STREFR (b c – b0)
  def int_tvm_strefr :
    GCCBuiltin<"__builtin_tvm_strefr">,
    Intrinsic<[llvm_TVMBuilder_ty],
              [llvm_TVMBuilder_ty, llvm_TVMCell_ty], [IntrNoMem]>;

  // STSLICER (b s – b0)
  def int_tvm_stslicer :
    GCCBuiltin<"__builtin_tvm_stslicer">,
    Intrinsic<[llvm_TVMBuilder_ty],
              [llvm_TVMBuilder_ty, llvm_TVMSlice_ty], [IntrNoMem]>;

  // STBR (b b0 – b00), concatenates two Builder s, equivalent to SWAP; STB
  def int_tvm_stbr :
    GCCBuiltin<"__builtin_tvm_stbr">,
    Intrinsic<[llvm_TVMBuilder_ty],
              [llvm_TVMBuilder_ty, llvm_TVMBuilder_ty], [IntrNoMem]>;

  // STREFQ	(c b – c b -1 or b0 0)
  def int_tvm_strefq :
    GCCBuiltin<"__builtin_tvm_strefq">,
    Intrinsic<[llvm_TVMCell_ty, llvm_TVMBuilder_ty, llvm_i257_ty],
              [llvm_TVMCell_ty, llvm_TVMBuilder_ty], [IntrNoMem]>;

  // STBREFQ (b0 b – b0 b -1 or b00 0)
  def int_tvm_stbrefq :
    GCCBuiltin<"__builtin_tvm_stbrefq">,
    Intrinsic<[llvm_TVMBuilder_ty, llvm_TVMBuilder_ty, llvm_i257_ty],
              [llvm_TVMBuilder_ty, llvm_TVMBuilder_ty], [IntrNoMem]>;

  // STSLICEQ (s b - s b -1 or b0 0)
  def int_tvm_stsliceq :
    GCCBuiltin<"__builtin_tvm_stsliceq">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_TVMBuilder_ty, llvm_i257_ty],
              [llvm_TVMSlice_ty, llvm_TVMBuilder_ty], [IntrNoMem]>;

  // STBQ (b0 b – b0 b -1 or b00 0)
  def int_tvm_stbq :
    GCCBuiltin<"__builtin_tvm_stbq">,
    Intrinsic<[llvm_TVMBuilder_ty, llvm_TVMBuilder_ty, llvm_i257_ty],
              [llvm_TVMBuilder_ty, llvm_TVMBuilder_ty], [IntrNoMem]>;

  // STBREFRQ (b b0 – b b0 -1 or b00 0)
  def int_tvm_stbrefrq :
    GCCBuiltin<"__builtin_tvm_stbrefrq">,
    Intrinsic<[llvm_TVMBuilder_ty, llvm_TVMBuilder_ty, llvm_i257_ty],
              [llvm_TVMBuilder_ty, llvm_TVMBuilder_ty], [IntrNoMem]>;

  // STSLICERQ (b s – b s -1 or b00 0)
  def int_tvm_stslicerq :
    GCCBuiltin<"__builtin_tvm_stslicerq">,
    Intrinsic<[llvm_TVMBuilder_ty, llvm_TVMSlice_ty, llvm_i257_ty],
              [llvm_TVMBuilder_ty, llvm_TVMSlice_ty], [IntrNoMem]>;

  // STBRQ (b b0 – b b0 -1 or b00 0)
  def int_tvm_stbrq :
    GCCBuiltin<"__builtin_tvm_stbrq">,
    Intrinsic<[llvm_TVMBuilder_ty, llvm_TVMBuilder_ty, llvm_i257_ty],
              [llvm_TVMBuilder_ty, llvm_TVMBuilder_ty], [IntrNoMem]>;


  // STREFCONST, equivalent to PUSHREF; STREFR
  // TODO

  // STREF2CONST, equivalent to STREFCONST; STREFCONST
  // TODO


  // ENDXC  (b x – c), if x6 = 0, creates a special or exotic cell from Builder b
  def int_tvm_endxc :
    GCCBuiltin<"__builtin_tvm_endxc">,
    Intrinsic<[llvm_TVMCell_ty], [llvm_TVMBuilder_ty, llvm_i257_ty], [IntrNoMem]>;

// STILE4 (x b – b0), stores a little-endian signed 32-bit integer
  def int_tvm_stile4 :
    GCCBuiltin<"__builtin_tvm_stile4">,
    Intrinsic<[llvm_TVMBuilder_ty],
              [llvm_i257_ty, llvm_TVMBuilder_ty], [IntrNoMem]>;  

  // STULE4 (x b – b0), stores a little-endian unsigned 32-bit integer
    def int_tvm_stule4 :
    GCCBuiltin<"__builtin_tvm_stule4">,
    Intrinsic<[llvm_TVMBuilder_ty],
              [llvm_i257_ty, llvm_TVMBuilder_ty], [IntrNoMem]>; 

  // STILE8 (x b – b0), stores a little-endian signed 64-bit integer
  def int_tvm_stile8 :
    GCCBuiltin<"__builtin_tvm_stile8">,
    Intrinsic<[llvm_TVMBuilder_ty],
              [llvm_i257_ty, llvm_TVMBuilder_ty], [IntrNoMem]>; 

  // STULE8 (x b – b0), stores a little-endian unsigned 64-bit integer
  def int_tvm_stule8 :
    GCCBuiltin<"__builtin_tvm_stule8">,
    Intrinsic<[llvm_TVMBuilder_ty],
              [llvm_i257_ty, llvm_TVMBuilder_ty], [IntrNoMem]>;

  // BDEPTH  (b – x), returns the depth of Builder b. If no cell references are stored in b, then x =  0; 
  // otherwise x is one plus the maximum of depths of cells referred to from b
  def int_tvm_bdepth :
    GCCBuiltin<"__builtin_tvm_bdepth">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMBuilder_ty]>;

  // BBITREFS (b – x y), returns the numbers of both data bits and cell references in b
  def int_tvm_bbitrefs :
    GCCBuiltin<"__builtin_tvm_bbitrefs">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_TVMBuilder_ty]>;

  // BREMBITREFS  (b – x0 y0)
  def int_tvm_brembitrefs :
    GCCBuiltin<"__builtin_tvm_brembitrefs">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_TVMBuilder_ty]>;

  // BCHKBITS cc + 1 (b –), checks whether cc + 1 bits can be stored into b, where 0 <= cc <= 255
  // TODO

  // BCHKBITS (b x – ), checks whether x bits can be stored into b, 0 <= x <= 1023. If there is no space for x more bits in b ... throws an exception
  def int_tvm_bchkbits :
    GCCBuiltin<"__builtin_tvm_bchkbits">,
    Intrinsic<[], [llvm_TVMBuilder_ty, llvm_i257_ty]>;

  // BCHKREFS (b y – ), checks whether y references can be stored into b, 0 <= y <= 7
  def int_tvm_bchkrefs :
    GCCBuiltin<"__builtin_tvm_bchkrefs">,
    Intrinsic<[], [llvm_TVMBuilder_ty, llvm_i257_ty]>;

  // BCHKBITREFS (b x y – ), checks whether x bits and y references can be stored into b, 0 <= x <= 1023, 0 <= y <= 7
  def int_tvm_bchkbitrefs :
    GCCBuiltin<"__builtin_tvm_bchkbitrefs">,
    Intrinsic<[], [llvm_TVMBuilder_ty, llvm_i257_ty, llvm_i257_ty]>;

  // BCHKBITSQ cc + 1 (b – ?), checks whether cc + 1 bits can be stored into b, where 0 <= cc <= 255
  // TODO

  // BCHKBITSQ (b x – ?), checks whether x bits can be stored into b, 0 <= x <= 1023
  def int_tvm_bchkbitsq :
    GCCBuiltin<"__builtin_tvm_bchkbitsq">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMBuilder_ty, llvm_i257_ty]>;

  // BCHKREFSQ (b y – ?), checks whether y references can be stored into b, 0 <= y <= 7
  def int_tvm_bchkrefsq :
    GCCBuiltin<"__builtin_tvm_bchkrefsq">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMBuilder_ty, llvm_i257_ty]>;

  // BCHKBITREFSQ (b x y – ?), checks whether x bits and y references can be stored into b, 0 <= x <= 1023, 0 <= y <= 7
  def int_tvm_bchkbitrefsq :
    GCCBuiltin<"__builtin_tvm_bchkbitrefsq">,
    Intrinsic<[], [llvm_TVMBuilder_ty, llvm_i257_ty, llvm_i257_ty]>;  

  // STZEROES (b n – b0), stores n binary zeroes into Builder b
  def int_tvm_stzeroes :
    GCCBuiltin<"__builtin_tvm_stzeroes">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMBuilder_ty, llvm_i257_ty]>;

  // STONES (b n – b0), stores n binary ones into Builder b
  def int_tvm_stones :
    GCCBuiltin<"__builtin_tvm_stones">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMBuilder_ty, llvm_i257_ty]>;

  // STSAME (b n x – b0), stores n binary xes (0 <= x <= 1) into Builder b
  def int_tvm_stsame :
    GCCBuiltin<"__builtin_tvm_stsame">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMBuilder_ty, llvm_i257_ty, llvm_i257_ty]>;

  // STSLICECONST sss (b –	b0), stores a constant subslice sss consisting of 0 <= x <= 3 
  //  references and up to 8y + 1 data bits, with 0 <= y <= 7.
  // TODO

  // STSLICECONST ‘0’ or STZERO (b – b0), stores one binary zero
  def int_tvm_stzero :
    GCCBuiltin<"__builtin_tvm_stzero">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMBuilder_ty]>;

  // STSLICECONST ‘1’ or STONE (b – b0), stores one binary one
  def int_tvm_stone :
    GCCBuiltin<"__builtin_tvm_stone">,
    Intrinsic<[llvm_TVMBuilder_ty], [llvm_TVMBuilder_ty]>;

// ******************************************* 
//
// A.7.2. Cell deserialization primitives
//
// *******************************************/

  // LDIQ cc + 1 (s – x s0 -1 or s 0), a quiet version of LDI
  def int_tvm_ldiq :
    GCCBuiltin<"__builtin_tvm_ldiq">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty, llvm_i257_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // PLDIQ cc + 1 (s – x -1 or 0), a quiet version of PLDI
  def int_tvm_pldiq :
    GCCBuiltin<"__builtin_tvm_pldiq">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // PLDUQ cc + 1 (s – x -1 or 0), a quiet version of PLDU
  def int_tvm_plduq :
    GCCBuiltin<"__builtin_tvm_plduq">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // PLDUZ 32(c + 1) (s - s x), preloads the first 32(c + 1) bits of Slice s into an unsigned integer x, for 0 <= c <= 7
  def int_tvm_plduz :
    GCCBuiltin<"__builtin_tvm_plduz">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_i257_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // D71A — LDSLICEXQ (s l – s00 s0 -1 or s 0), a quiet version of LDSLICEX
//  def int_tvm_ldslicexq :
//    GCCBuiltin<"__builtin_tvm_ldslicexq">,
//    Intrinsic<[llvm_TVMSlice_ty, llvm_TVMSlice_ty, llvm_i257_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // D71B — PLDSLICEXQ (s l – s0 -1 or 0), a quiet version of LDSLICEXQ
//  def int_tvm_pldslicexq :
//    GCCBuiltin<"__builtin_tvm_pldsliceq">,
//    Intrinsic<[llvm_TVMSlice_ty, llvm_i257_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // PLDSLICE cc + 1 (s - s00), returns the first 0 < cc + 1 <= 256 bits of s as s00
  def int_tvm_pldslice :
    GCCBuiltin<"__builtin_tvm_pldslice">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // LDSLICEQ cc + 1 (s – s00 s0 -1 or s 0), a quiet version of LDSLICE
  def int_tvm_ldsliceq :
    GCCBuiltin<"__builtin_tvm_ldsliceq">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_TVMSlice_ty, llvm_i257_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // PLDSLICEQ cc + 1 (s – s00 -1 or 0), a quiet version of PLDSLICE
  def int_tvm_pldsliceq :
    GCCBuiltin<"__builtin_tvm_pldsliceq">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_i257_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // SDCUTFIRST (s l – s0), returns the first 0 <= l <= 1023 bits of s. It is equivalent to PLDSLICEX
  def int_tvm_sdcutfirst :
    GCCBuiltin<"__builtin_tvm_sdcutfirst">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // SDSKIPFIRST (s l – s0), returns all but the first 0 <= l <= 1023 bits of s. It is equivalent to LDSLICEX; NIP
  def int_tvm_sdskipfirst :
    GCCBuiltin<"__builtin_tvm_sdskipfirst">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // SDCUTLAST (s l – s0), returns the last 0 <= l <= 1023 bits of s
  def int_tvm_sdcutlast :
    GCCBuiltin<"__builtin_tvm_sdcutlast">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // SDSKIPLAST (s l – s0), returns all but the last 0 <= l <= 1023 bits of s
  def int_tvm_sdskiplast :
    GCCBuiltin<"__builtin_tvm_sdskiplast">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // SDSUBSTR (s l l0 – s0), returns 0 <= l <= 1023 bits of s starting from offset 0 <= l <= 1023, 
  //  thus extracting a bit substring out of the data of s
  def int_tvm_sdsubstr :
    GCCBuiltin<"__builtin_tvm_sdsdsubstr">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty]>;

  // SDBEGINSX (s s0 – s00), checks whether s begins with (the data bits of) s0, and removes s0 from s	on success. 
  //  On failure throws a cell deserialization exception
  def int_tvm_sdbeginsx :
    GCCBuiltin<"__builtin_tvm_sdbeginsx">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_TVMSlice_ty]>;

  // SDBEGINSXQ (s s0 – s00 -1 or s 0), a quiet version of SDBEGINSX
  def int_tvm_sdbeginsxq :
    GCCBuiltin<"__builtin_tvm_sdbeginsxq">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_i257_ty], [llvm_TVMSlice_ty, llvm_TVMSlice_ty]>;

  // SDBEGINS (s – s00), checks whether s begins with constant bitstring sss of length 8x + 3 
  //   (with continuation bit assumed), where 0 <= x <= 127, and removes sss from s on success
  def int_tvm_sdbegins :
    GCCBuiltin<"__builtin_tvm_sdbegins">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // SDBEGINS ‘0’ (s – s00), checks whether s begins with a binary zero
  def int_tvm_sdbegins0 :
    GCCBuiltin<"__builtin_tvm_sdbegins0">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty]>;

  // SDBEGINS ‘1’ (s – s00), checks whether s begins with a binary one
  def int_tvm_sdbegins1 :
    GCCBuiltin<"__builtin_tvm_sdbegins1">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty]>;

  // SDBEGINSQ (s – s00 -1 or s 0), a quiet version of SDBEGINS
  def int_tvm_sdbeginsq :
    GCCBuiltin<"__builtin_tvm_sdbeginsq">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // SCUTFIRST (s l r – s0), returns the first 0 <= l <= 1023 bits and first 0 <= r <= 4 references of s
  def int_tvm_scutfirst :
    GCCBuiltin<"__builtin_tvm_scutfirst">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty]>;

  // SSKIPFIRST (s l r – s0)
  def int_tvm_sskipfirst :
    GCCBuiltin<"__builtin_tvm_sskipfirst">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty]>;

  // SCUTLAST (s l r – s0), returns the last 0 <= l <= 1023 data bits and last 0 <= r <= 4 references of s
  def int_tvm_scutlast :
    GCCBuiltin<"__builtin_tvm_scutlast">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty]>;

  // SSKIPLAST (s l r – s0)
  def int_tvm_sskiplast :
    GCCBuiltin<"__builtin_tvm_sskiplast">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty]>;

  // SUBSLICE (s l r l0 r0 – s0), returns 0 <= l0 <= 1023 bits and 0 <= r0 <= 4 references from Slice s
  def int_tvm_subslice :
    GCCBuiltin<"__builtin_tvm_subslice">,
    Intrinsic<[llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty, llvm_i257_ty, llvm_i257_ty]>;

  // SPLIT (s l r – s0 s00), splits the first 0 <= l <= 1023 data bits and first 0 <= r <= 4 
  //  references from s into s0, returning the remainder of s as s00
  def int_tvm_split :
    GCCBuiltin<"__builtin_tvm_split">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty]>;

  // SPLITQ (s l r – s0 s00 -1 or s 0), a quiet version of SPLIT
  def int_tvm_splitq :
    GCCBuiltin<"__builtin_tvm_splitq">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_TVMSlice_ty, llvm_i257_ty], [llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty]>;

  // XCTOS (c – s ?), transforms an ordinary or exotic cell into a Slice, as if it were an ordinary cell. 
  //  A flag is returned indicating whether c is exotic
  def int_tvm_xctos :
    GCCBuiltin<"__builtin_tvm_xctos">,
    Intrinsic<[llvm_TVMSlice_ty, llvm_i257_ty], [llvm_TVMCell_ty]>;

  // XLOAD (c – c0), loads an exotic cell c and returns an ordinary cell c0. If c is already ordinary, does nothing. 
  //  If c cannot be loaded, throws an exception
  def int_tvm_xload :
    GCCBuiltin<"__builtin_tvm_xload">,
    Intrinsic<[llvm_TVMCell_ty], [llvm_TVMCell_ty]>;

  // XLOADQ (c – c0 -1 or c 0), loads an exotic cell c as XLOAD, but returns 0 on failure
  def int_tvm_xloadq :
    GCCBuiltin<"__builtin_tvm_xloadq">,
    Intrinsic<[llvm_TVMCell_ty, llvm_i257_ty], [llvm_TVMCell_ty]>;

  // SCHKBITS (s l – ), checks whether there are at least l data bits in Slice s. If this is not the case, 
  //  throws a cell deserialisation (i.e., cell underflow) exception
  def int_tvm_schkbits :
    GCCBuiltin<"__builtin_tvm_schkbits">,
    Intrinsic<[], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // SCHKREFS (s r – ), checks whether there are at least r references in Slice s
  def int_tvm_schkrefs :
    GCCBuiltin<"__builtin_tvm_schkrefs">,
    Intrinsic<[], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // SCHKBITREFS (s l r – ), checks whether there are at least l data bits and r references in Slice s
  def int_tvm_schkbitrefs :
    GCCBuiltin<"__builtin_tvm_schkbitrefs">,
    Intrinsic<[], [llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty]>;

  // SCHKBITSQ (s l – ?), checks whether there are at least l data bits in Slice s
  def int_tvm_schkbitsq :
    GCCBuiltin<"__builtin_tvm_schkbitsq">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // SCHKREFSQ (s r – ?), checks whether there are at least r references in Slice s
  def int_tvm_schkrefsq :
    GCCBuiltin<"__builtin_tvm_schkrefsq">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // SCHKBITREFSQ (s l r – ?), checks whether there are at least l data bits and r references in Slice s
  def int_tvm_schkbitrefsq :
    GCCBuiltin<"__builtin_tvm_schkbitrefsq">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty, llvm_i257_ty, llvm_i257_ty]>;

  // PLDREFVAR (s n – c), returns the n-th cell reference of Slice s for 0 <= n <= 3
  def int_tvm_pldrefvar :
    GCCBuiltin<"__builtin_tvm_pldrefvar">,
    Intrinsic<[llvm_TVMCell_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // PLDREFIDX n (s – c), returns the n-th cell reference of Slice s, where 0 <= n <= 3
  def int_tvm_pldrefidx :
    GCCBuiltin<"__builtin_tvm_pldrefidx">,
    Intrinsic<[llvm_TVMCell_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // PLDREF (s – c), preloads the first cell reference of a Slice
  def int_tvm_pldref :
    GCCBuiltin<"__builtin_tvm_pldref">,
    Intrinsic<[llvm_TVMCell_ty], [llvm_TVMSlice_ty]>;

  // LDILE4 (s – x s0), loads a little-endian signed 32-bit integer
  def int_tvm_ldile4 :
    GCCBuiltin<"__builtin_tvm_ldile4">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty]>;

  // LDULE4 (s – x s0), loads a little-endian unsigned 32-bit integer
  def int_tvm_ldule4 :
    GCCBuiltin<"__builtin_tvm_ldule4">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty]>;

  // LDILE8 (s – x s0), loads a little-endian signed 64-bit integer
  def int_tvm_ldile8 :
    GCCBuiltin<"__builtin_tvm_ldile8">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty]>;

  // LDULE8 (s – x s0), loads a little-endian unsigned 64-bit integer
  def int_tvm_ldule8 :
    GCCBuiltin<"__builtin_tvm_ldule8">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty]>;

  // PLDILE4 (s – x), preloads a little-endian signed 32-bit integer
  def int_tvm_pldile4 :
    GCCBuiltin<"__builtin_tvm_pldile4">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty]>;

  // PLDULE4 (s – x), preloads a little-endian unsigned 32-bit integer
  def int_tvm_pldule4 :
    GCCBuiltin<"__builtin_tvm_pldule4">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty]>;

  // PLDILE8 (s – x), preloads a little-endian signed 64-bit integer
  def int_tvm_pldile8 :
    GCCBuiltin<"__builtin_tvm_pldile8">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty]>;

  // PLDULE8 (s – x), preloads a little-endian unsigned 64-bit integer
  def int_tvm_pldule8 :
    GCCBuiltin<"__builtin_tvm_pldule8">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty]>;

  // LDILE4Q (s – x s0 -1 or s 0), quietly loads a little-endian signed 32-bit integer
  def int_tvm_ldile4q :
    GCCBuiltin<"__builtin_tvm_ldile4q">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty, llvm_i257_ty], [llvm_TVMSlice_ty]>;

  // LDULE4Q (s – x s0 -1 or s 0), quietly loads a little-endian unsigned 32-bit integer
  def int_tvm_ldule4q :
    GCCBuiltin<"__builtin_tvm_ldule4q">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty, llvm_i257_ty], [llvm_TVMSlice_ty]>;

  // LDILE8Q (s – x s0 -1 or s 0), quietly loads a little-endian signed 64-bit integer
  def int_tvm_ldile8q :
    GCCBuiltin<"__builtin_tvm_ldile8q">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty, llvm_i257_ty], [llvm_TVMSlice_ty]>;

  // LDULE8Q (s – x s0 -1 or	s 0), quietly loads a little-endian unsigned 64-bit integer
  def int_tvm_ldule8q :
    GCCBuiltin<"__builtin_tvm_ldule8q">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty, llvm_i257_ty], [llvm_TVMSlice_ty]>;

  // PLDILE4Q (s – x -1 or 0), quietly preloads a little-endian signed 32-bit integer
  def int_tvm_pldile4q :
    GCCBuiltin<"__builtin_tvm_pldile4q">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_TVMSlice_ty]>;

  // PLDULE4Q (s – x -1 or	0), quietly preloads a little-endian unsigned 32-bit integer
  def int_tvm_pldule4q :
    GCCBuiltin<"__builtin_tvm_pldule4q">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_TVMSlice_ty]>;

  // PLDILE8Q (s – x -1 or 0), quietly preloads a little-endian signed 64-bit integer
  def int_tvm_pldile8q :
    GCCBuiltin<"__builtin_tvm_pldile8q">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_TVMSlice_ty]>;

  // PLDULE8Q (s –	x -1 or 0), quietly preloads a little-endian unsigned 64-bit integer
  def int_tvm_pldule8q :
    GCCBuiltin<"__builtin_tvm_pldule8q">,
    Intrinsic<[llvm_i257_ty, llvm_i257_ty], [llvm_TVMSlice_ty]>;

  // LDZEROES (s – n s0), returns the count n of leading zero bits in s, and removes these bits from s
  def int_tvm_ldzeroes :
    GCCBuiltin<"__builtin_tvm_ldzeroes">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty]>;

  // LDONES (s – n s0), returns the count n of leading one bits in s, and removes these bits from s
  def int_tvm_ldones :
    GCCBuiltin<"__builtin_tvm_ldones">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty]>;

  // LDSAME (s x – n s0), returns the count	n of leading bits equal to 0 <= x <= 1 in s, and removes these bits from s
  def int_tvm_ldsame :
    GCCBuiltin<"__builtin_tvm_ldsame">,
    Intrinsic<[llvm_i257_ty, llvm_TVMSlice_ty], [llvm_TVMSlice_ty, llvm_i257_ty]>;

  // SDEPTH (s – x), returns the depth of Slice s. If s has no references, then x = 0; 
  //  otherwise x is one plus the maximum of depths of cells referred to from s
  def int_tvm_sdepth :
    GCCBuiltin<"__builtin_tvm_sdepth">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMSlice_ty]>;

  // CDEPTH (c – x), returns the depth of Cell c. If c has no references, then x = 0; 
  //  otherwise x is one plus the maximum of depths of cells referred to from c
  def int_tvm_cdepth :
    GCCBuiltin<"__builtin_tvm_cdepth">,
    Intrinsic<[llvm_i257_ty], [llvm_TVMCell_ty]>;
}
