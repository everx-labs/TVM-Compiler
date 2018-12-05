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

def DUMMY  : TVMReg<"DUMMY">;
def DUMMYRESERVED  : TVMReg<"DRES">;
def I64 : TVMRegClass<[i64], 64, (add DUMMY, DUMMYRESERVED)>;
