//===-- TVMControlFlowInstrInfo.td - TVM Instruction defs -*- tablegen --*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
/// \file
/// This file describes Control Flow TVM instructions (A.7) in TableGen format.
/// Control flow in TVM is different from conventional one:
/// - there is no function scope, the is only nested continuations.
/// - jumps are more similar to call or invoke, after a continuation jump on
///   is executed, the control flow returns back to 'caller' BB.
/// - there are loops in ISA, not only for loops with fixed number
///   of iterations.
//
//===----------------------------------------------------------------------===//

let isTerminator = 1, hasCtrlDep = 1 in {
  let isReturn = 1, isBarrier = 1 in {
    defm RETURN_N : NRI<(outs), (ins variable_ops), [(TVMreturn)],
                        "RET", 0xdb30>;
    let isCodeGenOnly = 1 in
    defm FALLTHROUGH_RETURN : SI<(ins)>;
  } // isReturn = 1, isBarrier = 1

  let isBranch = 1 in {
    // The condition operand is a boolean value which TVM represents as i257.
    defm IFJMP : I<(outs), (ins I257:$cond, I257:$dst),
                   (outs), (ins),
                   [(TVMifjmp I257:$cond, I257:$dst)],
                    "IFJMP\t$dst, $cond", "IFJMP", 0xe0>;

    let isBarrier = 1 in {
    defm IFELSE : I<(outs), (ins I257:$cond, I257:$branch1, I257:$branch2),
                   (outs), (ins),
                   [(TVMifelse I257:$cond, I257:$branch1, I257:$branch2)],
                    "IFELSE\t$branch1, $branch2, $cond", "IFELSE", 0xe2>;

    defm IF : I<(outs), (ins I257:$cond, I257:$dst),
                (outs), (ins),
                [(TVMif I257:$cond, I257:$dst)],
                "IF\t$dst, $cond", "IF", 0xde>;
    defm IFNOT : I<(outs), (ins I257:$cond, I257:$dst),
                   (outs), (ins),
                   [(TVMif (not I257:$cond), I257:$dst)],
                   "IFNOT\t$dst, $cond", "IFNOT", 0xdf>;
    } // isBarrier = 1

    let isBarrier = 1 in {
      defm JMPX  : I<(outs), (ins I257:$dst),
                     (outs), (ins),
                     [(TVMjumpx i257:$dst)],
                     "JMPX\t$dst", "JMPX", 0xd9>;
    } // isBarrier = 1
  } // isBranch = 1
} // isTerminator = 1, hasCtrlDep = 1

// TODO: The approach with two CALLs was copied from WebAssembly backend.
// We need to investigate how it works and why 2 CALLs are needed
//
// TODO: If a function is called in many places, we may cosider a special
// "short" library variant, so that some common functions would not get
// long call: "PUSHINT x; CALL 1", but a short one: "CALL x"
let isCall = 1, hasCtrlDep = 1 in {
  defm CALL_VOID : I<(outs), (ins function_op:$callee, variable_ops),
                     (outs), (ins function_op:$callee),
                     [(TVMcall0 (TVMGlobalAddressWrapper(tglobaladdr:$callee)))],
                     "CALL\t$callee", "CALL\t$callee", 0xF0>;

  multiclass CALL_1<TVMRegClass OutReg> :
    I<(outs OutReg:$dst), (ins function_op:$callee, variable_ops),
      (outs), (ins function_op:$callee),
      [(set OutReg:$dst, (TVMcall1 (TVMGlobalAddressWrapper(tglobaladdr:$callee))))],
      "CALL\t$callee", "CALL\t$callee", 0xF0>;

  defm CALL_1_INT : CALL_1<I257>;
  defm CALL_1_SLICE : CALL_1<Slice>;
  defm CALL_1_BUILDER : CALL_1<Builder>;
  defm CALL_1_CELL : CALL_1<Cell>;
  defm CALL_1_TUPLE : CALL_1<Tuple>;

  defm CALL_N : I<(outs reglist:$regs, variable_ops),
                  (ins function_op:$callee, variable_ops),
                  (outs), (ins function_op:$callee),
                  [],
                  "CALL\t$callee", "CALL\t$callee", 0xF0>;
}

// Patterns for matching a direct call to an external symbol.
def : Pat<(i257 (TVMcall1 (TVMGlobalAddressWrapper texternalsym:$callee))),
          (CALL_1_INT texternalsym:$callee)>;
def : Pat<(TVMSlice (TVMcall1 (TVMGlobalAddressWrapper texternalsym:$callee))),
          (CALL_1_SLICE texternalsym:$callee)>;
def : Pat<(TVMBuilder (TVMcall1 (TVMGlobalAddressWrapper texternalsym:$callee))),
          (CALL_1_BUILDER texternalsym:$callee)>;
def : Pat<(TVMCell (TVMcall1 (TVMGlobalAddressWrapper texternalsym:$callee))),
          (CALL_1_CELL texternalsym:$callee)>;
def : Pat<(TVMTuple (TVMcall1 (TVMGlobalAddressWrapper texternalsym:$callee))),
          (CALL_1_TUPLE texternalsym:$callee)>;
def : Pat<(TVMcall0 (TVMGlobalAddressWrapper texternalsym:$callee)),
          (CALL_VOID texternalsym:$callee)>;

let isMoveImm = 1, isAsCheapAsAMove = 1, isReMaterializable = 1 in {
defm CONST_I257 : I<(outs I257:$res), (ins i257imm:$imm),
                    (outs), (ins i257imm:$imm),
                    [(set I257:$res, imm:$imm)],
                    "PUSHINT\t$res, $imm", "PUSHINT\t$imm", 0x82>;

defm CONST_U257 : I<(outs I257:$res), (ins UImm257:$uimm),
                    (outs), (ins UImm257:$uimm),
                    [(set I257:$res, (TVMconst_u257 timm:$uimm))],
                    "PUSHINT\t$res, $uimm", "PUSHINT\t$uimm", 0x82>;
}

