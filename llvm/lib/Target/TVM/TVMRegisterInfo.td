//===-- TVMRegisterInfo.td - TVM Register defs -------------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the TVM register file
//===----------------------------------------------------------------------===//

class TVMReg<string n> : Register<n> {
  let Namespace = "TVM";
}

class TVMRegClass<list<ValueType> regTypes, int alignment, dag regList>
     : RegisterClass<"TVM", regTypes, alignment, regList>;

//===----------------------------------------------------------------------===//
//  Registers
//===----------------------------------------------------------------------===//

// Special register used as the stack pointer.
def SP : TVMReg<"%SP">;

// The value stack "register". This is an opaque entity which serves to order
// uses and defs that must remain in LIFO order.
def VALUE_STACK : TVMReg<"STACK">;

// The incoming arguments "register". This is an opaque entity which serves to
// order the ARGUMENT instructions that are emulating live-in registers and
// must not be scheduled below other instructions.
def ARGUMENTS : TVMReg<"ARGUMENTS">;

def DUMMY  : TVMReg<"DUMMY">;
def I257 : TVMRegClass<[i257], 257, (add DUMMY, SP)>;

def DUMMY_SLICE : TVMReg<"DUMMY_SLICE">;
def Slice : TVMRegClass<[TVMSlice], 257, (add DUMMY_SLICE)>;

def DUMMY_BUILDER : TVMReg<"DUMMY_BUILDER">;
def Builder : TVMRegClass<[TVMBuilder], 257, (add DUMMY_BUILDER)>;

def DUMMY_CELL : TVMReg<"DUMMY_CELL">;
def Cell : TVMRegClass<[TVMCell], 257, (add DUMMY_CELL)>;

def DUMMY_TUPLE : TVMReg<"DUMMY_TUPLE">;
def Tuple : TVMRegClass<[TVMTuple], 257, (add DUMMY_TUPLE)>;
