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
    Intrinsic<[llvm_i257_ty], [llvm_i257_ty], []>;
  def int_tvm_setglobal :
    GCCBuiltin<"__builtin_tvm_setglobal">,
    Intrinsic<[], [llvm_i257_ty, llvm_i257_ty], []>;

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
}
