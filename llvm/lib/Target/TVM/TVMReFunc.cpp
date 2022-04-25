//===--- TVMReFunc.cpp - combining common expressions into functions ------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
///  Combining common expressions into functions
///
//===----------------------------------------------------------------------===//

#include <vector>
#include <variant>
#include <numeric>
#include <sstream>

#include "TVM.h"
#include "TVMExtras.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/IntrinsicsTVM.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Operator.h"
#include "llvm/Pass.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
using namespace llvm;

#define DEBUG_TYPE "tvm-refunc"

static cl::opt<bool> TVMEnableMReFunc("tvm-re-func", cl::NotHidden,
                                      cl::desc("Enable tvm re-func pass."),
                                      cl::init(false));

template<class Stream, class T0, class ... Ts>
Stream &operator<< (Stream &s,
                    std::variant<T0, Ts...> const &v)
{ std::visit([&](auto && arg){ s << arg;}, v); return s; }

template<typename T>
inline void *operator new(size_t, SpecificBumpPtrAllocator<T> &A) {
  return A.Allocate();
}

namespace {
class TVMReFunc final : public ModulePass {
  StringRef getPassName() const override {
    return "Combine common expressions into functions";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {}

  bool runOnModule(Module &M) override;
  bool processSendRawMsgUses(Function &F);

public:
  static char ID;
  explicit TVMReFunc() : ModulePass(ID) {}
};

namespace emulated {

struct value;

struct stu {
  template<class OStream>
  friend OStream &operator << (OStream &os, const stu &val) {
    os << "stu(" << *val.val << ", " << val.bitsize << ")";
    return os;
  }
  bool equal_nonrecursive(stu v) const {
    return bitsize == v.bitsize;
  }
  value *val;
  unsigned bitsize;
};
struct sti {
  template<class OStream>
  friend OStream &operator << (OStream &os, const sti &val) {
    os << "sti(" << *val.val << ", " << val.bitsize << ")";
    return os;
  }
  bool equal_nonrecursive(sti v) const {
    return bitsize == v.bitsize;
  }
  value *val;
  unsigned bitsize;
};
struct stref {
  template<class OStream>
  friend OStream &operator << (OStream &os, const stref &val) {
    os << "stref(" << *val.val << ")";
    return os;
  }
  bool equal_nonrecursive(stref) const { return true; }
  value *val;
};
struct stslice {
  template<class OStream>
  friend OStream &operator << (OStream &os, const stslice &val) {
    os << "stslice(" << *val.val << ")";
    return os;
  }
  bool equal_nonrecursive(stslice) const { return true; }
  value *val;
};
struct stvaruint16 {
  template<class OStream>
  friend OStream &operator << (OStream &os, const stvaruint16 &val) {
    os << "stvaruint16(" << *val.val << ")";
    return os;
  }
  bool equal_nonrecursive(stvaruint16) const { return true; }
  value *val;
};
struct stvaruint32 {
  template<class OStream>
  friend OStream &operator << (OStream &os, const stvaruint32 &val) {
    os << "stvaruint32(" << *val.val << ")";
    return os;
  }
  bool equal_nonrecursive(stvaruint32) const { return true; }
  value *val;
};
struct stvarint16 {
  template<class OStream>
  friend OStream &operator << (OStream &os, const stvarint16 &val) {
    os << "stvarint16(" << *val.val << ")";
    return os;
  }
  bool equal_nonrecursive(stvarint16) const { return true; }
  value *val;
};
struct stvarint32 {
  template<class OStream>
  friend OStream &operator << (OStream &os, const stvarint32 &val) {
    os << "stvarint32(" << *val.val << ")";
    return os;
  }
  bool equal_nonrecursive(stvarint32) const { return true; }
  value *val;
};
struct stdict {
  template<class OStream>
  friend OStream &operator << (OStream &os, const stdict &val) {
    os << "stdict(" << *val.val << ")";
    return os;
  }
  bool equal_nonrecursive(stdict) const { return true; }
  value *val;
};

using chunk =
  std::variant<stu, sti, stref, stslice, stvaruint16, stvaruint32,
               stvarint16, stvarint32, stdict>;

struct builder {
  template<class OStream>
  friend OStream &operator << (OStream &os, const builder &val) {
    os << "|newc()";
    for (auto ch : val.chunks) {
      os << "." << ch;
    }
    os << "|";
    return os;
  }
  void add_chunk(chunk ch) {
    chunks.push_back(ch);
  }
  void add_from_builder(builder b) {
    chunks.append(b.chunks.begin(), b.chunks.end());
  }
  size_t len() const;
  SmallVector<chunk, 8> chunks;
};

struct cell {
  template<class OStream>
  friend OStream &operator << (OStream &os, const cell &val) {
    os << val.b << ".endc()";
    return os;
  }
  builder b;
};

struct slice {
  template<class OStream>
  friend OStream &operator << (OStream &os, const slice &val) {
    os << val.b << ".endc().ctos()";
    return os;
  }
  builder b;
};

struct constant {
  template<class OStream>
  friend OStream &operator << (OStream &os, const constant &val) {
    os << val.val->getValue();
    return os;
  }
  llvm::ConstantInt *val;
};

struct unknown {
  template<class OStream>
  friend OStream &operator << (OStream &os, const unknown &) {
    os << "unknown";
    return os;
  }
};

struct null {
  template<class OStream>
  friend OStream &operator << (OStream &os, const null &) {
    os << "null";
    return os;
  }
};

struct binop {
  template<class OStream>
  friend OStream &operator << (OStream &os, const binop &val) {
    os << "(" << *val.left << " " << val.OpStr() << " " << *val.right << ")";
    return os;
  }
  const char *OpStr() const {
    switch (Opcode) {
    case BinaryOperator::Or: return "|";
    case BinaryOperator::And: return "&";
    case BinaryOperator::Add: return "+";
    case BinaryOperator::Sub: return "-";
    case BinaryOperator::UDiv: return "u/";
    case BinaryOperator::SDiv: return "/";
    default: return Instruction::getOpcodeName(Opcode);
    }
  }
  Instruction::BinaryOps Opcode;
  value *left;
  value *right;
};

struct global {
  template<class OStream>
  friend OStream &operator << (OStream &os, const global &val) {
    os << "global(" << val.idx << ")";
    return os;
  }
  unsigned idx;
};

using value_var = std::variant<unknown, builder, cell, slice, constant, null, binop, global>;
struct value : value_var {
  explicit value(Value *Val, const value_var &v) {
    *static_cast<value_var*>(this) = v;
    llvmVal = Val;
  }
  template<class OStream>
  friend OStream &operator << (OStream &os, const value &val) {
    os << *val.get();
    return os;
  }
  value_var *get() { return this; }
  const value_var *get() const { return this; }
  size_t len() const;
  void gather_params(LLVMContext &C, SmallVector<Type*, 10> &params) const;
  Value *gen_func(LLVMContext &C, Function *F, IRBuilder<> &B) const;
  Value *gen_call(Instruction *Instr, Function *F, value *fmt) const;

