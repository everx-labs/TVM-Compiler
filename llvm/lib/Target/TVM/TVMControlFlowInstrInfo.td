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

    defm IFNOTJMP : I<(outs), (ins I257:$cond, I257:$dst),
                      (outs), (ins), [],
                      "IFNOTJMP\t$dst, $cond", "IFNOTJMP", 0xe1>;

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
                     "CALLREF {{\n\t      CALL $callee\n\t    }}",
                     "CALLREF {{\n\t      CALL $callee\n\t    }}", 0xDB3C>;

  multiclass CALL_1<TVMRegClass OutReg> :
    I<(outs OutReg:$dst), (ins function_op:$callee, variable_ops),
      (outs), (ins function_op:$callee),
      [(set OutReg:$dst, (TVMcall1 (TVMGlobalAddressWrapper(tglobaladdr:$callee))))],
      "CALLREF {{\n\t      CALL $callee\n\t    }}",
      "CALLREF {{\n\t      CALL $callee\n\t    }}", 0xDB3C>;

  defm CALL_1_INT : CALL_1<I257>;
  defm CALL_1_SLICE : CALL_1<Slice>;
  defm CALL_1_BUILDER : CALL_1<Builder>;
  defm CALL_1_CELL : CALL_1<Cell>;
  defm CALL_1_TUPLE : CALL_1<Tuple>;

  defm CALL_N : I<(outs reglist:$regs, variable_ops),
                  (ins function_op:$callee, variable_ops),
                  (outs), (ins function_op:$callee),
                  [],
                  "CALLREF {{\n\t      CALL $callee\n\t    }}",
                  "CALLREF {{\n\t      CALL $callee\n\t    }}", 0xDB3C>;

  defm CALLDICT_VOID
    : I<(outs), (ins function_op:$callee, variable_ops),
        (outs), (ins function_op:$callee),
        [(TVMcalldict0 (TVMGlobalAddressWrapper(tglobaladdr:$callee)))],
        "CALL\t$callee", "CALL\t$callee", 0xF0>;

  multiclass CALLDICT_1<TVMRegClass OutReg> :
    I<(outs OutReg:$dst), (ins function_op:$callee, variable_ops),
      (outs), (ins function_op:$callee),
      [(set OutReg:$dst, (TVMcalldict1 (TVMGlobalAddressWrapper(tglobaladdr:$callee))))],
      "CALL\t$callee", "CALL\t$callee", 0xF0>;

  defm CALLDICT_1_INT : CALLDICT_1<I257>;
  defm CALLDICT_1_SLICE : CALLDICT_1<Slice>;
  defm CALLDICT_1_BUILDER : CALLDICT_1<Builder>;
  defm CALLDICT_1_CELL : CALLDICT_1<Cell>;
  defm CALLDICT_1_TUPLE : CALLDICT_1<Tuple>;

  defm CALLDICT_N : I<(outs reglist:$regs, variable_ops),
                      (ins function_op:$callee, variable_ops),
                      (outs), (ins function_op:$callee),
                      [],
                      "CALL\t$callee", "CALL\t$callee", 0xF0>;
}

// Patterns for matching a direct call to an external symbol.
def : Pat<(i257 (TVMcalldict1 (TVMGlobalAddressWrapper texternalsym:$callee))),
          (CALLDICT_1_INT texternalsym:$callee)>;
def : Pat<(TVMSlice (TVMcalldict1 (TVMGlobalAddressWrapper texternalsym:$callee))),
          (CALLDICT_1_SLICE texternalsym:$callee)>;
def : Pat<(TVMBuilder (TVMcalldict1 (TVMGlobalAddressWrapper texternalsym:$callee))),
          (CALLDICT_1_BUILDER texternalsym:$callee)>;
def : Pat<(TVMCell (TVMcalldict1 (TVMGlobalAddressWrapper texternalsym:$callee))),
          (CALLDICT_1_CELL texternalsym:$callee)>;
def : Pat<(TVMTuple (TVMcalldict1 (TVMGlobalAddressWrapper texternalsym:$callee))),
          (CALLDICT_1_TUPLE texternalsym:$callee)>;
def : Pat<(TVMcalldict0 (TVMGlobalAddressWrapper texternalsym:$callee)),
          (CALLDICT_VOID texternalsym:$callee)>;

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

defm PUSHROOT : I<(outs Cell:$root), (ins), (outs), (ins),
                  [(set Cell:$root, (int_tvm_get_persistent_data))],
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

let isPredicable = 1, isTerminator = 1, isBarrier = 1, hasCtrlDep = 1,
    hasSideEffects = 1 in
defm THROW : I<(outs), (ins uimm11:$exception),
               (outs), (ins uimm11:$exception),
               [(int_tvm_throw uimm11:$exception)],
               "THROW\t$exception", "THROW\t$exception", 0xf22>;

