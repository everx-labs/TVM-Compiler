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
//
//===----------------------------------------------------------------------===//

let isTerminator = 1, hasCtrlDep = 1 in {
  let isReturn = 1, isBarrier = 1 in {
    defm RETURN_I64 : NRI<(outs), (ins i64imm:$val), [(TVMreturn i64:$val)],
                          "RET", 0xdb30>;
    defm RETURN_VOID : NRI<(outs), (ins), [(TVMreturn)], "RET", 0xdb30>;
    let isCodeGenOnly = 1 in
    defm FALLTHROUGH_RETURN : SI<(ins)>;
  } // isReturn = 1, isBarrier = 1

  defm ISZERO : I<(outs I64:$dst), (ins I64:$src), (outs), (ins),
                   [(set I64:$dst, (setcc I64:$src, 0, SETEQ))],
                   "ISZERO\t$dst, $src", "ISZERO", 0xc000>;

  let isBranch = 1 in {
    // The condition operand is a boolean value which TVM represents as i64.
    defm IFJMP : I<(outs), (ins I64:$cond, bb_op:$dst),
                   (outs), (ins),
                   [(brcond I64:$cond, bb:$dst)],
                    "IFJMP\t$dst, $cond", "IFJMP", 0xe0>;

    let isBarrier = 1 in {
      defm JMPX  : I<(outs), (ins bb_op:$dst),
                     (outs), (ins),
                     [(br bb:$dst)],
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
  defm CALL_VOID : NRI<(outs), (ins i64imm:$callee, variable_ops),
                       [(TVMcall0 i64:$callee)], "CALL\t1", 0xF0>;
  defm CALL_1 : NRI<(outs I64:$dst), (ins i64imm:$callee, variable_ops),
                    [(set I64:$dst, (TVMcall1 i64:$callee))],
                    "CALL\t1", 0xF0>;
}

let isMoveImm = 1, isAsCheapAsAMove = 1, isReMaterializable = 1 in {
defm CONST_I64 : I<(outs I64:$res), (ins i64imm:$imm),
                   (outs), (ins i64imm:$imm),
                   [(set I64:$res, imm:$imm)],
                   "PUSHINT\t$res, $imm", "PUSHINT\t$imm", 0x82>;
}

defm PUSHCONT_MBB : I<(outs), (ins bb_op:$bb), (outs), (ins bb_op:$bb), 
                      [], "PUSHCONT", "PUSHCONT", 0x8f>;
defm PUSHCONT_FUNC : NRI<(outs), (ins function_op:$callee), [],
                         "PUSHCONT\t$callee", 0x8f>;

// There are no labels in TVM, so this functionality is emulated
// using ```PUSHINT label; CALL 1```
defm PUSHCONT_LABEL: NRI<(outs), (ins function_op:$callee), [],
                     "PUSHINT\t$callee", 0x82>;

defm PUSHC : I<(outs I64 : $root), (ins uimm8 : $regno),
               (outs), (ins uimm8 : $regno),
               [(set I64 : $root, (int_tvm_getreg uimm8 : $regno))],
               "PUSH\tc$regno", "PUSH\tc$regno", 0xed4>;

def : Pat<(TVMpushroot), (PUSHC (i64 4))>;

// TODO: Generalize to POP ci
defm POPROOT : I<(outs), (ins I64 : $root),
                 (outs), (ins),
                 [(int_tvm_set_persistent_data I64 : $root)],
                 "POPROOT\t$root", "POPROOT", 0xed54>;

defm THROWIF : I<(outs), (ins I64 : $cond, uimm6 : $exception),
                 (outs), (ins uimm6 : $exception),
                 [(int_tvm_throwif I64 : $cond, uimm6 : $exception)],
                 "THROWIF\t$exception, $cond", "THROWIF\t$exception", 0xf26>;

defm THROWIFNOT : I<(outs), (ins I64 : $cond, uimm6 : $exception),
                    (outs), (ins uimm6 : $exception),
                    [(int_tvm_throwif (not I64 : $cond), uimm6 : $exception)],
                    "THROWIFNOT\t$cond", "THROWIFNOT", 0xf2a>;

// Load and store subroutine calls
defm CALL_LOAD : I<(outs I64 : $value), (ins I64 : $addr),
                   (outs), (ins), [], "CALL_LOAD\t$addr", "CALL\t3", 0xF0>;
def : Pat<(load I64 : $addr), (CALL_LOAD I64 : $addr)>;

defm CALL_STORE : I<(outs), (ins I64 : $addr, I64 : $value),
                    (outs), (ins), [], "CALL_STORE\t$addr, $value", "CALL\t4", 0xF0>;
def : Pat<(store I64 : $value, I64 : $addr), (CALL_STORE I64 : $addr, I64 : $value)>;