  Value *llvmVal = nullptr;
};

struct len_visitor {
  size_t operator()(unknown) const { return 1; }
  size_t operator()(constant) const { return 1; }
  size_t operator()(null) const { return 1; }
  size_t operator()(global) const { return 1; }
  size_t operator()(binop v) const {
    return 1 + std::visit(*this, *v.left->get()) +
        std::visit(*this, *v.right->get());
  }
  size_t operator()(builder b) const {
    return std::accumulate(b.chunks.begin(), b.chunks.end(), 1ul,
                           [this](size_t acc, const chunk &ch){
      return acc + std::visit(*this, ch);
    });
  }
  size_t operator()(cell cl) const {
    return 1 + (*this)(cl.b);
  }
  size_t operator()(slice sl) const {
    return 1 + (*this)(sl.b);
  }
  //stu, sti, stref, stslice, stvaruint16, stvaruint32, stvarint16, stvarint32, stdict
  template<typename T>
  size_t operator()(T v) const {
    return 1 + std::visit(*this, *v.val->get());
  }
};
size_t value::len() const {
  return std::visit(len_visitor{}, *get());
}
size_t builder::len() const {
  return len_visitor{}(*this);
}

struct gather_params_visitor {
  void operator()(value *val) { std::visit((*this), *val->get()); }
  void operator()(unknown) { params.push_back(cur_ty); }
  void operator()(constant) {}
  void operator()(null) {}
  void operator()(global) {}
  void operator()(binop v) {
    cur_ty = Type::getInt257Ty(C);
    (*this)(v.left);
    cur_ty = Type::getInt257Ty(C);
    (*this)(v.right);
  }
  void operator()(builder b) {
    for (auto ch : b.chunks)
      std::visit((*this), ch);
  }
  void operator()(cell cl) { (*this)(cl.b); }
  void operator()(slice sl) { (*this)(sl.b); }
  void operator()(stu v) {
    cur_ty = Type::getInt257Ty(C);
    (*this)(v.val);
  }
  void operator()(sti v) {
    cur_ty = Type::getInt257Ty(C);
    (*this)(v.val);
  }
  void operator()(stref v) {
    cur_ty = Type::getTVMCellTy(C);
    (*this)(v.val);
  }
  void operator()(stslice v) {
    cur_ty = Type::getTVMSliceTy(C);
    (*this)(v.val);
  }
  void operator()(stvaruint16 v) {
    cur_ty = Type::getInt257Ty(C);
    (*this)(v.val);
  }
  void operator()(stvaruint32 v) {
    cur_ty = Type::getInt257Ty(C);
    (*this)(v.val);
  }
  void operator()(stvarint16 v) {
    cur_ty = Type::getInt257Ty(C);
    (*this)(v.val);
  }
  void operator()(stvarint32 v) {
    cur_ty = Type::getInt257Ty(C);
    (*this)(v.val);
  }
  void operator()(stdict v) {
    cur_ty = Type::getTVMCellTy(C);
    (*this)(v.val);
  }
  LLVMContext& C;
  SmallVector<Type*, 10> &params;
  Type *cur_ty;
};

void value::gather_params(LLVMContext& C,
                          SmallVector<Type*, 10> &params) const {
  std::visit(gather_params_visitor{C, params, nullptr}, *get());
}

using namespace Intrinsic;

// generate calculation sequence (from re-func representation into llvm IR)
struct gen_func_visitor {
  template<auto Intr, class... Args>
  CallInst *make_call(Args... args) {
    auto *Fn = Intrinsic::getDeclaration(F->getParent(), Intr);
    return B.CreateCall(Fn, ArrayRef<Value *>{args...});
  }
  Value *prc(value *val, Type *ty) {
    Value *rv = std::visit(*this, *val->get());
    if (rv->getType() != ty)
      rv = B.CreateBitCast(rv, ty);
    return rv;
  }
  Value *operator()(unknown) { return &*(F->arg_begin() + arg_idx++); }
  Value *operator()(constant v) {
    return v.val;
  }
  Value *operator()(null) { return B.CreateIntrinsic(tvm_pushnull, {}, {});
  }
  Value *operator()(global v) {
    auto *Idx = ConstantInt::get(Type::getInt257Ty(C), v.idx);
    return make_call<tvm_getglobal>(Idx);
  }
  Value *operator()(binop v) {
    auto *ty = Type::getInt257Ty(C);
    Value *l = prc(v.left, ty);
    Value *r = prc(v.right, ty);
    return B.CreateBinOp(v.Opcode, l, r);
  }
  Value *operator()(builder b) {
    cur_b = make_call<tvm_newc>();
    for (const auto &ch : b.chunks) {
      cur_b = std::visit(*this, ch);
    }
    return cur_b;
  }
  Value *operator()(cell cl) {
    return make_call<tvm_endc>((*this)(cl.b));
  }
  Value *operator()(slice sl) {
    return make_call<tvm_ctos>(make_call<tvm_endc>((*this)(sl.b)));
  }
  Value *operator()(stu v) {
    auto *ty = Type::getInt257Ty(C);
    auto *Sz = ConstantInt::get(ty, v.bitsize);
    return make_call<tvm_stu>(prc(v.val, ty), cur_b, Sz);
  }
  Value *operator()(sti v) {
    auto *ty = Type::getInt257Ty(C);
    auto *Sz = ConstantInt::get(ty, v.bitsize);
    return make_call<tvm_stu>(prc(v.val, ty), cur_b, Sz);
  }
  Value *operator()(stref v) {
    auto *ty = Type::getTVMCellTy(C);
    return make_call<tvm_stref>(prc(v.val, ty), cur_b);
  }
  Value *operator()(stslice v) {
    auto *ty = Type::getTVMSliceTy(C);
    return make_call<tvm_stslice>(prc(v.val, ty), cur_b);
  }
  Value *operator()(stvaruint16 v) {
    auto *ty = Type::getInt257Ty(C);
    return make_call<tvm_stvaruint16>(cur_b, prc(v.val, ty));
  }
  Value *operator()(stvaruint32 v) {
    auto *ty = Type::getInt257Ty(C);
    return make_call<tvm_stvaruint32>(cur_b, prc(v.val, ty));
  }
  Value *operator()(stvarint16 v) {
    auto *ty = Type::getInt257Ty(C);
    return make_call<tvm_stvarint16>(cur_b, prc(v.val, ty));
  }
  Value *operator()(stvarint32 v) {
    auto *ty = Type::getInt257Ty(C);
    return make_call<tvm_stvarint32>(cur_b, prc(v.val, ty));
  }
  Value *operator()(stdict v) {
    auto *ty = Type::getTVMCellTy(C);
    return make_call<tvm_stdict>(prc(v.val, ty), cur_b);
  }
  LLVMContext &C;
  Function *F;
  IRBuilder<> &B;
  unsigned arg_idx;
  Type *cur_ty;
  Value *cur_b;
};

Value *value::gen_func(LLVMContext &C, Function *F, IRBuilder<> &B) const {
  return std::visit(gen_func_visitor{C, F, B, 0, nullptr, nullptr}, *get());
}

// generates call like %cl = call cell @__serialize_funcN(%Arg0, ..., %ArgN)
struct gen_call_visitor {
  gen_call_visitor sub(const value *l) const {
    return gen_call_visitor{l, Args};
  }
  // When we met unknown in format, we need to prepare argument
  template<typename T>
  void operator()(T left, unknown) const {
    if (!left_val)
      llvm_unreachable("Null left_val in gen_call_visitor");
    Value *Arg = left_val->llvmVal;
    Args.push_back(Arg);
  }
  void operator()(constant, constant) const {}
  void operator()(null, null) const {}
  void operator()(global, global) const {}
  void operator()(binop l, binop r) const {
    std::visit(sub(l.left), *l.left->get(), *r.left->get());
    std::visit(sub(l.right), *l.right->get(), *r.right->get());
  }
  void operator()(builder l, builder r) const {
    if (l.chunks.size() != r.chunks.size())
      llvm_unreachable("incompatible builders in gen_call_visitor");
    for (size_t i = 0, sz = l.chunks.size(); i < sz; ++i) {
      std::visit(sub(nullptr), l.chunks[i], r.chunks[i]);
    }
  }
  void operator()(cell l, cell r) const {
    (*this)(l.b, r.b);
  }
  void operator()(slice l, slice r) const {
    (*this)(l.b, r.b);
  }
  void operator()(stu l, stu r) const {
    std::visit(sub(l.val), *l.val->get(), *r.val->get());
  }
  void operator()(sti l, sti r) const {
    std::visit(sub(l.val), *l.val->get(), *r.val->get());
  }
  void operator()(stref l, stref r) const {
    std::visit(sub(l.val), *l.val->get(), *r.val->get());
  }
  void operator()(stslice l, stslice r) const {
    std::visit(sub(l.val), *l.val->get(), *r.val->get());
  }
  void operator()(stvaruint16 l, stvaruint16 r) const {
    std::visit(sub(l.val), *l.val->get(), *r.val->get());
  }
  void operator()(stvaruint32 l, stvaruint32 r) const {
    std::visit(sub(l.val), *l.val->get(), *r.val->get());
  }
  void operator()(stvarint16 l, stvarint16 r) const {
    std::visit(sub(l.val), *l.val->get(), *r.val->get());
  }
  void operator()(stvarint32 l, stvarint32 r) const {
    std::visit(sub(l.val), *l.val->get(), *r.val->get());
  }
  void operator()(stdict l, stdict r) const {
    std::visit(sub(l.val), *l.val->get(), *r.val->get());
  }
  template<typename L, typename R>
  void operator()(L, R) const {
    llvm_unreachable("Incompatible types in gen_call_visitor");
  }

