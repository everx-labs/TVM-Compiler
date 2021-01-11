//===----------- TVMExtras.h - STL related functions ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
/// \file
/// This file contains templates that might be not immediately useful
/// throughout LLVM codebase, but they are used in TVM backend.
///
//
//===----------------------------------------------------------------------===//

// TODO: Consider to upstream case by case to STLExtras.h or wherever it
// appropriate.

#ifndef LLVM_LIB_TARGET_TVM_TVMEXTRAS_H
#define LLVM_LIB_TARGET_TVM_TVMEXTRAS_H

#include <type_traits>
#include <variant>

#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/Constants.h"

namespace llvm {
/// Finds an element and enforces that the search was successful.
template <typename R, typename T>
auto find_or_fail(R &&Range, const T &Val) -> decltype(adl_begin(Range)) {
  auto It = llvm::find(Range, Val);
  assert(It != std::end(Range));
  return It;
}

/// Finds an element and enforces that the search was successful.
template <typename R, typename UnaryPredicate>
auto find_if_or_fail(R &&Range, UnaryPredicate P)
    -> decltype(adl_begin(Range)) {
  auto It = llvm::find_if(Range, P);
  assert(It != std::end(Range));
  return It;
}

/// Check if an element exist in a range.
template <typename R, typename T> bool exist(R &&Range, const T &Val) {
  auto It = llvm::find(Range, Val);
  return It != std::end(Range);
}

template <typename T, typename... Ts,
          typename = std::enable_if_t<
              !std::is_same_v<std::decay_t<T>, std::variant<Ts...>>>>
bool operator==(const std::variant<Ts...> &Lhs, T &&Rhs) {
  using DecayT = std::decay_t<T>;
  static_assert((std::is_same_v<DecayT, Ts> || ...),
                "The variant can't hold T object");
  if (!std::holds_alternative<DecayT>(Lhs))
    return false;
  return std::get<DecayT>(Lhs) == Rhs;
}

template <typename T, typename... Ts,
          typename = std::enable_if_t<
              !std::is_same_v<std::decay_t<T>, std::variant<Ts...>>>>
bool operator!=(const std::variant<Ts...> &Lhs, T &&Rhs) {
  return !operator==(Lhs, Rhs);
}

template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded(Ts...)->overloaded<Ts...>;

inline ConstantInt *cimm(LLVMContext &C, unsigned value) {
  auto *Ty = IntegerType::get(C, 257);
  return ConstantInt::get(Ty, value, false);
}

} // namespace llvm

#endif // LLVM_LIB_TARGET_TVM_TVMEXTRAS_H
