//==- TVMCallingConv.td - Calling Conventions for TVM -*- tablegen -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This describes the calling conventions for TVM architecture.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// TVM Return Value Calling Convention
//===----------------------------------------------------------------------===//
def RetCC_TVM : CallingConv<[

  // TVM only has stack
  CCIfType<[i8],  CCAssignToStack<1, 8>>,
  CCIfType<[i16], CCAssignToStack<2, 8>>,
  CCIfType<[i32], CCAssignToStack<4, 8>>,
  CCIfType<[i64], CCAssignToStack<8, 8>>

]>;

//===----------------------------------------------------------------------===//
// TVM Argument Calling Conventions
//===----------------------------------------------------------------------===//
def CC_TVM_AssignStack : CallingConv<[

  // TVM only has stack
  // TODO: Consder CCPromoteToType
  CCIfType<[i8],  CCAssignToStack<1, 8>>,
  CCIfType<[i16], CCAssignToStack<2, 8>>,
  CCIfType<[i32], CCAssignToStack<4, 8>>,
  CCIfType<[i64], CCAssignToStack<8, 8>>

]>;