  const value *left_val;
  SmallVector<Value*, 10> &Args;
};
Value *value::gen_call(Instruction *Instr, Function *F, value *fmt) const {
  IRBuilder<> B(Instr);
  SmallVector<Value*, 10> Args;
  gen_call_visitor visitor{this, Args};
  std::visit(visitor, *get(), *fmt->get());
  return B.CreateCall(F, Args);
}

struct processor {
  template<typename T, typename... Args> value *make(Args... args) {
    return new (alloc) value(CurVal, value_var{T{args...}});
  }
  template<typename StOp>
  value *process_sti_stu(value *val, value *b, value *sz) {
    if (!std::holds_alternative<constant>(*sz)) {
      return make<unknown>();
    }
    auto *cptr = std::get<constant>(*sz).val;
    if (cptr->getValue().getActiveBits() > 32) {
      return make<unknown>();
    }
    if (!std::holds_alternative<builder>(*b)) {
      return make<unknown>();
    }
    auto new_chunk = StOp{val, static_cast<unsigned>(cptr->getZExtValue())};
    auto *rv = make<builder>(std::get<builder>(*b));
    std::get<builder>(*rv).add_chunk(new_chunk);
    return rv;
  }
  value *process_stu(value *val, value *b, value *sz) {
    return process_sti_stu<stu>(val, b, sz);
  }
  value *process_sti(value *val, value *b, value *sz) {
    return process_sti_stu<sti>(val, b, sz);
  }
  value *process_stref(value *val, value *b) {
    if (!std::holds_alternative<builder>(*b)) {
      return make<unknown>();
    }
    auto *rv = make<builder>(std::get<builder>(*b));
    std::get<builder>(*rv).add_chunk(stref{val});
    return rv;
  }
  value *process_stslice(value *val, value *b) {
    if (!std::holds_alternative<builder>(*b)) {
      return make<unknown>();
    }
    auto *rv = make<builder>(std::get<builder>(*b));
    if (std::holds_alternative<slice>(*val)) {
      std::get<builder>(*rv).add_from_builder(std::get<slice>(*val).b);
      return rv;
    } else {
      std::get<builder>(*rv).add_chunk(stslice{val});
      return rv;
    }
  }
  template<typename StOp>
  value *process_stvar(value *b, value *val) {
    if (!std::holds_alternative<builder>(*b)) {
      return make<unknown>();
    }
    auto new_chunk = StOp{val};
    auto *rv = make<builder>(std::get<builder>(*b));
    std::get<builder>(*rv).add_chunk(new_chunk);
    return rv;
  }
  value *process_stdict(value *val, value *b) {
    if (!std::holds_alternative<builder>(*b)) {
      return make<unknown>();
    }
    auto *rv = make<builder>(std::get<builder>(*b));
    std::get<builder>(*rv).add_chunk(stdict{val});
    return rv;
  }
  value *process_endc(value *b) {
    if (!std::holds_alternative<builder>(*b)) {
      return make<unknown>();
    }
    return make<cell>(std::get<builder>(*b));
  }
  value *process_ctos(value *cl) {
    if (!std::holds_alternative<cell>(*cl)) {
      return make<unknown>();
    }
    return make<slice>(std::get<cell>(*cl).b);
  }
  value *process_global(value *idx) {
    if (!std::holds_alternative<constant>(*idx)) {
      return make<unknown>();
    }
    auto *cptr = std::get<constant>(*idx).val;
    if (cptr->getValue().getActiveBits() > 32) {
      return make<unknown>();
    }
    return make<global>(static_cast<unsigned>(cptr->getZExtValue()));
  }

