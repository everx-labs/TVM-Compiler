#pragma once

#include <utility>
#include <tuple>

namespace tvm {

template<class S, size_t... Indices, class Tup>
S to_struct(std::index_sequence<Indices...>, Tup && tup) {
  return { std::get<Indices>(std::forward<Tup>(tup))... };
}

template<class S, class Tup>
S to_struct(Tup && tup) {
  using T = std::remove_reference_t<Tup>;
  return to_struct<S>(
    std::make_index_sequence<std::tuple_size_v<T>>{}, std::forward<Tup>(tup));
}

} // namespace tvm