let hasCtrlDep = 1, hasSideEffects = 1 in {
defm THROWIF : I<(outs), (ins I257:$cond, uimm11:$exception),
                 (outs), (ins uimm11:$exception),
                 [],
                 "THROWIF\t$exception, $cond", "THROWIF\t$exception", 0xf26>;

defm THROWIFNOT : I<(outs), (ins I257:$cond, uimm11:$exception),
                    (outs), (ins uimm11:$exception),
                    [],
                    "THROWIFNOT\t$exception, $cond",
                    "THROWIFNOT\t$exception", 0xf2a>;
}

let isTerminator = 1, isBarrier = 1, hasCtrlDep = 1,
    hasSideEffects = 1 in
defm THROWANY : I<(outs), (ins I257:$exception),
                  (outs), (ins),
                  [(int_tvm_throw I257:$exception)],
                  "THROWANY\t$exception", "THROWANY", 0xf2f0>;

// Load and store subroutine calls
multiclass CALL_LOAD<TVMRegClass RegClass, string suffix> :
    I<(outs RegClass : $value), (ins I257 : $addr),
      (outs), (ins),
      [], "CALL_LOAD\t$addr", "CALL\t$$:load" # suffix # "$$", 0xF0>;

defm CALL_LOAD_INT : I<(outs I257 : $value), (ins I257 : $addr),
      (outs), (ins),
      [], "CALL_LOAD\t$addr", "GETGLOB 13 CALLX", 0xF0>;

defm CALL_LOAD_BUILDER : CALL_LOAD<Builder, "_builder">;
defm CALL_LOAD_SLICE   : CALL_LOAD<Slice, "_slice">;
defm CALL_LOAD_CELL    : CALL_LOAD<Cell, "_cell">;

def : Pat<(load I257 : $addr), (CALL_LOAD_INT I257 : $addr)>;
def : Pat<(load I257 : $addr), (CALL_LOAD_BUILDER I257 : $addr)>;
def : Pat<(load I257 : $addr), (CALL_LOAD_SLICE I257 : $addr)>;
def : Pat<(load I257 : $addr), (CALL_LOAD_CELL I257 : $addr)>;

def : Pat<(zextload I257 : $addr), (CALL_LOAD_INT I257 : $addr)>;
def : Pat<(sextload I257 : $addr), (CALL_LOAD_INT I257 : $addr)>;

multiclass CALL_STORE<TVMRegClass RegClass, string suffix> :
    I<(outs), (ins I257 : $addr, RegClass : $value),
      (outs), (ins),
      [], "CALL_STORE\t$addr, $value", "CALL\t$$:store" # suffix # "$$", 0xF0>;

defm CALL_STORE_INT : I<(outs), (ins I257 : $addr, I257 : $value),
      (outs), (ins),
      [], "CALL_STORE\t$addr, $value", "GETGLOB 14 CALLX", 0xF0>;

defm CALL_STORE_BUILDER : CALL_STORE<Builder, "_builder">;
defm CALL_STORE_SLICE   : CALL_STORE<Slice, "_slice">;
defm CALL_STORE_CELL    : CALL_STORE<Cell, "_cell">;

def : Pat<(store I257 : $value, I257 : $addr),
          (CALL_STORE_INT I257 : $addr, I257 : $value)>;
def : Pat<(store Builder : $value, I257 : $addr),
          (CALL_STORE_BUILDER I257 : $addr, Builder : $value)>;
def : Pat<(store Slice : $value, I257 : $addr),
          (CALL_STORE_SLICE I257 : $addr, Slice : $value)>;
def : Pat<(store Cell : $value, I257 : $addr),
          (CALL_STORE_CELL I257 : $addr, Cell : $value)>;

def : Pat<(truncstore I257 : $value, I257 : $addr),
          (CALL_STORE_INT I257 : $addr, I257 : $value)>;

defm PUSH_GLOBAL_ADDRESS : I<(outs I257 : $res), (ins I257 : $in),
                             (outs), (ins I257 : $in),
                             [], "PUSHINT\t$res, $in", "PUSHINT\t$in", 0x82>;

def : Pat<(i257 (TVMGlobalAddressWrapper tglobaladdr : $addr)),
          (PUSH_GLOBAL_ADDRESS tglobaladdr : $addr)>;
def : Pat<(i257 (TVMGlobalAddressWrapper texternalsym : $addr)),
          (PUSH_GLOBAL_ADDRESS texternalsym : $addr)>;

defm BLESS : I<(outs I257:$cont), (ins Slice:$sl),
               (outs), (ins),
               [(set I257:$cont, (int_tvm_bless Slice:$sl))],
               "BLESS\t$cont, $sl", "BLESS", 0xed1e>;