  value *process_call(CallInst *Call) {
    if (auto *Func = Call->getCalledFunction()) {
      switch (Func->getIntrinsicID()) {
      case Intrinsic::tvm_stu:
        return process_stu(process(Call->getArgOperand(0)),
                           process(Call->getArgOperand(1)),
                           process(Call->getArgOperand(2)));
      case Intrinsic::tvm_sti:
        return process_sti(process(Call->getArgOperand(0)),
                           process(Call->getArgOperand(1)),
                           process(Call->getArgOperand(2)));
      case Intrinsic::tvm_stref:
        return process_stref(process(Call->getArgOperand(0)),
                             process(Call->getArgOperand(1)));
      case Intrinsic::tvm_stslice:
        return process_stslice(process(Call->getArgOperand(0)),
                               process(Call->getArgOperand(1)));
      case Intrinsic::tvm_stvarint16:
        return process_stvar<stvaruint16>(process(Call->getArgOperand(0)),
                                          process(Call->getArgOperand(1)));
      case Intrinsic::tvm_stvarint32:
        return process_stvar<stvaruint32>(process(Call->getArgOperand(0)),
                                          process(Call->getArgOperand(1)));
      case Intrinsic::tvm_stvaruint16:
        return process_stvar<stvaruint16>(process(Call->getArgOperand(0)),
                                          process(Call->getArgOperand(1)));
      case Intrinsic::tvm_stvaruint32:
        return process_stvar<stvaruint32>(process(Call->getArgOperand(0)),
                                          process(Call->getArgOperand(1)));
      case Intrinsic::tvm_stdict:
        return process_stdict(process(Call->getArgOperand(0)),
                              process(Call->getArgOperand(1)));
      case Intrinsic::tvm_endc:
        return process_endc(process(Call->getArgOperand(0)));
      case Intrinsic::tvm_ctos:
        return process_ctos(process(Call->getArgOperand(0)));
      case Intrinsic::tvm_newc:
        return make<builder>();
      case Intrinsic::tvm_pushnull:
        return make<null>();
      case Intrinsic::tvm_getglobal:
        return process_global(process(Call->getArgOperand(0)));
      case Intrinsic::tvm_cast_to_slice:
      case Intrinsic::tvm_cast_to_builder:
      case Intrinsic::tvm_cast_to_cell:
      case Intrinsic::tvm_cast_to_tuple:
      case Intrinsic::tvm_cast_from_slice:
      case Intrinsic::tvm_cast_from_builder:
      case Intrinsic::tvm_cast_from_cell:
      case Intrinsic::tvm_cast_from_tuple:
        return process(Call->getArgOperand(0));
      default:
        return make<unknown>();
      }
    } else {
      return make<unknown>();
    }
  }

