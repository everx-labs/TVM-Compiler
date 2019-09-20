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
    defm RETURN_I257 : NRI<(outs), (ins i257imm:$val), [(TVMreturn i257:$val)],
                          "RET", 0xdb30>;
    defm RETURN_SLICE : NRI<(outs), (ins Slice:$val), [(TVMreturn Slice:$val)],
                           "RET", 0xdb30>;
    defm RETURN_BUILDER : NRI<(outs), (ins Builder:$val),
                              [(TVMreturn Builder:$val)], "RET", 0xdb30>;
    defm RETURN_CELL : NRI<(outs), (ins Cell:$val), [(TVMreturn Cell:$val)],
                          "RET", 0xdb30>;

    defm RETURN_VOID : NRI<(outs), (ins), [(TVMreturn)], "RET", 0xdb30>;
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
  defm CALL_VOID : NRI<(outs), (ins variable_ops, i257imm:$callee),
                       [(TVMcall0 i257:$callee)], "CALL\t1", 0xF0>;
  defm CALL_1 : NRI<(outs I257:$dst), (ins variable_ops, i257imm:$callee),
                    [(set I257:$dst, (TVMcall1 i257:$callee))],
                    "CALL\t1", 0xF0>;
  defm CALL_1_SLICE : NRI<(outs Slice:$dst),
                          (ins variable_ops, i257imm:$callee),
                          [(set Slice:$dst, (TVMcall1 i257:$callee))],
                          "CALL\t1", 0xF0>;
  defm CALL_1_BUILDER : NRI<(outs Builder:$dst),
                            (ins variable_ops, i257imm:$callee),
                            [(set Builder:$dst, (TVMcall1 i257:$callee))],
                            "CALL\t1", 0xF0>;
  defm CALL_1_CELL : NRI<(outs Cell:$dst), (ins variable_ops, i257imm:$callee),
                         [(set Cell:$dst, (TVMcall1 i257:$callee))],
                         "CALL\t1", 0xF0>;
}

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

defm THROW : I<(outs), (ins uimm6 : $exception),
               (outs), (ins uimm6 : $exception),
               [(int_tvm_throw uimm6 : $exception)],
               "THROW\t$exception", "THROW\t$exception", 0xf22>;

defm THROWIF : I<(outs), (ins I257 : $cond, uimm6 : $exception),
                 (outs), (ins uimm6 : $exception),
                 [(int_tvm_throwif I257 : $cond, uimm6 : $exception)],
                 "THROWIF\t$exception, $cond", "THROWIF\t$exception", 0xf26>;

defm THROWIFNOT : I<(outs), (ins I257 : $cond, uimm6 : $exception),
                    (outs), (ins uimm6 : $exception),
                    [(int_tvm_throwif (not I257 : $cond), uimm6 : $exception)],
                    "THROWIFNOT\t$exception, $cond",
                    "THROWIFNOT\t$exception", 0xf2a>;

// Load and store subroutine calls
defm CALL_LOAD : I<(outs I257 : $value), (ins I257 : $addr),
                   (outs), (ins),
                   [], "CALL_LOAD\t$addr", "CALL\t$$:load$$", 0xF0>;
def : Pat<(load I257 : $addr), (CALL_LOAD I257 : $addr)>;
def : Pat<(zextload I257 : $addr), (CALL_LOAD I257 : $addr)>;
def : Pat<(sextload I257 : $addr), (CALL_LOAD I257 : $addr)>;

defm CALL_STORE : I<(outs), (ins I257 : $addr, I257 : $value),
                    (outs), (ins),
                    [], "CALL_STORE\t$addr, $value", "CALL\t$$:store$$", 0xF0>;
def : Pat<(store I257 : $value, I257 : $addr),
          (CALL_STORE I257 : $addr, I257 : $value)>;

def : Pat<(truncstore I257 : $value, I257 : $addr),
          (CALL_STORE I257 : $addr, I257 : $value)>;

defm CALL_FRAMEIDX : I<(outs I257 : $offset), (ins I257 : $idx),
                       (outs), (ins), [],
                       "CALL_FRAMEIDX\t$idx", "CALL\t$$:frameidx$$", 0xF0>;

defm CALL_ENTER : I<(outs), (ins I257 : $framesize), (outs), (ins),
                    [], "CALL_ENTER\t$framesize", "CALL\t$$:enter$$", 0xF0>;

defm CALL_LEAVE : I<(outs), (ins I257 : $framesize), (outs), (ins),
                    [], "CALL_LEAVE\t$framesize", "CALL\t$$:leave$$", 0xF0>;

defm PUSH_GLOBAL_ADDRESS : I<(outs I257 : $res), (ins I257 : $in),
                             (outs), (ins I257 : $in),
                             [], "PUSHINT\t$res, $in", "PUSHINT\t$in", 0x82>;

def : Pat<(i257 (TVMGlobalAddressWrapper tglobaladdr : $addr)),
          (PUSH_GLOBAL_ADDRESS tglobaladdr : $addr)>;
def : Pat<(i257 (TVMGlobalAddressWrapper texternalsym : $addr)),
          (PUSH_GLOBAL_ADDRESS texternalsym : $addr)>;
