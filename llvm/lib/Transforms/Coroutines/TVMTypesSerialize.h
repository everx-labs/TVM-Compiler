//===- TVMTypesSerialize.h - Serialization/Deserialization for TVM --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// Serialization/Deserialization of TVM types into cells.
// Used for serialization of coroutine frames.
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TRANSFORMS_COROUTINES_TVMTYPESSERIALIZE_H
#define LLVM_LIB_TRANSFORMS_COROUTINES_TVMTYPESSERIALIZE_H

#include "llvm/ADT/STLExtras.h"
#include "llvm/Transforms/Coroutines.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IntrinsicsTVM.h"

#include <variant>
#include <vector>

namespace llvm {

template<class Stream, class T0, class ... Ts>
Stream &operator<< (Stream &s,
                    std::variant<T0, Ts...> const &v)
{ std::visit([&](auto && arg){ s << arg;}, v); return s; }

namespace coro {

#ifdef TVMINTR
#error TVMINTR override
#endif
#define TVMINTR(FUNC) \
  Intrinsic::getDeclaration(&TheModule, FUNC)

class int_ {
public:
  template<class OStream>
  friend OStream &operator << (OStream &os, const int_ &val) {
    os << "int" << val.sz_;
    return os;
  }
  void store(Module &TheModule, IRBuilder<> &Builder, Value *&CellBuilder) {
    auto *Load = Builder.CreateLoad(ptr_);
    auto *Val = Builder.CreateSExt(Load, Builder.getInt257Ty());
    CellBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_sti),
      {Val, CellBuilder, Builder.getInt257(sz_)});
  }
  void load(Module &TheModule, IRBuilder<> &Builder, Value *&ReadSlice) {
    auto *ElemSz = Builder.getInt257(sz_);
    auto *ValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldi),
                                           {ReadSlice, ElemSz});
    auto *Val = Builder.CreateExtractValue(ValAndSlice, 0);
    ReadSlice = Builder.CreateExtractValue(ValAndSlice, 1);
    Val = Builder.CreateTruncOrBitCast(Val, ptr_->getType()->getPointerElementType());
    Builder.CreateStore(Val, ptr_);
  }
  unsigned sz_;
  Value *ptr_;
};
class uint_ {
public:
  template<class OStream>
  friend OStream &operator << (OStream &os, const uint_ &val) {
    os << "uint" << val.sz_;
    return os;
  }
  void store(Module &TheModule, IRBuilder<> &Builder, Value *&CellBuilder) {
    auto *Load = Builder.CreateLoad(ptr_);
    auto *Val = Builder.CreateZExt(Load, Builder.getInt257Ty());
    CellBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_stu),
      {Val, CellBuilder, Builder.getInt257(sz_)});
  }
  void load(Module &TheModule, IRBuilder<> &Builder, Value *&ReadSlice) {
    auto *ElemSz = Builder.getInt257(sz_);
    auto *ValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldu),
                                           {ReadSlice, ElemSz});
    auto *Val = Builder.CreateExtractValue(ValAndSlice, 0);
    ReadSlice = Builder.CreateExtractValue(ValAndSlice, 1);
    Val = Builder.CreateTruncOrBitCast(Val, ptr_->getType()->getPointerElementType());
    Builder.CreateStore(Val, ptr_);
  }
  unsigned sz_;
  Value *ptr_;
};
class address_ {
public:
  template<class OStream>
  friend OStream &operator << (OStream &os, const address_ &) {
    os << "address";
    return os;
  }
  void store(Module &TheModule, IRBuilder<> &Builder, Value *&CellBuilder) {
    auto *Load = Builder.CreateLoad(ptr_);
    CellBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_stslice),
      {Load, CellBuilder});
  }
  void load(Module &TheModule, IRBuilder<> &Builder, Value *&ReadSlice) {
    auto *ValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldmsgaddr),
                                           {ReadSlice});
    auto *Val = Builder.CreateExtractValue(ValAndSlice, 0);
    ReadSlice = Builder.CreateExtractValue(ValAndSlice, 1);
    Builder.CreateStore(Val, ptr_);
  }
  Value *ptr_;
};
class slice_ {
  static constexpr unsigned SliceSizeBits = 10;
public:
  template<class OStream>
  friend OStream &operator << (OStream &os, const slice_ &val) {
    os << "slice" << val.sz_;
    return os;
  }
  void store(Module &TheModule, IRBuilder<> &Builder, Value *&CellBuilder) {
    auto *Load = Builder.CreateLoad(ptr_);
    CellBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_stslice),
      {Load, CellBuilder});
  }
  void load(Module &TheModule, IRBuilder<> &Builder, Value *&ReadSlice) {
    auto *SliceSz = Builder.getInt257(sz_);
    auto *ValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldslice),
                                           {ReadSlice, SliceSz});
    auto *Val = Builder.CreateExtractValue(ValAndSlice, 0);
    ReadSlice = Builder.CreateExtractValue(ValAndSlice, 1);
    Builder.CreateStore(Val, ptr_);
  }
  unsigned sz_;
  Value *ptr_;
};
// slice with statically unknown size will be stored into separate cell
class dyn_slice_ {
public:
  template<class OStream>
  friend OStream &operator << (OStream &os, const dyn_slice_ &) {
    os << "dyn_slice";
    return os;
  }
  void store(Module &TheModule, IRBuilder<> &Builder, Value *&CellBuilder) {
    auto *Load = Builder.CreateLoad(ptr_);
    auto *NewC = Builder.CreateCall(TVMINTR(Intrinsic::tvm_newc), {});
    NewC = Builder.CreateCall(TVMINTR(Intrinsic::tvm_stslice), {Load, NewC});
    NewC = Builder.CreateCall(TVMINTR(Intrinsic::tvm_endc), {NewC});
    CellBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_stref),
      {NewC, CellBuilder});
  }
  void load(Module &TheModule, IRBuilder<> &Builder, Value *&ReadSlice) {
    auto *ValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldrefrtos),
                                           {ReadSlice});
    auto *Val = Builder.CreateExtractValue(ValAndSlice, 1);
    ReadSlice = Builder.CreateExtractValue(ValAndSlice, 0);
    Builder.CreateStore(Val, ptr_);
  }
  Value *ptr_;
};
class cell_ {
public:
  template<class OStream>
  friend OStream &operator << (OStream &os, const cell_ &) {
    os << "cell";
    return os;
  }
  void store(Module &TheModule, IRBuilder<> &Builder, Value *&CellBuilder) {
    auto *Load = Builder.CreateLoad(ptr_);
    CellBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_stdict),
      {Load, CellBuilder});
  }
  void load(Module &TheModule, IRBuilder<> &Builder, Value *&ReadSlice) {
    auto *ValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_lddict),
                                           {ReadSlice});
    auto *Val = Builder.CreateExtractValue(ValAndSlice, 0);
    ReadSlice = Builder.CreateExtractValue(ValAndSlice, 1);
    Builder.CreateStore(Val, ptr_);
  }
  Value *ptr_;
};
// we serializing builder into cell,
// and creating new builder on load (with stslice'ed old builder content)
class builder_ {
public:
  template<class OStream>
  friend OStream &operator << (OStream &os, const builder_ &) {
    os << "builder";
    return os;
  }
  void store(Module &TheModule, IRBuilder<> &Builder, Value *&CellBuilder) {
    auto *Load = Builder.CreateLoad(ptr_);
    auto *Cell = Builder.CreateCall(TVMINTR(Intrinsic::tvm_endc), {Load});
    CellBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_stref),
      {Cell, CellBuilder});
  }
  void load(Module &TheModule, IRBuilder<> &Builder, Value *&ReadSlice) {
    auto *ValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldrefrtos),
                                           {ReadSlice});
    auto *Val = Builder.CreateExtractValue(ValAndSlice, 1);
    ReadSlice = Builder.CreateExtractValue(ValAndSlice, 0);
    Value *NewBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_newc), {});
    NewBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_stslice),
      {Val, NewBuilder});
    Builder.CreateStore(NewBuilder, ptr_);
  }
  Value *ptr_;
};
class header_ {
public:
  template<class OStream>
  friend OStream &operator << (OStream &os, const header_ &) {
    os << "header_";
    return os;
  }
  // header is loaded/stored outside (in smart_switcher), nothing to do here
  void store(Module &, IRBuilder<> &, Value *&) {}
  void load(Module &, IRBuilder<> &, Value *&) {}
};