  // main process function
  value *process(Value *Val) {
    Value *OldVal = CurVal;
    CurVal = Val;
    value *rv;
    if (auto *Call = dyn_cast<CallInst>(Val)) {
      rv = process_call(Call);
    } else if (auto *CVal = dyn_cast<ConstantInt>(Val)) {
      rv = make<constant>(CVal);
    } else if (auto *BOp = dyn_cast<BinaryOperator>(Val)) {
      rv = make<binop>(BOp->getOpcode(),
                       process(BOp->getOperand(0)),
                       process(BOp->getOperand(1)));
    } else {
      rv = make<unknown>();
    }
    CurVal = OldVal;
    return rv;
  }
  Value *CurVal = nullptr;
  SpecificBumpPtrAllocator<value> &alloc;
};

struct val_compare {
  // (value*, break_cmp)
  std::tuple<value*, bool> operator()(unknown, unknown) const {
    return {nullptr, false};
  }
  std::tuple<value*, bool> operator()(constant left, constant right) const {
    if (left.val == right.val)
      return {nullptr, false};
    return  {p.make<unknown>(), false};
  }
  std::tuple<value*, bool> operator()(null, null) const {
    return {nullptr, false};
  }
  std::tuple<value*, bool> operator()(cell l, cell r) const;
  std::tuple<value*, bool> operator()(binop left, binop right) const {
    if (left.Opcode != right.Opcode)
      return {nullptr, true};
    auto [left_cmp, left_break] =
        std::visit(*this, *left.left->get(), *right.left->get());
    if (left_break)
        return {nullptr, true};
    auto [right_cmp, right_break] =
        std::visit(*this, *left.right->get(), *right.right->get());
    if (right_break)
        return {nullptr, true};
    if (!left_cmp && !right_cmp) // both are fully equal
      return {nullptr, false};
    if (!left_cmp || !right_cmp) {
      if (!left_cmp) left_cmp = left.left;
      if (!right_cmp) right_cmp = right.left;
    }
    return {p.make<binop>(left.Opcode, left_cmp, right_cmp), false};
  }
  std::tuple<value*, bool> operator()(global left, global right) const {
    if (left.idx == right.idx)
      return {nullptr, false};
    return {nullptr, true};
  }
  template<typename T>
  std::tuple<value*, bool> operator()(T, T) const {
    return {nullptr, true};
  }
  template<typename T1, typename T2>
  std::tuple<value*, bool> operator()(T1, T2) const {
    return {nullptr, true};
  }
  processor &p;
};

struct chunk_compare {
  // (chunk | break_cmp)
  template<typename T>
  std::variant<chunk, bool> operator()(T left, T right) {
    if (left.equal_nonrecursive(right)) {
      auto [cmp, break_cmp] =
          std::visit(val_compare{p},
                     *left.val->get(), *right.val->get());
      if (break_cmp) cmp = p.make<unknown>();
      if (!cmp) // no changes, fully equal
        return false;
      if constexpr (std::is_same_v<T, stu> || std::is_same_v<T, sti>)
        return chunk(T{cmp, left.bitsize});
      else
        return chunk(T{cmp});
    }
    return true; // break_cmp = true
  }
  template<typename T1, typename T2>
  std::variant<chunk, bool> operator()(T1, T2) {
    return true; // break_cmp = true
  }
  processor &p;
};

builder merge_builders(processor &p, const builder &b1, const builder &b2,
                       bool &full_equal, bool &left_prefix) {
  full_equal = false;
  left_prefix = false;

  bool changed = false;
  builder rv;
  for (size_t i = 0, sz = std::min(b1.chunks.size(), b2.chunks.size());
       i < sz; ++i) {
    auto l = b1.chunks[i];
    auto r = b2.chunks[i];
    auto cmp = std::visit(chunk_compare{p}, l, r);
    if (std::holds_alternative<chunk>(cmp)) {
      changed = true;
      rv.chunks.push_back(std::get<chunk>(cmp));
    } else {
      bool break_cmp = std::get<bool>(cmp);
      if (break_cmp) {
        return rv;
      } else {
        rv.chunks.push_back(l);
      }
    }
  }
  full_equal = !changed && b1.chunks.size() == b2.chunks.size();
  left_prefix = !changed && b1.chunks.size() < b2.chunks.size();

  return rv;
}

std::tuple<value*, bool> val_compare::operator()(cell l, cell r) const {
  if (l.b.chunks.size() != r.b.chunks.size()) {
    return {p.make<unknown>(), false};
  }

  bool full_equal, left_prefix;
  auto b = merge_builders(p, l.b, r.b, full_equal, left_prefix);

  if (full_equal)
    return {nullptr, false};
  if (b.chunks.size() != l.b.chunks.size()) {
    return {p.make<unknown>(), false};
  }
  return {p.make<cell>(b), false};
}

struct sendmsg {
  CallInst *Instr;
  value *val;
  size_t best_candidate;
};

struct candidate {
  value *val;
  size_t len;
  size_t idx;
  bool operator<(const candidate &v) const {
    return (len > v.len) || ((len == v.len) && (idx > v.idx));
  }
};

Function *makeFunction(Module &M, value *val, size_t func_idx) {
  LLVMContext& C = M.getContext();
  Type *RetTy = std::visit(
    overloaded{
      [&](emulated::builder){ return Type::getTVMBuilderTy(C); },
      [&](emulated::cell){ return Type::getTVMCellTy(C); },
      [&](emulated::slice){ return Type::getTVMSliceTy(C); },
      [&](emulated::unknown){ return Type::getVoidTy(C); },
      [&](emulated::constant){ return Type::getVoidTy(C); },
      [&](emulated::null){ return Type::getVoidTy(C); },
      [&](emulated::binop){ return Type::getVoidTy(C); },
      [&](emulated::global){ return Type::getVoidTy(C); }
    }, *val->get());
  if (RetTy->isVoidTy())
    llvm_unreachable("Unsupported serialize function return type");

  SmallVector<Type*, 10> params;
  val->gather_params(C, params);
  auto *FTy = FunctionType::get(RetTy, params, false);

  std::stringstream name_ss;
  name_ss << "__serialize_func" << func_idx;
  auto *F = cast<Function>(M.getOrInsertFunction(name_ss.str(), FTy).getCallee());
  F->addFnAttr(Attribute::NoInline);
  F->addFnAttr(Attribute::NoRecurse);
  F->setLinkage(GlobalValue::InternalLinkage);
  unsigned ArgIdx = 0;
  for (Argument &Arg : F->args()) {
    std::stringstream ss;
    ss << "Arg" << ArgIdx;
    Arg.setName(ss.str());
  }
  auto *Builder = new IRBuilder<>(BasicBlock::Create(C, "entry", F));
  Value *Rv = val->gen_func(C, F, *Builder);
  Builder->CreateRet(Rv);
  return F;
}

struct merger {
  std::tuple<size_t, size_t> find_candidate(value *val) {
    auto cur_len = val->len();
    size_t max_merge = 0;
    size_t cur_best = 0;
    for (const auto &cur_candidate : candidates) {
      if (cur_candidate.len > cur_len)
        continue;
      auto b1 = std::get<cell>(*cur_candidate.val).b;
      auto b2 = std::get<cell>(*val).b;
      bool full_equal, left_prefix;
      auto b = merge_builders(p, b1, b2, full_equal, left_prefix);
      if (full_equal || left_prefix) {
        return {cur_candidate.idx, cur_candidate.len};
      }
      auto merge_len = b.len();
      if (merge_len > max_merge) {
        cur_best = cur_candidate.idx;
        max_merge = merge_len;
      }
      if (max_merge >= cur_candidate.len) {
        return {cur_best, max_merge};
      }
    }
    return {cur_best, max_merge};
  }
  value *merge_cells(value *cl1, value *cl2) const {
    auto b1 = std::get<cell>(*cl1).b;
    auto b2 = std::get<cell>(*cl2).b;
    bool full_equal, left_prefix;
    auto b = merge_builders(p, b1, b2, full_equal, left_prefix);
    if (full_equal || left_prefix)
      return cl1;
    return p.make<cell>(b);
  }
  void process(Module &M) {
    for (const auto &cur_msg : sendmsgs) {
      auto cur_len = cur_msg.val->len();
      if (cur_len < 5)
        continue;
      auto [best_idx, max_depth] = find_candidate(cur_msg.val);
      if (!best_idx || max_depth < std::min<size_t>(10ul, cur_len) || cur_len > max_depth + 5) {
        candidates.insert({cur_msg.val, cur_len, candidates.size() + 1});
      }
    }

    std::map<size_t, size_t> weights;
    for (auto &cur_msg : sendmsgs) {
      if (cur_msg.val->len() < 5)
        continue;
      auto [best_idx, max_depth] = find_candidate(cur_msg.val);
      cur_msg.best_candidate = best_idx;
      auto it = weights.find(best_idx);
      if (it == weights.end())
          it = weights.insert({best_idx, 0ul}).first;
      ++it->second;
    }
    for (const auto &cur_weight : weights) {
      if (cur_weight.second > 1) {
        auto idx = cur_weight.first;
        auto it_c = std::find_if(candidates.begin(), candidates.end(),
                                 [=](candidate v){return v.idx == idx;});
        if (it_c == candidates.end())
          llvm_unreachable("ReFunc candidate missed");
        candidate best_c = *it_c;
        value *best_val = best_c.val;
        size_t msg_idx = 0;
        for (const auto &cur_msg : sendmsgs) {
          if (cur_msg.best_candidate == idx) {
            best_val = merge_cells(best_val, cur_msg.val);
          }
          ++msg_idx;
        }
        Function *F = makeFunction(M, best_val, idx);

        for (const auto &cur_msg : sendmsgs) {
          if (cur_msg.val->len() < 5)
            continue;
          if (cur_msg.best_candidate == idx) {
            Value *clArg = cur_msg.val->gen_call(cur_msg.Instr, F, best_val);
            auto *Fn = Intrinsic::getDeclaration(F->getParent(),
                                                 Intrinsic::tvm_sendrawmsg);
            auto *Flag = cur_msg.Instr->getArgOperand(1);
            IRBuilder<> B(cur_msg.Instr);
            B.CreateCall(Fn, ArrayRef<Value *>{clArg, Flag});
            cur_msg.Instr->eraseFromParent();
          }
        }
      }
    }
  }
  processor &p;
  SmallVector<sendmsg, 20> sendmsgs;
  std::set<candidate> candidates;
};

} // namespace emulated

void printRecursive(raw_ostream &ROS, llvm::Value* Val) {
  if (auto *Call = dyn_cast<CallInst>(Val)) {
    if (auto *Func = Call->getCalledFunction()) {
      auto FName = Func->getName();
      if (FName.startswith("llvm.tvm."))
        ROS << FName.drop_front(StringRef("llvm.tvm.").size());
      else
        ROS << FName;
    } else {
      ROS << "(*F)";
    }
    ROS << "(";
    for (unsigned i = 0, sz = Call->getNumArgOperands(); i < sz; ++i) {
      if (i) ROS << ";";
      printRecursive(ROS, Call->getArgOperand(i));
    }
    ROS << ")";
  } else if (auto *CVal = dyn_cast<ConstantInt>(Val)) {
    ROS << CVal->getValue();
  } else if (auto *BOp = dyn_cast<BinaryOperator>(Val)) {
    if (BOp->getOpcode() == BinaryOperator::Or) {
      printRecursive(ROS, BOp->getOperand(0));
      ROS << "|";
      printRecursive(ROS, BOp->getOperand(1));
    } else {
      Val->print(ROS);
    }
  } else {
    Val->print(ROS);
  }
}

} // End anonymous namespace

char TVMReFunc::ID = 0;
INITIALIZE_PASS(TVMReFunc, DEBUG_TYPE,
                "Combine common expressions into functions", false, false)

ModulePass *llvm::createTVMReFuncPass() { return new TVMReFunc(); }

bool TVMReFunc::processSendRawMsgUses(Function &F) {
  Intrinsic::ID ID = F.getIntrinsicID();
  bool Changed = false;
  SpecificBumpPtrAllocator<emulated::value> Allocator;

  emulated::processor proc{nullptr, Allocator};
  emulated::merger merger{proc, {}, {}};

  for (auto I = F.user_begin(), E = F.user_end(); I != E;) {
    Instruction *Inst = cast<Instruction>(*I);

    auto *Call = dyn_cast<CallInst>(Inst);
    if (!Call) continue;
    auto *val = proc.process(Call->getArgOperand(0));
    merger.sendmsgs.push_back({Call, val, 0});

    ++I;
  }
  merger.process(*F.getParent());
  Changed = true;
  return Changed;
}

bool TVMReFunc::runOnModule(Module &M) {
  if (!TVMEnableMReFunc)
    return false;
  bool Changed = false;

  for (Function &F : M) {
    if (!F.isDeclaration())
      continue;

    switch (F.getIntrinsicID()) {
    case Intrinsic::tvm_sendrawmsg:
      if (processSendRawMsgUses(F))
        Changed = true;
      break;
    default:
      break;
    }
  }
  return Changed;
}
