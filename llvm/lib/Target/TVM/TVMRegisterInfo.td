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

class TVMReg<bits<4> num, string n, list<string> alt = []> : Register<n> {
  field bits<4> Num = num;
  let Namespace = "TVM";
}

//===----------------------------------------------------------------------===//
//  Registers
//===----------------------------------------------------------------------===//

def DUMMY  : TVMReg<0,  "r0">;
def GR : RegisterClass<"TVM", [i64], 64,
  (add DUMMY)>;