using type_serializer =
  std::variant<int_, uint_, address_, slice_, dyn_slice_,
               cell_, builder_, header_>;

} // End namespace coro.

using type_serializer = coro::type_serializer;

class cell_pattern {
  static constexpr unsigned max_cell_bits = 1023;
  static constexpr unsigned max_cell_refs = 4;
public:
  cell_pattern() : bits_(0), refs_(0) {}
  template<class OStream>
  friend OStream &operator << (OStream &os, const cell_pattern &val) {
    os << "(bits:" << val.bits_ << ", refs:" << val.refs_ << "){";
    if (!val.types_.empty()) {
      os << val.types_.front();
      for (auto ty : make_range(std::next(val.types_.begin()), val.types_.end()))
        os << ", " << ty;
    }
    os << "}";
    return os;
  }
  bool fits(unsigned bits, unsigned refs, bool last) const {
    if (bits_ + bits > max_cell_bits)
      return false;
    if (refs_ + refs > max_cell_refs)
      return false;
    if (refs_ + refs == max_cell_refs && !last)
      return false;
    return true;
  }
  void add(const type_serializer &ty, unsigned bits, unsigned refs) {
    bits_ += bits;
    refs_ += refs;
    types_.push_back(ty);
  }
  Value *store(Module &TheModule, IRBuilder<> &Builder,
               Value *CellBuilder, Value *NextCell) const {
    for (const auto &ty : types_) {
      std::visit([&](auto arg){ arg.store(TheModule, Builder, CellBuilder); }, ty);
    }
    if (NextCell)
      CellBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_stref),
                                       {NextCell, CellBuilder});
    return Builder.CreateCall(TVMINTR(Intrinsic::tvm_endc), {CellBuilder});
  }
  void load(Module &TheModule, IRBuilder<> &Builder, Value *&ReadSlice) const {
    for (const auto &ty : types_) {
      std::visit([&](auto arg){ arg.load(TheModule, Builder, ReadSlice); }, ty);
    }
  }
  std::vector<type_serializer> types_;
  unsigned bits_;
  unsigned refs_;
};