let isMoveImm = 1, AddedComplexity = 99 in
defm PUSHCONT_MBB : I<(outs I257:$res), (ins bb_op:$bb, i257imm:$fake_op),
                      (outs), (ins bb_op:$bb),
                      [(set I257:$res, (BBWrapper bb:$bb, timm:$fake_op))],
                      "PUSHCONT", "PUSHCONT", 0x8f>;

defm PUSHCONT_FUNC : NRI<(outs), (ins function_op:$callee), [],
                         "PUSHCONT\t$callee", 0x8f>;

// There are no labels in TVM, so this functionality is emulated
// using ```PUSHINT label; CALL 1```
defm PUSHCONT_LABEL: NRI<(outs), (ins function_op:$callee), [],
                     "PUSHINT\t$callee", 0x82>;

defm PUSHC : I<(outs I257 : $root), (ins uimm8 : $regno),
               (outs), (ins uimm8 : $regno),
               [(set I257 : $root, (int_tvm_getreg uimm8 : $regno))],
               "PUSH\tc$regno", "PUSH\tc$regno", 0xed4>;

defm PUSHROOT : I<(outs Cell:$root), (ins),
               (outs), (ins),
               [(set Cell:$root, (TVMpushroot))],
               "PUSHROOT", "PUSHROOT", 0xed44>;

defm POPC : I<(outs), (ins I257 : $root, uimm8 : $regno),
              (outs), (ins uimm8 : $regno),
              [(int_tvm_setreg uimm8 : $regno, I257 : $root)],
              "POP\tc$regno, $root", "POP\tc$regno", 0xed5>;

// TODO: Generalize to POP ci
defm POPROOT : I<(outs), (ins Cell:$root),
                 (outs), (ins),
                 [(int_tvm_set_persistent_data Cell:$root)],
                 "POPROOT\t$root", "POPROOT", 0xed54>;

defm THROW : I<(outs), (ins uimm11:$exception),
               (outs), (ins uimm11:$exception),
               [(int_tvm_throw uimm11:$exception)],
               "THROW\t$exception", "THROW\t$exception", 0xf22>;

defm THROWIF : I<(outs), (ins I257:$cond, uimm11:$exception),
                 (outs), (ins uimm11:$exception),
                 [(int_tvm_throwif I257:$cond, uimm11:$exception)],
                 "THROWIF\t$exception, $cond", "THROWIF\t$exception", 0xf26>;

defm THROWIFNOT : I<(outs), (ins I257:$cond, uimm11:$exception),
                    (outs), (ins uimm11:$exception),
                    [(int_tvm_throwif (not I257:$cond), uimm11:$exception)],
                    "THROWIFNOT\t$exception, $cond",
                    "THROWIFNOT\t$exception", 0xf2a>;

// Load and store subroutine calls
multiclass CALL_LOAD<TVMRegClass RegClass> :
    I<(outs RegClass : $value), (ins I257 : $addr),
      (outs), (ins),
      [], "CALL_LOAD\t$addr", "CALL\t$$:load$$", 0xF0>;

defm CALL_LOAD_INT     : CALL_LOAD<I257>;
defm CALL_LOAD_BUILDER : CALL_LOAD<Builder>;
defm CALL_LOAD_SLICE   : CALL_LOAD<Slice>;

def : Pat<(load I257 : $addr), (CALL_LOAD_INT I257 : $addr)>;
def : Pat<(load I257 : $addr), (CALL_LOAD_BUILDER I257 : $addr)>;
def : Pat<(load I257 : $addr), (CALL_LOAD_SLICE I257 : $addr)>;

def : Pat<(zextload I257 : $addr), (CALL_LOAD_INT I257 : $addr)>;
def : Pat<(sextload I257 : $addr), (CALL_LOAD_INT I257 : $addr)>;

multiclass CALL_STORE<TVMRegClass RegClass> :
    I<(outs), (ins I257 : $addr, RegClass : $value),
      (outs), (ins),
      [], "CALL_STORE\t$addr, $value", "CALL\t$$:store$$", 0xF0>;

defm CALL_STORE_INT     : CALL_STORE<I257>;
defm CALL_STORE_BUILDER : CALL_STORE<Builder>;
defm CALL_STORE_SLICE   : CALL_STORE<Slice>;

def : Pat<(store I257 : $value, I257 : $addr),
          (CALL_STORE_INT I257 : $addr, I257 : $value)>;
def : Pat<(store Builder : $value, I257 : $addr),
          (CALL_STORE_BUILDER I257 : $addr, Builder : $value)>;
def : Pat<(store Slice : $value, I257 : $addr),
          (CALL_STORE_SLICE I257 : $addr, Slice : $value)>;

def : Pat<(truncstore I257 : $value, I257 : $addr),
          (CALL_STORE_INT I257 : $addr, I257 : $value)>;

defm PUSH_GLOBAL_ADDRESS : I<(outs I257 : $res), (ins I257 : $in),
                             (outs), (ins I257 : $in),
                             [], "PUSHINT\t$res, $in", "PUSHINT\t$in", 0x82>;

def : Pat<(i257 (TVMGlobalAddressWrapper tglobaladdr : $addr)),
          (PUSH_GLOBAL_ADDRESS tglobaladdr : $addr)>;
def : Pat<(i257 (TVMGlobalAddressWrapper texternalsym : $addr)),
          (PUSH_GLOBAL_ADDRESS texternalsym : $addr)>;