class types_pattern {
  // Must be equal to schema::estimate_element<awaiting_record>::max_bits
  static constexpr unsigned header_max_bits = 296;
public:
  types_pattern(Module &TheModule, IRBuilder<> &Builder, StructType *FrameTy,
                Value *Frame) {
    // skipping already represented header
    cells_.push_back(cell_pattern{});
    cells_.back().add(coro::header_{}, header_max_bits, 1);
    // skipping resume_ptr, cleanup_ptr and promise
    auto Elems = drop_begin(FrameTy->elements(), 3);
    unsigned Idx = 3;
    for (Type *Ty : Elems) {
      auto *ElemPtr =
        Builder.CreateConstInBoundsGEP2_32(FrameTy, Frame, 0, Idx);
      if (Ty->isPointerTy()) {
        contract_ptrs_.push_back(ElemPtr);
      }
      prepare_element(TheModule, Builder, Ty, ElemPtr,
                      Idx + 1 == FrameTy->elements().size());
      ++Idx;
    }
  }
  void prepare_element(Module &TheModule, IRBuilder<> &Builder, Type *Ty,
                       Value *Ptr, bool last) {
    // TODO: generate error if pointer is not contract class type
    if (Ty->isPointerTy())
      return;
    const auto &DL = TheModule.getDataLayout();
    if (StructType *ElemSTy = dyn_cast<StructType>(Ty)) {
      unsigned Idx = 0;
      for (Type *Ty : ElemSTy->elements()) {
        auto *ElemPtr =
          Builder.CreateConstInBoundsGEP2_32(ElemSTy, Ptr, 0, Idx);
        prepare_element(TheModule, Builder, Ty, ElemPtr, last);
        ++Idx;
      }
      return;
    } else if (ArrayType *ElemArray = dyn_cast<ArrayType>(Ty)) {
      Type *SubTy = ElemArray->getElementType();
      for (unsigned Idx = 0; Idx < ElemArray->getNumElements(); ++Idx) {
        auto *ElemPtr =
          Builder.CreateConstInBoundsGEP2_32(ElemArray, Ptr, 0, Idx);
        prepare_element(TheModule, Builder, SubTy, ElemPtr, last);
      }
      return;
    }
    unsigned Sz = static_cast<unsigned>(DL.getTypeSizeInBits(Ty));
    assert(Sz <= 257);
    if (Ty->isTVMSliceTy()) {
      // TODO: support address and known-fixed-size slices here
      check_overflow(0, 1, last);
      cells_.back().add(coro::dyn_slice_{Ptr}, 0, 1);
    } else if (Ty->isTVMCellTy()) {
      check_overflow(0, 1, last);
      cells_.back().add(coro::cell_{Ptr}, 1, 1);
    } else if (Ty->isTVMBuilderTy()) {
      check_overflow(0, 1, last);
      cells_.back().add(coro::builder_{Ptr}, 0, 1);
    } else if (Sz <= 256) {
      check_overflow(Sz, 0, last);
      cells_.back().add(coro::uint_{Sz, Ptr}, Sz, 0);
    } else {
      check_overflow(Sz, 0, last);
      cells_.back().add(coro::int_{Sz, Ptr}, Sz, 0);
    }
  }
  // In CellBuilder we already have serialized awaiting frame header
  Value *store(Module &TheModule, IRBuilder<> &Builder, Value *StartBuilder) {
    if (cells_.empty())
      return Builder.CreateCall(TVMINTR(Intrinsic::tvm_endc), {StartBuilder});
    Value *NextCell = nullptr;
    for (const auto &cl : make_range(cells_.rbegin(), std::prev(cells_.rend()))) {
      Value *CellBuilder = Builder.CreateCall(TVMINTR(Intrinsic::tvm_newc), {});
      NextCell = cl.store(TheModule, Builder, CellBuilder, NextCell);
    }
    return cells_.front().store(TheModule, Builder, StartBuilder, NextCell);
  }
  void load(Module &TheModule, IRBuilder<> &Builder, Value *ReadSlice,
            Value *NewThis) {
    for (auto *ptr : contract_ptrs_) {
      Builder.CreateStore(NewThis->stripPointerCasts(), ptr);
    }
    if (cells_.empty())
      return;
    cells_.front().load(TheModule, Builder, ReadSlice);
    for (const auto &cl : make_range(std::next(cells_.begin()), cells_.end())) {
      auto *ValAndSlice = Builder.CreateCall(TVMINTR(Intrinsic::tvm_ldrefrtos),
                                             {ReadSlice});
      ReadSlice = Builder.CreateExtractValue(ValAndSlice, 1);
      cl.load(TheModule, Builder, ReadSlice);
    }
  }
  void check_overflow(unsigned bits, unsigned refs, bool last) {
    if (cells_.empty() || !cells_.back().fits(bits, refs, last))
      cells_.push_back(cell_pattern());
  }
  template<class OStream>
  friend OStream &operator << (OStream &os, const types_pattern &val) {
    os << "{";
    if (!val.cells_.empty()) {
      os << val.cells_.front();
      for (auto ty : make_range(std::next(val.cells_.begin()), val.cells_.end()))
        os << " | " << ty;
    }
    os << "}";
    return os;
  }
  std::vector<cell_pattern> cells_;
  std::vector<Value*> contract_ptrs_;
};

#undef TVMINTR

} // End namespace llvm

#endif
